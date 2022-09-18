#include "LIBGPU.H"
#include "EMULATOR_RENDER_NVN.H"
#include "Core/Debug/EMULATOR_LOG.H"
#include "Core/Render/EMULATOR_RENDER_COMMON.H"
#include <stdio.h>

#if defined(PLATFORM_NX)

nn::gfx::BlendState g_BlendState[BM_COUNT];
nn::gfx::Texture g_Texture;
nn::gfx::Texture g_vramTexture;
nn::gfx::Texture g_rg8lutTexture;
nn::gfx::Texture g_whiteTexture;

#define MAX_SHADER_STACK_SIZE (32)
static ShaderID shaderStack[MAX_SHADER_STACK_SIZE] = {};
static int shaderStackIndex = 0;
#define GET_SHADER() shaderStack[shaderStackIndex++];

#define MAX_TEXTURE_STACK_SIZE (32)
static TextureID textureStack[MAX_TEXTURE_STACK_SIZE] = {};
static int textureStackIndex = 0;
#define GET_TEXTURE() textureStack[textureStackIndex++]; \

extern void Emulator_DoPollEvent();
extern void Emulator_WaitForTimestep(int count);
extern void Emulator_GenerateCommonTextures();
extern void Emulator_CreateGlobalShaders();
extern void Emulator_DestroyTextures();
extern void Emulator_DestroyGlobalShaders();
extern void Emulator_CreateVertexBuffer();

const char* renderBackendName = "NVN";

enum MemoryPoolType
{
	MemoryPoolType_CpuCached_GpuCached,
	MemoryPoolType_CpuUncached_GpuCached,
	MemoryPoolType_CpuInvisible_GpuCached_Compressible,
	MemoryPoolType_End
};

const float Vertices[] =
{
	0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, -0.5f, 0.0f, 1.0f, 1.0f
};

const int Indices[] =
{
	0, 1, 2, 3
};

unsigned int dynamic_vertex_buffer_index = 0;

int g_BufferDescriptorBaseIndex = 0;
int g_TextureDescriptorBaseIndex = 0;
int g_SamplerDescriptorBaseIndex = 0;

const size_t GraphicsSystemMemorySize = 8 * 1024 * 1024;

nn::util::BytePtr g_pMemoryHeap(NULL);
nn::util::BytePtr g_pMemory(NULL);

const size_t MemoryPoolSize[MemoryPoolType_End] =
{
	16 * 1024 * 1024,
	16 * 1024 * 1024,
	20 * 1024 * 1024
};

void* g_pPoolMemory[MemoryPoolType_End] = {};
ptrdiff_t g_MemoryPoolOffset[MemoryPoolType_End] = {};

size_t g_MaxScratchMemory = 0;
ptrdiff_t g_ScratchMemoryOffset = 0;

int g_commandListRecording = FALSE;

nn::vi::Display* g_display;
nn::vi::Layer* g_layer;
nn::gfx::Device g_Device;
nn::gfx::MemoryPool g_MemoryPool[MemoryPoolType_End];
nn::gfx::Queue g_Queue;
nn::gfx::CommandBuffer g_CommandBuffer;
nn::gfx::ResShaderFile* g_pResShaderFile;
nn::gfx::DescriptorPool g_TextureDescriptorPool;
nn::gfx::Fence g_Fence;
nn::gfx::ResTextureFile* g_pResTextureFile;

nn::gfx::ViewportScissorState* g_ViewportScissor = NULL;
nn::gfx::RasterizerState g_RasterizerState;
nn::gfx::DepthStencilState g_DepthStencilState;
nn::gfx::Buffer g_VertexBuffer;
nn::gfx::Buffer g_ConstantBuffer;
nn::gfx::Sampler g_Sampler;
nn::gfx::Sampler g_Sampler2;
nn::gfx::DescriptorPool g_BufferDescriptorPool;
nn::gfx::DescriptorPool g_SamplerDescriptorPool;
NVNtextureBuilder g_textureBuilder;

unsigned int dynamic_vertex_buffer;
unsigned int dynamic_vertex_array;

int FRAME_INDEX = 0;

ShaderID* Shader_Compile(const char* path_vs, const char* path_ps)
{
	ShaderID* program = &GET_SHADER();

	FILE* f = fopen(path_vs, "rb");
	char* vs_list = NULL;
	char* ps_list = NULL;
	unsigned int vs_size = 0;
	unsigned int ps_size = 0;

	NN_ASSERT_NOT_NULL(f);
	if (f != NULL)
	{
		fseek(f, 0, SEEK_END);
		vs_size = ftell(f);
		fseek(f, 0, SEEK_SET);

		vs_list = new char[vs_size];

		fread(vs_list, vs_size, 1, f);

		fclose(f);
	}

	f = fopen(path_ps, "rb");
	NN_ASSERT_NOT_NULL(f);

	if (f != NULL)
	{
		fseek(f, 0, SEEK_END);
		ps_size = ftell(f);
		fseek(f, 0, SEEK_SET);

		ps_list = new char[ps_size];

		fread(ps_list, ps_size, 1, f);

		fclose(f);
	}


	nn::gfx::Shader::InfoType info;
	info.SetDefault();
	info.SetSeparationEnabled(false);
	nn::gfx::ShaderCode codeVS;
	nn::gfx::ShaderCode codePS;

	codeVS.codeSize = static_cast<uint32_t>(vs_size);
	codeVS.pCode = vs_list;
	codePS.codeSize = static_cast<uint32_t>(ps_size);
	codePS.pCode = ps_list;
	info.SetShaderCodePtr(nn::gfx::ShaderStage_Vertex, &codeVS);
	info.SetShaderCodePtr(nn::gfx::ShaderStage_Pixel, &codePS);
	info.SetSourceFormat(nn::gfx::ShaderSourceFormat_Glsl);
	info.SetCodeType(nn::gfx::ShaderCodeType_Source);
	
	nn::gfx::ShaderInitializeResult result = program->VSPS.Initialize(&g_Device, info);
	NN_ASSERT(result == nn::gfx::ShaderInitializeResult_Success);
	NN_UNUSED(result);

#define OFFSETOF(T, E)     ((size_t)&(((T*)0)->E))

	nn::gfx::VertexState::InfoType vsInfo;
	vsInfo.SetDefault();
	ptrdiff_t stride = sizeof(Vertex);
	nn::gfx::VertexAttributeStateInfo attribs[3];
	{
		attribs[0].SetDefault();
		attribs[0].SetNamePtr("a_position");
		attribs[0].SetBufferIndex(0);
#if defined(PGXP)
		attribs[0].SetFormat(nn::gfx::AttributeFormat_32_32_32_32_Float);
#else
		attribs[0].SetFormat(nn::gfx::AttributeFormat_16_16_16_16_SintToFloat);
#endif
		attribs[0].SetOffset(OFFSETOF(Vertex, x));

		int slotPosition = program->VSPS.GetInterfaceSlot(nn::gfx::ShaderStage_Vertex, nn::gfx::ShaderInterfaceType_Input, "a_position");
		if (slotPosition >= 0)
		{
			attribs[0].SetShaderSlot(slotPosition);
		}
	}
	{
		attribs[1].SetDefault();
		attribs[1].SetNamePtr("a_texcoord");
		attribs[1].SetBufferIndex(0);
		attribs[1].SetFormat(nn::gfx::AttributeFormat_8_8_8_8_UintToFloat);
		attribs[1].SetOffset(OFFSETOF(Vertex, u));
		int slotTexcoord = program->VSPS.GetInterfaceSlot(nn::gfx::ShaderStage_Vertex, nn::gfx::ShaderInterfaceType_Input, "a_texcoord");
		if (slotTexcoord >= 0)
		{
			attribs[1].SetShaderSlot(slotTexcoord);
		}
	}
	{
		attribs[2].SetDefault();
		attribs[2].SetNamePtr("a_color");
		attribs[2].SetBufferIndex(0);
		attribs[2].SetFormat(nn::gfx::AttributeFormat_8_8_8_8_Unorm);
		attribs[2].SetOffset(OFFSETOF(Vertex, r));
		int slotColor = program->VSPS.GetInterfaceSlot(nn::gfx::ShaderStage_Vertex, nn::gfx::ShaderInterfaceType_Input, "a_color");
		if (slotColor >= 0)
		{
			attribs[2].SetShaderSlot(slotColor);
		}
	}
	nn::gfx::VertexBufferStateInfo buffer;
	{
		buffer.SetDefault();
		buffer.SetStride(stride);
	}
	vsInfo.SetVertexAttributeStateInfoArray(attribs, 3);
	vsInfo.SetVertexBufferStateInfoArray(&buffer, 1);
	size_t size = nn::gfx::VertexState::GetRequiredMemorySize(vsInfo);
	g_pMemory.AlignUp(nn::gfx::VertexState::RequiredMemoryInfo_Alignment);
	program->vertexState.SetMemory(g_pMemory.Get(), size);
	g_pMemory.Advance(size);
	program->vertexState.Initialize(&g_Device, vsInfo, NULL);

	int slotVRAMTexture = program->VSPS.GetInterfaceSlot(nn::gfx::ShaderStage_Pixel, nn::gfx::ShaderInterfaceType_Sampler, "s_texture");
	int slotLUTTexture = program->VSPS.GetInterfaceSlot(nn::gfx::ShaderStage_Pixel, nn::gfx::ShaderInterfaceType_Sampler, "s_lut");

	if (slotVRAMTexture >= 0)
	{
		program->textureBindings[SLOT_VRAM] = slotVRAMTexture;
	}

	if (slotLUTTexture >= 0)
	{
		program->textureBindings[SLOT_LUT] = slotLUTTexture;
	}

	return program;
}

void Emulator_DestroyVertexBuffer()
{
	dynamic_vertex_buffer = 0;
	dynamic_vertex_array = 0;
}

void Emulator_ResetDevice()
{
	return;
	if (!g_resettingDevice)
	{
		g_resettingDevice = TRUE;

		Emulator_DestroyVertexBuffer();

		Emulator_DestroyTextures();

		Emulator_DestroyGlobalShaders();

		Emulator_CreateGlobalShaders();

		Emulator_GenerateCommonTextures();

		Emulator_CreateVertexBuffer();

		g_resettingDevice = FALSE;
	}
}

void Emulator_DestroyTextures()
{
	//glDeleteTextures(1, &vramTexture);
	//glDeleteTextures(1, &rg8lutTexture);
	//glDeleteTextures(1, &whiteTexture);

	//vramTexture = 0;
	//rg8lutTexture = 0;
	//whiteTexture = 0;
}

void Emulator_DestroyGlobalShaders()
{
	//glDeleteProgram(g_gte_shader_4);
	//glDeleteProgram(g_gte_shader_8);
	//glDeleteProgram(g_gte_shader_16);
	//glDeleteProgram(g_blit_shader);

	//g_gte_shader_4 = 0;
	//g_gte_shader_8 = 0;
	//g_gte_shader_16 = 0;
	//g_blit_shader = 0;
}

void* ReadResource(const char* filename)
{
	nn::Result result;
	nn::fs::FileHandle hFile;

	int64_t fileSize = 0;
	result = nn::fs::OpenFile(&hFile, filename, nn::fs::OpenMode_Read);
	NN_ASSERT(result.IsSuccess());

	result = nn::fs::GetFileSize(&fileSize, hFile);
	NN_ASSERT(result.IsSuccess());

	nn::util::BinaryFileHeader fileHeader;
	size_t readSize;
	result = nn::fs::ReadFile(&readSize, hFile, 0, &fileHeader, sizeof(nn::util::BinaryFileHeader));
	NN_ASSERT(result.IsSuccess());
	NN_ASSERT(readSize == sizeof(nn::util::BinaryFileHeader));
	size_t alignment = fileHeader.GetAlignment();

	g_pMemory.AlignUp(alignment);
	void* pBuffer = g_pMemory.Get();
	result = nn::fs::ReadFile(&readSize, hFile, 0, pBuffer, static_cast<size_t>(fileSize));
	NN_ASSERT(result.IsSuccess());
	NN_ASSERT(readSize == static_cast<size_t>(fileSize));
	g_pMemory.Advance(static_cast<ptrdiff_t>(fileSize));

	nn::fs::CloseFile(hFile);

	return pBuffer;
}

void Emulator_InitialiseLayer()
{
	nn::Result result = nn::vi::OpenDefaultDisplay(&g_display);
	NN_ASSERT(result.IsSuccess());
	NN_UNUSED(result);

	result = nn::vi::GetDisplayResolution(&windowWidth, &windowHeight, g_display);
	NN_ASSERT(result.IsSuccess());

	result = nn::vi::CreateLayer(&g_layer, g_display);
	NN_ASSERT(result.IsSuccess());

	result = nn::vi::SetLayerScalingMode(g_layer, nn::vi::ScalingMode_FitToLayer);
	NN_ASSERT(result.IsSuccess());
}

void Emulator_InitialiseDevice()
{
	nn::gfx::Device::InfoType info;
	info.SetDefault();
	info.SetApiVersion(nn::gfx::ApiMajorVersion, nn::gfx::ApiMinorVersion);
	g_Device.Initialize(info);
}

void Emulator_InitialiseMemoryPool()
{
	const int MemoryPoolProperty[MemoryPoolType_End] =
	{
		nn::gfx::MemoryPoolProperty_CpuCached | nn::gfx::MemoryPoolProperty_GpuCached,
		nn::gfx::MemoryPoolProperty_CpuUncached | nn::gfx::MemoryPoolProperty_GpuCached,
		nn::gfx::MemoryPoolProperty_CpuInvisible | nn::gfx::MemoryPoolProperty_GpuCached
			| nn::gfx::MemoryPoolProperty_Compressible
	};
	nn::gfx::MemoryPool::InfoType info;
	for (int idxMemoryPool = 0; idxMemoryPool < MemoryPoolType_End; ++idxMemoryPool)
	{
		info.SetDefault();
		info.SetMemoryPoolProperty(MemoryPoolProperty[idxMemoryPool]);
		size_t alignment = nn::gfx::MemoryPool::GetPoolMemoryAlignment(&g_Device, info);
		size_t granularity = nn::gfx::MemoryPool::GetPoolMemorySizeGranularity(&g_Device, info);
		g_pPoolMemory[idxMemoryPool] = malloc(MemoryPoolSize[idxMemoryPool]);
		void* pPoolMemoryAligned = nn::util::BytePtr(
			g_pPoolMemory[idxMemoryPool]).AlignUp(alignment).Get();
		size_t memoryPoolSizeAligned = nn::util::align_down(MemoryPoolSize[idxMemoryPool], granularity);
		info.SetPoolMemory(pPoolMemoryAligned, memoryPoolSizeAligned);
		g_MemoryPool[idxMemoryPool].Initialize(&g_Device, info);
		g_MemoryPoolOffset[idxMemoryPool] = 0;
	}
}

nn::gfx::SwapChain g_SwapChain;
void Emulator_InitialiseSwapChain()
{
	int width;
	int height;
	nn::Result result = nn::vi::GetDisplayResolution(&width, &height, g_display);
	NN_ASSERT(result.IsSuccess());
	NN_UNUSED(result);

	nn::gfx::SwapChain::InfoType info;

	info.SetDefault();
	info.SetLayer(g_layer);
	info.SetWidth(width);
	info.SetHeight(height);
	info.SetFormat(nn::gfx::ImageFormat_R8_G8_B8_A8_Unorm);
	info.SetBufferCount(2);

	if (NN_STATIC_CONDITION(nn::gfx::SwapChain::IsMemoryPoolRequired))
	{
		ptrdiff_t& offset = g_MemoryPoolOffset[MemoryPoolType_CpuInvisible_GpuCached_Compressible];
		size_t size = g_SwapChain.CalculateScanBufferSize(&g_Device, info);
		offset = nn::util::align_up(offset, nn::gfx::SwapChain::GetScanBufferAlignment(&g_Device, info));
		g_SwapChain.Initialize(&g_Device, info, &g_MemoryPool[MemoryPoolType_CpuInvisible_GpuCached_Compressible], offset, size);
		offset += size;
	}
	else
	{
		g_SwapChain.Initialize(&g_Device, info, NULL, 0, 0);
	}
}

void Emulator_InitialiseQueue()
{
	nn::gfx::Queue::InfoType info;
	info.SetDefault();
	info.SetCapability(nn::gfx::QueueCapability_Graphics);
	g_Queue.Initialize(&g_Device, info);
}

void Emulator_InitialiseCommandBuffer()
{
	nn::gfx::CommandBuffer::InfoType info;
	info.SetDefault();
	info.SetQueueCapability(nn::gfx::QueueCapability_Graphics);
	info.SetCommandBufferType(nn::gfx::CommandBufferType_Direct);
	g_CommandBuffer.Initialize(&g_Device, info);
}

// Initialize the rasterizer state
void Emulator_InitialiseRasterizerState()
{
	nn::gfx::RasterizerState::InfoType info;
	info.SetDefault();
	info.SetCullMode(nn::gfx::CullMode_None);
	info.SetPrimitiveTopologyType(nn::gfx::PrimitiveTopologyType_Triangle);
	info.SetScissorEnabled(true);
	info.SetDepthClipEnabled(false);
	g_RasterizerState.Initialize(&g_Device, info);
}

void Emulator_InitialiseDepthStencilState()
{
	nn::gfx::DepthStencilState::InfoType info;
	info.SetDefault();
	info.SetDepthTestEnabled(false);
	info.SetDepthWriteEnabled(false);
	g_DepthStencilState.Initialize(&g_Device, info);
}

void Emulator_InitialiseVertexBuffer()
{
	nn::gfx::Buffer::InfoType info;
	info.SetDefault();
	info.SetSize(sizeof(Vertex) * MAX_NUM_POLY_BUFFER_VERTICES);
	info.SetGpuAccessFlags(nn::gfx::GpuAccess_VertexBuffer);

	if (NN_STATIC_CONDITION(nn::gfx::Buffer::IsMemoryPoolRequired))
	{
		ptrdiff_t& offset = g_MemoryPoolOffset[MemoryPoolType_CpuUncached_GpuCached];
		offset = nn::util::align_up(offset, nn::gfx::Buffer::GetBufferAlignment(&g_Device, info));
		g_VertexBuffer.Initialize(&g_Device, info, &g_MemoryPool[MemoryPoolType_CpuUncached_GpuCached], offset, info.GetSize());
		offset += info.GetSize();
	}
	else
	{
		g_VertexBuffer.Initialize(&g_Device, info, NULL, 0, 0);
	}
}

void Emulator_InitialiseBlendStates()
{
	
	for (int i = 0; i < BM_COUNT; i++)
	{
		nn::gfx::BlendState::InfoType info;
		nn::gfx::BlendTargetStateInfo targetInfo;
		targetInfo.SetDefault();
		info.SetDefault();
		info.SetBlendTargetStateInfoArray(&targetInfo, 1);
		size_t size = nn::gfx::BlendState::GetRequiredMemorySize(info);
		g_pMemory.AlignUp(nn::gfx::BlendState::RequiredMemoryInfo_Alignment);
		g_BlendState[i].SetMemory(g_pMemory.Get(), size);
		g_pMemory.Advance(size);
		switch (i)
		{
		case BM_NONE:
			break;
		case BM_AVERAGE:
			targetInfo.SetBlendEnabled(TRUE);

			targetInfo.SetSourceColorBlendFactor(nn::gfx::BlendFactor_ConstantColor);
			targetInfo.SetDestinationColorBlendFactor(nn::gfx::BlendFactor_ConstantColor);
			targetInfo.SetColorBlendFunction(nn::gfx::BlendFunction_Add);

			targetInfo.SetSourceAlphaBlendFactor(nn::gfx::BlendFactor_ConstantColor);
			targetInfo.SetDestinationAlphaBlendFactor(nn::gfx::BlendFactor_ConstantColor);
			targetInfo.SetAlphaBlendFunction(nn::gfx::BlendFunction_Add);

			info.SetIndependentBlendEnabled(TRUE);
			info.SetBlendConstant(128.0f * (1.0f / 255.0f), 128.0f * (1.0f / 255.0f), 128.0f * (1.0f / 255.0f), 128.0f * (1.0f / 255.0f));
			break;
		case BM_ADD:
			targetInfo.SetBlendEnabled(TRUE);

			targetInfo.SetSourceColorBlendFactor(nn::gfx::BlendFactor_One);
			targetInfo.SetDestinationColorBlendFactor(nn::gfx::BlendFactor_One);
			targetInfo.SetColorBlendFunction(nn::gfx::BlendFunction_Add);

			targetInfo.SetSourceAlphaBlendFactor(nn::gfx::BlendFactor_One);
			targetInfo.SetDestinationAlphaBlendFactor(nn::gfx::BlendFactor_One);
			targetInfo.SetAlphaBlendFunction(nn::gfx::BlendFunction_Add);

			info.SetIndependentBlendEnabled(TRUE);
			info.SetBlendConstant(1.0f, 1.0f, 1.0f, 1.0f);

			break;
		case BM_SUBTRACT:
			targetInfo.SetBlendEnabled(TRUE);

			targetInfo.SetSourceColorBlendFactor(nn::gfx::BlendFactor_One);
			targetInfo.SetDestinationColorBlendFactor(nn::gfx::BlendFactor_One);
			targetInfo.SetColorBlendFunction(nn::gfx::BlendFunction_ReverseSubtract);

			targetInfo.SetSourceAlphaBlendFactor(nn::gfx::BlendFactor_One);
			targetInfo.SetDestinationAlphaBlendFactor(nn::gfx::BlendFactor_One);
			targetInfo.SetAlphaBlendFunction(nn::gfx::BlendFunction_ReverseSubtract);

			info.SetIndependentBlendEnabled(TRUE);
			info.SetBlendConstant(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		case BM_ADD_QUATER_SOURCE:
			targetInfo.SetBlendEnabled(TRUE);

			targetInfo.SetSourceColorBlendFactor(nn::gfx::BlendFactor_ConstantAlpha);
			targetInfo.SetDestinationColorBlendFactor(nn::gfx::BlendFactor_One);
			targetInfo.SetColorBlendFunction(nn::gfx::BlendFunction_Add);

			targetInfo.SetSourceAlphaBlendFactor(nn::gfx::BlendFactor_ConstantAlpha);
			targetInfo.SetDestinationAlphaBlendFactor(nn::gfx::BlendFactor_One);
			targetInfo.SetAlphaBlendFunction(nn::gfx::BlendFunction_Add);

			info.SetIndependentBlendEnabled(TRUE);
			info.SetBlendConstant(64.0f * (1.0f / 255.0f), 64.0f * (1.0f / 255.0f), 64.0f * (1.0f / 255.0f), 64.0f * (1.0f / 255.0f));
			break;
		}

		g_BlendState[i].Initialize(&g_Device, info);
	}
}

void Emulator_InitialiseConstantBuffer()
{
	nn::gfx::Buffer::InfoType info;
	info.SetDefault();
	info.SetSize(sizeof(float) * 16);
	info.SetGpuAccessFlags(nn::gfx::GpuAccess_ConstantBuffer);
	if (NN_STATIC_CONDITION(nn::gfx::Buffer::IsMemoryPoolRequired))
	{
		ptrdiff_t& offset = g_MemoryPoolOffset[MemoryPoolType_CpuUncached_GpuCached];
		offset = nn::util::align_up(offset, nn::gfx::Buffer::GetBufferAlignment(&g_Device, info));
		g_ConstantBuffer.Initialize(&g_Device, info, &g_MemoryPool[MemoryPoolType_CpuUncached_GpuCached], offset, info.GetSize());
		offset += info.GetSize();
	}
	else
	{
		g_ConstantBuffer.Initialize(&g_Device, info, NULL, 0, 0);
	}
}

void Emulator_InitialiseSampler()
{
	nn::gfx::Sampler::InfoType info;
	info.SetDefault();
	info.SetFilterMode(nn::gfx::FilterMode_MinPoint_MagPoint_MipLinear);
	info.SetAddressU(nn::gfx::TextureAddressMode_Repeat);
	info.SetAddressV(nn::gfx::TextureAddressMode_Repeat);
	info.SetAddressW(nn::gfx::TextureAddressMode_Repeat);
	info.SetComparisonFunction(nn::gfx::ComparisonFunction_Always);
	info.SetMinLod(-3.402823466E+38f);
	info.SetMaxLod(3.402823466E+38f);
	info.SetMaxAnisotropy(1);

	g_Sampler.Initialize(&g_Device, info);

	info.SetDefault();
	info.SetFilterMode(nn::gfx::FilterMode_MinPoint_MagPoint_MipLinear);
	info.SetAddressU(nn::gfx::TextureAddressMode_ClampToEdge);
	info.SetAddressV(nn::gfx::TextureAddressMode_ClampToEdge);
	info.SetAddressW(nn::gfx::TextureAddressMode_Repeat);
	info.SetComparisonFunction(nn::gfx::ComparisonFunction_Always);
	info.SetMinLod(-3.402823466E+38f);
	info.SetMaxLod(3.402823466E+38f);
	info.SetMaxAnisotropy(1);
	g_Sampler2.Initialize(&g_Device, info);
}

void Emulator_InitialiseBufferDescriptorPool()
{
	nn::gfx::DescriptorPool::InfoType info;
	info.SetDefault();
	info.SetDescriptorPoolType(nn::gfx::DescriptorPoolType_BufferView);
	info.SetSlotCount(g_BufferDescriptorBaseIndex + 1);
	size_t size = nn::gfx::DescriptorPool::CalculateDescriptorPoolSize(&g_Device, info);
	ptrdiff_t& offset = g_MemoryPoolOffset[MemoryPoolType_CpuUncached_GpuCached];
	offset = nn::util::align_up(offset, nn::gfx::DescriptorPool::GetDescriptorPoolAlignment(&g_Device, info));
	g_BufferDescriptorPool.Initialize(&g_Device, info, &g_MemoryPool[MemoryPoolType_CpuUncached_GpuCached], offset, size);
	offset += size;
}

void Emulator_InitialiseSamplerDescriptorPool()
{
	nn::gfx::DescriptorPool::InfoType info;
	info.SetDefault();
	info.SetDescriptorPoolType(nn::gfx::DescriptorPoolType_Sampler);
	info.SetSlotCount(g_SamplerDescriptorBaseIndex + SLOT_MAX);
	size_t size = nn::gfx::DescriptorPool::CalculateDescriptorPoolSize(&g_Device, info);
	ptrdiff_t& offset = g_MemoryPoolOffset[MemoryPoolType_CpuUncached_GpuCached];
	offset = nn::util::align_up(offset, nn::gfx::DescriptorPool::GetDescriptorPoolAlignment(&g_Device, info));
	g_SamplerDescriptorPool.Initialize(&g_Device, info, &g_MemoryPool[MemoryPoolType_CpuUncached_GpuCached], offset, size);
	offset += size;
}

void Emulator_InitialiseTextureDescriptorPool()
{
	nn::gfx::DescriptorPool::InfoType info;
	info.SetDefault();
	info.SetDescriptorPoolType(nn::gfx::DescriptorPoolType_TextureView);
	info.SetSlotCount(g_TextureDescriptorBaseIndex + SLOT_MAX);
	size_t size = nn::gfx::DescriptorPool::CalculateDescriptorPoolSize(&g_Device, info);
	ptrdiff_t& offset = g_MemoryPoolOffset[MemoryPoolType_CpuUncached_GpuCached];
	offset = nn::util::align_up(offset, nn::gfx::DescriptorPool::GetDescriptorPoolAlignment(&g_Device, info));
	g_TextureDescriptorPool.Initialize(&g_Device, info, &g_MemoryPool[MemoryPoolType_CpuUncached_GpuCached], offset, size);
	offset += size;
}

void Emulator_InitialiseFence()
{
	nn::gfx::Fence::InfoType info;
	info.SetDefault();
	g_Fence.Initialize(&g_Device, info);
}

void Emulator_InitialiseResTextureFile()
{
	void* pResource = ReadResource("Contents:/SampleTexture.bntx");
	g_pResTextureFile = nn::gfx::ResTextureFile::ResCast(pResource);
	g_pResTextureFile->Initialize(&g_Device);
	for (int idxTexture = 0, textureCount = g_pResTextureFile->GetTextureDic()->GetCount();
		idxTexture < textureCount; ++idxTexture)
	{
		g_pResTextureFile->GetResTexture(idxTexture)->Initialize(&g_Device);
	}
}

void FinalizeResTextureFile()
{
	for (int idxTexture = 0, textureCount = g_pResTextureFile->GetTextureDic()->GetCount();
		idxTexture < textureCount; ++idxTexture)
	{
		g_pResTextureFile->GetResTexture(idxTexture)->Finalize(&g_Device);
	}
	g_pResTextureFile->Finalize(&g_Device);
}

void Emulator_InitialiseResShaderFile()
{
	void* pResource = ReadResource("Contents:/SampleShader.bnsh");
	g_pResShaderFile = nn::gfx::ResShaderFile::ResCast(pResource);
	nn::gfx::ResShaderContainer* pContainer = g_pResShaderFile->GetShaderContainer();
	NN_ASSERT_NOT_NULL(pContainer);
	pContainer->Initialize(&g_Device);

	for (int idxVariation = 0, variationCount = pContainer->GetShaderVariationCount();
		idxVariation < variationCount; ++idxVariation)
	{
		nn::gfx::ResShaderProgram* pProgram = pContainer->GetResShaderProgram(idxVariation);
		nn::gfx::ShaderInitializeResult result = pProgram->Initialize(&g_Device);
		NN_ASSERT_EQUAL(result, nn::gfx::ShaderInitializeResult_Success);
		NN_UNUSED(result);
	}

#if NN_GFX_IS_TARGET_NVN
	g_MaxScratchMemory = nn::gfx::NvnGetMaxRecommendedScratchMemorySize(&g_Device, &g_pResShaderFile, 1);
	int scratchMemoryAlignment;
	nvnDeviceGetInteger(g_Device.ToData()->pNvnDevice,
		NVN_DEVICE_INFO_SHADER_SCRATCH_MEMORY_ALIGNMENT, &scratchMemoryAlignment);
	ptrdiff_t& offset = g_MemoryPoolOffset[MemoryPoolType_CpuInvisible_GpuCached_Compressible];
	offset = nn::util::align_up(offset, scratchMemoryAlignment);
	g_ScratchMemoryOffset = offset;
	offset += g_MaxScratchMemory;
#endif
	NN_UNUSED(g_MaxScratchMemory);
	NN_UNUSED(g_ScratchMemoryOffset);
}

void FinalizeResShaderFile()
{
	auto pContainer = g_pResShaderFile->GetShaderContainer();
	NN_ASSERT_NOT_NULL(pContainer);

	for (int idxVariation = 0, variationCount = pContainer->GetShaderVariationCount();
		idxVariation < variationCount; ++idxVariation)
	{
		nn::gfx::ResShaderProgram* pProgram = pContainer->GetResShaderProgram(idxVariation);
		pProgram->Finalize(&g_Device);
	}

	pContainer->Finalize(&g_Device);
}

void Emulator_InitialiseGfxObjects()
{
	g_pMemoryHeap.Reset(malloc(1024 * 1024));
	g_pMemory = g_pMemoryHeap;

	Emulator_InitialiseDevice();

#if NN_GFX_IS_TARGET_NVN
	nn::gfx::Device::DataType& deviceData = nn::gfx::AccessorToData(g_Device);
	nvnDeviceGetInteger(deviceData.pNvnDevice,
		NVN_DEVICE_INFO_RESERVED_TEXTURE_DESCRIPTORS, &g_TextureDescriptorBaseIndex);
	nvnDeviceGetInteger(deviceData.pNvnDevice,
		NVN_DEVICE_INFO_RESERVED_SAMPLER_DESCRIPTORS, &g_SamplerDescriptorBaseIndex);
#endif

	Emulator_InitialiseMemoryPool();

	Emulator_InitialiseSwapChain();
	Emulator_InitialiseQueue();

	Emulator_InitialiseCommandBuffer();

	Emulator_InitialiseRasterizerState();
	Emulator_InitialiseBlendStates();
	Emulator_InitialiseDepthStencilState();

	Emulator_InitialiseVertexBuffer();
	Emulator_InitialiseConstantBuffer();
	Emulator_InitialiseSampler();

	Emulator_InitialiseBufferDescriptorPool();
	Emulator_InitialiseSamplerDescriptorPool();
	Emulator_InitialiseTextureDescriptorPool();

	Emulator_InitialiseFence();

	Emulator_InitialiseResTextureFile();
	Emulator_InitialiseResShaderFile();

	NN_ASSERT(g_pMemoryHeap.Distance(g_pMemory.Get()) < 1024 * 1024);
	for (int idxMemoryPool = 0; idxMemoryPool < MemoryPoolType_End; idxMemoryPool++)
	{
		NN_ASSERT(static_cast<size_t>(g_MemoryPoolOffset[idxMemoryPool])
			< MemoryPoolSize[idxMemoryPool]);
	}
}

void Emulator_SetVertexBuffer()
{
	nn::gfx::GpuAddress gpuAddress;
	g_VertexBuffer.GetGpuAddress(&gpuAddress);
	g_CommandBuffer.SetVertexBuffer(0, gpuAddress, sizeof(Vertex), sizeof(Vertex) * MAX_NUM_POLY_BUFFER_VERTICES);
}

void Emulator_BeginCommandBuffer()
{
	g_CommandBuffer.Reset();
	ptrdiff_t& offset = g_MemoryPoolOffset[MemoryPoolType_CpuUncached_GpuCached];
	offset = nn::util::align_up(offset, nn::gfx::CommandBuffer::GetCommandMemoryAlignment(&g_Device));
	g_CommandBuffer.AddCommandMemory(&g_MemoryPool[MemoryPoolType_CpuUncached_GpuCached], offset + (FRAME_INDEX % 2) * 1024 * 1024, 1024 * 1024);
	g_pMemory.AlignUp(nn::gfx::CommandBuffer::GetControlMemoryAlignment(&g_Device));
	g_CommandBuffer.AddControlMemory(g_pMemory.Get(), 1024);
	g_CommandBuffer.Begin();
	g_commandListRecording = TRUE;

}

void Emulator_EndCommandBuffer()
{
	g_CommandBuffer.End();
	g_commandListRecording = FALSE;
}

void Emulator_BeginPass()
{
	Emulator_BeginCommandBuffer();

	nn::gfx::ColorTargetView* pTarget = g_SwapChain.AcquireNextScanBufferView();
	g_CommandBuffer.SetRenderTargets(1, &pTarget, NULL);

	//g_CommandBuffer.InvalidateMemory(nn::gfx::GpuAccess_Texture | nn::gfx::GpuAccess_IndexBuffer | nn::gfx::GpuAccess_ConstantBuffer | nn::gfx::GpuAccess_VertexBuffer);
	g_CommandBuffer.SetDescriptorPool(&g_BufferDescriptorPool);
	g_CommandBuffer.SetDescriptorPool(&g_TextureDescriptorPool);
	g_CommandBuffer.SetDescriptorPool(&g_SamplerDescriptorPool);

	g_CommandBuffer.SetRasterizerState(&g_RasterizerState);
	g_CommandBuffer.SetDepthStencilState(&g_DepthStencilState);

	g_BufferDescriptorPool.BeginUpdate();
	{
		nn::gfx::GpuAddress gpuAddress;
		g_ConstantBuffer.GetGpuAddress(&gpuAddress);
		g_BufferDescriptorPool.SetBufferView(g_BufferDescriptorBaseIndex, gpuAddress, sizeof(float) * 16);
	}
	g_BufferDescriptorPool.EndUpdate();

	g_SamplerDescriptorPool.BeginUpdate();
	{
		g_SamplerDescriptorPool.SetSampler(g_SamplerDescriptorBaseIndex + SLOT_VRAM, &g_Sampler);
		g_SamplerDescriptorPool.SetSampler(g_SamplerDescriptorBaseIndex + SLOT_LUT, &g_Sampler2);
	}
	g_SamplerDescriptorPool.EndUpdate();
}

void Emulator_EndPass()
{
	Emulator_EndCommandBuffer();

	if (FRAME_INDEX > 0)
	{
		g_Fence.Sync(nn::TimeSpan::FromSeconds(1));
	}

	g_Queue.ExecuteCommand(&g_CommandBuffer, &g_Fence);
}

void FinalizeGfxObjects()
{
	FinalizeResShaderFile();
	FinalizeResTextureFile();

	g_Fence.Finalize(&g_Device);

	g_Sampler.Finalize(&g_Device);
	g_ConstantBuffer.Finalize(&g_Device);
	g_VertexBuffer.Finalize(&g_Device);

	g_RasterizerState.Finalize(&g_Device);
	//g_BlendState.Finalize(&g_Device);
	g_DepthStencilState.Finalize(&g_Device);

	g_BufferDescriptorPool.Finalize(&g_Device);
	g_TextureDescriptorPool.Finalize(&g_Device);
	g_SamplerDescriptorPool.Finalize(&g_Device);

	g_ViewportScissor->Finalize(&g_Device);
	g_CommandBuffer.Finalize(&g_Device);
	g_SwapChain.Finalize(&g_Device);
	g_Queue.Finalize(&g_Device);
	for (int idxMemoryPool = 0; idxMemoryPool < MemoryPoolType_End; ++idxMemoryPool)
	{
		g_MemoryPool[idxMemoryPool].Finalize(&g_Device);
		free(g_pPoolMemory[idxMemoryPool]);
	}
	g_Device.Finalize();

	free(g_pMemoryHeap.Get());
}

int Emulator_InitialiseNNContext(char* windowName)
{
	nn::Result result;

	size_t cacheSize = 0;
	result = nn::fs::QueryMountRomCacheSize(&cacheSize);
	NN_ASSERT(result.IsSuccess());

	void* mountRomCacheBuffer = malloc(cacheSize);
	NN_ASSERT_NOT_NULL(mountRomCacheBuffer);

	result = nn::fs::MountRom("Contents", mountRomCacheBuffer, cacheSize);
	NN_ASSERT(result.IsSuccess());

	nn::vi::Initialize();
	Emulator_InitialiseLayer();

	nn::gfx::Initialize();
	Emulator_InitialiseGfxObjects();

#if 0
	g_BufferDescriptorPool.BeginUpdate();
	{
		nn::gfx::GpuAddress gpuAddress;
		g_ConstantBuffer.GetGpuAddress(&gpuAddress);
		g_BufferDescriptorPool.SetBufferView(g_BufferDescriptorBaseIndex, gpuAddress, sizeof(float) * 4);
	}
	g_BufferDescriptorPool.EndUpdate();

	g_TextureDescriptorPool.BeginUpdate();
	{
		g_TextureDescriptorPool.SetTextureView(g_TextureDescriptorBaseIndex,
			g_pResTextureFile->GetResTexture(0)->GetTextureView());
	}
	g_TextureDescriptorPool.EndUpdate();

	g_SamplerDescriptorPool.BeginUpdate();
	{
		g_SamplerDescriptorPool.SetSampler(g_SamplerDescriptorBaseIndex, &g_Sampler);
	}
	g_SamplerDescriptorPool.EndUpdate();
#endif

	//Destruction

#if 0
	g_Queue.Sync();

	FinalizeGfxObjects();
	nn::gfx::Finalize();

	nn::vi::DestroyLayer(g_layer);
	nn::vi::CloseDisplay(g_display);
	nn::vi::Finalize();

	nn::fs::Unmount("Contents");
	free(mountRomCacheBuffer);
#endif

	return TRUE;
}

void Emulator_CreateGlobalShaders()
{
	g_gte_shader_4 = Shader_Compile("NVN/gte_shader_4_vs.glsl", "NVN/gte_shader_4_ps.glsl");
	g_gte_shader_8 = Shader_Compile("NVN/gte_shader_8_vs.glsl", "NVN/gte_shader_8_ps.glsl");
	g_gte_shader_16 = Shader_Compile("NVN/gte_shader_16_vs.glsl", "NVN/gte_shader_16_ps.glsl");
	g_blit_shader = Shader_Compile("NVN/blit_shader_vs.glsl", "NVN/blit_shader_ps.glsl");
}

void Emulator_CreateTexture(TextureID* texture, int width, int height, char* initialData, nn::gfx::ImageFormat type, int bytesPerPixel)
{
	nn::gfx::Texture::InfoType info;
	info.SetDefault();
	info.SetGpuAccessFlags(nn::gfx::GpuAccess_Texture);
	info.SetWidth(width);
	info.SetHeight(height);
	info.SetImageStorageDimension(nn::gfx::ImageStorageDimension_2d);
	info.SetImageFormat(type);
	info.SetMipCount(1);
	info.SetTileMode(nn::gfx::TileMode_Linear);

	size_t alignment = nn::gfx::Texture::CalculateMipDataAlignment(&g_Device, info);
	size_t size = nn::gfx::Texture::CalculateMipDataSize(&g_Device, info);
	size_t pitch = nn::gfx::Texture::GetRowPitch(&g_Device, info);
	ptrdiff_t& offset = g_MemoryPoolOffset[MemoryPoolType_CpuCached_GpuCached];
	nn::gfx::MemoryPool& memoryPool = g_MemoryPool[MemoryPoolType_CpuCached_GpuCached];
	offset = nn::util::align_up(offset, alignment);

	if (initialData != NULL)
	{
		uint8_t* pixels = nn::util::BytePtr(memoryPool.Map(), offset).Get< uint8_t >();

		for (int i = 0; i < info.GetHeight(); i++)
		{
			memcpy(pixels, initialData, info.GetWidth() * bytesPerPixel);
			pixels += pitch;
			initialData += info.GetWidth() * bytesPerPixel;
		}

		memoryPool.FlushMappedRange(offset, size);
		memoryPool.Unmap();
	}

	texture->texture.Initialize(&g_Device, info, &memoryPool, offset, size);
	texture->offset = offset;
	texture->size = size;
	texture->pitch = pitch;
	offset += size;

	nn::gfx::TextureView::InfoType texViewInfo;
	texViewInfo.SetDefault();
	texViewInfo.SetImageDimension(nn::gfx::ImageDimension_2d);
	texViewInfo.SetImageFormat(type);
	texViewInfo.SetTexturePtr(&texture->texture);
	texViewInfo.SetChannelMapping(nn::gfx::ChannelMapping_Red, nn::gfx::ChannelMapping_Green, nn::gfx::ChannelMapping_Blue, nn::gfx::ChannelMapping_Alpha);
	texture->textureView.Initialize(&g_Device, texViewInfo);
}

unsigned char pixelData[64 * 64 * sizeof(unsigned int)];

void Emulator_GenerateCommonTextures()
{
	whiteTexture = &GET_TEXTURE();
	memset(pixelData, 0xFF, sizeof(pixelData));
	Emulator_CreateTexture(whiteTexture, 64, 64, (char*)&pixelData, nn::gfx::ImageFormat_R8_G8_B8_A8_Unorm, sizeof(unsigned int));

	rg8lutTexture = &GET_TEXTURE();
	Emulator_CreateTexture(rg8lutTexture, LUT_WIDTH, LUT_HEIGHT, (char*)Emulator_GenerateRG8LUT(), nn::gfx::ImageFormat_R8_G8_B8_A8_Unorm, sizeof(unsigned int));

	vramTexture = &GET_TEXTURE();
	Emulator_CreateTexture(vramTexture, VRAM_WIDTH, VRAM_HEIGHT, NULL, nn::gfx::ImageFormat_R8_G8_Unorm, sizeof(unsigned short));

	//TEMP

	//ENDTEMP

	g_TextureDescriptorPool.GetDescriptorSlot(&vramTexture->textureDescriptor, g_TextureDescriptorBaseIndex + SLOT_VRAM);
	g_TextureDescriptorPool.GetDescriptorSlot(&rg8lutTexture->textureDescriptor, g_TextureDescriptorBaseIndex + SLOT_LUT);
	g_TextureDescriptorPool.GetDescriptorSlot(&whiteTexture->textureDescriptor, g_TextureDescriptorBaseIndex + SLOT_DUMMY);

	g_TextureDescriptorPool.BeginUpdate();
	{
		g_TextureDescriptorPool.SetTextureView(g_TextureDescriptorBaseIndex + SLOT_VRAM, &vramTexture->textureView);
		g_TextureDescriptorPool.SetTextureView(g_TextureDescriptorBaseIndex + SLOT_LUT, &rg8lutTexture->textureView);
		g_TextureDescriptorPool.SetTextureView(g_TextureDescriptorBaseIndex + SLOT_DUMMY, &whiteTexture->textureView);
	}
	g_TextureDescriptorPool.EndUpdate();
}

void Emulator_CreateVertexBuffer()///@TODO OGLES
{

}

int Emulator_CreateCommonResources()
{
	memset(vram, 0, VRAM_WIDTH * VRAM_HEIGHT * sizeof(unsigned short));
	
	Emulator_GenerateCommonTextures();
	
	Emulator_CreateGlobalShaders();

	//glDisable(GL_DEPTH_TEST);
	//glBlendColor(0.5f, 0.5f, 0.5f, 0.25f);

	Emulator_CreateVertexBuffer();

	//Emulator_ResetDevice();

	return TRUE;
}

void Emulator_Ortho2D(float left, float right, float bottom, float top, float znear, float zfar)
{
	float a = 2.0f / (right - left);
	float b = 2.0f / (top - bottom);
	float c = 2.0f / (znear - zfar);

	float x = (left + right) / (left - right);
	float y = (bottom + top) / (bottom - top);
	float z = (znear + zfar) / (znear - zfar);

	float ortho[16] = {
		a, 0, 0, 0,
		0, b, 0, 0,
		0, 0, c, 0,
		x, y, z, 1
	};

	float* pConstantBuffer = g_ConstantBuffer.Map<float>();
	memcpy(pConstantBuffer, ortho, sizeof(float) * 16);
	g_ConstantBuffer.Unmap();
}

void Emulator_SetTextureAndShader(TextureID* texture, ShaderID* shader)
{
	if (g_texturelessMode) {
		//texture = whiteTexture;
	}

	if (g_lastBoundTexture[0] == texture && g_lastBoundTexture[1] == rg8lutTexture) {
		//return;
	}

	g_CommandBuffer.SetShader(&shader->VSPS, nn::gfx::ShaderStageBit_Vertex);
	g_CommandBuffer.SetShader(&shader->VSPS, nn::gfx::ShaderStageBit_Pixel);

	g_CommandBuffer.SetVertexState(&shader->vertexState);

	nn::gfx::DescriptorSlot constantBufferDescriptor;
	g_BufferDescriptorPool.GetDescriptorSlot(&constantBufferDescriptor, g_BufferDescriptorBaseIndex);

	shader->u_projection = shader->VSPS.GetInterfaceSlot(nn::gfx::ShaderStage_Vertex, nn::gfx::ShaderInterfaceType_ConstantBuffer, "Mat");
	if (shader->u_projection >= 0)
	{
		Emulator_Ortho2D(0.0f, activeDispEnv.disp.w, activeDispEnv.disp.h, 0.0f, 0.0f, 1.0f);

		g_CommandBuffer.SetConstantBuffer(shader->u_projection, nn::gfx::ShaderStage_Vertex, constantBufferDescriptor);
	}

	nn::gfx::DescriptorSlot samplerDescriptor;
	nn::gfx::DescriptorSlot samplerDescriptor2;
	g_SamplerDescriptorPool.GetDescriptorSlot(&samplerDescriptor, g_SamplerDescriptorBaseIndex + SLOT_VRAM);
	g_SamplerDescriptorPool.GetDescriptorSlot(&samplerDescriptor2, g_SamplerDescriptorBaseIndex + SLOT_LUT);

	g_CommandBuffer.SetTextureStateTransition(&texture->texture, NULL, nn::gfx::TextureState_DataTransfer, 0, nn::gfx::TextureState_ShaderRead, nn::gfx::ShaderStageBit_Pixel);
	g_CommandBuffer.SetTextureStateTransition(&rg8lutTexture->texture, NULL, nn::gfx::TextureState_DataTransfer, 0, nn::gfx::TextureState_ShaderRead, nn::gfx::ShaderStageBit_Pixel);

	g_CommandBuffer.SetTextureAndSampler(shader->textureBindings[SLOT_VRAM], nn::gfx::ShaderStage_Pixel, texture->textureDescriptor, samplerDescriptor);
	g_CommandBuffer.SetTextureAndSampler(shader->textureBindings[SLOT_LUT], nn::gfx::ShaderStage_Pixel, rg8lutTexture->textureDescriptor, samplerDescriptor2);

	g_lastBoundTexture[0] = texture;
	g_lastBoundTexture[1] = rg8lutTexture;
}

void Emulator_DestroyTexture(TextureID texture)
{
	//glDeleteTextures(1, &texture);
}

void Emulator_Clear(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b)
{
	if (g_commandListRecording)
	{
		nn::gfx::ColorTargetView* pTarget = g_SwapChain.AcquireNextScanBufferView();
		g_CommandBuffer.ClearColor(pTarget, r / 255.0f, g / 255.0f, b / 255.0f, 1.0f, NULL);
	}
}

#define NOFILE 0

#if !defined(__EMSCRIPTEN__) && !defined(__ANDROID__)

void Emulator_SaveVRAM(const char* outputFileName, int x, int y, int width, int height, int bReadFromFrameBuffer)
{
#if NOFILE
	return;
#endif
	FILE* f = fopen(outputFileName, "wb");
	
	if (f == NULL)
	{
		return;
	}

	unsigned char TGAheader[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };
	unsigned char header[6];
	header[0] = (width % 256);
	header[1] = (width / 256);
	header[2] = (height % 256);
	header[3] = (height / 256);
	header[4] = 16;
	header[5] = 0;

	fwrite(TGAheader, sizeof(unsigned char), 12, f);
	fwrite(header, sizeof(unsigned char), 6, f);

	//512 const is hdd sector size
	int numSectorsToWrite = (width * height * sizeof(unsigned short)) / 512;
	int numRemainingSectorsToWrite = (width * height * sizeof(unsigned short)) % 512;

	for (int i = 0; i < numSectorsToWrite; i++)
	{
		fwrite(&vram[i * 512 / sizeof(unsigned short)], 512, 1, f);
	}

	for (int i = 0; i < numRemainingSectorsToWrite; i++)
	{
		fwrite(&vram[numSectorsToWrite * 512 / sizeof(unsigned short)], numRemainingSectorsToWrite, 1, f);
	}

	fclose(f);
}
#endif

void Emulator_StoreFrameBuffer(int x, int y, int w, int h)
{
	short* fb = NULL;// (short*)malloc(w * h * sizeof(short));

	int* data = NULL;// (int*)malloc(w * h * sizeof(int));
	//glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);

#define FLIP_Y (h - fy - 1)
#define SWAP_RB

	unsigned int* data_src = (unsigned int*)data;
	unsigned short* data_dst = (unsigned short*)fb;

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			unsigned int  c = *data_src++;
			unsigned char b = ((c >> 3) & 0x1F);
			unsigned char g = ((c >> 11) & 0x1F);
			unsigned char r = ((c >> 19) & 0x1F);
#if defined(SWAP_RB)
			*data_dst++ = b | (g << 5) | (r << 10) | 0x8000;
#else
			* data_dst++ = r | (g << 5) | (b << 10) | 0x8000;
#endif
		}
	}

	short* ptr = (short*)vram + VRAM_WIDTH * y + x;

	for (int fy = 0; fy < h; fy++) {
		short* fb_ptr = fb + (h * FLIP_Y / h) * w;

		for (int fx = 0; fx < w; fx++) {
			ptr[fx] = fb_ptr[w * fx / w];
		}

		ptr += VRAM_WIDTH;
	}

	//free(data);

#undef FLIP_Y

	//free(fb);

	vram_need_update = TRUE;
}

void Emulator_UpdateVRAM()
{
	if (!vram_need_update) {
		return;
	}
	vram_need_update = FALSE;

	vramTexture->texture.Finalize(&g_Device);
	vramTexture->textureView.Finalize(&g_Device);
	ptrdiff_t& offset = g_MemoryPoolOffset[MemoryPoolType_CpuCached_GpuCached];
	offset -= vramTexture->size;

	Emulator_CreateTexture(vramTexture, VRAM_WIDTH, VRAM_HEIGHT, (char*)&vram[0], nn::gfx::ImageFormat_R8_G8_Unorm, sizeof(short));
	
	g_TextureDescriptorPool.GetDescriptorSlot(&vramTexture->textureDescriptor, g_TextureDescriptorBaseIndex + SLOT_VRAM);

	g_TextureDescriptorPool.BeginUpdate();
	{
		g_TextureDescriptorPool.SetTextureView(g_TextureDescriptorBaseIndex + SLOT_VRAM, &vramTexture->textureView);
	}
	g_TextureDescriptorPool.EndUpdate();
}

void Emulator_SetWireframe(int enable)
{
//	glPolygonMode(GL_FRONT_AND_BACK, enable ? GL_LINE : GL_FILL);
}

void Emulator_SetBlendMode(BlendMode blendMode)
{
	if (g_PreviousBlendMode == blendMode)
	{
		return;
	}

	g_CommandBuffer.SetBlendState(&g_BlendState[blendMode]);

	g_PreviousBlendMode = blendMode;
}
void Emulator_DrawTriangles(int start_vertex, int triangles)
{
	if (triangles <= 0)
		return;

	g_CommandBuffer.Draw(nn::gfx::PrimitiveTopology_TriangleList, triangles * 3, dynamic_vertex_buffer_index);

	dynamic_vertex_buffer_index += triangles * 3;
}

void Emulator_UpdateVertexBuffer(const Vertex* vertices, int num_vertices)
{
	eassert(num_vertices <= MAX_NUM_POLY_BUFFER_VERTICES);

	if (num_vertices <= 0)
		return;

	char* pMapped = (char*)g_VertexBuffer.Map();
	memcpy(pMapped + dynamic_vertex_buffer_index * sizeof(Vertex), vertices, num_vertices * sizeof(Vertex));
	g_VertexBuffer.Unmap();

	vbo_was_dirty_flag = TRUE;
}

void Emulator_SetViewPort(int x, int y, int width, int height)
{
	short offset_x = activeDispEnv.screen.x;
	short offset_y = activeDispEnv.screen.y;

	if (g_ViewportScissor != NULL)
	{
		g_ViewportScissor->Finalize(&g_Device);
		g_ViewportScissor = NULL;
	}

	if (g_ViewportScissor == NULL)
	{
		g_ViewportScissor = new nn::gfx::ViewportScissorState();
	}

	nn::gfx::ViewportScissorState::InfoType info;
	info.SetDefault();

	nn::gfx::ViewportStateInfo viewportInfo;
	{
		viewportInfo.SetDefault();
		viewportInfo.SetOriginX(x + offset_x);
		viewportInfo.SetOriginY(y + -offset_y);
		viewportInfo.SetWidth(static_cast<float>(width));
		viewportInfo.SetHeight(static_cast<float>(height));
	}

	info.SetViewportStateInfoArray(&viewportInfo, 1);
	
	g_ViewportScissor->Initialize(&g_Device, info);

	if (g_ViewportScissor != NULL)
	{
		g_CommandBuffer.SetViewportScissorState(g_ViewportScissor);
	}
}

void Emulator_SwapWindow()
{
	unsigned int timer = 1;

#if defined(SINGLE_THREADED)
	Emulator_CounterWrapper(0, &timer);
#endif

	Emulator_WaitForTimestep(1);
	g_Queue.Present(&g_SwapChain, 1);
	g_Queue.Flush();
	FRAME_INDEX++;
}

void Emulator_WaitForTimestep(int count)
{
	
}

void Emulator_SetRenderTarget(const RenderTargetID& frameBufferObject)
{
	//glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
}

#endif