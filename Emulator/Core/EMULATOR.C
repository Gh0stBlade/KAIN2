#include "EMULATOR.H"

#include "EMULATOR_VERSION.H"
#include "EMULATOR_GLOBALS.H"
#include "Public/EMULATOR_PUBLIC.H"
#include "Debug/CRASHHANDLER.H"
#include "Debug/BOUNTY_LIST.H"
#include "Core/Input/EMULATOR_INPUT.H"
#include "Core/Render/EMULATOR_RENDER_COMMON.H"

#include "LIBGPU.H"
#include "LIBGTE.H"
#include "LIBETC.H"
#include "LIBPAD.H"
#if !defined(__ANDROID__) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12)
#include <thread>
#endif
#include <assert.h>
#include <stdlib.h>

int g_useHintedTouchUIFont = FALSE;

#if defined(SDL2) || (defined(OGLES) && defined(_WINDOWS))
extern SDL_Window* g_window;
#endif

extern int splitAgain;

#if defined(NTSC_VERSION)
#define COUNTER_UPDATE_INTERVAL (263)
#else
#define COUNTER_UPDATE_INTERVAL (313)
#endif

#include <stdio.h>
#include <string.h>

struct TouchButtonVRAM touchButtonsUI[10];

#if defined(D3D11)


#if defined(UWP) && !defined(UWP_SDL2)

using namespace Windows::Gaming::Input;

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Platform;

CoreWindow^ g_window;
std::thread g_uiThread;
char* g_windowName;
int g_windowReady = 0;

ref class App sealed : public IFrameworkView
{
	bool WindowClosed;
	unsigned int kbInputs;

public:
	virtual void Initialize(CoreApplicationView^ AppView)
	{
		AppView->Activated += ref new TypedEventHandler
			<CoreApplicationView^, IActivatedEventArgs^>(this, &App::OnActivated);
		CoreApplication::Suspending +=
			ref new EventHandler<SuspendingEventArgs^>(this, &App::Suspending);
		CoreApplication::Resuming +=
			ref new EventHandler<Object^>(this, &App::Resuming);
		
		WindowClosed = false;    // initialize to false
	}
	virtual void SetWindow(CoreWindow^ Window)
	{
		Window->Closed += ref new TypedEventHandler
			<CoreWindow^, CoreWindowEventArgs^>(this, &App::Closed);

		Window->KeyDown +=
			ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &App::OnKeyPressed);

		Window->KeyUp +=
			ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &App::OnKeyReleased);
	
		kbInputs = 0xFFFF;
	}

	void App::OnKeyPressed(CoreWindow^ sender, KeyEventArgs^ args)
	{
		if (args->VirtualKey == Windows::System::VirtualKey::X)//Square
		{
			kbInputs &= ~0x8000;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::V)//Circle
		{
			kbInputs &= ~0x2000;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::Z)//Triangle
		{
			kbInputs &= ~0x1000;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::C)//Cross
		{
			kbInputs &= ~0x4000;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::LeftShift)//L1
		{
			kbInputs &= ~0x400;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::RightShift)//R1
		{
			kbInputs &= ~0x800;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::Up)//UP
		{
			kbInputs &= ~0x10;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::Down)//DOWN
		{
			kbInputs &= ~0x40;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::Left)//LEFT
		{
			kbInputs &= ~0x80;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::Right)//RIGHT
		{
			kbInputs &= ~0x20;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::LeftControl)//L2
		{
			kbInputs &= ~0x100;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::RightControl)//R2
		{
			kbInputs &= ~0x200;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::Space)//SELECT
		{
			kbInputs &= ~0x1;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::Enter)//START
		{
			kbInputs &= ~0x8;
		}

		padData[0][0] = 0;
		padData[0][1] = 0x41;
		((unsigned short*)padData[0])[1] = kbInputs;
	}

	void App::OnKeyReleased(CoreWindow^ sender, KeyEventArgs^ args)
	{
		if (args->VirtualKey == Windows::System::VirtualKey::X)//Square
		{
			kbInputs |= 0x8000;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::V)//Circle
		{
			kbInputs |= 0x2000;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::Z)//Triangle
		{
			kbInputs |= 0x1000;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::C)//Cross
		{
			kbInputs |= 0x4000;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::LeftShift)//L1
		{
			kbInputs |= 0x400;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::RightShift)//R1
		{
			kbInputs |= 0x800;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::Up)//UP
		{
			kbInputs |= 0x10;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::Down)//DOWN
		{
			kbInputs |= 0x40;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::Left)//LEFT
		{
			kbInputs |= 0x80;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::Right)//RIGHT
		{
			kbInputs |= 0x20;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::LeftControl)//L2
		{
			kbInputs |= 0x100;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::RightControl)//R2
		{
			kbInputs |= 0x200;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::Space)//SELECT
		{
			kbInputs |= 0x1;
		}

		if (args->VirtualKey == Windows::System::VirtualKey::Enter)//START
		{
			kbInputs |= 0x8;
		}

		padData[0][0] = 0;
		padData[0][1] = 0x41;
		((unsigned short*)padData[0])[1] = kbInputs;
	}

	virtual void Load(String^ EntryPoint) {}
	virtual void Run()
	{
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();

		g_window = Window;
		g_windowReady = 1;

		while (!WindowClosed)
		{
			Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
		}
	}
	virtual void Uninitialize() {}

	void OnActivated(CoreApplicationView^ CoreAppView, IActivatedEventArgs^ Args)
	{
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();
		Window->Activate();
	}

	void Closed(CoreWindow^ sender, CoreWindowEventArgs^ args)
	{
		WindowClosed = true;    // time to end the endless loop
	}

	void Suspending(Object^ Sender, SuspendingEventArgs^ Args) {}
	void Resuming(Object^ Sender, Object^ Args) {}
};


// the class definition that creates our core framework
ref class AppSource sealed : IFrameworkViewSource
{
public:
	virtual IFrameworkView^ CreateView()
	{
		return ref new App();    // create the framework view and return it
	}
};

[MTAThread]    // define main() as a multi-threaded-apartment function

void CreateUWPApplication()
{
	CoreApplication::Run(ref new AppSource());
}

#endif

#endif

#if defined(OGLES)

#elif defined(D3D11)

#elif defined(D3D12)


#elif defined(VULKAN)
	VkBuffer dynamic_vertex_buffer;
	VkDeviceMemory dynamic_vertex_buffer_memory;
	VkDeviceSize dynamic_vertex_buffer_index;

#if defined(_DEBUG)///@TODO sort me out! not all of that is for debug!

	std::vector<const char*> g_validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	std::vector<const char*> g_availableExtensions = {
		"VK_EXT_debug_report"
	};
	VkInstance g_vkInstance;
	
	VkDebugReportCallbackEXT g_debugCallback;

	VkSurfaceKHR g_surface;

	VkPhysicalDevice physical_devices;

	unsigned int graphics_QueueFamilyIndex;
	unsigned int present_QueueFamilyIndex;

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapchain;

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	VkSurfaceFormatKHR surfaceFormat;
	VkExtent2D swapchainSize;

	std::vector<VkImage> swapchainImages;
	uint32_t swapchainImageCount;
	std::vector<VkImageView> swapchainImageViews;

	VkRenderPass render_pass;
	std::vector<VkFramebuffer> swapchainFramebuffers;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

#define VERTEX_BIT (0)
#define FRAGMENT_BIT (1)

	ShaderID g_activeShader;
	TextureID g_activeTexture;

	VkPipelineShaderStageCreateInfo g_shaderStages[2];
	std::array<VkVertexInputAttributeDescription, 3> g_attributeDescriptions;
	std::array<VkVertexInputAttributeDescription, 3> g_attributeDescriptionsBlit;
	VkVertexInputBindingDescription g_bindingDescription;
	VkVertexInputBindingDescription g_bindingDescriptionBlit;

	VkViewport g_viewport;
	VkPipelineRasterizationStateCreateInfo g_rasterizer;
	
	unsigned int frameIndex;
	unsigned int currentFrame = 0;
	const int MAX_FRAMES_IN_FLIGHT = 2;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	VkDescriptorPool descriptorPool[2];
	VkDescriptorSetLayout descriptorSetLayout[2];
	std::vector<VkDescriptorSet> descriptorSets[2];
	int g_activeDescriptor = 0;

	VkPipelineColorBlendAttachmentState g_colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo g_colorBlend;

	VkPipeline g_graphicsPipeline;
	unsigned int g_vertexBufferMemoryBound = FALSE;
	unsigned int imageIndex = 0;
	bool begin_pass_flag = FALSE;
	bool begin_commands_flag = FALSE;
#endif
#endif

int g_otSize = 0;
char* pVirtualMemory = NULL;
SysCounter counters[3] = { 0 };
#if !defined(__ANDROID__) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX) && !defined(PLATFORM_NX_ARM) && !defined(D3D12)
std::thread counter_thread;
#endif
#if defined(__ANDROID__)
#define malloc SDL_malloc
#define free SDL_free
#endif

int g_texturelessMode = 0;
int g_emulatorPaused = 0;

#if !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12)  
void Emulator_ResetDevice()
{
#if defined(EGL) || defined(OGLES)///@TODO &&
	extern EGLDisplay eglDisplay;
	eglSwapInterval(eglDisplay, g_swapInterval);
#elif defined(D3D9) || defined(XED3D)
	if (dynamic_vertex_buffer) {
		dynamic_vertex_buffer->Release();
		dynamic_vertex_buffer = NULL;
	}

	d3dpp.PresentationInterval = g_swapInterval ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.BackBufferWidth = windowWidth;
	d3dpp.BackBufferHeight = windowHeight;
	HRESULT hr = d3ddev->Reset(&d3dpp);
	assert(!FAILED(hr));
#if defined(D3D9)
	hr = d3ddev->CreateVertexBuffer(sizeof(Vertex) * MAX_NUM_POLY_BUFFER_VERTICES, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, &dynamic_vertex_buffer, NULL);
#elif defined(XED3D)
	hr = d3ddev->CreateVertexBuffer(sizeof(Vertex) * MAX_NUM_POLY_BUFFER_VERTICES, D3DUSAGE_WRITEONLY, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, &dynamic_vertex_buffer, NULL);
#endif
	assert(!FAILED(hr));

	d3ddev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	d3ddev->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	d3ddev->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	d3ddev->SetSamplerState(1, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);

#elif defined(D3D11)
	if (dynamic_vertex_buffer) {
		dynamic_vertex_buffer->Release();
		dynamic_vertex_buffer = NULL;
	}

	Emulator_DestroyGlobalShaders();
	Emulator_DestroyConstantBuffers();
	swapChain->Release();
	d3ddev->Release();
	d3dcontext->Release();
	renderTargetView->Release();
	vramTexture->Release();
	vramBaseTexture->Release();
	whiteTexture->Release();
	
	if (rg8lutTexture != NULL)
	{
		rg8lutTexture->Release();
	}

	if (samplerState != NULL)
	{
		samplerState->Release();
	}
	
	if (rg8lutSamplerState != NULL)
	{
		rg8lutSamplerState->Release();
	}

	swapChain = NULL;
	d3ddev = NULL;
	d3dcontext = NULL;
	renderTargetView = NULL;
	vramTexture = NULL;
	vramBaseTexture = NULL;
	rg8lutTexture = NULL;
	samplerState = NULL;
	rg8lutSamplerState = NULL;

#if defined(SDL2)
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(g_window, &wmInfo);
#endif

	DXGI_MODE_DESC bd;
	ZeroMemory(&bd, sizeof(DXGI_MODE_DESC));

	bd.Width = windowWidth;
	bd.Height = windowHeight;
	bd.RefreshRate.Numerator = 60;
	bd.RefreshRate.Denominator = 1;
	bd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

#if defined(UWP)
	DXGI_SWAP_CHAIN_DESC1 sd;
#else
	DXGI_SWAP_CHAIN_DESC sd;
#endif
	memset(&sd, 0, sizeof(sd));
#if !defined(UWP)
	sd.Windowed = TRUE;
	sd.BufferDesc = bd;
#else
	sd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.Width = windowWidth;
	sd.Height = windowHeight;
#endif
#if !defined(UWP)
	sd.BufferCount = 1;
#else
	sd.BufferCount = 2;
#endif
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
#if !defined(UWP)
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
#else
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
#endif

#if defined(SDL2) && !defined(UWP_SDL2)
	sd.OutputWindow = wmInfo.info.win.window;
#endif

#if defined(_DEBUG)
	unsigned int deviceCreationFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
	unsigned int deviceCreationFlags = 0;
#endif

#if defined(UWP)
	HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceCreationFlags, NULL, 0, D3D11_SDK_VERSION, &d3ddev, NULL, &d3dcontext);
	assert(!FAILED(hr));

	IDXGIDevice3* dxgiDevice = NULL;
	IDXGIAdapter* dxgiAdapter = NULL;
	IDXGIFactory2* dxgiFactory = NULL;

	hr = d3ddev->QueryInterface(__uuidof(IDXGIDevice3), (void**)&dxgiDevice);

	assert(!FAILED(hr));

	hr = dxgiDevice->GetAdapter(&dxgiAdapter);

	assert(!FAILED(hr));

	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory);

	assert(!FAILED(hr));

#if defined(UWP) && defined(SDL2)
	hr = dxgiFactory->CreateSwapChainForCoreWindow(d3ddev, reinterpret_cast<IUnknown*>(wmInfo.info.winrt.window), &sd, NULL, &swapChain);
#else
	hr = dxgiFactory->CreateSwapChainForComposition(d3ddev, &sd, NULL, &swapChain);
#endif

	///@FIXME Crash-UWP likely something not being free'd resulting in window not being free.
	assert(!FAILED(hr));
#else
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceCreationFlags, NULL, 0, D3D11_SDK_VERSION, &sd, &swapChain, &d3ddev, NULL, &d3dcontext);
	assert(!FAILED(hr));
#endif
	ID3D11Texture2D* backBuffer;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	assert(!FAILED(hr));

	hr = d3ddev->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
	assert(!FAILED(hr));

	backBuffer->Release();

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex) * MAX_NUM_POLY_BUFFER_VERTICES;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	hr = d3ddev->CreateBuffer(&vbd, NULL, &dynamic_vertex_buffer);
	assert(!FAILED(hr));

	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.ArraySize = 1;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	td.Format = DXGI_FORMAT_R8G8_UNORM;
	td.Width = VRAM_WIDTH;
	td.Height = VRAM_HEIGHT;
	td.MipLevels = 1;
	td.MiscFlags = 0;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DYNAMIC;
	hr = d3ddev->CreateTexture2D(&td, NULL, &vramBaseTexture);
	assert(!FAILED(hr));
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = 1;

	d3ddev->CreateShaderResourceView(vramBaseTexture, &srvd, &vramTexture);
	assert(!FAILED(hr));

	Emulator_CreateGlobalShaders();
	Emulator_CreateConstantBuffers();
	Emulator_GenerateCommonTextures();

	vram_need_update = TRUE;
	UINT offset = 0;
	UINT stride = sizeof(Vertex);
	d3dcontext->IASetVertexBuffers(0, 1, &dynamic_vertex_buffer, &stride, &offset);
	d3dcontext->OMSetRenderTargets(1, &renderTargetView, NULL);
	d3dcontext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Emulator_CreateRasterState(FALSE);
#elif defined(D3D12)

	//for (int i = frameIndex ^ 1; i < frameCount; ++i)
	{
		//Emulator_WaitForPreviousFrame();
	}

	if (d3ddev != NULL)
	{
		d3ddev->Release();
		d3ddev = NULL;
	}

	if (swapChain != NULL)
	{
		swapChain->Release();
		swapChain = NULL;
	}

	if (commandQueue != NULL)
	{
		commandQueue->Release();
		commandQueue = NULL;
	}

	if (renderTargetDescriptorHeap != NULL)
	{
		renderTargetDescriptorHeap->Release();
		renderTargetDescriptorHeap = NULL;
	}

	if (commandList != NULL)
	{
		commandList->Release();
		commandList = NULL;
	}

	fence->Release();
	commandAllocator->Release();

	for (int i = 0; i < frameCount; i++)
	{
		renderTargets[i]->Release();
		projectionMatrixBufferHeap[i]->Release();
		projectionMatrixBuffer[i]->Release();
		fenceValue[i] = 0;
	}

	for (int i = 0; i < frameCount; i++)
	{
		if (dynamic_vertex_buffer[i] = NULL)
		{
			dynamic_vertex_buffer[i]->Release();
			dynamic_vertex_buffer[i] = NULL;
		}

		if (dynamic_vertex_buffer_heap[i] = NULL)
		{
			dynamic_vertex_buffer_heap[i]->Release();
			dynamic_vertex_buffer_heap[i] = NULL;
		}
	}

	Emulator_DestroyGlobalShaders();

	if (vramBaseTexture != NULL)
	{
		vramBaseTexture->Release();
		vramBaseTexture = NULL;
	}

	if (rg8lutTexture.m_textureResource != NULL)
	{
		rg8lutTexture.m_textureResource->Release();
		rg8lutTexture.m_textureResource = NULL;
	}

	if (rg8lutTexture.m_srvHeap != NULL)
	{
		rg8lutTexture.m_srvHeap->Release();
		rg8lutTexture.m_srvHeap = NULL;
	}

	if (rg8lutTexture.m_uploadHeap != NULL)
	{
		rg8lutTexture.m_uploadHeap->Release();
		rg8lutTexture.m_uploadHeap = NULL;
	}

	if (whiteTexture.m_textureResource != NULL)
	{
		whiteTexture.m_textureResource->Release();
		whiteTexture.m_textureResource = NULL;
	}

	if (whiteTexture.m_srvHeap != NULL)
	{
		whiteTexture.m_srvHeap->Release();
		whiteTexture.m_srvHeap = NULL;
	}

	if (whiteTexture.m_uploadHeap != NULL)
	{
		whiteTexture.m_uploadHeap->Release();
		whiteTexture.m_uploadHeap = NULL;
	}

	if (vramTexture.m_textureResource != NULL)
	{
		vramTexture.m_textureResource->Release();
		vramTexture.m_textureResource = NULL;
	}

	if (vramTexture.m_srvHeap != NULL)
	{
		vramTexture.m_srvHeap->Release();
		vramTexture.m_srvHeap = NULL;
	}

	if (vramTexture.m_uploadHeap != NULL)
	{
		vramTexture.m_uploadHeap->Release();
		vramTexture.m_uploadHeap = NULL;
	}

	CloseHandle(fenceEvent);

	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(g_window, &wmInfo);

	IDXGIFactory2* factory = NULL;
	HRESULT hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory));

	if (!SUCCEEDED(hr)) {
		eprinterr("Failed to create DXGI factory2!\n");
		return;
	}

	int adapterIndex = 0;
	int adapterFound = FALSE;
	IDXGIAdapter1* adapter = NULL;
	while (factory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 adapterDesc;
		adapter->GetDesc1(&adapterDesc);

		if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), NULL);

		if (SUCCEEDED(hr))
		{
			adapterFound = TRUE;
			break;
		}

		adapterIndex++;
	}

	if (!adapterFound) {
		eprinterr("Failed to locate D3D12 compatible adapter!\n");
		return;
	}

	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}

	hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&d3ddev));
	if (!SUCCEEDED(hr)) {
		eprinterr("Failed to create D3D12 device!\n");
		return;
	}

	hr = d3ddev->GetDeviceRemovedReason();

	D3D12_COMMAND_QUEUE_DESC commandQDesc;
	ZeroMemory(&commandQDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	commandQDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	hr = d3ddev->CreateCommandQueue(&commandQDesc, IID_PPV_ARGS(&commandQueue));
	if (!SUCCEEDED(hr)) {
		eprinterr("Failed to create D3D12 command queue!\n");
		return;
	}

	DXGI_SAMPLE_DESC sampleDesc;
	ZeroMemory(&sampleDesc, sizeof(DXGI_SAMPLE_DESC));
	sampleDesc.Count = 1;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC1));
	swapChainDesc.BufferCount = frameCount;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc = sampleDesc;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Width = windowWidth;
	swapChainDesc.Height = windowHeight;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFSDesc;
	ZeroMemory(&swapChainFSDesc, sizeof(swapChainFSDesc));
	swapChainFSDesc.Windowed = TRUE;

	IDXGISwapChain1* tempSwapChain;

	hr = factory->CreateSwapChainForHwnd(commandQueue, wmInfo.info.win.window, &swapChainDesc, &swapChainFSDesc, NULL, &tempSwapChain);

	if (!SUCCEEDED(hr)) {
		eprinterr("Failed to create swap chain!\n");
		return;
	}

	swapChain = (IDXGISwapChain3*)tempSwapChain;
	frameIndex = swapChain->GetCurrentBackBufferIndex();

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	ZeroMemory(&heapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	heapDesc.NumDescriptors = frameCount;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	hr = d3ddev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&renderTargetDescriptorHeap));
	if (!SUCCEEDED(hr)) {
		eprinterr("Failed to create RTV Descripter heap!\n");
		return;
	}

	renderTargetDescriptorSize = d3ddev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(renderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	for (int i = 0; i < frameCount; i++)
	{
		hr = swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));
		if (FAILED(hr))
		{
			return;
		}

		d3ddev->CreateRenderTargetView(renderTargets[i], NULL, rtvHandle);

		rtvHandle.Offset(1, renderTargetDescriptorSize);
	}

	hr = d3ddev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	if (FAILED(hr))
	{
		return;
	}

	hr = d3ddev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, NULL, IID_PPV_ARGS(&commandList));
	if (FAILED(hr))
	{
		return;
	}

	commandList->Close();

	hr = d3ddev->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if (FAILED(hr))
	{
		return;
	}

	for (int i = 0; i < frameCount; i++)
	{
		fenceValue[i] = 0;
	}

	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (fenceEvent == NULL)
	{
		eprinterr("Failed to create fence event!\n");
		return;
	}

	for (int i = 0; i < frameCount; i++)
	{
		hr = d3ddev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(MAX_NUM_POLY_BUFFER_VERTICES * sizeof(Vertex)), D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&dynamic_vertex_buffer[i]));
		assert(SUCCEEDED(hr));

		hr = d3ddev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(MAX_NUM_POLY_BUFFER_VERTICES * sizeof(Vertex)), D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&dynamic_vertex_buffer_heap[i]));
		assert(SUCCEEDED(hr));
	}

	D3D12_RESOURCE_DESC td;

	ZeroMemory(&td, sizeof(td));
	td.Width = VRAM_WIDTH;
	td.Height = VRAM_HEIGHT;
	td.MipLevels = td.DepthOrArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8_UNORM;
	td.SampleDesc.Count = 1;
	td.Flags = D3D12_RESOURCE_FLAG_NONE;
	td.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	hr = d3ddev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &td, D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(&vramTexture.m_textureResource));
	assert(!FAILED(hr));

	hr = d3ddev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &td, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&vramTexture.m_uploadHeap));
	assert(!FAILED(hr));

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc;
	ZeroMemory(&srvHeapDesc, sizeof(srvHeapDesc));
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	hr = d3ddev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&vramTexture.m_srvHeap));
	assert(SUCCEEDED(hr));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = td.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	d3ddev->CreateShaderResourceView(vramTexture.m_textureResource, &srvDesc, vramTexture.m_srvHeap->GetCPUDescriptorHandleForHeapStart());


	Emulator_CreateConstantBuffers();
	Emulator_CreateGlobalShaders();
	Emulator_GenerateCommonTextures();

	D3D12_RESOURCE_DESC textureResourceDesc;
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	textureResourceDesc.Alignment = 0;
	textureResourceDesc.Width = VRAM_WIDTH * VRAM_HEIGHT * sizeof(short);
	textureResourceDesc.Height = 1;
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	textureResourceDesc.SampleDesc.Count = 1;
	textureResourceDesc.SampleDesc.Quality = 0;
	textureResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	textureResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	hr = d3ddev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &textureResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&vramBaseTexture));
	assert(!FAILED(hr));

	ZeroMemory(&textureResourceDesc, sizeof(textureResourceDesc));
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	textureResourceDesc.Alignment = 0;
	textureResourceDesc.Width = MAX_NUM_POLY_BUFFER_VERTICES * sizeof(Vertex);
	textureResourceDesc.Height = 1;
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	textureResourceDesc.SampleDesc.Count = 1;
	textureResourceDesc.SampleDesc.Quality = 0;
	textureResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	textureResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	for (int i = 0; i < frameCount; i++)
	{
		hr = d3ddev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(MAX_NUM_POLY_BUFFER_VERTICES * sizeof(Vertex)), D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&dynamic_vertex_buffer[i]));
		assert(SUCCEEDED(hr));

		hr = d3ddev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(MAX_NUM_POLY_BUFFER_VERTICES * sizeof(Vertex)), D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&dynamic_vertex_buffer_heap[i]));
		assert(SUCCEEDED(hr));
	}

	if (begin_pass_flag)
	{
		begin_commands_flag = FALSE;
		begin_pass_flag = FALSE;
		Emulator_BeginPass();
	}
	else if (begin_commands_flag)
	{
		begin_commands_flag = FALSE;
		Emulator_BeginCommandBuffer();
	}
#elif defined(VULKAN)

	if (dynamic_vertex_buffer) {
		vkDestroyBuffer(device, dynamic_vertex_buffer, NULL);
		dynamic_vertex_buffer = VK_NULL_HANDLE;
	}

	enum ShaderID::ShaderType lastShaderBound = g_activeShader.T;

	Emulator_DestroySyncObjects();
	Emulator_DestroyGlobalShaders();
	Emulator_DestroyConstantBuffers();
	Emulator_DestroyDescriptorSetLayout();
	Emulator_DestroyDescriptorPool();
	Emulator_DestroyVulkanCommandBuffers();
	Emulator_DestroyVulkanCommandPool();
	Emulator_DestroyVulkanFrameBuffers();
	Emulator_DestroyVulkanRenderPass();
	Emulator_DestroyVulkanImageViews();

	vkDestroySwapchainKHR(device, swapchain, NULL);
	swapchain = VK_NULL_HANDLE;
	swapchainImages.clear();

	vkDestroyImageView(device, vramTexture.textureImageView, NULL);
	vkDestroyImage(device, vramTexture.textureImage, NULL);
	vkFreeMemory(device, vramTexture.textureImageMemory, NULL);
	vkDestroyBuffer(device, vramTexture.stagingBuffer, NULL);
	vkFreeMemory(device, vramTexture.stagingBufferMemory, NULL);

	vkDestroyImageView(device, whiteTexture.textureImageView, NULL);
	vkDestroyImage(device, whiteTexture.textureImage, NULL);
	vkFreeMemory(device, whiteTexture.textureImageMemory, NULL);
	vkDestroyBuffer(device, whiteTexture.stagingBuffer, NULL);
	vkFreeMemory(device, whiteTexture.stagingBufferMemory, NULL);

	vkDestroyImageView(device, rg8lutTexture.textureImageView, NULL);
	vkDestroyImage(device, rg8lutTexture.textureImage, NULL);
	vkFreeMemory(device, rg8lutTexture.textureImageMemory, NULL);
	vkDestroyBuffer(device, rg8lutTexture.stagingBuffer, NULL);
	vkFreeMemory(device, rg8lutTexture.stagingBufferMemory, NULL);

	vkDestroyDevice(device, NULL);
	device = VK_NULL_HANDLE;

	Emulator_SelectVulkanPhysicalDevice();
	Emulator_SelectVulkanQueueFamily();

	if (Emulator_CreateVulkanDevice() != VK_SUCCESS)
	{
		eprinterr("Failed to create device (vkCreateDevice)!\n");
		assert(FALSE);
		return;
	}

	if (Emulator_CreateVulkanSwapChain() != TRUE)
	{
		eprinterr("Failed to create vulkan swap chain!\n");
		assert(FALSE);
		return;
	}

	Emulator_CreateVulkanImageViews();
	Emulator_CreateVulkanRenderPass();
	Emulator_CreateVulkanFrameBuffers();
	Emulator_CreateVulkanCommandPool();
	Emulator_CreateVulkanCommandBuffers();
	Emulator_CreateSyncObjects();
	Emulator_CreateUniformBuffers();
	Emulator_CreateDescriptorPool();//TOFREE
	Emulator_CreateDescriptorSetLayout();//TOFREE

	unsigned int pixelData = 0xFFFFFFFF;
	int texWidth = 1;
	int texHeight = 1;
	VkDeviceSize imageSize = texWidth * texHeight * sizeof(unsigned int);

	Emulator_CreateVulkanBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, whiteTexture.stagingBuffer, whiteTexture.stagingBufferMemory);

	void* data = NULL;
	vkMapMemory(device, whiteTexture.stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, &pixelData, static_cast<size_t>(imageSize));
	vkUnmapMemory(device, whiteTexture.stagingBufferMemory);

	Emulator_CreateImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, whiteTexture.textureImage, whiteTexture.textureImageMemory);
	whiteTexture.textureImageView = Emulator_CreateImageView(whiteTexture.textureImage, VK_FORMAT_R8G8B8A8_UNORM);

	Emulator_TransitionImageLayout(whiteTexture.textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	Emulator_CopyBufferToImage(whiteTexture.stagingBuffer, whiteTexture.textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
	Emulator_TransitionImageLayout(whiteTexture.textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	texWidth = VRAM_WIDTH;
	texHeight = VRAM_HEIGHT;
	imageSize = texWidth * texHeight * sizeof(unsigned short);

	Emulator_CreateVulkanBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vramTexture.stagingBuffer, vramTexture.stagingBufferMemory);
	Emulator_CreateImage(texWidth, texHeight, VK_FORMAT_R8G8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vramTexture.textureImage, vramTexture.textureImageMemory);
	vramTexture.textureImageView = Emulator_CreateImageView(vramTexture.textureImage, VK_FORMAT_R8G8_UNORM);

	Emulator_TransitionImageLayout(vramTexture.textureImage, VK_FORMAT_R8G8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	Emulator_TransitionImageLayout(vramTexture.textureImage, VK_FORMAT_R8G8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	imageSize = LUT_WIDTH * LUT_HEIGHT * sizeof(unsigned int);

	Emulator_CreateVulkanBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, rg8lutTexture.stagingBuffer, rg8lutTexture.stagingBufferMemory);

	data = NULL;
	vkMapMemory(device, rg8lutTexture.stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, Emulator_GenerateRG8LUT(), static_cast<size_t>(imageSize));
	vkUnmapMemory(device, rg8lutTexture.stagingBufferMemory);

	Emulator_CreateImage(LUT_WIDTH, LUT_HEIGHT, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, rg8lutTexture.textureImage, rg8lutTexture.textureImageMemory);
	rg8lutTexture.textureImageView = Emulator_CreateImageView(rg8lutTexture.textureImage, VK_FORMAT_R8G8B8A8_UNORM);

	Emulator_TransitionImageLayout(rg8lutTexture.textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	Emulator_CopyBufferToImage(rg8lutTexture.stagingBuffer, rg8lutTexture.textureImage, static_cast<uint32_t>(LUT_WIDTH), static_cast<uint32_t>(LUT_HEIGHT));
	Emulator_TransitionImageLayout(rg8lutTexture.textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);


	Emulator_CreateGlobalShaders();

	//VTX buffer
	VkBufferCreateInfo bufferInfo;
	memset(&bufferInfo, 0, sizeof(VkBufferCreateInfo));

	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(Vertex) * MAX_NUM_POLY_BUFFER_VERTICES;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, NULL, &dynamic_vertex_buffer) != VK_SUCCESS)
	{
		eprinterr("Failed to create vertex buffer!\n");
		assert(FALSE);
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, dynamic_vertex_buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = Emulator_FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	if (vkAllocateMemory(device, &allocInfo, NULL, &dynamic_vertex_buffer_memory) != VK_SUCCESS)
	{
		eprinterr("Failed to allocate vertex buffer!\n");
		assert(FALSE);
	}

	currentFrame = 0;

	g_vertexBufferMemoryBound = FALSE;

	if (begin_pass_flag)
	{
		begin_commands_flag = FALSE;
		begin_pass_flag = FALSE;
		Emulator_BeginPass();
	}
	else if (begin_commands_flag)
	{
		begin_commands_flag = FALSE;
		Emulator_BeginCommandBuffer();
	}

#if 0

	Emulator_BeginPass();

	switch (lastShaderBound)
	{
	case ShaderID::ShaderType::gte_4:
		Emulator_SetShader(g_gte_shader_4);
		break;
	case ShaderID::ShaderType::gte_8:
		Emulator_SetShader(g_gte_shader_8);
		break;
	case ShaderID::ShaderType::gte_16:
		Emulator_SetShader(g_gte_shader_16);
		break;
	case ShaderID::ShaderType::blit:
		Emulator_SetShader(g_blit_shader);
		break;
	}

#endif
#endif
}

#if defined(D3D9) || defined(XED3D)
int Emulator_InitialiseD3D9Context(char* windowName)
{
#if defined(SDL2)
	g_window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
	if (g_window == NULL)
	{
		eprinterr("Failed to initialise SDL window!\n");
		return FALSE;
	}

	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(g_window, &wmInfo);
#endif
	memset(&d3dpp, 0, sizeof(d3dpp));
#if defined(D3D9)
	d3dpp.Windowed               = TRUE;
#elif defined(XED3D)
	d3dpp.Windowed               = FALSE;
#endif
	d3dpp.BackBufferCount        = 1;
	d3dpp.BackBufferFormat       = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferWidth        = windowWidth;
	d3dpp.BackBufferHeight       = windowHeight;
	d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
#if defined(D3D9)
	d3dpp.hDeviceWindow          = wmInfo.info.win.window;
#endif
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_ONE;

	d3d =  Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d) {
		eprinterr("Failed to initialise D3D\n");
		return FALSE;
	}

#if defined(D3D9)
	HRESULT hr = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
#elif defined(XED3D)
	HRESULT hr = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
#endif
	
	if (FAILED(hr)) {
		eprinterr("Failed to obtain D3D9 device!\n");
		return FALSE;
	}

	return TRUE;
}
#endif
#endif

#if defined(D3D11) && 0
static int Emulator_InitialiseD3D11Context(char* windowName)
{
#if defined(SDL2)
	g_window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
	if (g_window == NULL)
	{
		eprinterr("Failed to initialise SDL window!\n");
		return FALSE;
	}

	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(g_window, &wmInfo);
#endif

	DXGI_MODE_DESC bd;
	ZeroMemory(&bd, sizeof(DXGI_MODE_DESC));

	bd.Width = windowWidth;
	bd.Height = windowHeight;
	bd.RefreshRate.Numerator = 60;
	bd.RefreshRate.Denominator = 1;
	bd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

#if defined(UWP)
	DXGI_SWAP_CHAIN_DESC1 sd;
#else
	DXGI_SWAP_CHAIN_DESC sd;
#endif
	memset(&sd, 0, sizeof(sd));
#if !defined(UWP)
	sd.Windowed = TRUE;
	sd.BufferDesc = bd;
#else
	sd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.Width = windowWidth;
	sd.Height = windowHeight;
#endif
#if !defined(UWP)
	sd.BufferCount = 1;
#else
	sd.BufferCount = 2;
#endif
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
#if !defined(UWP)
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
#else
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
#endif
#if defined(SDL2) && !defined(UWP_SDL2)
	sd.OutputWindow = wmInfo.info.win.window;
#endif

#if defined(_DEBUG)
	unsigned int deviceCreationFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
	unsigned int deviceCreationFlags = 0;
#endif

#if defined(UWP)

	HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceCreationFlags, NULL, 0, D3D11_SDK_VERSION, &d3ddev, NULL, &d3dcontext);
	
	if (!SUCCEEDED(hr)) {
		eprinterr("Failed to initialise D3D Device\n");
		return FALSE;
	}

	IDXGIDevice3* dxgiDevice = NULL;
	IDXGIAdapter* dxgiAdapter = NULL;
	IDXGIFactory2* dxgiFactory = NULL;

	hr = d3ddev->QueryInterface(__uuidof(IDXGIDevice3), (void**)&dxgiDevice);

	if (!SUCCEEDED(hr)) {
		eprinterr("Failed to query interface of dxgiDevice\n");
		return FALSE;
	}

	hr = dxgiDevice->GetAdapter(&dxgiAdapter);

	if (!SUCCEEDED(hr)) {
		eprinterr("Failed to get adapter\n");
		return FALSE;
	}

	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory);

	if (!SUCCEEDED(hr)) {
		eprinterr("Failed to get factory\n");
		return FALSE;
	}

#if defined(UWP) && defined(SDL2)
	hr = dxgiFactory->CreateSwapChainForCoreWindow(d3ddev, reinterpret_cast<IUnknown*>(wmInfo.info.winrt.window), &sd, NULL, &swapChain);
#else
	hr = dxgiFactory->CreateSwapChainForComposition(d3ddev, &sd, NULL, &swapChain);
#endif

	if (!SUCCEEDED(hr)) {
		eprinterr("Failed to create swapchain\n");
		return FALSE;
	}
#else
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceCreationFlags, NULL, 0, D3D11_SDK_VERSION, &sd, &swapChain, &d3ddev, NULL, &d3dcontext);

	if (!SUCCEEDED(hr)) {
		eprinterr("Failed to initialise D3D\n");
		return FALSE;
	}
#endif

	ID3D11Texture2D* backBuffer;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (!SUCCEEDED(hr)) {
		eprinterr("Failed to get back buffer!\n");
		return FALSE;
	}

	hr = d3ddev->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
	if (!SUCCEEDED(hr)) {
		eprinterr("Failed to create render target view!\n");
		return FALSE;
	}

	backBuffer->Release();

	return TRUE;
}
#endif

#if defined(D3D12)

#endif

#if defined(VULKAN)
#if defined(_DEBUG)
static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanReportFunc(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData)
{
	printf("VULKAN VALIDATION: %s\n", msg);
	return VK_FALSE;
}

PFN_vkGetInstanceProcAddr SDL2_vkGetInstanceProcAddr = VK_NULL_HANDLE;
PFN_vkCreateDebugReportCallbackEXT SDL2_vkCreateDebugReportCallbackEXT = VK_NULL_HANDLE;

void Emulator_CreateVulkanDebugLayer()
{
	SDL2_vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)SDL_Vulkan_GetVkGetInstanceProcAddr();
	SDL2_vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(g_vkInstance, "vkCreateDebugReportCallbackEXT");

	VkDebugReportCallbackCreateInfoEXT debugCallbackCreateInfo;
	memset(&debugCallbackCreateInfo, 0, sizeof(VkDebugReportCallbackCreateInfoEXT));
	debugCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	debugCallbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	debugCallbackCreateInfo.pfnCallback = VulkanReportFunc;

	SDL2_vkCreateDebugReportCallbackEXT(g_vkInstance, &debugCallbackCreateInfo, NULL, &g_debugCallback);
}
#endif

int Emulator_CreateVulkanSurface()
{
	return SDL_Vulkan_CreateSurface(g_window, g_vkInstance, &g_surface);
}

int Emulator_CreateVulkanInstance(char* windowName)
{
	unsigned int numExtensions = 0;
	SDL_Vulkan_GetInstanceExtensions(g_window, &numExtensions, NULL);

	unsigned int additionalExtensionsCount = g_availableExtensions.size();
	numExtensions += additionalExtensionsCount;
	g_availableExtensions.resize(numExtensions);

	SDL_Vulkan_GetInstanceExtensions(g_window, &numExtensions, g_availableExtensions.data() + additionalExtensionsCount);

	VkApplicationInfo appInfo;
	memset(&appInfo, 0, sizeof(VkApplicationInfo));

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = windowName;
	appInfo.apiVersion = VK_API_VERSION_1_3;
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.pEngineName = EMULATOR_NAME;


	VkInstanceCreateInfo createInfo;
	memset(&createInfo, 0, sizeof(VkInstanceCreateInfo));

	//unsigned int layerCount;
	//vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	//g_validationLayers.resize(layerCount);
	//vkEnumerateInstanceLayerProperties(&layerCount, g_validationLayers.data());

	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledLayerCount = g_validationLayers.size();
	createInfo.ppEnabledLayerNames = g_validationLayers.data();
	createInfo.enabledExtensionCount = g_availableExtensions.size();
	createInfo.ppEnabledExtensionNames = g_availableExtensions.data();

	return vkCreateInstance(&createInfo, NULL, &g_vkInstance);
}

void Emulator_SelectVulkanPhysicalDevice()
{
	std::vector<VkPhysicalDevice> physicalDevices;
	unsigned int physicalDeviceCount = 0;

	vkEnumeratePhysicalDevices(g_vkInstance, &physicalDeviceCount, NULL);
	physicalDevices.resize(physicalDeviceCount);
	vkEnumeratePhysicalDevices(g_vkInstance, &physicalDeviceCount, physicalDevices.data());

	physical_devices = physicalDevices[0];
}

void Emulator_SelectVulkanQueueFamily()
{
	std::vector<VkQueueFamilyProperties> queueFamilyProperties;
	unsigned int queueFamilyCount;

	vkGetPhysicalDeviceQueueFamilyProperties(physical_devices, &queueFamilyCount, NULL);
	queueFamilyProperties.resize(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physical_devices, &queueFamilyCount, queueFamilyProperties.data());

	int graphicIndex = -1;
	int presentIndex = -1;

	int i = 0;
	for (const auto& queueFamily : queueFamilyProperties)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			graphicIndex = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physical_devices, i, g_surface, &presentSupport);
		
		if (queueFamily.queueCount > 0 && presentSupport)
		{
			presentIndex = i;
		}

		if (graphicIndex != -1 && presentIndex != -1)
		{
			break;
		}

		i++;
	}

	graphics_QueueFamilyIndex = graphicIndex;
	present_QueueFamilyIndex = presentIndex;
}

int Emulator_CreateVulkanDevice()
{
	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	const float queue_priority[] = { 1.0f };

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { graphics_QueueFamilyIndex, present_QueueFamilyIndex };

	float queuePriority = queue_priority[0];
	for (int queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = graphics_QueueFamilyIndex;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = queueCreateInfos.size();
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = deviceExtensions.size();
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	createInfo.enabledLayerCount = validationLayers.size();
	createInfo.ppEnabledLayerNames = validationLayers.data();

	VkResult result = vkCreateDevice(physical_devices, &createInfo, NULL, &device);

	vkGetDeviceQueue(device, graphics_QueueFamilyIndex, 0, &graphicsQueue);
	vkGetDeviceQueue(device, present_QueueFamilyIndex, 0, &presentQueue);

	return result;
}

int Emulator_CreateVulkanSwapChain()
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_devices, g_surface, &surfaceCapabilities);

	std::vector<VkSurfaceFormatKHR> surfaceFormats;
	uint32_t surfaceFormatsCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physical_devices, g_surface,	&surfaceFormatsCount, NULL);
	surfaceFormats.resize(surfaceFormatsCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physical_devices, g_surface, &surfaceFormatsCount, surfaceFormats.data());

	if (surfaceFormats[0].format != VK_FORMAT_B8G8R8A8_UNORM)
	{
		return FALSE;
	}

	surfaceFormat = surfaceFormats[0];
	swapchainSize.width = windowWidth;
	swapchainSize.height = windowHeight;

	unsigned int imageCount = surfaceCapabilities.minImageCount + 1;
	if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount)
	{
		imageCount = surfaceCapabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = g_surface;
	createInfo.minImageCount = surfaceCapabilities.minImageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = swapchainSize;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	
	unsigned int queueFamilyIndices[] = { graphics_QueueFamilyIndex, present_QueueFamilyIndex };
	if (graphics_QueueFamilyIndex != present_QueueFamilyIndex)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = surfaceCapabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
	createInfo.clipped = VK_TRUE;

	vkCreateSwapchainKHR(device, &createInfo, NULL, &swapchain);

	vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, NULL);
	swapchainImages.resize(swapchainImageCount);
	vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, swapchainImages.data());

	return TRUE;
}

VkImageView Emulator_CreateVulkanImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
	{
		eprinterr("Failed to create image view!\n");
	}

	return imageView;
}


void Emulator_CreateVulkanImageViews()
{
	swapchainImageViews.resize(swapchainImages.size());

	for (unsigned int i = 0; i < swapchainImages.size(); i++)
	{
		swapchainImageViews[i] = Emulator_CreateVulkanImageView(swapchainImages[i], surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);
	}
}

void Emulator_DestroyVulkanImageViews()
{
	for (unsigned int i = 0; i < swapchainImages.size(); i++)
	{
		vkDestroyImageView(device, swapchainImageViews[i], NULL);
	}

	swapchainImageViews.clear();
}

unsigned int Emulator_FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physical_devices, &memProperties);

	for (unsigned int i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	eprinterr("Failed to find memory type!\n");
	return 0;//?
}

VkBool32 Emulator_GetSupportedDepthFormat(VkPhysicalDevice physicalDevice, VkFormat* depthFormat)
{
	std::vector<VkFormat> depthFormats = {
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM
	};

	for (auto& format : depthFormats)
	{
		VkFormatProperties formatProps;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProps);
		if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
		{
			*depthFormat = format;
			return true;
		}
	}

	return false;
}


void Emulator_CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
	{
		eprinterr("Failed to create image!\n");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = Emulator_FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) 
	{
		eprinterr("Failed to allocate memory!\n");
	}

	vkBindImageMemory(device, image, imageMemory, 0);
}

void Emulator_CreateVulkanRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = surfaceFormat.format;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &render_pass) != VK_SUCCESS) 
	{
		eprinterr("Failed to create render pass\n");
		assert(FALSE);
	}
}

void Emulator_DestroyVulkanRenderPass()
{
	vkDestroyRenderPass(device, render_pass, NULL);
	render_pass = VK_NULL_HANDLE;
}

void Emulator_DestroyVulkanFrameBuffers()
{
	for (unsigned int i = 0; i < swapchainImageViews.size(); i++)
	{
		vkDestroyFramebuffer(device, swapchainFramebuffers[i], NULL);
	}
}

void Emulator_CreateVulkanFrameBuffers()
{
	swapchainFramebuffers.resize(swapchainImageViews.size());

	for (unsigned int i = 0; i < swapchainImageViews.size(); i++)
	{
		std::vector<VkImageView> attachments(1);
		attachments[0] = swapchainImageViews[i];
		//attachments[1] = depthImageView;

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = render_pass;
		framebufferInfo.attachmentCount = static_cast<unsigned int>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapchainSize.width;
		framebufferInfo.height = swapchainSize.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, NULL, &swapchainFramebuffers[i]) != VK_SUCCESS)
		{
			eprinterr("Failed to create vulkan frame buffer!\n");
		}
	}
}

void Emulator_CreateVulkanCommandPool()
{
	VkCommandPoolCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
	createInfo.queueFamilyIndex = graphics_QueueFamilyIndex;
	vkCreateCommandPool(device, &createInfo, nullptr, &commandPool);
}

void Emulator_CreateVulkanCommandBuffers()
{
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		eprinterr("Failed to allocate command buffers!\n");
		assert(FALSE);
	}
}

void Emulator_CreateDescriptorPool()
{
	for (int i = 0; i < 2; i++)
	{
		std::array<VkDescriptorPoolSize, 4> poolSizes{};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		poolSizes[2].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		poolSizes[2].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		poolSizes[3].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		poolSizes[3].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

		if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool[i]) != VK_SUCCESS)
		{
			eprinterr("Failed to create descriptor pool!\n");
		}
	}
}

void Emulator_DestroyDescriptorPool()
{
	for (int i = 0; i < 2; i++)
	{
		vkDestroyDescriptorPool(device, descriptorPool[i], NULL);
		descriptorPool[i] = VK_NULL_HANDLE;
	}
}

void Emulator_CreateDescriptorSetLayout() 
{
	for (int i = 0; i < 2; i++)
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.pImmutableSamplers = NULL;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = 2;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = NULL;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		VkDescriptorSetLayoutBinding textureLayoutBinding{};
		textureLayoutBinding.binding = 4;
		textureLayoutBinding.descriptorCount = 1;
		textureLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		textureLayoutBinding.pImmutableSamplers = NULL;
		textureLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		VkDescriptorSetLayoutBinding textureLayoutBinding2{};
		textureLayoutBinding2.binding = 5;
		textureLayoutBinding2.descriptorCount = 1;
		textureLayoutBinding2.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		textureLayoutBinding2.pImmutableSamplers = NULL;
		textureLayoutBinding2.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 4> bindings = { uboLayoutBinding, samplerLayoutBinding, textureLayoutBinding, textureLayoutBinding2 };
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout[i]) != VK_SUCCESS)
		{
			eprinterr("Failed to create descriptor set layout!");
			assert(FALSE);
		}
	}
}

void Emulator_CreateUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(float) * 16;

	uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
		Emulator_CreateVulkanBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void Emulator_UpdateDescriptorSets()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			TextureID texture;

			if (j == 0)
			{
				texture = vramTexture;
			}
			else
			{
				texture = whiteTexture;
			}

			VkDescriptorBufferInfo uniformInfo{};
			uniformInfo.buffer = uniformBuffers[i];
			uniformInfo.offset = 0;
			uniformInfo.range = sizeof(float) * 16;

			VkDescriptorImageInfo samplerInfo{};
			samplerInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			samplerInfo.imageView = texture.textureImageView;
			samplerInfo.sampler = g_shaders[0]->SS;

			VkDescriptorImageInfo textureInfo{};
			textureInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			textureInfo.imageView = texture.textureImageView;
			textureInfo.sampler = VK_NULL_HANDLE;

			VkDescriptorImageInfo textureInfo2{};
			textureInfo2.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			textureInfo2.imageView = rg8lutTexture.textureImageView;
			textureInfo2.sampler = VK_NULL_HANDLE;

			std::array<VkWriteDescriptorSet, 4> descriptorWrites{};

			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = descriptorSets[i][j];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &uniformInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = descriptorSets[i][j];
			descriptorWrites[1].dstBinding = 2;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &samplerInfo;

			descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[2].dstSet = descriptorSets[i][j];
			descriptorWrites[2].dstBinding = 4;
			descriptorWrites[2].dstArrayElement = 0;
			descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			descriptorWrites[2].descriptorCount = 1;
			descriptorWrites[2].pImageInfo = &textureInfo;

			descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[3].dstSet = descriptorSets[i][j];
			descriptorWrites[3].dstBinding = 5;
			descriptorWrites[3].dstArrayElement = 0;
			descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			descriptorWrites[3].descriptorCount = 1;
			descriptorWrites[3].pImageInfo = &textureInfo2;

			vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, NULL);
		}
	}
}

void Emulator_CreateDescriptorSets()
{
	for (int i = 0; i < 2; i++)
	{
		std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout[i]);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool[i];
		allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		allocInfo.pSetLayouts = layouts.data();

		descriptorSets[i].resize(MAX_FRAMES_IN_FLIGHT);
		if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets[i].data()) != VK_SUCCESS)
		{
			eprinterr("Failed to allocate descriptor sets!\n");
		}
	}

	Emulator_UpdateDescriptorSets();
}

void Emulator_CreateSyncObjects()
{
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) 
		{
			eprinterr("Failed to create sync objects!\n");
			assert(FALSE);
		}
	}
}

static int Emulator_InitialiseVulkanContext(char* windowName)
{
#if defined(SDL2)
	g_window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
	if (g_window == NULL)
	{
		eprinterr("Failed to initialise SDL window!\n");
		return FALSE;
	}
#endif

	if (Emulator_CreateVulkanInstance(windowName))
	{
		eprinterr("Failed to create instance (vkCreateInstance)!\n");
		return FALSE;
	}

#if defined(_DEBUG)
	Emulator_CreateVulkanDebugLayer();
#endif

	if (Emulator_CreateVulkanSurface() == SDL_FALSE)
	{
		eprinterr("Failed to create vulkan surface!\n");
		return FALSE;
	}

	Emulator_SelectVulkanPhysicalDevice();
	Emulator_SelectVulkanQueueFamily();

	if (Emulator_CreateVulkanDevice() != VK_SUCCESS)
	{
		eprinterr("Failed to create device (vkCreateDevice)!\n");
		return FALSE;
	}

	if (Emulator_CreateVulkanSwapChain() != TRUE)
	{
		eprinterr("Failed to create vulkan swap chain!\n");
		return FALSE;
	}

	Emulator_CreateVulkanImageViews();
	Emulator_CreateVulkanRenderPass();
	Emulator_CreateVulkanFrameBuffers();
	Emulator_CreateVulkanCommandPool();
	Emulator_CreateVulkanCommandBuffers();
	Emulator_CreateSyncObjects();
	Emulator_CreateUniformBuffers();
	Emulator_CreateDescriptorPool();
	Emulator_CreateDescriptorSetLayout();

	return TRUE;
}
#endif

#if defined(OGLES) && 0
EGLint majorVersion = 0, minorVersion = 0;
EGLContext eglContext = NULL;
EGLSurface eglSurface = NULL;
EGLConfig eglConfig = NULL;
EGLDisplay eglDisplay = NULL;
int numConfigs = 0;

const EGLint config16bpp[] =
{
#if OGLES_VERSION == 2
        EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,
#elif OGLES_VERSION == 3
		EGL_RENDERABLE_TYPE,EGL_OPENGL_ES3_BIT,
#endif
		EGL_BUFFER_SIZE,32,
		EGL_RED_SIZE,8,
		EGL_GREEN_SIZE,8,
		EGL_BLUE_SIZE,8,
		EGL_ALPHA_SIZE,8,
		EGL_STENCIL_SIZE,0,
		EGL_DEPTH_SIZE,16,
		EGL_NONE
};

static int Emulator_InitialiseGLESContext(char* windowName)
{
	unsigned int windowFlags = SDL_WINDOW_OPENGL;

#if defined(__ANDROID__)
	windowFlags |= SDL_WINDOW_FULLSCREEN;
#elif defined(__EMSCRIPTEN__)
	windowFlags |= SDL_WINDOW_RESIZABLE;
#endif

    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	g_window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, windowFlags);

	if(g_window == NULL)
    {
	    eprinterr("Failed to create SDL window!\n");
    }

	if (!eglInitialize(eglDisplay, &majorVersion, &minorVersion))
	{
		eprinterr("eglInitialize failure! Error: %x\n", eglGetError());
		return FALSE;
	}

	eglBindAPI(EGL_OPENGL_ES_API);

	if (!eglChooseConfig(eglDisplay, config16bpp, &eglConfig, 1, &numConfigs))
	{
		printf("eglChooseConfig failed\n");
		if (eglContext == 0)
		{
			printf("Error code: %d\n", eglGetError());
		}
	}

	SDL_SysWMinfo systemInfo;
	SDL_VERSION(&systemInfo.version);
	SDL_GetWindowWMInfo(g_window, &systemInfo);
#if defined(__EMSCRIPTEN__)
	EGLNativeWindowType dummyWindow;
	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (EGLNativeWindowType)dummyWindow, NULL);
#elif defined(__ANDROID__)
	eglSurface = systemInfo.info.android.surface;
#elif defined(__WINDOWS__)
	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (EGLNativeWindowType)systemInfo.info.win.window, NULL);
#endif
	if (eglSurface == EGL_NO_SURFACE)
	{
		eprinterr("eglSurface failure! Error: %x\n", eglGetError());
		return FALSE;
	}

	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, OGLES_VERSION, EGL_NONE };
	eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, contextAttribs);

	if (eglContext == EGL_NO_CONTEXT) {
        eprinterr("eglContext failure! Error: %x\n", eglGetError());
        return FALSE;
    }

	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

	return TRUE;
}

#endif

void Emulator_HintOTSize(int ots)
{
	g_otSize = ots;
}

static int Emulator_InitialiseSDL2(char* windowName, int width, int height)
{
#if defined(XED3D)
	windowWidth = 1280;
	windowHeight = 720;
#else
	windowWidth  = width;
	windowHeight = height;
#endif

#if defined(SDL2)

	unsigned int flags = SDL_INIT_VIDEO | SDL_INIT_TIMER;

#if defined(SDL2_MIXER)
	flags |= SDL_INIT_AUDIO;
#endif

	//Initialise SDL2
	if (SDL_Init(flags) == 0)
	{
		SDL_version ver;
		SDL_GetVersion(&ver);
		eprintf("Initialised SDL2 Version: %d.%d.%d\n", ver.major, ver.minor, ver.patch);

#if !defined(RO_DOUBLE_BUFFERED)
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
#endif

#if defined(OGLES)

#if defined(__ANDROID__)
        //Override to full screen.
        SDL_DisplayMode displayMode;
        if(SDL_GetCurrentDisplayMode(0, &displayMode) == 0)
        {
            windowWidth = displayMode.w;
            windowHeight = displayMode.h;
        }
#endif

#if defined(__ANDROID__)
		SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft");
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_EGL, 1);
#endif
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OGLES_VERSION);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#elif defined(OGL)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

#if defined(_DEBUG)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
#endif
	}
	else
	{
		eprinterr("Error: Failed to initialise SDL\n");
		return FALSE;
	}
#endif

#if defined(OGL)
	if (Emulator_InitialiseGLContext(windowName) == FALSE)
	{
		eprinterr("Failed to Initialise GL Context!\n");
		return FALSE;
	}
#elif defined(OGLES)
	if (Emulator_InitialiseGLESContext(windowName) == FALSE)
	{
		eprinterr("Failed to Initialise GLES Context!\n");
		return FALSE;
	}
#elif defined(D3D9) || defined(XED3D)
	if (Emulator_InitialiseD3D9Context(windowName) == FALSE)
	{
		eprinterr("Failed to Initialise D3D9 Context!\n");
		return FALSE;
	}
#elif defined(D3D11)
	if (Emulator_InitialiseD3D11Context(windowName) == FALSE)
	{
		eprinterr("Failed to Initialise D3D11 Context!\n");
		return FALSE;
	}
#elif defined(D3D12)
	if (Emulator_InitialiseD3D12Context(windowName) == FALSE)
	{
		eprinterr("Failed to Initialise D3D12 Context!\n");
		return FALSE;
	}
#elif defined(VULKAN)
	if (Emulator_InitialiseVulkanContext(windowName) == FALSE)
	{
		eprinterr("Failed to Initialise Vulkan Context!\n");
		return FALSE;
	}
#elif defined(SN_TARGET_PSP2)
	if(Emulator_InitialiseGXMContext(windowName) == FALSE)
	{
		eprinterr("Failed to Initialise GXM Context!\n");
	}
#elif defined(PLATFORM_NX)
	if (Emulator_InitialiseNNContext(windowName) == FALSE)
	{
		eprinterr("Failed to Initialise NX Context!\n");
	}
#endif

	return TRUE;
}

static int Emulator_InitialiseGLEW()
{
#if defined(GLEW)
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		return FALSE;
	}
#endif
	return TRUE;
}

static int Emulator_InitialiseCore()
{
	return TRUE;
}

void Emulator_Initialise(char* windowName, int width, int height)
{
	eprintf("Initialising %s.\n", EMULATOR_NAME);
	eprintf("VERSION: %d.%d\n", EMULATOR_MAJOR_VERSION, EMULATOR_MINOR_VERSION);
	eprintf("Compile Date: %s Time: %s\n", EMULATOR_COMPILE_DATE, EMULATOR_COMPILE_TIME);
	
#if defined(UWP) && !defined(UWP_SDL2)
	g_windowName = windowName;
	//Thread required because UI will block game execution
	g_uiThread = std::thread(CreateUWPApplication);

	while (!g_windowReady)
	{
	}
#endif

	char finalWindowName[128];
	sprintf(finalWindowName, "%s - %s - v%d.%d", windowName, renderBackendName, GAME_MAJOR_VERSION, GAME_MINOR_VERSION);

	if (Emulator_InitialiseSDL2(finalWindowName, width, height) == FALSE)
	{
		eprinterr("Failed to Intialise SDL2\n");
		Emulator_ShutDown();
	}

#if defined(GLEW)
	if (Emulator_InitialiseGLEW() == FALSE)
	{
		eprinterr("Failed to Intialise GLEW\n");
		Emulator_ShutDown();
	}

#if defined(_DEBUG)
	extern void GLAPIENTRY Emulator_HandleGLDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void* userParam);
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(&Emulator_HandleGLDebug, NULL);
#endif

#endif

	if (Emulator_InitialiseCore() == FALSE)
	{
		eprinterr("Failed to Intialise Emulator Core.\n");
		Emulator_ShutDown();
	}

	if (Emulator_CreateCommonResources() == FALSE)
	{
		eprinterr("Failed to Intialise GL.\n");
		Emulator_ShutDown();
	}

#if defined(SDL2)
	g_swapTime = SDL_GetTicks() - FIXED_TIME_STEP;
#elif defined(UWP) || defined(XED3D)
	g_swapTime = GetTickCount() - FIXED_TIME_STEP;
#endif

#if !defined(__ANDROID__)
	//counter_thread = std::thread(Emulator_CounterLoop);
#endif
}

void Emulator_CounterLoop()
{
	unsigned int current_time;
	unsigned int last_time = 0;
#if defined(SDL2)
	unsigned int start = SDL_GetTicks();
#else
	unsigned int start = 0;
#endif

	while (TRUE)
	{
#if defined(SDL2)
		current_time = SDL_GetTicks();
#elif defined(XED3D)
		current_time = GetTickCount();
#else
		current_time = 0;
#endif

		if (current_time > last_time + 1000)
		{
			for (int i = 0; i < 3; i++)
			{
				//if (!counters[i].IsStopped)
				{
					
					//counters[i].cycle = SDL_GetTicks() - start;
					if (counters[i].target > 0)
					{
						counters[i].cycle += counters[i].value;

						if (counters[i].cycle >= counters[i].target)
						{
							counters[i].cycle %= counters[i].target;

							if (counters[i].padding00 != NULL)
							{
								counters[i].padding00();
							}
						}
					}
				}
			}
			last_time = current_time;
		}
	}
}

#if defined(USE_THREADS)
void Emulator_CounterWrapper(int timerID)
#else
unsigned int Emulator_CounterWrapper(unsigned int interval, void* pTimerID)
#endif
{
#if !defined(USE_THREADS)
	unsigned int timerID = ((unsigned int*)pTimerID)[0];
#endif

	{
#if defined(USE_THREADS)
#define CW_FPS 61

		int now = SDL_GetTicks();
		int lastFrame = SDL_GetTicks();

		while (TRUE)
		{
			now = SDL_GetTicks();
			int delta = now - lastFrame;
			lastFrame = now;

			if (delta < (1000 / CW_FPS))
			{
				Sleep((1000 / CW_FPS) - delta);
			}

			counters[timerID].padding00();
		}
		
#else
		if (counters[timerID].padding00 != NULL)
		{
			counters[timerID].padding00();
		}
#endif
			
	}

	//static int count = 0;
	//wchar_t buff[32];
	//_swprintf(buff, L"%d\n", count);
	//OutputDebugStringW(buff);
	//count++;

#if !defined(USE_THREADS)
	return interval;
#endif
}

void Emulator_GenerateLineArray(struct Vertex* vertex, short* p0, short* p1)
{
	// swap line coordinates for left-to-right and up-to-bottom direction
	if (p0[0] > p1[0]) {
		short *tmp = p0;
		p0 = p1;
		p1 = tmp;
	} else if (p0[0] == p1[0]) {
		 if (p0[1] > p1[1]) {
			short *tmp = p0;
			p0 = p1;
			p1 = tmp;
		 }
	}

	int dx = p1[0] - p0[0];
	int dy = p1[1] - p0[1];

	if (dx > abs(dy)) { // horizontal
		vertex[0].x = p0[0];
		vertex[0].y = p0[1];

		vertex[1].x = p1[0] + 1;
		vertex[1].y = p1[1];

		vertex[2].x = vertex[1].x;
		vertex[2].y = vertex[1].y + 1;

		vertex[3].x = vertex[0].x;
		vertex[3].y = vertex[0].y + 1;
	} else { // vertical
		vertex[0].x = p0[0];
		vertex[0].y = p0[1];

		vertex[1].x = p1[0];
		vertex[1].y = p1[1] + 1;

		vertex[2].x = vertex[1].x + 1;
		vertex[2].y = vertex[1].y;

		vertex[3].x = vertex[0].x + 1;
		vertex[3].y = vertex[0].y;
	} // TODO diagonal line alignment
}

#if defined(PGXP)
void Emulator_ResetPGXPCache()
{
	// Reset the ztable.
	memset(&pgxp_vertex_buffer[0], 0, pgxp_vertex_count * sizeof(PGXPVertex));

	pgxp_vertex_count = 0;
}
#endif

void Emulator_GenerateVertexArrayTriangle(struct Vertex* vertex, short* p0, short* p1, short* p2, short z)
{
	assert(p0);
	assert(p1);
	assert(p2);

#if defined(PGXP)
	PGXPVertex* pgxp_vertex_0 = NULL;
	PGXPVertex* pgxp_vertex_1 = NULL;
	PGXPVertex* pgxp_vertex_2 = NULL;

	for (int i = 0; i < pgxp_vertex_count; i++)
	{
		if (pgxp_vertex_0 != NULL && pgxp_vertex_1 != NULL && pgxp_vertex_2 != NULL)
		{
			break;
		}

		if(pgxp_vertex_buffer[i].used == TRUE)
		{
			//continue;
		}

		if (pgxp_vertex_buffer[i].originalSXY2 == ((unsigned int*)p0)[0] && pgxp_vertex_0 == NULL /*&& pgxp_vertex_buffer[i].originalSZ3 <= z + 4096*/)
		{
			pgxp_vertex_0 = &pgxp_vertex_buffer[i];
			pgxp_vertex_0->used = TRUE;
			continue;
		}

		if (pgxp_vertex_buffer[i].originalSXY2 == ((unsigned int*)p1)[0] && pgxp_vertex_1 == NULL /*&& pgxp_vertex_buffer[i].originalSZ3 <= z + 4096*/ )
		{
			pgxp_vertex_1 = &pgxp_vertex_buffer[i];
			pgxp_vertex_1->used = TRUE;
			continue;
		}

		if (pgxp_vertex_buffer[i].originalSXY2 == ((unsigned int*)p2)[0] && pgxp_vertex_2 == NULL /* && pgxp_vertex_buffer[i].originalSZ3 <= z + 4096*/)
		{
			pgxp_vertex_2 = &pgxp_vertex_buffer[i];
			pgxp_vertex_2->used = TRUE;
			continue;
		}
	}

	//Copy over position
	if (pgxp_vertex_0 != NULL)
	{
		vertex[0].x = pgxp_vertex_0->x;
		vertex[0].y = pgxp_vertex_0->y;
		vertex[0].z = 0.95f;
		vertex[0].w = pgxp_vertex_0->z;
	}
	else
	{
		vertex[0].x = (float)p0[0];
		vertex[0].y = (float)p0[1];
	}

	if (pgxp_vertex_1 != NULL)
	{
		vertex[1].x = pgxp_vertex_1->x;
		vertex[1].y = pgxp_vertex_1->y;
		vertex[1].z = 0.95f;
		vertex[1].w = pgxp_vertex_1->z;
	}
	else
	{
		vertex[1].x = (float)p1[0];
		vertex[1].y = (float)p1[1];
	}

	if (pgxp_vertex_2 != NULL)
	{
		vertex[2].x = pgxp_vertex_2->x;
		vertex[2].y = pgxp_vertex_2->y;
		vertex[2].z = 0.95f;
		vertex[2].w = pgxp_vertex_2->z;
	}
	else
	{
		vertex[2].x = (float)p2[0];
		vertex[2].y = (float)p2[1];
	}
#else
	vertex[0].x = p0[0];
	vertex[0].y = p0[1];

	vertex[1].x = p1[0];
	vertex[1].y = p1[1];

	vertex[2].x = p2[0];
	vertex[2].y = p2[1];
#endif
}


void Emulator_DoSplitHackQuad(short* p0, short* p1, short* p2, short* p3)
{
	if (p0[0] == p1[0] && p0[0] == p2[0] && p0[0] == p3[0])
	{
		splitAgain = TRUE;
	}
	else
	{
		splitAgain = FALSE;
	}
}

void Emulator_GenerateVertexArrayQuad(struct Vertex* vertex, short* p0, short* p1, short* p2, short* p3, short z)
{
	assert(p0);
	assert(p1);
	assert(p2);
	assert(p3);

#if defined(PGXP)

	PGXPVertex* pgxp_vertex_0 = NULL;
	PGXPVertex* pgxp_vertex_1 = NULL;
	PGXPVertex* pgxp_vertex_2 = NULL;
	PGXPVertex* pgxp_vertex_3 = NULL;

	for (int i = 0; i < pgxp_vertex_count; i++)
	{
		if (pgxp_vertex_0 != NULL && pgxp_vertex_1 != NULL && pgxp_vertex_2 != NULL && pgxp_vertex_3 != NULL)
		{
			break;
		}

		if (pgxp_vertex_buffer[i].used == TRUE)
		{
			//continue;
		}

		if (pgxp_vertex_buffer[i].originalSXY2 == ((unsigned int*)p0)[0] && pgxp_vertex_0 == NULL)
		{
			pgxp_vertex_0 = &pgxp_vertex_buffer[i];
			pgxp_vertex_0->used = TRUE;
			continue;
		}

		if (pgxp_vertex_buffer[i].originalSXY2 == ((unsigned int*)p1)[0] && pgxp_vertex_1 == NULL)
		{
			pgxp_vertex_1 = &pgxp_vertex_buffer[i];
			pgxp_vertex_1->used = TRUE;
			continue;
		}

		if (pgxp_vertex_buffer[i].originalSXY2 == ((unsigned int*)p2)[0] && pgxp_vertex_2 == NULL)
		{
			pgxp_vertex_2 = &pgxp_vertex_buffer[i];
			pgxp_vertex_2->used = TRUE;
			continue;
		}

		if (pgxp_vertex_buffer[i].originalSXY2 == ((unsigned int*)p3)[0] && pgxp_vertex_3 == NULL)
		{
			pgxp_vertex_3 = &pgxp_vertex_buffer[i];
			pgxp_vertex_3->used = TRUE;
			continue;
		}
	}

	//Copy over position
	if (pgxp_vertex_0 != NULL)
	{
		vertex[0].x = pgxp_vertex_0->x;
		vertex[0].y = pgxp_vertex_0->y;
		vertex[0].z = 0.95f;
		vertex[0].w = pgxp_vertex_0->z;
	}
	else
	{
		vertex[0].x = (float)p0[0];
		vertex[0].y = (float)p0[1];
#if defined(PGXP)
		vertex[0].z = 0.95f;
		vertex[0].w = 1.0f;
#endif
	}

	if (pgxp_vertex_1 != NULL)
	{
		vertex[1].x = pgxp_vertex_1->x;
		vertex[1].y = pgxp_vertex_1->y;
		vertex[1].z = 0.95f;
		vertex[1].w = pgxp_vertex_1->z;
	}
	else
	{
		vertex[1].x = (float)p1[0];
		vertex[1].y = (float)p1[1];
#if defined(PGXP)
		vertex[1].z = 0.95f;
		vertex[1].w = 1.0f;
#endif
	}

	if (pgxp_vertex_2 != NULL)
	{
		vertex[2].x = pgxp_vertex_2->x;
		vertex[2].y = pgxp_vertex_2->y;
		vertex[2].z = 0.95f;
		vertex[2].w = pgxp_vertex_2->z;
	}
	else
	{
		vertex[2].x = (float)p2[0];
		vertex[2].y = (float)p2[1];
#if defined(PGXP)
		vertex[2].z = 0.95f;
		vertex[2].w = 1.0f;
#endif
	}

	if (pgxp_vertex_3 != NULL)
	{
		vertex[3].x = pgxp_vertex_3->x;
		vertex[3].y = pgxp_vertex_3->y;
		vertex[3].z = 0.95f;
		vertex[3].w = pgxp_vertex_3->z;
	}
	else
	{
		vertex[3].x = (float)p3[0];
		vertex[3].y = (float)p3[1];
#if defined(PGXP)
		vertex[3].z = 0.95f;
		vertex[3].w = 1.0f;
#endif
	}
#else
	vertex[0].x = p0[0];
	vertex[0].y = p0[1];

	vertex[1].x = p1[0];
	vertex[1].y = p1[1];

	vertex[2].x = p2[0];
	vertex[2].y = p2[1];

	vertex[3].x = p3[0];
	vertex[3].y = p3[1];
#endif
}

void Emulator_GenerateVertexArrayRect(struct Vertex* vertex, short* p0, short w, short h, short z)
{
	assert(p0);

#if defined(PGXP)
	PGXPVertex* pgxp_vertex_0 = NULL;

	for (int i = 0; i < pgxp_vertex_count; i++)
	{
		if (pgxp_vertex_0 != NULL)
		{
			break;
		}

		if (pgxp_vertex_buffer[i].used == TRUE)
		{
			//continue;
		}

		if (pgxp_vertex_buffer[i].originalSXY2 == ((unsigned int*)p0)[0] && pgxp_vertex_0 == NULL)
		{
			pgxp_vertex_0 = &pgxp_vertex_buffer[i];
			pgxp_vertex_0->used = TRUE;
			continue;
		}
	}


	if (pgxp_vertex_0 != NULL)
	{
		vertex[0].x = pgxp_vertex_0->x;
		vertex[0].y = pgxp_vertex_0->y;
	}
	else
	{
		vertex[0].x = (float)p0[0];
		vertex[0].y = (float)p0[1];
	}

	vertex[1].x = vertex[0].x;
	vertex[1].y = vertex[0].y + h;

	vertex[2].x = vertex[0].x + w;
	vertex[2].y = vertex[0].y + h;

	vertex[3].x = vertex[0].x + w;
	vertex[3].y = vertex[0].y;

#else
	vertex[0].x = p0[0];
	vertex[0].y = p0[1];

	vertex[1].x = vertex[0].x;
	vertex[1].y = vertex[0].y + h;

	vertex[2].x = vertex[0].x + w;
	vertex[2].y = vertex[0].y + h;

	vertex[3].x = vertex[0].x + w;
	vertex[3].y = vertex[0].y;
#endif
}

void Emulator_GenerateTexcoordArrayQuad(struct Vertex* vertex, unsigned char* uv0, unsigned char* uv1, unsigned char* uv2, unsigned char* uv3, short page, short clut, unsigned char dither)
{
	assert(uv0);
	assert(uv1);
	assert(uv2);
	assert(uv3);

#if defined(PGXP) && 0
	/*
	Locate polygon in ztable
	*/

	PGXPPolygon* z0 = NULL;
	PGXPPolygon* z1 = NULL;
	PGXPPolygon* z2 = NULL;
	PGXPPolygon* z3 = NULL;

	//Can speed this up by storing last index found and using as start iter
	for (int i = pgxp_polgon_table_index; i > -1; i--)
	{
		if (uv0 != NULL)
		{
			if (((unsigned int*)uv0)[0] == pgxp_polygons[i].originalSXY)
			{
				z0 = &pgxp_polygons[i];
				//z0->bUsed = TRUE;
			}
		}

		if (uv1 != NULL)
		{
			if (((unsigned int*)uv1)[0] == pgxp_polygons[i].originalSXY)
			{
				z1 = &pgxp_polygons[i];
				//z1->bUsed = TRUE;
			}
		}

		if (uv2 != NULL)
		{
			if (((unsigned int*)uv2)[0] == pgxp_polygons[i].originalSXY)
			{
				z2 = &pgxp_polygons[i];
				//z2->bUsed = TRUE;
			}
		}

		if (uv3 != NULL)
		{
			if (((unsigned int*)uv3)[0] == pgxp_polygons[i].originalSXY)
			{
				z3 = &pgxp_polygons[i];
				//z3->bUsed = TRUE;
			}
		}

		if ((z0 != NULL || uv0 == NULL) && (z1 != NULL || uv1 == NULL) && (z2 != NULL || uv2 == NULL) && (z3 != NULL || uv3 == NULL))
			break;
	}

	//Copy over uvs
	if (uv0 != NULL)
	{
		vertex[0].x = p0[0];
		vertex[0].y = p0[1];
	}

	if (uv1 != NULL)
	{
		vertex[1].x = p1[0];
		vertex[1].y = p1[1];
	}
	else
	{
		if (w != -1 && h != -1)
		{
			vertex[1].x = p0[0];
			vertex[1].y = p0[1] + h;
		}
	}

	if (uv2 != NULL)
	{
		vertex[2].x = p2[0];
		vertex[2].y = p2[1];
	}
	else
	{
		if (w != -1 && h != -1)
		{
			vertex[2].x = p0[0] + w;
			vertex[2].y = p0[1] + h;
		}
	}

	if (uv3 != NULL)
	{
		vertex[3].x = p3[0];
		vertex[3].y = p3[1];
	}
	else
	{
		if (w != -1 && h != -1)
		{
			vertex[3].x = p0[0] + w;
			vertex[3].y = p0[1];
		}
	}
#else
	const unsigned char bright = 2;

	vertex[0].u      = uv0[0];
	vertex[0].v      = uv0[1];

	vertex[0].bright = bright;
	vertex[0].dither = dither;
	vertex[0].page   = page;
	vertex[0].clut   = clut;

	vertex[1].u      = uv1[0];
	vertex[1].v      = uv1[1];
	vertex[1].bright = bright;
	vertex[1].dither = dither;
	vertex[1].page   = page;
	vertex[1].clut   = clut;

	vertex[2].u      = uv2[0];
	vertex[2].v      = uv2[1];
	vertex[2].bright = bright;
	vertex[2].dither = dither;
	vertex[2].page   = page;
	vertex[2].clut   = clut;

	vertex[3].u      = uv3[0];
	vertex[3].v      = uv3[1];
	vertex[3].bright = bright;
	vertex[3].dither = dither;
	vertex[3].page   = page;
	vertex[3].clut   = clut;
#endif
}

void Emulator_GenerateTexcoordArrayTriangle(struct Vertex* vertex, unsigned char* uv0, unsigned char* uv1, unsigned char* uv2, short page, short clut, unsigned char dither)
{
	assert(uv0);
	assert(uv1);
	assert(uv2);

#if defined(PGXP) && 0
	#error COPY IMPLEMENTATION FROM Emulator_GenerateTexcoordArrayQuad
#else
	const unsigned char bright = 2;

	vertex[0].u      = uv0[0];
	vertex[0].v      = uv0[1];

	vertex[0].bright = bright;
	vertex[0].dither = dither;
	vertex[0].page   = page;
	vertex[0].clut   = clut;

	vertex[1].u      = uv1[0];
	vertex[1].v      = uv1[1];
	vertex[1].bright = bright;
	vertex[1].dither = dither;
	vertex[1].page   = page;
	vertex[1].clut   = clut;

	vertex[2].u      = uv2[0];
	vertex[2].v      = uv2[1];
	vertex[2].bright = bright;
	vertex[2].dither = dither;
	vertex[2].page   = page;
	vertex[2].clut   = clut;
#endif
}

void Emulator_GenerateTexcoordArrayRect(struct Vertex* vertex, unsigned char* uv, short page, short clut, short w, short h)
{
	assert(uv);
	//assert(int(uv[0]) + w <= 255);
	//assert(int(uv[1]) + h <= 255);
	// TODO
	if ((int)uv[0] + w > 255) w = 255 - uv[0];
	if ((int)uv[1] + h > 255) h = 255 - uv[1];

	const unsigned char bright = 2;
	const unsigned char dither = 0;

	vertex[0].u      = uv[0];
	vertex[0].v      = uv[1];

	vertex[0].bright = bright;
	vertex[0].dither = dither;
	vertex[0].page   = page;
	vertex[0].clut   = clut;

	vertex[1].u      = uv[0];
	vertex[1].v      = uv[1] + h;
	vertex[1].bright = bright;
	vertex[1].dither = dither;
	vertex[1].page   = page;
	vertex[1].clut   = clut;

	vertex[2].u      = uv[0] + w;
	vertex[2].v      = uv[1] + h;
	vertex[2].bright = bright;
	vertex[2].dither = dither;
	vertex[2].page   = page;
	vertex[2].clut   = clut;

	vertex[3].u      = uv[0] + w;
	vertex[3].v      = uv[1];
	vertex[3].bright = bright;
	vertex[3].dither = dither;
	vertex[3].page   = page;
	vertex[3].clut   = clut;
}

void Emulator_GenerateTexcoordArrayLineZero(struct Vertex* vertex, unsigned char dither)
{
	const unsigned char bright = 1;

	vertex[0].u      = 0;
	vertex[0].v      = 0;
	vertex[0].bright = bright;
	vertex[0].dither = dither;
	vertex[0].page   = 0;
	vertex[0].clut   = 0;

	vertex[1].u      = 0;
	vertex[1].v      = 0;
	vertex[1].bright = bright;
	vertex[1].dither = dither;
	vertex[1].page   = 0;
	vertex[1].clut   = 0;

	vertex[2].u      = 0;
	vertex[2].v      = 0;
	vertex[2].bright = bright;
	vertex[2].dither = dither;
	vertex[2].page   = 0;
	vertex[2].clut   = 0;

	vertex[3].u      = 0;
	vertex[3].v      = 0;
	vertex[3].bright = bright;
	vertex[3].dither = dither;
	vertex[3].page   = 0;
	vertex[3].clut   = 0;
}

void Emulator_GenerateTexcoordArrayTriangleZero(struct Vertex* vertex, unsigned char dither)
{
	const unsigned char bright = 1;

	vertex[0].u      = 0;
	vertex[0].v      = 0;
	vertex[0].bright = bright;
	vertex[0].dither = dither;
	vertex[0].page   = 0;
	vertex[0].clut   = 0;

	vertex[1].u      = 0;
	vertex[1].v      = 0;
	vertex[1].bright = bright;
	vertex[1].dither = dither;
	vertex[1].page   = 0;
	vertex[1].clut   = 0;

	vertex[2].u      = 0;
	vertex[2].v      = 0;
	vertex[2].bright = bright;
	vertex[2].dither = dither;
	vertex[2].page   = 0;
	vertex[2].clut   = 0;
}

void Emulator_GenerateTexcoordArrayQuadZero(struct Vertex* vertex, unsigned char dither)
{
	const unsigned char bright = 1;

	vertex[0].u      = 0;
	vertex[0].v      = 0;
	vertex[0].bright = bright;
	vertex[0].dither = dither;
	vertex[0].page   = 0;
	vertex[0].clut   = 0;

	vertex[1].u      = 0;
	vertex[1].v      = 0;
	vertex[1].bright = bright;
	vertex[1].dither = dither;
	vertex[1].page   = 0;
	vertex[1].clut   = 0;

	vertex[2].u      = 0;
	vertex[2].v      = 0;
	vertex[2].bright = bright;
	vertex[2].dither = dither;
	vertex[2].page   = 0;
	vertex[2].clut   = 0;

	vertex[3].u      = 0;
	vertex[3].v      = 0;
	vertex[3].bright = bright;
	vertex[3].dither = dither;
	vertex[3].page   = 0;
	vertex[3].clut   = 0;
}

void Emulator_GenerateColourArrayLine(struct Vertex* vertex, unsigned char* col0, unsigned char* col1)
{
	assert(col0);
	assert(col1);

	vertex[0].r = col0[0];
	vertex[0].g = col0[1];
	vertex[0].b = col0[2];
	vertex[0].a = 255;

	vertex[1].r = col1[0];
	vertex[1].g = col1[1];
	vertex[1].b = col1[2];
	vertex[1].a = 255;

	vertex[2].r = col1[0];
	vertex[2].g = col1[1];
	vertex[2].b = col1[2];
	vertex[2].a = 255;

	vertex[3].r = col0[0];
	vertex[3].g = col0[1];
	vertex[3].b = col0[2];
	vertex[3].a = 255;
}

void Emulator_GenerateColourArrayTriangle(struct Vertex* vertex, unsigned char* col0, unsigned char* col1, unsigned char* col2)
{
	assert(col0);
	assert(col1);
	assert(col2);

	vertex[0].r = col0[0];
	vertex[0].g = col0[1];
	vertex[0].b = col0[2];
	vertex[0].a = 255;

	vertex[1].r = col1[0];
	vertex[1].g = col1[1];
	vertex[1].b = col1[2];
	vertex[1].a = 255;

	vertex[2].r = col2[0];
	vertex[2].g = col2[1];
	vertex[2].b = col2[2];
	vertex[2].a = 255;
}

void Emulator_GenerateColourArrayQuad(struct Vertex* vertex, unsigned char* col0, unsigned char* col1, unsigned char* col2, unsigned char* col3, int bMaxCol)
{
	assert(col0);
	assert(col1);
	assert(col2);
	assert(col3);

	if (bMaxCol)
	{
		vertex[0].r = 128;
		vertex[0].g = 128;
		vertex[0].b = 128;
		vertex[0].a = 128;
	}
	else
	{
		vertex[0].r = col0[0];
		vertex[0].g = col0[1];
		vertex[0].b = col0[2];
		vertex[0].a = 255;
	}

	if (bMaxCol)
	{
		vertex[1].r = 128;
		vertex[1].g = 128;
		vertex[1].b = 128;
		vertex[1].a = 255;
	}
	else
	{
		vertex[1].r = col1[0];
		vertex[1].g = col1[1];
		vertex[1].b = col1[2];
		vertex[1].a = 255;
	}

	if (bMaxCol)
	{
		vertex[2].r = 128;
		vertex[2].g = 128;
		vertex[2].b = 128;
		vertex[2].a = 255;
	}
	else
	{
		vertex[2].r = col2[0];
		vertex[2].g = col2[1];
		vertex[2].b = col2[2];
		vertex[2].a = 255;
	}

	if (bMaxCol)
	{
		vertex[3].r = 128;
		vertex[3].g = 128;
		vertex[3].b = 128;
		vertex[3].a = 255;
	}
	else
	{
		vertex[3].r = col3[0];
		vertex[3].g = col3[1];
		vertex[3].b = col3[2];
		vertex[3].a = 255;
	}
}

#if defined(OGLES) && 0
GLint u_Projection;

#define GTE_DISCARD\
	"		if (color_rg.x + color_rg.y == 0.0) { discard; }\n"

#define GTE_DECODE_RG\
	"		fragColor = texture2D(s_lut, color_rg);\n"

#define GTE_FETCH_DITHER_FUNC\
	"		mat4 dither = mat4(\n"\
	"			-4.0,  +0.0,  -3.0,  +1.0,\n"\
	"			+2.0,  -2.0,  +3.0,  -1.0,\n"\
	"			-3.0,  +1.0,  -4.0,  +0.0,\n"\
	"			+3.0,  -1.0,  +2.0,  -2.0) / 255.0;\n"\
	"		vec3 DITHER(const ivec2 dc) { \n"\
	"		for (int i = 0; i < 4; i++) {"\
	"			for (int j = 0; j < 4; j++) {"\
	"			if(i == dc.x && j == dc.y) {"\
	"				return vec3(dither[i][j] * v_texcoord.w); }\n"\
	"				}"\
	"			}"\
	"		}"

#define GTE_DITHERING\
	"		fragColor *= v_color;\n"\
	"		ivec2 dc = ivec2(fract(gl_FragCoord.xy / 4.0) * 4.0);\n"\
	"		fragColor.xyz += DITHER(dc);\n"

	#define GTE_FETCH_VRAM_FUNC\
		"	uniform sampler2D s_texture;\n"\
		"	uniform sampler2D s_lut;\n"\
		"	vec2 VRAM(vec2 uv) { return texture2D(s_texture, uv).rg; }\n"

#if defined(PGXP)
	#define GTE_PERSPECTIVE_CORRECTION \
		"		gl_Position.z = a_z;\n" \
		"		gl_Position *= a_w;\n"
#else
	#define GTE_PERSPECTIVE_CORRECTION
#endif

const char* gte_shader_4 =
	"varying vec4 v_texcoord;\n"
	"varying vec4 v_color;\n"
	"varying vec4 v_page_clut;\n"
	"#ifdef VERTEX\n"
	"	attribute vec4 a_position;\n"
	"	attribute vec4 a_texcoord; // uv, color multiplier, dither\n"
	"	attribute vec4 a_color;\n"
	"	attribute float a_z;\n"
	"	attribute float a_w;\n"
	"	uniform mat4 Projection;\n"
	"	void main() {\n"
	"		v_texcoord = a_texcoord;\n"
	"		v_color = a_color;\n"
	"		v_color.xyz *= a_texcoord.z;\n"
	"		v_page_clut.x = fract(a_position.z / 16.0) * 1024.0;\n"
	"		v_page_clut.y = floor(a_position.z / 16.0) * 256.0;\n"
	"		v_page_clut.z = fract(a_position.w / 64.0);\n"
	"		v_page_clut.w = floor(a_position.w / 64.0) / 512.0;\n"
	"		gl_Position = Projection * vec4(a_position.xy, 0.0, 1.0);\n"
	GTE_PERSPECTIVE_CORRECTION
	"	}\n"
	"#else\n"
	GTE_FETCH_DITHER_FUNC
	GTE_FETCH_VRAM_FUNC
	"	void main() {\n"
	"		vec2 uv = (v_texcoord.xy * vec2(0.25, 1.0) + v_page_clut.xy) * vec2(1.0 / 1024.0, 1.0 / 512.0);\n"
	"		vec2 comp = VRAM(uv);\n"
	"		int index = int(fract(v_texcoord.x / 4.0 + 0.0001) * 4.0);\n"
	"\n"
	"		float v = 0.0;\n"
	"		if(index / 2 == 0) { \n"
	"			v = comp[0] * (255.0 / 16.0);\n"
	"		} else {	\n"
	"			v = comp[1] * (255.0 / 16.0);\n"
	"		}\n"
	"		float f = floor(v);\n"
	"\n"
	"		vec2 c = vec2( (v - f) * 16.0, f );\n"
	"\n"
	"		vec2 clut_pos = v_page_clut.zw;\n"
	"		clut_pos.x += mix(c[0], c[1], fract(float(index) / 2.0) * 2.0) / 1024.0;\n"
	"		vec2 color_rg = VRAM(clut_pos);\n"
	GTE_DISCARD
	GTE_DECODE_RG
	GTE_DITHERING
	"	}\n"
	"#endif\n";

const char* gte_shader_8 =
	"varying vec4 v_texcoord;\n"
	"varying vec4 v_color;\n"
	"varying vec4 v_page_clut;\n"
	"#ifdef VERTEX\n"
	"	attribute vec4 a_position;\n"
	"	attribute vec4 a_texcoord; // uv, color multiplier, dither\n"
	"	attribute vec4 a_color;\n"
	"	attribute float a_z;\n"
	"	attribute float a_w;\n"
	"	uniform mat4 Projection;\n"
	"	void main() {\n"
	"		v_texcoord = a_texcoord;\n"
	"		v_color = a_color;\n"
	"		v_color.xyz *= a_texcoord.z;\n"
	"		v_page_clut.x = fract(a_position.z / 16.0) * 1024.0;\n"
	"		v_page_clut.y = floor(a_position.z / 16.0) * 256.0;\n"
	"		v_page_clut.z = fract(a_position.w / 64.0);\n"
	"		v_page_clut.w = floor(a_position.w / 64.0) / 512.0;\n"
	"		gl_Position = Projection * vec4(a_position.xy, 0.0, 1.0);\n"
	GTE_PERSPECTIVE_CORRECTION
	"	}\n"
	"#else\n"
	GTE_FETCH_VRAM_FUNC
	GTE_FETCH_DITHER_FUNC
	"	void main() {\n"
	"		vec2 uv = (v_texcoord.xy * vec2(0.5, 1.0) + v_page_clut.xy) * vec2(1.0 / 1024.0, 1.0 / 512.0);\n"
	"		vec2 comp = VRAM(uv);\n"
	"\n"
	"		vec2 clut_pos = v_page_clut.zw;\n"
	"		int index = int(mod(v_texcoord.x, 2.0));\n"
	"		if(index == 0) { \n"
	"			clut_pos.x += comp[0] * 255.0 / 1024.0;\n"
	"		} else {	\n"
	"			clut_pos.x += comp[1] * 255.0 / 1024.0;\n"
	"		}\n"
	"		vec2 color_rg = VRAM(clut_pos);\n"
	GTE_DISCARD
	GTE_DECODE_RG
	GTE_DITHERING
	"	}\n"
	"#endif\n";

const char* gte_shader_16 =
	"varying vec4 v_texcoord;\n"
	"varying vec4 v_color;\n"
	"#ifdef VERTEX\n"
	"	attribute vec4 a_position;\n"
	"	attribute vec4 a_texcoord; // uv, color multiplier, dither\n"
	"	attribute vec4 a_color;\n"
	"	attribute float a_z;\n"
	"	attribute float a_w;\n"
	"	uniform mat4 Projection;\n"
	"	void main() {\n"
	"		vec2 page\n;"
	"		page.x = fract(a_position.z / 16.0) * 1024.0\n;"
	"		page.y = floor(a_position.z / 16.0) * 256.0;\n;"
	"		v_texcoord = a_texcoord;\n"
	"		v_texcoord.xy += page;\n"
	"		v_texcoord.xy *= vec2(1.0 / 1024.0, 1.0 / 512.0);\n"
	"		v_color = a_color;\n"
	"		v_color.xyz *= a_texcoord.z;\n"
	"		gl_Position = Projection * vec4(a_position.xy, 0.0, 1.0);\n"
	GTE_PERSPECTIVE_CORRECTION
	"	}\n"
	"#else\n"
	GTE_FETCH_VRAM_FUNC
	GTE_FETCH_DITHER_FUNC
	"	void main() {\n"
	"		vec2 color_rg = VRAM(v_texcoord.xy);\n"
	GTE_DISCARD
	GTE_DECODE_RG
	GTE_DITHERING
	"	}\n"
	"#endif\n";

const char* blit_shader =
	"varying vec4 v_texcoord;\n"
	"#ifdef VERTEX\n"
	"	attribute vec4 a_position;\n"
	"	attribute vec4 a_texcoord;\n"
	"	void main() {\n"
	"		v_texcoord = a_texcoord * vec4(8.0 / 1024.0, 8.0 / 512.0, 0.0, 0.0);\n"
	"		gl_Position = vec4(a_position.xy, 0.0, 1.0);\n"
	"	}\n"
	"#else\n"
	GTE_FETCH_VRAM_FUNC
	"	void main() {\n"
	"		vec2 color_rg = VRAM(v_texcoord.xy);\n"
	GTE_DECODE_RG
	"	}\n"
	"#endif\n";

void Shader_CheckShaderStatus(GLuint shader)
{
    char info[1024];
    glGetShaderInfoLog(shader, sizeof(info), NULL, info);
    if (info[0] && strlen(info) > 8)
    {
        eprinterr("%s\n", info);
        assert(FALSE);
    }
}

void Shader_CheckProgramStatus(GLuint program)
{
    char info[1024];
    glGetProgramInfoLog(program, sizeof(info), NULL, info);
    if (info[0] && strlen(info) > 8)
    {
        eprinterr("%s\n", info);
        //assert(FALSE);
    }
}

ShaderID Shader_Compile(const char *source)
{
#if defined(ES2_SHADERS)
    const char *GLSL_HEADER_VERT =
        "#version 100\n"
        "precision lowp  int;\n"
        "precision highp float;\n"
        "#define VERTEX\n";

    const char *GLSL_HEADER_FRAG =
        "#version 100\n"
        "precision lowp  int;\n"
        "precision highp float;\n"
        "#define fragColor gl_FragColor\n";
#elif defined(ES3_SHADERS)
    const char *GLSL_HEADER_VERT =
        "#version 300 es\n"
        "precision lowp  int;\n"
        "precision highp float;\n"
        "#define VERTEX\n"
        "#define varying   out\n"
        "#define attribute in\n"
        "#define texture2D texture\n";

    const char *GLSL_HEADER_FRAG =
        "#version 300 es\n"
        "precision lowp  int;\n"
        "precision highp float;\n"
        "#define varying     in\n"
        "#define texture2D   texture\n"
        "out vec4 fragColor;\n";
#else
    const char *GLSL_HEADER_VERT =
        "#version 330\n"
        "#define VERTEX\n"
        "#define varying   out\n"
        "#define attribute in\n"
        "#define texture2D texture\n";

    const char *GLSL_HEADER_FRAG =
        "#version 330\n"
        "#define varying     in\n"
        "#define texture2D   texture\n"
        "out vec4 fragColor;\n";
#endif

    const char *vs_list[] = { GLSL_HEADER_VERT, source };
    const char *fs_list[] = { GLSL_HEADER_FRAG, source };

    GLuint program = glCreateProgram();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 2, vs_list, NULL);
    glCompileShader(vertexShader);
    Shader_CheckShaderStatus(vertexShader);
    glAttachShader(program, vertexShader);
    glDeleteShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 2, fs_list, NULL);
    glCompileShader(fragmentShader);
    Shader_CheckShaderStatus(fragmentShader);
    glAttachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    glBindAttribLocation(program, a_position, "a_position");
    glBindAttribLocation(program, a_texcoord, "a_texcoord");
    glBindAttribLocation(program, a_color,    "a_color");

#if defined(PGXP)
	glBindAttribLocation(program, a_z, "a_z");
	glBindAttribLocation(program, a_w, "a_w");
#endif

    glLinkProgram(program);
    Shader_CheckProgramStatus(program);

	//GLint texArray[2] = { vramTexture, rg8lutTexture };
    glUseProgram(program);
	//glUniform1iv(glGetUniformLocation(program, "s_texture"), 2, texArray);
	glUniform1i(glGetUniformLocation(program, "s_texture"), 0);
	glUniform1i(glGetUniformLocation(program, "s_lut"), 1);
    glUseProgram(0);

    return program;
}
#elif defined(D3D9) || defined(XED3D)



#elif defined(D3D11) && 0


#elif defined(D3D12) && 0

#elif defined(VULKAN)

#include "shaders/Vulkan/gte_shader_4_vs.h"
#include "shaders/Vulkan/gte_shader_4_ps.h"
#include "shaders/Vulkan/gte_shader_8_vs.h"
#include "shaders/Vulkan/gte_shader_8_ps.h"
#include "shaders/Vulkan/gte_shader_16_vs.h"
#include "shaders/Vulkan/gte_shader_16_ps.h"
#include "shaders/Vulkan/blit_shader_vs.h"
#include "shaders/Vulkan/blit_shader_ps.h"

ShaderID* g_shaders[] = { &g_gte_shader_4, &g_gte_shader_8, &g_gte_shader_16, &g_blit_shader };

#define Shader_Compile(name) Shader_Compile_Internal((DWORD*)name##_vs, (DWORD*)name##_ps, sizeof(name##_vs), sizeof(name##_ps))

ShaderID Shader_Compile_Internal(const DWORD* vs_data, const DWORD* ps_data, const unsigned int vs_size, const unsigned int ps_size)
{
	ShaderID shader;

	static int shaderType = 0;

	VkShaderModuleCreateInfo createInfo;
	memset(&createInfo, 0, sizeof(VkShaderModuleCreateInfo));

	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = vs_size;
	createInfo.pCode = (const uint32_t*)vs_data;
	
	shader.VS.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader.VS.stage = VK_SHADER_STAGE_VERTEX_BIT;
	shader.VS.pName = "main";
	shader.VS.flags = 0;
	shader.VS.pNext = NULL;
	shader.VS.pSpecializationInfo = NULL;

	if (vkCreateShaderModule(device, &createInfo, NULL, &shader.VS.module) != VK_SUCCESS)
	{
		assert(FALSE);
	}

	memset(&createInfo, 0, sizeof(VkShaderModuleCreateInfo));
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = ps_size;
	createInfo.pCode = (const uint32_t*)ps_data;

	shader.PS.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader.PS.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shader.PS.pName = "main";
	shader.PS.flags = 0;
	shader.PS.pNext = NULL;
	shader.PS.pSpecializationInfo = NULL;

	if (vkCreateShaderModule(device, &createInfo, NULL, &shader.PS.module) != VK_SUCCESS)
	{
		assert(FALSE);
	}

#define OFFSETOF(T, E)     ((size_t)&(((T*)0)->E))

	memset(&g_attributeDescriptions[0], 0, sizeof(VkVertexInputAttributeDescription));
	memset(&g_attributeDescriptions[1], 0, sizeof(VkVertexInputAttributeDescription));
	memset(&g_attributeDescriptions[2], 0, sizeof(VkVertexInputAttributeDescription));

#if defined(PGXP)
	g_attributeDescriptions[0].binding = 0;
	g_attributeDescriptions[0].location = 0;
	g_attributeDescriptions[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	g_attributeDescriptions[0].offset = OFFSETOF(Vertex, x);
#else
	g_attributeDescriptions[0].binding = 0;
	g_attributeDescriptions[0].location = 0;
	g_attributeDescriptions[0].format = VK_FORMAT_R16G16B16A16_SINT;
	g_attributeDescriptions[0].offset = OFFSETOF(Vertex, x);
#endif

	g_attributeDescriptions[1].binding = 0;
	g_attributeDescriptions[1].location = 1;
	g_attributeDescriptions[1].format = VK_FORMAT_R8G8B8A8_UINT;
	g_attributeDescriptions[1].offset = OFFSETOF(Vertex, u);

	g_attributeDescriptions[2].binding = 0;
	g_attributeDescriptions[2].location = 2;
	g_attributeDescriptions[2].format = VK_FORMAT_R8G8B8A8_UNORM;
	g_attributeDescriptions[2].offset = OFFSETOF(Vertex, r);


	memset(&g_bindingDescription, 0, sizeof(VkVertexInputBindingDescription));
	g_bindingDescription.binding = 0;
	g_bindingDescription.stride = sizeof(Vertex);
	g_bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;


	///@FIXME can be created more than once on same pointer? bug!
	VkPhysicalDeviceProperties properties;
	memset(&properties, 0, sizeof(VkPhysicalDeviceProperties));

	vkGetPhysicalDeviceProperties(physical_devices, &properties);

	VkSamplerCreateInfo samplerInfo;
	memset(&samplerInfo, 0, sizeof(VkSamplerCreateInfo));

	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_NEAREST;
	samplerInfo.minFilter = VK_FILTER_NEAREST;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	if (vkCreateSampler(device, &samplerInfo, nullptr, &shader.SS) != VK_SUCCESS) 
	{
		eprinterr("Failed to create sampler state!\n");
	}


	VkDescriptorSetLayoutBinding descriptorLayoutUniformBuffer = { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, NULL };
	VkDescriptorSetLayoutBinding descriptorLayoutSampler = { 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, NULL };
	VkDescriptorSetLayoutBinding descriptorLayoutTexture = { 4, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, VK_SHADER_STAGE_FRAGMENT_BIT, NULL };
	VkDescriptorSetLayoutBinding descriptorLayoutTexture2 = { 5, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, VK_SHADER_STAGE_FRAGMENT_BIT, NULL };

	VkDescriptorSetLayoutBinding bindings[] =
	{
		descriptorLayoutUniformBuffer,
		descriptorLayoutSampler,
		descriptorLayoutTexture,
		descriptorLayoutTexture2,
	};

	VkDescriptorSetLayoutCreateInfo resourceLayoutInfo;
	memset(&resourceLayoutInfo, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
	resourceLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	resourceLayoutInfo.pNext = NULL;
	resourceLayoutInfo.bindingCount = 4;
	resourceLayoutInfo.pBindings = bindings;

	vkCreateDescriptorSetLayout(device, &resourceLayoutInfo, NULL, &shader.DL);


	VkPipelineLayoutCreateInfo pipelineLayoutInfo;
	memset(&pipelineLayoutInfo, 0, sizeof(VkPipelineLayoutCreateInfo));
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &shader.DL;

	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, NULL, &shader.PL) != VK_SUCCESS)
	{
		eprinterr("Failed to create pipeline layout!\n");
		assert(FALSE);
	}

	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	memset(&colorBlendAttachment, 0, sizeof(VkPipelineColorBlendAttachmentState));

	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending;
	memset(&colorBlending, 0, sizeof(VkPipelineColorBlendStateCreateInfo));
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	Emulator_CreatePipelineState(shader, &shader.GP, &colorBlending);

	shader.T = (enum ShaderID::ShaderType)shaderType++;

#undef OFFSETOF

	return shader;
}
#endif

#if defined(VULKAN)
void Emulator_DestroyGlobalShaders()
{
	if (g_gte_shader_4.VS.module)
	{
		vkDestroyShaderModule(device, g_gte_shader_4.VS.module, NULL);
		g_gte_shader_4.VS.module = VK_NULL_HANDLE;
	}

	if (g_gte_shader_4.PS.module)
	{
		vkDestroyShaderModule(device, g_gte_shader_4.PS.module, NULL);
		g_gte_shader_4.PS.module = VK_NULL_HANDLE;
	}

	if (g_gte_shader_4.DL)
	{
		vkDestroyDescriptorSetLayout(device, g_gte_shader_4.DL, NULL);
		g_gte_shader_4.DL = VK_NULL_HANDLE;
	}

	if (g_gte_shader_4.PL)
	{
		vkDestroyPipelineLayout(device, g_gte_shader_4.PL, NULL);
		g_gte_shader_4.PL = VK_NULL_HANDLE;
	}

	if (g_gte_shader_4.GP)
	{
		vkDestroyPipeline(device, g_gte_shader_4.GP, NULL);
		g_gte_shader_4.GP = VK_NULL_HANDLE;
	}

	if (g_gte_shader_4.SS)
	{
		vkDestroySampler(device, g_gte_shader_4.SS, NULL);
		g_gte_shader_4.SS = VK_NULL_HANDLE;
	}

	if (g_gte_shader_8.VS.module)
	{
		vkDestroyShaderModule(device, g_gte_shader_8.VS.module, NULL);
		g_gte_shader_8.VS.module = VK_NULL_HANDLE;
	}

	if (g_gte_shader_8.PS.module)
	{
		vkDestroyShaderModule(device, g_gte_shader_8.PS.module, NULL);
		g_gte_shader_8.PS.module = VK_NULL_HANDLE;
	}

	if (g_gte_shader_8.DL)
	{
		vkDestroyDescriptorSetLayout(device, g_gte_shader_8.DL, NULL);
		g_gte_shader_8.DL = VK_NULL_HANDLE;
	}
	
	if (g_gte_shader_8.PL)
	{
		vkDestroyPipelineLayout(device, g_gte_shader_8.PL, NULL);
		g_gte_shader_8.PL = VK_NULL_HANDLE;
	}

	if (g_gte_shader_8.GP)
	{
		vkDestroyPipeline(device, g_gte_shader_8.GP, NULL);
		g_gte_shader_8.GP = VK_NULL_HANDLE;
	}

	if (g_gte_shader_8.SS)
	{
		vkDestroySampler(device, g_gte_shader_8.SS, NULL);
		g_gte_shader_8.SS = VK_NULL_HANDLE;
	}

	if (g_gte_shader_16.VS.module)
	{
		vkDestroyShaderModule(device, g_gte_shader_16.VS.module, NULL);
		g_gte_shader_16.VS.module = VK_NULL_HANDLE;
	}

	if (g_gte_shader_16.PS.module)
	{
		vkDestroyShaderModule(device, g_gte_shader_16.PS.module, NULL);
		g_gte_shader_16.PS.module = VK_NULL_HANDLE;
	}

	if (g_gte_shader_16.DL)
	{
		vkDestroyDescriptorSetLayout(device, g_gte_shader_16.DL, NULL);
		g_gte_shader_16.DL = VK_NULL_HANDLE;
	}

	if (g_gte_shader_16.PL)
	{
		vkDestroyPipelineLayout(device, g_gte_shader_16.PL, NULL);
		g_gte_shader_16.PL = VK_NULL_HANDLE;
	}

	if (g_gte_shader_16.GP)
	{
		vkDestroyPipeline(device, g_gte_shader_16.GP, NULL);
		g_gte_shader_16.GP = VK_NULL_HANDLE;
	}

	if (g_gte_shader_16.SS)
	{
		vkDestroySampler(device, g_gte_shader_16.SS, NULL);
		g_gte_shader_16.SS = VK_NULL_HANDLE;
	}

	if (g_blit_shader.VS.module)
	{
		vkDestroyShaderModule(device, g_blit_shader.VS.module, NULL);
		g_blit_shader.VS.module = VK_NULL_HANDLE;
	}

	if (g_blit_shader.PS.module)
	{
		vkDestroyShaderModule(device, g_blit_shader.PS.module, NULL);
		g_blit_shader.PS.module = VK_NULL_HANDLE;
	}

	if (g_blit_shader.DL)
	{
		vkDestroyDescriptorSetLayout(device, g_blit_shader.DL, NULL);
		g_blit_shader.DL = VK_NULL_HANDLE;
	}

	if (g_blit_shader.PL)
	{
		vkDestroyPipelineLayout(device, g_blit_shader.PL, NULL);
		g_blit_shader.PL = VK_NULL_HANDLE;
	}

	if (g_blit_shader.GP)
	{
		vkDestroyPipeline(device, g_blit_shader.GP, NULL);
		g_blit_shader.GP = VK_NULL_HANDLE;
	}

	if (g_blit_shader.SS)
	{
		vkDestroySampler(device, g_blit_shader.SS, NULL);
		g_blit_shader.SS = VK_NULL_HANDLE;
	}
}
#endif

#if !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12) 
void Emulator_GenerateCommonTextures()
{
	unsigned int pixelData = 0xFFFFFFFF;
#if defined(OGL) || defined(OGLES)
	glGenTextures(1, &whiteTexture);
	glBindTexture(GL_TEXTURE_2D, whiteTexture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#if (OGLES_VERSION != 2)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixelData);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glGenTextures(1, &rg8lutTexture);
	glBindTexture(GL_TEXTURE_2D, rg8lutTexture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#if (OGLES_VERSION != 2)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, LUT_WIDTH, LUT_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, Emulator_GenerateRG8LUT());
	glBindTexture(GL_TEXTURE_2D, 0);

#elif defined(D3D9) || defined(XED3D)
	HRESULT hr = d3ddev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &whiteTexture, NULL);
	assert(!FAILED(hr));
	D3DLOCKED_RECT rect;
	hr = whiteTexture->LockRect(0, &rect, NULL, 0);
	assert(!FAILED(hr));
	memcpy(rect.pBits, &pixelData, sizeof(pixelData));
	whiteTexture->UnlockRect(0);

	hr = d3ddev->CreateTexture(LUT_WIDTH, LUT_HEIGHT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &rg8lutTexture, NULL);
	assert(!FAILED(hr));
	hr = rg8lutTexture->LockRect(0, &rect, NULL, 0);
	assert(!FAILED(hr));
	memcpy(rect.pBits, Emulator_GenerateRG8LUT(), 256*256*4);
	rg8lutTexture->UnlockRect(0);
#elif defined(D3D11)
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = 1;
	td.Height = 1;
	td.MipLevels = td.ArraySize = 1;
	td.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	ID3D11Texture2D* t = NULL;

	D3D11_SUBRESOURCE_DATA srd;
	ZeroMemory(&srd, sizeof(srd));
	srd.pSysMem = (void*)&pixelData;
	srd.SysMemPitch = td.Width * sizeof(unsigned int);
	srd.SysMemSlicePitch = 0;

	HRESULT hr = d3ddev->CreateTexture2D(&td, &srd, &t);
	assert(!FAILED(hr));
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = td.MipLevels;
	srvd.Texture2D.MostDetailedMip = 0;
	hr = d3ddev->CreateShaderResourceView(t, &srvd, &whiteTexture);
	assert(!FAILED(hr));
	t->Release();

	ZeroMemory(&td, sizeof(td));
	td.Width = LUT_WIDTH;
	td.Height = LUT_HEIGHT;
	td.MipLevels = td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	t = NULL;

	ZeroMemory(&srd, sizeof(srd));
	srd.pSysMem = (void*)Emulator_GenerateRG8LUT();
	srd.SysMemPitch = td.Width * sizeof(unsigned int);
	srd.SysMemSlicePitch = 0;

	hr = d3ddev->CreateTexture2D(&td, &srd, &t);
	assert(!FAILED(hr));
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = td.MipLevels;
	srvd.Texture2D.MostDetailedMip = 0;
	hr = d3ddev->CreateShaderResourceView(t, &srvd, &rg8lutTexture);
	assert(!FAILED(hr));
	t->Release();

#elif defined(D3D12)
	D3D12_RESOURCE_DESC td;

	ZeroMemory(&td, sizeof(td));
	td.Width = 1;
	td.Height = 1;
	td.MipLevels = td.DepthOrArraySize = 1;
	td.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.Flags = D3D12_RESOURCE_FLAG_NONE;
	td.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	HRESULT hr = d3ddev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &td, D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(&whiteTexture.m_textureResource));
	assert(!FAILED(hr));

	unsigned int uploadBufferSize = td.Width * td.Height * sizeof(unsigned int);

	hr = d3ddev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize), D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&whiteTexture.m_uploadHeap));
	assert(!FAILED(hr));

	D3D12_SUBRESOURCE_DATA srd;
	ZeroMemory(&srd, sizeof(srd));
	srd.pData = (void*)&pixelData;
	srd.RowPitch = td.Width * sizeof(unsigned int);
	srd.SlicePitch = 0;

	Emulator_BeginCommandBuffer();
	UpdateSubresources(commandList, whiteTexture.m_textureResource, whiteTexture.m_uploadHeap, 0, 0, 1, &srd);
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(whiteTexture.m_textureResource, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	Emulator_EndCommandBuffer();

	Emulator_WaitForPreviousFrame();

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	hr = d3ddev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&whiteTexture.m_srvHeap));
	assert(SUCCEEDED(hr));


	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = td.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	d3ddev->CreateShaderResourceView(whiteTexture.m_textureResource, &srvDesc, whiteTexture.m_srvHeap->GetCPUDescriptorHandleForHeapStart());



	ZeroMemory(&td, sizeof(td));
	td.Width = LUT_WIDTH;
	td.Height = LUT_HEIGHT;
	td.MipLevels = td.DepthOrArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;


	hr = d3ddev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &td, D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(&rg8lutTexture.m_textureResource));
	assert(!FAILED(hr));

	uploadBufferSize = td.Width * td.Height * sizeof(unsigned int);

	hr = d3ddev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize), D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&rg8lutTexture.m_uploadHeap));
	assert(!FAILED(hr));


	ZeroMemory(&srd, sizeof(srd));
	srd.pData = (void*)Emulator_GenerateRG8LUT();
	srd.RowPitch = td.Width * sizeof(unsigned int);
	srd.SlicePitch = 0;

	Emulator_BeginCommandBuffer();
	UpdateSubresources(commandList, rg8lutTexture.m_textureResource, rg8lutTexture.m_uploadHeap, 0, 0, 1, &srd);
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rg8lutTexture.m_textureResource, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	Emulator_EndCommandBuffer();
	Emulator_WaitForPreviousFrame();

	ZeroMemory(&srvHeapDesc, sizeof(srvHeapDesc));
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	hr = d3ddev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&rg8lutTexture.m_srvHeap));
	assert(SUCCEEDED(hr));


	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = td.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	d3ddev->CreateShaderResourceView(rg8lutTexture.m_textureResource, &srvDesc, rg8lutTexture.m_srvHeap->GetCPUDescriptorHandleForHeapStart());

#elif defined(VULKAN)
	int texWidth = 1;
	int texHeight = 1;
	VkDeviceSize imageSize = texWidth * texHeight * sizeof(unsigned int);

	Emulator_CreateVulkanBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, whiteTexture.stagingBuffer, whiteTexture.stagingBufferMemory);
	
	void* data = NULL;
	vkMapMemory(device, whiteTexture.stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, &pixelData, static_cast<size_t>(imageSize));
	vkUnmapMemory(device, whiteTexture.stagingBufferMemory);
	
	Emulator_CreateImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, whiteTexture.textureImage, whiteTexture.textureImageMemory);
	whiteTexture.textureImageView = Emulator_CreateImageView(whiteTexture.textureImage, VK_FORMAT_R8G8B8A8_UNORM);

	Emulator_TransitionImageLayout(whiteTexture.textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	Emulator_CopyBufferToImage(whiteTexture.stagingBuffer, whiteTexture.textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
	Emulator_TransitionImageLayout(whiteTexture.textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	imageSize = LUT_WIDTH * LUT_HEIGHT * sizeof(unsigned int);

	Emulator_CreateVulkanBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, rg8lutTexture.stagingBuffer, rg8lutTexture.stagingBufferMemory);

	data = NULL;
	vkMapMemory(device, rg8lutTexture.stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, Emulator_GenerateRG8LUT(), static_cast<size_t>(imageSize));
	vkUnmapMemory(device, rg8lutTexture.stagingBufferMemory);

	Emulator_CreateImage(LUT_WIDTH, LUT_HEIGHT, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, rg8lutTexture.textureImage, rg8lutTexture.textureImageMemory);
	rg8lutTexture.textureImageView = Emulator_CreateImageView(rg8lutTexture.textureImage, VK_FORMAT_R8G8B8A8_UNORM);

	Emulator_TransitionImageLayout(rg8lutTexture.textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	Emulator_CopyBufferToImage(rg8lutTexture.stagingBuffer, rg8lutTexture.textureImage, static_cast<uint32_t>(LUT_WIDTH), static_cast<uint32_t>(LUT_HEIGHT));
	Emulator_TransitionImageLayout(rg8lutTexture.textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
#else
	//#error
#endif
}
#endif

#if !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12) 
int Emulator_CreateCommonResources()
{
	memset(vram, 0, VRAM_WIDTH * VRAM_HEIGHT * sizeof(unsigned short));
	Emulator_GenerateCommonTextures();
#if !defined(VULKAN) && !defined(D3D12)
	Emulator_CreateGlobalShaders();
#endif

#if defined(D3D11) || defined(VULKAN) || defined(D3D12)
	Emulator_CreateConstantBuffers();
#endif

#if defined(OGLES)
	glDisable(GL_DEPTH_TEST);
	glBlendColor(0.5f, 0.5f, 0.5f, 0.25f);

	glGenTextures(1, &vramTexture);
	glBindTexture(GL_TEXTURE_2D, vramTexture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, VRAM_INTERNAL_FORMAT, VRAM_WIDTH, VRAM_HEIGHT, 0, VRAM_FORMAT, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenBuffers(1, &dynamic_vertex_buffer);
	glGenVertexArrays(1, &dynamic_vertex_array);
	glBindVertexArray(dynamic_vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, dynamic_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MAX_NUM_POLY_BUFFER_VERTICES, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(a_position);
	glEnableVertexAttribArray(a_texcoord);

	glEnableVertexAttribArray(a_color);
#if defined(PGXP)
	glVertexAttribPointer(a_position, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), &((Vertex*)NULL)->x);
#else
	glVertexAttribPointer(a_position, 4, GL_SHORT, GL_FALSE, sizeof(Vertex), &((Vertex*)NULL)->x);
#endif
	glVertexAttribPointer(a_texcoord, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), &((Vertex*)NULL)->u);
	glVertexAttribPointer(a_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), &((Vertex*)NULL)->r);
#if defined(PGXP)
	glVertexAttribPointer(a_z, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), &((Vertex*)NULL)->z);
	glVertexAttribPointer(a_w, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), &((Vertex*)NULL)->w);

	glEnableVertexAttribArray(a_z);
	glEnableVertexAttribArray(a_w);
#endif
	//glBindVertexArray(0);
#elif defined(D3D9) || defined(XED3D)
	if (FAILED(d3ddev->CreateTexture(VRAM_WIDTH, VRAM_HEIGHT, 1, 0, D3DFMT_A8L8, D3DPOOL_MANAGED, &vramTexture, NULL)))
	{
		eprinterr("Failed to create render target texture!\n");
		return FALSE;
	}

#define OFFSETOF(T, E)     ((size_t)&(((T*)0)->E))

	const D3DVERTEXELEMENT9 VERTEX_DECL[] = {
#if defined(PGXP)
		{0, OFFSETOF(Vertex, x), D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0}, // a_position
#else
		{0, OFFSETOF(Vertex, x), D3DDECLTYPE_SHORT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0}, // a_position
#endif
		{0, OFFSETOF(Vertex, u), D3DDECLTYPE_UBYTE4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0}, // a_texcoord
		{0, OFFSETOF(Vertex, r), D3DDECLTYPE_UBYTE4N,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0}, // a_color
		D3DDECL_END()
	};

	d3ddev->CreateVertexDeclaration(VERTEX_DECL, &vertexDecl);

#undef OFFSETOF
#elif defined(D3D11)

	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.ArraySize = 1;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	td.Format = DXGI_FORMAT_R8G8_UNORM;
	td.Width = VRAM_WIDTH;
	td.Height = VRAM_HEIGHT;
	td.MipLevels = 1;
	td.MiscFlags = 0;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(d3ddev->CreateTexture2D(&td, NULL, &vramBaseTexture)))
	{
		eprinterr("Failed to create render target texture!\n");
		return FALSE;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = 1;

	if (FAILED(d3ddev->CreateShaderResourceView(vramBaseTexture, &srvd, &vramTexture)))
	{
		eprinterr("Failed to create shader resource view!\n");
		return FALSE;
	}
#elif defined(D3D12)
	D3D12_RESOURCE_DESC td;

	ZeroMemory(&td, sizeof(td));
	td.Width = VRAM_WIDTH;
	td.Height = VRAM_HEIGHT;
	td.MipLevels = td.DepthOrArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8_UNORM;
	td.SampleDesc.Count = 1;
	td.Flags = D3D12_RESOURCE_FLAG_NONE;
	td.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	D3D12_HEAP_PROPERTIES heapProperties;
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;

	HRESULT hr = d3ddev->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &td, D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(&vramTexture.m_textureResource));
	assert(!FAILED(hr));

	hr = d3ddev->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &td, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&vramTexture.m_uploadHeap));
	assert(!FAILED(hr));

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	hr = d3ddev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&vramTexture.m_srvHeap));
	assert(SUCCEEDED(hr));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = td.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	d3ddev->CreateShaderResourceView(vramTexture.m_textureResource, &srvDesc, vramTexture.m_srvHeap->GetCPUDescriptorHandleForHeapStart());

	ZeroMemory(&td, sizeof(td));
	td.Width = LUT_WIDTH;
	td.Height = LUT_HEIGHT;
	td.MipLevels = td.DepthOrArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8_UNORM;
	td.SampleDesc.Count = 1;
	td.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	D3D12_RESOURCE_DESC textureResourceDesc;
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	textureResourceDesc.Alignment = 0;
	textureResourceDesc.Width = VRAM_WIDTH * VRAM_HEIGHT * sizeof(short);
	textureResourceDesc.Height = 1;
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	textureResourceDesc.SampleDesc.Count = 1;
	textureResourceDesc.SampleDesc.Quality = 0;
	textureResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	textureResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;


	ZeroMemory(&heapProperties, sizeof(heapProperties));
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;

	hr = d3ddev->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &textureResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&vramBaseTexture));
	assert(!FAILED(hr));

	Emulator_CreateGlobalShaders();
#elif defined(VULKAN)
	int texWidth = VRAM_WIDTH;
	int texHeight = VRAM_HEIGHT;
	VkDeviceSize imageSize = texWidth * texHeight * sizeof(unsigned short);

	Emulator_CreateVulkanBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vramTexture.stagingBuffer, vramTexture.stagingBufferMemory);
	Emulator_CreateImage(texWidth, texHeight, VK_FORMAT_R8G8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vramTexture.textureImage, vramTexture.textureImageMemory);
	vramTexture.textureImageView = Emulator_CreateImageView(vramTexture.textureImage, VK_FORMAT_R8G8_UNORM);

	Emulator_TransitionImageLayout(vramTexture.textureImage, VK_FORMAT_R8G8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	Emulator_TransitionImageLayout(vramTexture.textureImage, VK_FORMAT_R8G8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	Emulator_CreateGlobalShaders();
#endif

	Emulator_ResetDevice();

	return TRUE;
}


void Emulator_Ortho2D(float left, float right, float bottom, float top, float znear, float zfar)
{
	float a = 2.0f / (right - left);
	float b = 2.0f / (top - bottom);
	float c = 2.0f / (znear - zfar);
	
	float x = (left + right) / (left - right);
	float y = (bottom + top) / (bottom - top);

#if defined(OGL) || defined(OGLES) || defined(SN_TARGET_PSP2) || defined(PLATFORM_NX) // -1..1
	float z = (znear + zfar) / (znear - zfar);
#elif defined(D3D9) || defined (XED3D) || defined(D3D11) || defined(D3D12) || defined(VULKAN)// 0..1
	float z = znear / (znear - zfar);
#endif

	float ortho[16] = {
		a, 0, 0, 0,
		0, b, 0, 0,
		0, 0, c, 0,
		x, y, z, 1
	};

#if defined(OGLES)
	glUniformMatrix4fv(u_Projection, 1, GL_FALSE, ortho);
#elif defined(D3D9) || defined(XED3D)
	d3ddev->SetVertexShaderConstantF(u_Projection, ortho, 4);
#elif defined(D3D11)
	Emulator_UpdateProjectionConstantBuffer(&ortho[0]);
	Emulator_SetConstantBuffers();
#elif defined(D3D12)
	Emulator_UpdateProjectionConstantBuffer(ortho);
#elif defined(VULKAN)
	Emulator_UpdateProjectionConstantBuffer(ortho);
#endif
}
#endif

#if !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12) 
void Emulator_SetShader(const ShaderID shader)
{
#if defined(OGL) || defined(OGLES)
	glUseProgram(shader);
#elif defined(D3D9) || defined(XED3D)
	d3ddev->SetVertexShader(shader.VS);
	d3ddev->SetPixelShader(shader.PS);
#elif defined(D3D11)
	d3dcontext->VSSetShader(shader.VS, NULL, 0);
	d3dcontext->PSSetShader(shader.PS, NULL, 0);
	d3dcontext->IASetInputLayout(shader.IL);
#elif defined(D3D12)
	if (begin_commands_flag && begin_pass_flag && !g_resetDeviceOnNextFrame)
	{
		commandList->SetGraphicsRootSignature(shader.RS);
		commandList->SetPipelineState(shader.GPS[g_CurrentBlendMode]);
		commandList->OMSetBlendFactor(shader.BF);
		Emulator_SetConstantBuffers();
	}

#elif defined(VULKAN)
	g_shaderStages[VERTEX_BIT] = shader.VS;
	g_shaderStages[FRAGMENT_BIT] = shader.PS;
	g_graphicsPipeline = shader.GP;
	g_activeShader = shader;

	if (begin_commands_flag && begin_pass_flag && !g_resetDeviceOnNextFrame)
	{
		vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, g_graphicsPipeline);
	}
#else
	//#error
#endif

#if !defined(_PATCH)
	Emulator_Ortho2D(0.0f, activeDispEnv.disp.w, activeDispEnv.disp.h, 0.0f, 0.0f, 1.0f);
#endif
}
#endif

#if !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12) 
void Emulator_SetTexture(TextureID texture, enum TexFormat texFormat)
{
	switch (texFormat)
	{
	case TF_4_BIT:
		Emulator_SetShader(g_gte_shader_4);
		break;
	case TF_8_BIT:
		Emulator_SetShader(g_gte_shader_8);
		break;
	case TF_16_BIT:
		Emulator_SetShader(g_gte_shader_16);
		break;
	}

	if (g_texturelessMode) {
		texture = whiteTexture;
	}

#if defined(VULKAN)

	///@FIXME broken!
	if (g_lastBoundTexture[0].textureImage == texture.textureImage && g_lastBoundTexture[1].textureImage == rg8lutTexture.textureImage) {
		//return;
	}
#elif defined(D3D12)
	if (g_lastBoundTexture[0].m_textureResource == texture.m_textureResource && g_lastBoundTexture[1].m_textureResource == rg8lutTexture.m_textureResource) {
		//return;
	}
#else
	if (g_lastBoundTexture[0] == texture && g_lastBoundTexture[1] == rg8lutTexture) {
		//return;
	}
#endif

#if defined(OGL) || defined(OGLES)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, rg8lutTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#elif defined(D3D9) || defined(XED3D)
	d3ddev->SetTexture(0, texture);
	d3ddev->SetTexture(1, rg8lutTexture);
#elif defined(D3D11)
	d3dcontext->PSSetShaderResources(0, 1, &texture);
	d3dcontext->PSSetShaderResources(1, 1, &rg8lutTexture);

	if (samplerState == NULL)
	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.MinLOD = -D3D11_FLOAT32_MAX;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		sampDesc.MaxAnisotropy = 1;
		d3ddev->CreateSamplerState(&sampDesc, &samplerState);
	}

	if (rg8lutSamplerState == NULL)
	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.MinLOD = -D3D11_FLOAT32_MAX;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		sampDesc.MaxAnisotropy = 1;
		d3ddev->CreateSamplerState(&sampDesc, &rg8lutSamplerState);
	}

	ID3D11SamplerState* samplerStates[2] = { samplerState, rg8lutSamplerState };

	d3dcontext->PSSetSamplers(0, 2, samplerStates);
#elif defined(D3D12)
	ID3D12DescriptorHeap* ppHeapsSRV[] = { texture.m_srvHeap };
	commandList->SetDescriptorHeaps(_countof(ppHeapsSRV), ppHeapsSRV);
	commandList->SetGraphicsRootDescriptorTable(0, texture.m_srvHeap->GetGPUDescriptorHandleForHeapStart());
	ID3D12DescriptorHeap* ppHeapsSRV2[] = { rg8lutTexture.m_srvHeap };
	commandList->SetDescriptorHeaps(_countof(ppHeapsSRV2), ppHeapsSRV2);
	commandList->SetGraphicsRootDescriptorTable(1, rg8lutTexture.m_srvHeap->GetGPUDescriptorHandleForHeapStart());
#elif defined(VULKAN)
	g_activeTexture = texture;
	if (g_activeTexture.textureImage == vramTexture.textureImage)
	{
		g_activeDescriptor = 0;
	}
	else
	{
		g_activeDescriptor = 1;
	}

	vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, g_activeShader.PL, 0, 1, &descriptorSets[currentFrame][g_activeDescriptor], 0, NULL);

#else
	//#error
#endif

	g_lastBoundTexture[0] = texture;
	g_lastBoundTexture[1] = rg8lutTexture;
}
#endif

#if !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12) 
void Emulator_DestroyTexture(TextureID texture)
{
#if defined(OGL) || defined(OGLES)
    glDeleteTextures(1, &texture);
#elif defined(D3D9) || defined(XED3D) || defined(D3D11)
    texture->Release();
#elif defined(D3D12)
	///@TODO D3D12
	UNIMPLEMENTED();
#elif defined(VULKAN)
	UNIMPLEMENTED();
#else
    //#error
#endif
}
#endif

#if !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12) 
void Emulator_Clear(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b)
{
// TODO clear rect if it's necessary
///@FIXME set scissor!
#if defined(OGL) || defined(OGLES)
	glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
#elif defined(D3D9) || defined(XED3D)
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, 0xFF000000 | (r << 16) | (g << 8) | (b), 1.0f, 0);
#elif defined(D3D11)
	FLOAT clearColor[4]{ r / 255.0f, g / 255.0f, b / 255.0f, 1.0f };
	d3dcontext->ClearRenderTargetView(renderTargetView, clearColor);
#elif defined(D3D12)
	FLOAT clearColor[4]{ r / 255.0f, g / 255.0f, b / 255.0f, 1.0f };
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(renderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, d3ddev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, NULL);
#elif defined(VULKAN)
	VkClearColorValue clearColor = { r / 255.0f, g / 255.0f, b / 255.0f, 1.0f };
	
	VkClearValue clearValue;
	memset(&clearValue, 0, sizeof(VkClearValue));

	clearValue.color = clearColor;
	VkImageSubresourceRange imageRange;
	memset(&imageRange, 0, sizeof(VkImageSubresourceRange));

	imageRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageRange.levelCount = 1;
	imageRange.layerCount = 1;
	
	VkCommandBuffer buff = Emulator_BeginSingleTimeCommands();
	
	Emulator_TransitionImageLayout(swapchainImages[0], VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	Emulator_EndSingleTimeCommands(buff);
	vkCmdClearColorImage(commandBuffers[currentFrame], swapchainImages[0], VK_IMAGE_LAYOUT_GENERAL, &clearColor, 1, &imageRange);
	
	VkCommandBuffer buff2 = Emulator_BeginSingleTimeCommands();
	Emulator_TransitionImageLayout(swapchainImages[0], VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
	Emulator_EndSingleTimeCommands(buff2);
#else
	//#error
#endif
}
#endif

#define NOFILE 0

#if !defined(__EMSCRIPTEN__) && !defined(__ANDROID__) && !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12)

void Emulator_SaveVRAM(const char* outputFileName, int x, int y, int width, int height, int bReadFromFrameBuffer)
{
#if NOFILE
	return;
#endif

#if defined(OGL) || defined(OGLES) || 1
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

#elif defined(D3D9)
	//D3DXSaveSurfaceToFile(outputFileName, D3DXIFF_TGA, vramFrameBuffer, NULL, NULL);
#elif defined(D3D11)
	
#endif
}
#endif

#if !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12) 
void Emulator_StoreFrameBuffer(int x, int y, int w, int h)
{
	short *fb = (short*)malloc(w * h * sizeof(short));

#if defined(OGL) || defined(OGLES)
	int *data = (int*)malloc(w * h * sizeof(int));
	glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);

	#define FLIP_Y (h - fy - 1)
	#define SWAP_RB
#elif defined(D3D9) || defined (XED3D)
	IDirect3DSurface9 *srcSurface, *dstSurface;
	HRESULT hr;
	D3DLOCKED_RECT rect;
	hr = d3ddev->GetRenderTarget(0, &srcSurface);
	assert(!FAILED(hr));
#if defined(D3D9)
	hr = d3ddev->CreateOffscreenPlainSurface(windowWidth, windowHeight, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &dstSurface, NULL);
	assert(!FAILED(hr));
	hr = d3ddev->GetRenderTargetData(srcSurface, dstSurface);
	assert(!FAILED(hr));
#elif defined(XED3D)
	D3DLOCKED_RECT srcRect;
	dstSurface = new IDirect3DSurface9();
	D3DSURFACE_PARAMETERS  pSurfaceParams;
	memset(&pSurfaceParams, 0, sizeof(D3DSURFACE_PARAMETERS));
	pSurfaceParams.Base = 0;
	pSurfaceParams.HierarchicalZBase = 0;
	DWORD dwSurfaceSize = XGSetSurfaceHeader( windowWidth, windowHeight, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, &pSurfaceParams, dstSurface, NULL);
	XGOffsetSurfaceAddress(dstSurface, 0, 0);
	hr = dstSurface->LockRect(&rect, NULL, D3DLOCK_NOOVERWRITE);
	assert(!FAILED(hr));
	hr = srcSurface->LockRect(&srcRect, NULL, D3DLOCK_NOOVERWRITE);
	assert(!FAILED(hr));
	memcpy(rect.pBits, srcRect.pBits, VRAM_WIDTH * VRAM_HEIGHT * sizeof(unsigned short));
	hr = dstSurface->UnlockRect();
	assert(!FAILED(hr));
	hr = srcSurface->UnlockRect();
	assert(!FAILED(hr));
#endif
	hr = dstSurface->LockRect(&rect, NULL, D3DLOCK_READONLY);
	assert(!FAILED(hr));
	assert(windowWidth * 4 == rect.Pitch);

	int *data = (int*)rect.pBits;

	#define FLIP_Y (fy)
#elif defined(D3D11)
	ID3D11Texture2D* backBuffer;
	HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	assert(!FAILED(hr));
	ID3D11Texture2D* newBackBuffer = NULL;
	D3D11_TEXTURE2D_DESC description;
	backBuffer->GetDesc(&description);
	description.BindFlags = 0;
	description.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	description.Usage = D3D11_USAGE_STAGING;
	hr = d3ddev->CreateTexture2D(&description, NULL, &newBackBuffer);
	assert(!FAILED(hr));
	d3dcontext->CopyResource(newBackBuffer, backBuffer);
	D3D11_MAPPED_SUBRESOURCE resource;
	unsigned int subResource = D3D11CalcSubresource(0, 0, 0);
	hr = d3dcontext->Map(newBackBuffer, subResource, D3D11_MAP_READ_WRITE, 0, &resource);
	assert(!FAILED(hr));
	
	int* data = (int*)resource.pData;

#define FLIP_Y (fy)
#define SWAP_RB
#elif defined(D3D12) || defined(VULKAN)
#define FLIP_Y (fy)
	int* data = NULL;
	assert(FALSE);//Needs implementing for framebuffer write backs!
	return;
#elif defined(SN_TARGET_PSP2)
	#define FLIP_Y (fy)
	int* data = NULL;
	assert(FALSE);//Needs implementing for framebuffer write backs!
	return;
#elif defined(PLATFORM_NX)
#define FLIP_Y (fy)
	int* data = NULL;
	assert(FALSE);//Needs implementing for framebuffer write backs!
	return;
#endif

	unsigned int   *data_src = (unsigned int*)data;
	unsigned short *data_dst = (unsigned short*)fb;

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			unsigned int  c = *data_src++;
			unsigned char b = ((c >>  3) & 0x1F);
			unsigned char g = ((c >> 11) & 0x1F);
			unsigned char r = ((c >> 19) & 0x1F);
#if defined(SWAP_RB)
			*data_dst++ = b | (g << 5) | (r << 10) | 0x8000;
#else
			*data_dst++ = r | (g << 5) | (b << 10) | 0x8000;
#endif
		}
	}

#if 0
	unsigned short* src = (unsigned short*)fb;
	unsigned short* dst = vram + x + y * VRAM_WIDTH;
	
	for (int i = 0; i < h; i++) {
		memcpy(dst, src, w * sizeof(unsigned short));
		src += w;
		dst += VRAM_WIDTH;
	}
#elif 1

	short* ptr = (short*)vram + VRAM_WIDTH * y + x;

	for (int fy = 0; fy < h; fy++) {
		short* fb_ptr = fb + (h * FLIP_Y / h) * w;

		for (int fx = 0; fx < w; fx++) {
			ptr[fx] = fb_ptr[w * fx / w];
		}

		ptr += VRAM_WIDTH;
	}
#endif

#if defined(OGL) || defined(OGLES)
	free(data);
#elif defined(D3D9) || defined(XED3D)
	dstSurface->UnlockRect();

	dstSurface->Release();
	srcSurface->Release();
#elif defined(D3D11)
	d3dcontext->Unmap(newBackBuffer, subResource);
	newBackBuffer->Release();
#endif

	#undef FLIP_Y

	free(fb);

	vram_need_update = TRUE;
}
#endif

void Emulator_CopyVRAM(unsigned short *src, int x, int y, int w, int h, int dst_x, int dst_y)
{
	vram_need_update = TRUE;

    int stride = w;

    if (!src) {
        src    = vram;
        stride = VRAM_WIDTH;
    }

    src += x + y * stride;

    unsigned short *dst = vram + dst_x + dst_y * VRAM_WIDTH;

    for (int i = 0; i < h; i++) {
        memcpy(dst, src, w * sizeof(short));
        dst += VRAM_WIDTH;
        src += stride;
    }
}

void Emulator_ReadVRAM(unsigned short *dst, int x, int y, int dst_w, int dst_h)
{
	unsigned short *src = vram + x + VRAM_WIDTH * y;

	for (int i = 0; i < dst_h; i++) {
		memcpy(dst, src, dst_w * sizeof(short));
		dst += dst_w;
		src += VRAM_WIDTH;
	}
}

#if !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12) 
void Emulator_UpdateVRAM()
{
	if (!vram_need_update) {
		return;
	}
	vram_need_update = FALSE;

#if defined(OGL) || defined(OGLES)
	glBindTexture(GL_TEXTURE_2D, vramTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, VRAM_WIDTH, VRAM_HEIGHT, VRAM_FORMAT, GL_UNSIGNED_BYTE, vram);
#elif defined(D3D9) || defined(XED3D)
	D3DLOCKED_RECT rect;
	HRESULT hr = vramTexture->LockRect(0, &rect, NULL, 0);
	assert(!FAILED(hr));
#if defined(D3D9)
	memcpy(rect.pBits, vram, VRAM_WIDTH * VRAM_HEIGHT * sizeof(short));
#elif defined(XED3D)
	XGTEXTURE_DESC srcDesc;
	memset(&srcDesc, 0, sizeof(XGTEXTURE_DESC));
	XGGetTextureDesc((D3DBaseTexture*)vramTexture, 0, &srcDesc);
	//Have to tile the texture for X360 GPU.
	XGTileSurface(rect.pBits, rect.Pitch / srcDesc.BytesPerBlock, srcDesc.HeightInBlocks, NULL, vram, rect.Pitch, NULL, srcDesc.BytesPerBlock);
#if 0
	unsigned short* pPixel = (unsigned short*)rect.pBits;
	for(int y = 0; y < VRAM_WIDTH; y++)
	{
		for(int x = 0; x < VRAM_HEIGHT; x++)
		{
			unsigned short pixel = pPixel[x + rect.Pitch * y];
			unsigned char temp = pixel & 0xFF;
			pixel >>= 8;
			pixel |= temp << 8;
			pPixel[x + rect.Pitch * y] = pixel;
		}
	}
#endif
#endif
	vramTexture->UnlockRect(0);
#elif defined(D3D11)
	D3D11_MAPPED_SUBRESOURCE sr;
	HRESULT hr = d3dcontext->Map(vramBaseTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &sr);
	assert(!FAILED(hr));
	memcpy(sr.pData, vram, VRAM_WIDTH * VRAM_HEIGHT * sizeof(short));
	d3dcontext->Unmap(vramBaseTexture, 0);
#elif defined(D3D12)

	if (begin_commands_flag)
	{
		CD3DX12_RANGE readRange(0, 0);

		void* pTextureData = NULL;

		HRESULT hr = vramBaseTexture->Map(0, &readRange, (void**)&pTextureData);
		assert(!FAILED(hr));
		memcpy(pTextureData, vram, (VRAM_WIDTH * VRAM_HEIGHT) * sizeof(short));
		vramBaseTexture->Unmap(0, NULL);

		D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
		footprint.Footprint.Width = VRAM_WIDTH;
		footprint.Footprint.Height = VRAM_HEIGHT;
		footprint.Footprint.Depth = 1;
		footprint.Footprint.RowPitch = VRAM_WIDTH * sizeof(short);
		footprint.Footprint.Format = DXGI_FORMAT_R8G8_UNORM;

		CD3DX12_TEXTURE_COPY_LOCATION src(vramBaseTexture, footprint);
		CD3DX12_TEXTURE_COPY_LOCATION dest(vramTexture.m_textureResource, 0);

		commandList->CopyTextureRegion(&dest, 0, 0, 0, &src, NULL);
	}

#elif defined(VULKAN)

	TextureID newVramTexture;
	unsigned int texWidth = VRAM_WIDTH;
	unsigned int texHeight = VRAM_HEIGHT;
	unsigned int imageSize = texWidth * texHeight * sizeof(unsigned short);

	Emulator_CreateVulkanBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, newVramTexture.stagingBuffer, newVramTexture.stagingBufferMemory);

	void* data = NULL;
	vkMapMemory(device, newVramTexture.stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, vram, imageSize);
	vkUnmapMemory(device, newVramTexture.stagingBufferMemory);

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		texWidth,
		texHeight,
		1
	};

	VkCommandBuffer buff = Emulator_BeginSingleTimeCommands();

	Emulator_TransitionImageLayout(vramTexture.textureImage, VK_FORMAT_R8G8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	vkCmdCopyBufferToImage(buff, newVramTexture.stagingBuffer, vramTexture.textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	Emulator_EndSingleTimeCommands(buff);

	Emulator_TransitionImageLayout(vramTexture.textureImage, VK_FORMAT_R8G8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(device, newVramTexture.stagingBuffer, NULL);
	vkFreeMemory(device, newVramTexture.stagingBufferMemory, NULL);

#endif
}
#endif

void Emulator_BlitVRAM()
{
	if (activeDispEnv.isinter)
	{
		//Emulator_StoreFrameBuffer(activeDispEnv.disp.x, activeDispEnv.disp.y, activeDispEnv.disp.w, activeDispEnv.disp.h);
		return;
	}

	Emulator_SetTextureAndShader(vramTexture, Emulator_GetGTEShader(TF_COUNT));
	Emulator_SetBlendMode(BM_NONE);

#if defined(_PATCH2)
	u_char l, t, r, b; 

	if (activeDispEnv.disp.x != activeDrawEnv.clip.x || activeDispEnv.disp.y != activeDrawEnv.clip.y)
	{
		l = activeDrawEnv.clip.x / 8;
		t = activeDrawEnv.clip.y / 8;
		r = activeDrawEnv.clip.w / 8 + l;
		b = activeDrawEnv.clip.h / 8 + t;
	}
	else
	{
		l = activeDispEnv.disp.x / 8;
		t = activeDispEnv.disp.y / 8;
		r = activeDispEnv.disp.w / 8 + l;
		b = activeDispEnv.disp.h / 8 + t;
	}
#else
	u_char l = activeDispEnv.disp.x / 8;
	u_char t = activeDispEnv.disp.y / 8;
	u_char r = activeDispEnv.disp.w / 8 + l;
	u_char b = activeDispEnv.disp.h / 8 + t;
#endif

	struct Vertex blit_vertices[] =
	{
		{ +1, +1,    0, 0,    r, t,    0, 0,    0, 0, 0, 0 },
		{ -1, -1,    0, 0,    l, b,    0, 0,    0, 0, 0, 0 },
		{ -1, +1,    0, 0,    l, t,    0, 0,    0, 0, 0, 0 },

		{ +1, -1,    0, 0,    r, b,    0, 0,    0, 0, 0, 0 },
		{ -1, -1,    0, 0,    l, b,    0, 0,    0, 0, 0, 0 },
		{ +1, +1,    0, 0,    r, t,    0, 0,    0, 0, 0, 0 },
	};

#if defined(_PATCH)
	Emulator_SetViewPort(0.0f, 0.0f, windowWidth, windowHeight);
	Emulator_Ortho2D(0.0f, windowWidth, windowHeight, 0.0f, 0.0f, 1.0f);
#endif

	Emulator_UpdateVertexBuffer(blit_vertices, 6);
	Emulator_SetWireframe(FALSE);

	Emulator_DrawTriangles(0, 2);
	Emulator_SetWireframe(g_wireframeMode);
}

void Emulator_DoDebugKeys(int nKey, int down); // forward decl

#if defined(TOUCH_UI)

unsigned short resultTouchKeysPressed = 0;

struct Quad
{
	DVECTOR p[4];
};

int Emulator_IsPointInSquare(int x, int y, struct Quad* q)
{
	short maxX = q->p[0].vx;
	short minX = q->p[0].vx;
	short maxY = q->p[0].vy;
	short minY = q->p[0].vy;

	for (int i = 0; i < 4; i++)
	{
		if (maxX < q->p[i].vx)
		{
			maxX = q->p[i].vx;
		}

		if (q->p[i].vx < minX)
		{
			minX = q->p[i].vx;
		}

		if (maxY < q->p[i].vy)
		{
			maxY = q->p[i].vy;
		}

		if (q->p[i].vy < minY)
		{
			minY = q->p[i].vy;
		}
	}

	if (x < minX || x > maxX || y < minY || y > maxY) 
	{
		return FALSE;
	}

	return TRUE;
}

void Emulator_HandleTouchEvent(int x, int y)
{
	int rx = (x * 512) / windowWidth;
	int ry  = (y * 240) / windowHeight;

	int dist = 16;
	int cx = 32;
	int cy = 172;

	unsigned short mapper[4] = {
		0x8,
		0x1,
		0x2,
		0x4,
	};
	
	//printf("X: %d, Y: %d, RX: %d, RY: %d\n", x, y, rx, ry);

	for (int i = 0; i < 4; i++)
	{
		int dx = (i % 2) ? 0 : 1;
		int dy = dx ? 0 : 1;
		int ndist = (i >= 2) ? dist : -dist;

		int mx = dx ? ndist * 2 : 0;
		int my = dy ? ndist * 2 : 0;

		struct Quad q;

		q.p[0].vx = cx + mx;
		q.p[0].vy = cy + my;

		q.p[1].vx = cx + mx + 32;
		q.p[1].vy = cy + my;

		q.p[2].vx = cx + mx;
		q.p[2].vy = cy + my + 32;

		q.p[3].vx = cx + mx + 32;
		q.p[3].vy = cy + my + 32;

		if (Emulator_IsPointInSquare(rx, ry, &q))
		{
			resultTouchKeysPressed |= mapper[i] << 4;
		}
	}

	cx = 512 - 64;
	cy = 172;
	
	for (int i = 0; i < 4; i++)
	{
		int dx = (i % 2) ? 0 : 1;
		int dy = dx ? 0 : 1;
		int ndist = (i >= 2) ? dist : -dist;

		int mx = dx ? ndist * 2 : 0;
		int my = dy ? ndist * 2 : 0;

		struct Quad q;

		q.p[0].vx = cx + mx;
		q.p[0].vy = cy + my;

		q.p[1].vx = cx + mx + 32;
		q.p[1].vy = cy + my;

		q.p[2].vx = cx + mx;
		q.p[2].vy = cy + my + 32;

		q.p[3].vx = cx + mx + 32;
		q.p[3].vy = cy + my + 32;

		if (Emulator_IsPointInSquare(rx, ry, &q))
		{
			resultTouchKeysPressed |= mapper[i] << 12;
		}
	}

	cx = 512 / 2;
	cy = 240 - 32;
	dist = 32;

	for (int i = 0; i < 2; i++)
	{
		int dx = (i % 2) ? 0 : 1;
		int ndist = (i != 0) ? dist : -dist;
		int mx = dx ? ndist * 2 : 0;

		struct Quad q;

		q.p[0].vx = cx + mx;
		q.p[0].vy = cy;

		q.p[1].vx = cx + mx + 32;
		q.p[1].vy = cy;

		q.p[2].vx = cx + mx;
		q.p[2].vy = cy + 16;

		q.p[3].vx = cx + mx + 32;
		q.p[3].vy = cy + 16;

		if (Emulator_IsPointInSquare(rx, ry, &q))
		{
			resultTouchKeysPressed |= mapper[!i];
		}
	}
}
#endif

void Emulator_DoPollEvent()
{
#if defined(SDL2)
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
#if defined(TOUCH_UI)
			case SDL_MOUSEBUTTONUP:
				Emulator_HandleTouchEvent(event.button.x, event.button.y);
				break;
			case SDL_FINGERUP:
				Emulator_HandleTouchEvent((int)event.tfinger.x, (int)event.tfinger.y);
			break;
#endif
			case SDL_CONTROLLERDEVICEADDED:
				Emulator_AddController(event.cdevice.which);
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				SDL_GameControllerClose(padHandle[event.cdevice.which]);
				break;
			case SDL_QUIT:
				Emulator_ShutDown();
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					windowWidth = event.window.data1;
					windowHeight = event.window.data2;

#if defined(__EMSCRIPTEN__)
					SDL_SetWindowSize(g_window, windowWidth, windowHeight);
#endif
					g_resetDeviceOnNextFrame = TRUE;
					break;
				case SDL_WINDOWEVENT_CLOSE:
					Emulator_ShutDown();
					break;
				}
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			{
				int nKey = event.key.keysym.scancode;

				// lshift/right shift
				if (nKey == SDL_SCANCODE_RSHIFT)
					nKey = SDL_SCANCODE_LSHIFT;
				else if (nKey == SDL_SCANCODE_RCTRL)
					nKey = SDL_SCANCODE_LCTRL;
				else if (nKey == SDL_SCANCODE_RALT)
					nKey = SDL_SCANCODE_LALT;
#if defined(_DEBUG)
				Emulator_DoDebugKeys(nKey, (event.type == SDL_KEYUP) ? FALSE : TRUE);
#endif
				break;
			}
		}
	}
#endif
}

int Emulator_BeginScene()
{
	Emulator_DoPollEvent();

	if (g_resetDeviceOnNextFrame == TRUE)
	{
		Emulator_ResetDevice();
		g_resetDeviceOnNextFrame = FALSE;
	}

	if (begin_scene_flag)
		return FALSE;

	assert(!begin_scene_flag);

#if defined(VULKAN)
	g_lastBoundTexture[0].textureImage = NULL;
	g_lastBoundTexture[1].textureImage = NULL;
#elif defined(D3D12)
	g_lastBoundTexture[0].m_textureResource = NULL;
	g_lastBoundTexture[1].m_textureResource = NULL;
#elif defined(PLATFORM_NX)
	//g_lastBoundTexture[0] = NULL;
	//g_lastBoundTexture[1] = NULL;
#else
	g_lastBoundTexture[0] = NULL;
	g_lastBoundTexture[1] = NULL;
#endif

#if defined(OGLES)
	glBindVertexArray(dynamic_vertex_array);
#elif defined(D3D9) || defined(XED3D)
	d3ddev->BeginScene();
	d3ddev->SetVertexDeclaration(vertexDecl);
	d3ddev->SetStreamSource(0, dynamic_vertex_buffer, 0, sizeof(Vertex));
#elif defined(D3D11)
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	d3dcontext->IASetVertexBuffers(0, 1, &dynamic_vertex_buffer, &stride, &offset);
#elif defined(D3D12)
	dynamic_vertex_buffer_index = 0;
	Emulator_UpdateVRAM();
	Emulator_BeginPass();

	dynamic_vertex_buffer_view.BufferLocation = dynamic_vertex_buffer[frameIndex]->GetGPUVirtualAddress();
	dynamic_vertex_buffer_view.StrideInBytes = sizeof(Vertex);
	dynamic_vertex_buffer_view.SizeInBytes = MAX_NUM_POLY_BUFFER_VERTICES * sizeof(Vertex);

	commandList->IASetVertexBuffers(0, 1, &dynamic_vertex_buffer_view);

#elif defined(VULKAN)
	dynamic_vertex_buffer_index = 0;
	Emulator_UpdateVRAM();
	Emulator_BeginPass();

	VkBuffer vertexBuffers[] = { dynamic_vertex_buffer };
	VkDeviceSize offsets[] = { 0 };

	if (g_vertexBufferMemoryBound == FALSE)
	{
		vkBindBufferMemory(device, dynamic_vertex_buffer, dynamic_vertex_buffer_memory, 0);
		g_vertexBufferMemoryBound = TRUE;
	}

	vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
#elif defined(PLATFORM_NX)
	dynamic_vertex_buffer_index = 0;

	Emulator_UpdateVRAM();
	Emulator_BeginPass();
	Emulator_SetVertexBuffer();
#endif

#if !defined(VULKAN) && !defined(D3D12) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12) 
	Emulator_UpdateVRAM();
#endif

	Emulator_SetViewPort(0, 0, windowWidth, windowHeight);

	begin_scene_flag = TRUE;

	if (g_wireframeMode)
	{
		Emulator_SetWireframe(TRUE);
	}

	return TRUE;
}

#if !defined(__EMSCRIPTEN__) && !defined(__ANDROID__)
int Emulator_DoesFileExist(const char* fileName)
{
	FILE* f = fopen(fileName, "rb");
	
	if (f != NULL)
	{
		fclose(f);
		return 1;
	}

	return 0;
}

char* screenshotExtensions[] = { "TGA", "BMP" };

int Emulator_GetScreenshotNumber(int mode)
{
	int fileNumber = 0;
	char buff[64];
	do
	{
		sprintf(buff, "SCREENSHOT_%d.%s", fileNumber++, screenshotExtensions[mode]);

	} while (Emulator_DoesFileExist(buff) == 1);

	return fileNumber - 1;
}

void Emulator_TakeScreenshot(int mode)
{
#if defined(SDL2)
	unsigned char* pixels = new unsigned char[windowWidth * windowHeight * sizeof(unsigned int)];

#if defined(OGL) || defined(OGLES)
	glReadPixels(0, 0, windowWidth, windowHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
#endif

	if (mode == SCREENSHOT_MODE_TGA)
	{
		SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, windowWidth, windowHeight, 8 * 4, windowWidth * 4, 0, 0, 0, 0);

		char buff[64];
		sprintf(buff, "SCREENSHOT_%d.%s", Emulator_GetScreenshotNumber(mode), screenshotExtensions[mode]);

		FILE* f = fopen(buff, "wb");
		unsigned char TGAheader[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };
		unsigned char header[6];
		header[0] = (windowWidth % 256);
		header[1] = (windowWidth / 256);
		header[2] = (windowHeight % 256);
		header[3] = (windowHeight / 256);
		header[4] = 32;
		header[5] = 0;

		fwrite(TGAheader, sizeof(unsigned char), 12, f);
		fwrite(header, sizeof(unsigned char), 6, f);

		struct pixel
		{
			unsigned char b;
			unsigned char g;
			unsigned char r;
			unsigned char a;
		};

		pixel* p = (pixel*)surface->pixels;

		for (int y = 0; y < windowHeight; y++)
		{
			for (int x = 0; x < windowWidth; x++)
			{
				unsigned char temp = p->b;
				p->b = p->r;
				p->r = temp;
				p++;
			}
		}

		fwrite(surface->pixels, windowWidth * windowHeight * sizeof(unsigned int), 1, f);
		fclose(f);

		SDL_FreeSurface(surface);
	}
	else if (mode == SCREENSHOT_MODE_BMP)
	{
		SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, windowWidth, windowHeight, 8 * 4, windowWidth * 4, 0, 0, 0, 0);
		char buff[64];
		sprintf(buff, "SCREENSHOT_%d.%s", Emulator_GetScreenshotNumber(mode), screenshotExtensions[mode]);
		SDL_SaveBMP(surface, buff);
		SDL_FreeSurface(surface);
	}
	
	delete[] pixels;
#endif
}
#endif

void Emulator_CreateMemoryCard(int channel)
{
	char buf[16];
	sprintf(&buf[0], "%ld.MCD", channel);
	FILE* f = fopen(buf, "wb+");
#define MEMCARD_LENGTH 0x1C000
	unsigned short magic = 0x434D;
	fwrite(&magic, sizeof(unsigned short), 1, f);

	fseek(f, 125, SEEK_CUR);

	unsigned char unk4 = 0xE;
	fwrite(&unk4, sizeof(unsigned char), 1, f);

	for (int i = 0; i < 15; i++)
	{
		unsigned char unk1 = 0xA0;

		fwrite(&unk1, sizeof(unsigned char), 1, f);
		fseek(f, 7, SEEK_CUR);

		unsigned short unk2 = 0xFFFF;
		fwrite(&unk2, sizeof(unsigned short), 1, f);
		fseek(f, 117, SEEK_CUR);

		fwrite(&unk1, sizeof(unsigned char), 1, f);
	}

	for (int i = 0; i < 20; i++)
	{
		unsigned int unk3 = 0xFFFFFFFF;
		fwrite(&unk3, sizeof(unsigned int), 1, f);
		fseek(f, 124, SEEK_CUR);
	}

	for (int i = 0; i < 864; i++)
	{
		unsigned int unk5 = 0xFFFFFFFF;
		fwrite(&unk5, sizeof(unsigned int), 1, f);
	}

	fwrite(&magic, sizeof(unsigned short), 1, f);

	fseek(f, 125, SEEK_CUR);

	unk4 = 0xE;
	fwrite(&unk4, sizeof(unsigned char), 1, f);

	for (int i = 0; i < 30720; i++)
	{
		unsigned int unk6 = 0xFFFFFFFF;
		fwrite(&unk6, sizeof(unsigned int), 1, f);
	}

	fclose(f);
}

void Emulator_DoDebugKeys(int nKey, int down)
{
#if defined(SDL2)

	if (nKey == SDL_SCANCODE_BACKSPACE)
	{
		if(down)
		{
			if (g_swapInterval != 0)
			{
				g_swapInterval = 0;
#if !defined(VULKAN)
				Emulator_ResetDevice();
#endif
			}
		}
		else
		{
			if (g_swapInterval != SWAP_INTERVAL)
			{
				g_swapInterval = SWAP_INTERVAL;
#if !defined(VULKAN)
				Emulator_ResetDevice();
#endif
			}
		}
	}

	if (!down)
	{
		switch (nKey)
		{
			case SDL_SCANCODE_1:
				g_wireframeMode ^= 1;
				break;

			case SDL_SCANCODE_2:
				g_texturelessMode ^= 1;
				break;

			case SDL_SCANCODE_3:
				g_emulatorPaused ^= 1;
				break;
			case SDL_SCANCODE_UP:
			case SDL_SCANCODE_DOWN:
				if (g_emulatorPaused)
					g_polygonSelected += (nKey == SDL_SCANCODE_UP) ? 3 : -3;
				break;

#if !defined(__EMSCRIPTEN__) && !defined(__ANDROID__)
			case SDL_SCANCODE_4:
				Emulator_TakeScreenshot(SCREENSHOT_MODE_TGA);
				break;
			case SDL_SCANCODE_5:
				Emulator_SaveVRAM("VRAM.TGA", 0, 0, VRAM_WIDTH, VRAM_HEIGHT, TRUE);
				break;
			case SDL_SCANCODE_6:
#if !defined(NO_BOUNTY_LIST_EXPORT)
				Emulator_SaveBountyList();
#endif
				break;
#endif
		}
	}
#endif
}

unsigned short kbInputs = 0xFFFF;

#if defined(SDL2)
void Emulator_RumbleGameController(SDL_GameController* pad, unsigned char* padRumbleData)
{
#define PSX_MIN 0
#define PSX_MAX 255

#define SDL_MIN 0
#define SDL_MAX 65535

#define TRANSLATE(x) ((SDL_MAX - SDL_MIN) * (x - PSX_MIN) / (PSX_MAX - PSX_MIN)) + SDL_MIN

	if (padRumbleData != NULL)
	{
		SDL_GameControllerRumble(pad, TRANSLATE(padRumbleData[1]), TRANSLATE(padRumbleData[1]), 100);

		if (SDL_JoystickHasRumbleTriggers(SDL_GameControllerGetJoystick(pad)))
		{
			SDL_GameControllerRumbleTriggers(pad, TRANSLATE(padRumbleData[1]), TRANSLATE(padRumbleData[1]), 100);
		}
	}
}

void Emulator_TranslateControllerType(void* padData, SDL_GameController* padHandle)
{
	struct PadData
	{
		unsigned char status;
		unsigned char size : 4;
		unsigned char type : 4;
	};

	PadData* pd = (PadData*)padData;

	bool hasLeftAnalog = SDL_GameControllerHasAxis(padHandle, SDL_CONTROLLER_AXIS_LEFTX) & SDL_GameControllerHasAxis(padHandle, SDL_CONTROLLER_AXIS_LEFTY);
	bool hasRightAnalog = SDL_GameControllerHasAxis(padHandle, SDL_CONTROLLER_AXIS_RIGHTX) & SDL_GameControllerHasAxis(padHandle, SDL_CONTROLLER_AXIS_RIGHTY);

	if (hasLeftAnalog && hasRightAnalog)
	{
		//Analog controller
		pd->type = 5;
		pd->size = 3;
	}
	else
	{
		//Non-analog controller.
		pd->type = 4;
		pd->size = 1;
	}

	pd->status = 0;
}
#endif

void Emulator_UpdateInput(int poll)
{
	// also poll events here
	if (poll)
	{
		Emulator_DoPollEvent();
	}

#if defined(SDL2)

	if (padAllowCommunication)
	{
		kbInputs = UpdateKeyboardInput();
	}
	else
	{
		kbInputs = 0xFFFFu;
	}

	//Update pad
	if (SDL_NumJoysticks() > 0)
	{
		for (int i = 0; i < MAX_CONTROLLERS; i++)
		{
			if (padHandle[i] != NULL && padAllowCommunication)
			{
				unsigned short controllerInputs = UpdateGameControllerInput(padHandle[i]);

				Emulator_TranslateControllerType(padData[i], padHandle[i]);
				((unsigned short*)padData[i])[1] = kbInputs & controllerInputs;

				unsigned short analogData[2];
				UpdateGameControllerAnalogInput(padHandle[i], &analogData[0], &analogData[1]);

				((unsigned short*)padData[i])[2] = analogData[0];
				((unsigned short*)padData[i])[3] = analogData[1];

				if (SDL_GameControllerHasRumble(padHandle[i]))
				{
					Emulator_RumbleGameController(padHandle[i], padRumbleData[i]);
				}
			}
			else
			{
				unsigned short controllerInputs = 0xFFFFu;
				((unsigned short*)padData[i])[1] = kbInputs & controllerInputs;
			}
		}
	}
	else
	{
		//Update keyboard
		if (padData[0] != NULL)
		{
			Emulator_TranslateControllerType(padData[0], padHandle[0]);
			((unsigned short*)padData[0])[1] = kbInputs;
			((unsigned short*)padData[0])[2] = 128;//Maybe not required.
			((unsigned short*)padData[0])[3] = 128;
		}
	}
#elif defined(PLATFORM_NX)
	extern void Emulator_UpdateInputNVN();
	Emulator_UpdateInputNVN();

	if (padAllowCommunication && padData[0] != NULL)
	{
		extern unsigned short UpdateTouchInput();
		kbInputs = UpdateTouchInput();

		((unsigned short*)padData[0])[1] = kbInputs;
		((unsigned short*)padData[0])[2] = 128;//Maybe not required.
		((unsigned short*)padData[0])[3] = 128;
	}
	else
	{
		kbInputs = 0xFFFFu;
	}
#endif

#if !defined(__ANDROID__)
    ///@TODO SDL_NumJoysticks always reports > 0 for some reason on Android.
    //((unsigned short*)padData[0])[1] = UpdateKeyboardInput();
#endif
}

void Emulator_UpdateInputDebug()
{
	// also poll events here
	Emulator_DoPollEvent();

#if defined(SDL2)
	kbInputs = UpdateKeyboardInputDebug();

	//Update pad
	if (SDL_NumJoysticks() > 0)
	{
		for (int i = 0; i < MAX_CONTROLLERS; i++)
		{
			if (padHandle[i] != NULL)
			{
				unsigned short controllerInputs = UpdateGameControllerInput(padHandle[i]);

				padData[i][0] = 0;
				padData[i][1] = 0x41;//?
				((unsigned short*)padData[i])[1] = kbInputs & controllerInputs;
			}
		}
	}

#endif

#if defined(__ANDROID__) || defined(__EMSCRIPTEN__)
	///@TODO SDL_NumJoysticks always reports > 0 for some reason on Android.
	((unsigned short*)padData[0])[1] = UpdateKeyboardInput();
#endif
}

unsigned int Emulator_GetFPS()
{
#if defined(SDL2)
#define FPS_INTERVAL 1.0

	static unsigned int lastTime = SDL_GetTicks();
	static unsigned int currentFps = 0;
	static unsigned int passedFrames = 0;

	passedFrames++;
	if (lastTime < SDL_GetTicks() - FPS_INTERVAL * 1000)
	{
		lastTime = SDL_GetTicks();
		currentFps = passedFrames;
		passedFrames = 0;
	}

	return currentFps;
#else
	return 0;
#endif
	
}

#if !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12) 
void Emulator_SwapWindow()
{
	unsigned int timer = 1;

#if defined(SINGLE_THREADED)
	Emulator_CounterWrapper(0, &timer);
#endif

	Emulator_WaitForTimestep(1);

#if defined(RO_DOUBLE_BUFFERED)
#if defined(OGL)
	SDL_GL_SwapWindow(g_window);
#elif defined(OGLES)
	eglSwapBuffers(eglDisplay, eglSurface);
#elif defined(D3D9) || defined(XED3D)
	if (d3ddev->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST) {
		Emulator_ResetDevice();
	}
#elif defined(D3D11) || defined(D3D12)

	HRESULT hr = swapChain->Present(g_swapInterval, 0);
	
#if defined(D3D12)
	Emulator_WaitForPreviousFrame();
#endif

	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
		Emulator_ResetDevice();
	}

#elif defined(VULKAN)
	VkPipelineStageFlags waitDestStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	VkResult vkr = vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]);
	if (vkr != VK_SUCCESS)
	{
		eprinterr("Failed to submit queue!\n");
		assert(FALSE);
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapchain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(presentQueue, &presentInfo);

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
#else
	//#error
#endif
#else
	glFinish();
#endif
}
#endif

#if !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12) 
void Emulator_WaitForTimestep(int count)
{
#if defined(SDL2)
	if (g_swapInterval > 0) 
	{
		int delta = g_swapTime + FIXED_TIME_STEP*count - SDL_GetTicks();

		if (delta > 0) {
			SDL_Delay(delta);
		}
	}

	g_swapTime = SDL_GetTicks();
#elif defined(XED3D) || defined(D3D11)
	if (g_swapInterval > 0) 
	{
		int delta = g_swapTime + FIXED_TIME_STEP*count - GetTickCount();

		if (delta > 0) {
			DWORD dwStart = GetTickCount();

		while(( GetTickCount() - dwStart ) < delta ){ }

		}
	}

	g_swapTime = GetTickCount();
#endif
}
#endif

void Emulator_EndScene()
{
#if defined(VULKAN) || defined(D3D12) || defined(PLATFORM_NX)

	dynamic_vertex_buffer_index = 0;

	Emulator_EndPass();
#endif

	if (!begin_scene_flag)
		return;

	if (!vbo_was_dirty_flag)
		return;

	assert(begin_scene_flag);

	if (g_wireframeMode)
	{
		Emulator_SetWireframe(FALSE);
	}

#if defined(OGL) || defined(OGLES)
	//glBindVertexArray(0);
#elif defined(D3D9) || defined(XED3D)
	d3ddev->EndScene();
#elif defined(D3D12)

#elif defined(VULKAN)

#endif

	begin_scene_flag = FALSE;
	vbo_was_dirty_flag = FALSE;

	Emulator_SwapWindow();
}

void Emulator_ResetTouchInput()
{
	resultTouchKeysPressed = 0;
}

void Emulator_ShutDown()
{
#if defined(OGL) || defined(OGLES)
	Emulator_DestroyTexture(vramTexture);
	Emulator_DestroyTexture(whiteTexture);
	Emulator_DestroyTexture(rg8lutTexture);
#endif

#if defined(SDL2)
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
#endif

#if defined(D3D9) || defined(XED3D)
	d3ddev->Release();
	d3d->Release();
	///@TODO release shaders.
#elif defined(D3D11)
	d3ddev->Release();
	d3dcontext->Release();
	vramBaseTexture->Release();
	///@TODO release shaders.
#elif defined(D3D12)
	///@TODO D3D12
#endif

#if defined(SDL2)
	SDL_DestroyWindow(g_window);
	SDL_Quit();
#endif

#if !defined(SN_TARGET_PSP2)
	exit(EXIT_SUCCESS);
#endif
}

#if !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12) 
void Emulator_SetBlendMode(enum BlendMode blendMode)
{
	if (g_PreviousBlendMode == blendMode)
	{
		return;
	}

#if defined(OGL) || defined(OGLES)
	if (g_PreviousBlendMode == BM_NONE)
	{
		glEnable(GL_BLEND);
	}

	switch (blendMode)
	{
	case BM_NONE:
		glDisable(GL_BLEND);
		break;
	case BM_AVERAGE:
		glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_COLOR);
		glBlendEquation(GL_FUNC_ADD);
		break;
	case BM_ADD:
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_FUNC_ADD);
		break;
	case BM_SUBTRACT:
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		break;
	case BM_ADD_QUATER_SOURCE:
		glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE);
		glBlendEquation(GL_FUNC_ADD);
		break;
	}
#elif defined(D3D9) || defined(XED3D)
	if (g_PreviousBlendMode == BM_NONE)
	{
		d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	}

	switch (blendMode)
	{
	case BM_NONE:
		d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		break;
	case BM_AVERAGE:
		d3ddev->SetRenderState(D3DRS_BLENDFACTOR, D3DCOLOR_RGBA(128, 128, 128, 128));
		d3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
		d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BLENDFACTOR);
		break;
	case BM_ADD:
		d3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case BM_SUBTRACT:
		d3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case BM_ADD_QUATER_SOURCE:
		d3ddev->SetRenderState(D3DRS_BLENDFACTOR, D3DCOLOR_RGBA(64, 64, 64, 64));
		d3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
		d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	}
#elif defined(D3D11)
	if (blendState != NULL)
	{
		blendState->Release();
		blendState = NULL;
	}

	if (g_PreviousBlendMode == BM_NONE)
	{
		D3D11_BLEND_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.AlphaToCoverageEnable = FALSE;
		bd.IndependentBlendEnable = FALSE;
		bd.RenderTarget[0].BlendEnable = TRUE;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		HRESULT hr = d3ddev->CreateBlendState(&bd, &blendState);
		assert(SUCCEEDED(hr));
		FLOAT blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		d3dcontext->OMSetBlendState(blendState, blendFactor, -1);
	}

	switch (blendMode)
	{
	case BM_NONE:
	{
		d3dcontext->OMSetBlendState(NULL, 0, -1);
		break;
	}
	case BM_AVERAGE:
	{
		D3D11_BLEND_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.AlphaToCoverageEnable = FALSE;
		bd.IndependentBlendEnable = TRUE;
		bd.RenderTarget[0].BlendEnable = TRUE;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_BLEND_FACTOR;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_BLEND_FACTOR;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_BLEND_FACTOR;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_BLEND_FACTOR;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		HRESULT hr = d3ddev->CreateBlendState(&bd, &blendState);
		assert(SUCCEEDED(hr));
		FLOAT blendFactor[4] = { 128.0f * (1.0f / 255.0f), 128.0f * (1.0f / 255.0f), 128.0f * (1.0f / 255.0f), 128.0f * (1.0f / 255.0f) };
		d3dcontext->OMSetBlendState(blendState, blendFactor, -1);
		break;
	}
	case BM_ADD:
	{
		D3D11_BLEND_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.AlphaToCoverageEnable = FALSE;
		bd.IndependentBlendEnable = TRUE;
		bd.RenderTarget[0].BlendEnable = TRUE;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		HRESULT hr = d3ddev->CreateBlendState(&bd, &blendState);
		assert(SUCCEEDED(hr));
		FLOAT blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		d3dcontext->OMSetBlendState(blendState, blendFactor, -1);
		break;
	}
	case BM_SUBTRACT:
	{
		D3D11_BLEND_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.AlphaToCoverageEnable = FALSE;
		bd.IndependentBlendEnable = TRUE;
		bd.RenderTarget[0].BlendEnable = TRUE;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_REV_SUBTRACT;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		HRESULT hr = d3ddev->CreateBlendState(&bd, &blendState);
		assert(SUCCEEDED(hr));
		FLOAT blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		d3dcontext->OMSetBlendState(blendState, blendFactor, -1);
		break;
	}
	case BM_ADD_QUATER_SOURCE:
	{
		D3D11_BLEND_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.AlphaToCoverageEnable = FALSE;
		bd.IndependentBlendEnable = TRUE;
		bd.RenderTarget[0].BlendEnable = TRUE;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_BLEND_FACTOR;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_BLEND_FACTOR;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		HRESULT hr = d3ddev->CreateBlendState(&bd, &blendState);
		assert(SUCCEEDED(hr));
		FLOAT blendFactor[4] = { 64.0f * (1.0f / 255.0f), 64.0f * (1.0f / 255.0f), 64.0f * (1.0f / 255.0f), 64.0f * (1.0f / 255.0f) };
		d3dcontext->OMSetBlendState(blendState, blendFactor, -1);
		break;
	}
	}
#elif defined(D3D12)
	g_CurrentBlendMode = blendMode;
#elif defined(VULKAN)

	
	memset(&g_colorBlendAttachment, 0, sizeof(VkPipelineColorBlendAttachmentState));

	memset(&g_colorBlend, 0, sizeof(VkPipelineColorBlendStateCreateInfo));
	g_colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	g_colorBlend.logicOpEnable = VK_FALSE;
	g_colorBlend.logicOp = VK_LOGIC_OP_COPY;
	g_colorBlend.attachmentCount = 1;
	g_colorBlend.pAttachments = &g_colorBlendAttachment;

	g_colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	if (g_PreviousBlendMode == BM_NONE)
	{
		g_colorBlendAttachment.blendEnable = VK_TRUE;
		g_colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		g_colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		g_colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		g_colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		g_colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		g_colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		g_colorBlend.blendConstants[0] = 1.0f;
		g_colorBlend.blendConstants[1] = 1.0f;
		g_colorBlend.blendConstants[2] = 1.0f;
		g_colorBlend.blendConstants[3] = 1.0f;
	}

	switch (blendMode)
	{
	case BM_NONE:
	{
		g_colorBlendAttachment.blendEnable = VK_FALSE;
		g_colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		g_colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		g_colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		g_colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		g_colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		g_colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		g_colorBlend.blendConstants[0] = 1.0f;
		g_colorBlend.blendConstants[1] = 1.0f;
		g_colorBlend.blendConstants[2] = 1.0f;
		g_colorBlend.blendConstants[3] = 1.0f;
		break;
	}
	case BM_AVERAGE:
	{
		g_colorBlendAttachment.blendEnable = VK_TRUE;
		g_colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_CONSTANT_COLOR;
		g_colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_CONSTANT_COLOR;
		g_colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		g_colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_CONSTANT_COLOR;
		g_colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_CONSTANT_COLOR;
		g_colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		g_colorBlend.blendConstants[0] = 128.0f * (1.0f / 255.0f);
		g_colorBlend.blendConstants[1] = 128.0f * (1.0f / 255.0f);
		g_colorBlend.blendConstants[2] = 128.0f * (1.0f / 255.0f);
		g_colorBlend.blendConstants[3] = 128.0f * (1.0f / 255.0f);
		break;
	}
	case BM_ADD:
	{
		g_colorBlendAttachment.blendEnable = VK_TRUE;
		g_colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		g_colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
		g_colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		g_colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		g_colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		g_colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		g_colorBlend.blendConstants[0] = 1.0f;
		g_colorBlend.blendConstants[1] = 1.0f;
		g_colorBlend.blendConstants[2] = 1.0f;
		g_colorBlend.blendConstants[3] = 1.0f;
		break;
	}
	case BM_SUBTRACT:
	{
		g_colorBlendAttachment.blendEnable = VK_TRUE;
		g_colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		g_colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
		g_colorBlendAttachment.colorBlendOp = VK_BLEND_OP_REVERSE_SUBTRACT;
		g_colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		g_colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		g_colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_REVERSE_SUBTRACT;

		g_colorBlend.blendConstants[0] = 1.0f;
		g_colorBlend.blendConstants[1] = 1.0f;
		g_colorBlend.blendConstants[2] = 1.0f;
		g_colorBlend.blendConstants[3] = 1.0f;
		break;
	}
	case BM_ADD_QUATER_SOURCE:
	{
		g_colorBlendAttachment.blendEnable = VK_TRUE;
		g_colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_CONSTANT_COLOR;
		g_colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
		g_colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		g_colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_CONSTANT_COLOR;
		g_colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		g_colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		g_colorBlend.blendConstants[0] = 64.0f * (1.0f / 255.0f);
		g_colorBlend.blendConstants[1] = 64.0f * (1.0f / 255.0f);
		g_colorBlend.blendConstants[2] = 64.0f * (1.0f / 255.0f);
		g_colorBlend.blendConstants[3] = 64.0f * (1.0f / 255.0f);
		break;
	}
	}


#if defined(VULKAN)
	Emulator_CreatePipelineState(g_activeShader, &g_activeShader.GP, NULL);
	g_graphicsPipeline = g_activeShader.GP;

	if (begin_pass_flag)
	{
		vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, g_graphicsPipeline);
	}
#endif

#else
	//#error
#endif

	g_PreviousBlendMode = blendMode;
}
#endif

void Emulator_SetPGXPVertexCount(int vertexCount)
{
#if defined(PGXP)
	pgxp_vertex_count = vertexCount;
#endif
}

#if !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12) 
void Emulator_SetViewPort(int x, int y, int width, int height)
{
	float offset_x = (float)activeDispEnv.screen.x;
	float offset_y = (float)activeDispEnv.screen.y;

#if defined(OGL) || defined(OGLES)
	glViewport(x + offset_x, y + -offset_y, width, height);
#elif defined(D3D9) || defined(XED3D)
	D3DVIEWPORT9 viewport;
	viewport.X      = x + offset_x;
	viewport.Y      = y + offset_y;
	viewport.Width  = width;
	viewport.Height = height;
	viewport.MinZ   = 0.0f;
	viewport.MaxZ   = 1.0f;
	d3ddev->SetViewport(&viewport);
#elif defined(D3D11)
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX	= (float)x + offset_x;
	viewport.TopLeftY	= (float)y + offset_y;
	viewport.Width		= (float)width;
	viewport.Height		= (float)height;
	viewport.MinDepth	= 0.0f;
	viewport.MaxDepth	= 1.0f;
	d3dcontext->RSSetViewports(1, &viewport);
#elif defined(D3D12)
	D3D12_VIEWPORT viewport;
	viewport.TopLeftX = (float)x + offset_x;
	viewport.TopLeftY = (float)y + offset_y;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	commandList->RSSetViewports(1, &viewport);

	D3D12_RECT rect;
	rect.left = 0;
	rect.right = width;
	rect.top = 0;
	rect.bottom = height;
	commandList->RSSetScissorRects(1, &rect);
#elif defined(VULKAN)
	memset(&g_viewport, 0, sizeof(VkViewport));
	g_viewport.x = (float)x + offset_x;
	g_viewport.y = (float)y + offset_y + height;
	g_viewport.width = (float)width;
	g_viewport.height = -((float)height);
	g_viewport.minDepth = 0.0f;
	g_viewport.maxDepth = 1.0f;
	
	if (begin_pass_flag && !g_resetDeviceOnNextFrame)
	{
		vkCmdSetViewport(commandBuffers[currentFrame], 0, 1, &g_viewport);
	}
#else
	//#error
#endif
}

void Emulator_SetRenderTarget(const RenderTargetID frameBufferObject)
{
#if defined(OGL) || defined(OGLES)
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
#elif defined(D3D9) || defined(XED3D)
	d3ddev->SetRenderTarget(0, frameBufferObject);
#elif defined(D3D11)
	d3dcontext->OMSetRenderTargets(1, &frameBufferObject, NULL);
#elif defined(D3D12)
	
#elif defined(VULKAN)
#else
 //   #error
#endif
}

#endif

#if !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12) 
void Emulator_SetWireframe(int enable) 
{
#if defined(OGL)
	glPolygonMode(GL_FRONT_AND_BACK, enable ? GL_LINE : GL_FILL);
#elif defined(D3D9) || defined(XED3D)
	d3ddev->SetRenderState(D3DRS_FILLMODE, enable ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
#elif defined(D3D11) || defined(D3D12)
	Emulator_CreateRasterState(enable ? TRUE : FALSE);
#elif defined(VULKAN)
	Emulator_CreateRasterState(enable ? TRUE : FALSE);
#elif defined(OGLES)

#else
//#error
#endif
}
#endif

#if !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12) 
void Emulator_UpdateVertexBuffer(const struct Vertex *vertices, int num_vertices)
{
	assert(num_vertices <= MAX_NUM_POLY_BUFFER_VERTICES);

	if (num_vertices <= 0)
		return;

#if defined(OGL) || defined(OGLES)
	glBufferSubData(GL_ARRAY_BUFFER, 0, num_vertices * sizeof(Vertex), vertices);
#elif defined(D3D9) || defined(XED3D)
	void *ptr;
#if defined(D3D9)
	dynamic_vertex_buffer->Lock(0, 0, &ptr, D3DLOCK_DISCARD);
#elif defined(XED3D)
	d3ddev->SetStreamSource(0, NULL, 0, 0);
	dynamic_vertex_buffer->Lock(0, 0, &ptr, 0);
#endif
	memcpy(ptr, vertices, num_vertices * sizeof(Vertex));
	dynamic_vertex_buffer->Unlock();

#if defined(XED3D)
	d3ddev->SetStreamSource(0, dynamic_vertex_buffer, 0, sizeof(Vertex));
#endif
#elif defined(D3D11)
	D3D11_MAPPED_SUBRESOURCE sr;
	d3dcontext->Map(dynamic_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sr);
	memcpy(sr.pData, vertices, num_vertices * sizeof(Vertex));
	d3dcontext->Unmap(dynamic_vertex_buffer, 0);
#elif defined(D3D12)

	if (begin_commands_flag)
	{
		commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dynamic_vertex_buffer[frameIndex], D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_DEST));

		void* pMap = NULL;
		CD3DX12_RANGE readRange(0, 0);
		HRESULT hr = dynamic_vertex_buffer_heap[frameIndex]->Map(0, &readRange, &pMap);
		assert(SUCCEEDED(hr));

		memcpy((char*)pMap + dynamic_vertex_buffer_index * sizeof(Vertex), vertices, num_vertices * sizeof(Vertex));
		dynamic_vertex_buffer_heap[frameIndex]->Unmap(0, NULL);

		commandList->CopyBufferRegion(dynamic_vertex_buffer[frameIndex], dynamic_vertex_buffer_index * sizeof(Vertex), dynamic_vertex_buffer_heap[frameIndex], dynamic_vertex_buffer_index * sizeof(Vertex), num_vertices * sizeof(Vertex));
		commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dynamic_vertex_buffer[frameIndex], D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
	}

#elif defined(VULKAN)
	void* data = NULL;
	if (vkMapMemory(device, dynamic_vertex_buffer_memory, dynamic_vertex_buffer_index * sizeof(Vertex), num_vertices * sizeof(Vertex), 0, &data) != VK_SUCCESS)
	{
		eprinterr("Failed to map vertex buffer memory\n");
		assert(FALSE);
	}
	memcpy(data, vertices, num_vertices * sizeof(Vertex));
	vkUnmapMemory(device, dynamic_vertex_buffer_memory);
#else
	//#error
#endif

	vbo_was_dirty_flag = TRUE;
}
#endif

#if !defined(OGL) && !defined(D3D9) && !defined(D3D11) && !defined(OGLES) && !defined(SN_TARGET_PSP2) && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(D3D12) 
void Emulator_DrawTriangles(int start_vertex, int triangles)
{
	if(triangles <= 0)
		return;

	//Emulator_UpdateToActiveDrawEnv();

#if defined(OGL) || defined(OGLES)
	glDrawArrays(GL_TRIANGLES, start_vertex, triangles * 3);
#elif defined(D3D9) || defined(XED3D)
	d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, start_vertex, triangles);
#elif defined(D3D11)
	d3dcontext->Draw(triangles * 3, start_vertex);
#elif defined(D3D12)
	if (!g_resetDeviceOnNextFrame)
	{
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->DrawInstanced(triangles * 3, 1, dynamic_vertex_buffer_index, 0);
		dynamic_vertex_buffer_index += triangles * 3;
	}
#elif defined(VULKAN)
	if (!g_resetDeviceOnNextFrame)
	{
		vkCmdDraw(commandBuffers[currentFrame], triangles * 3, 1, dynamic_vertex_buffer_index, 0);
		dynamic_vertex_buffer_index += triangles * 3;
	}
#else
	//#error
#endif
}
#endif

#if defined(D3D12) && 0

void Emulator_CreateGraphicsPipelineState(ShaderID* shader, D3D12_GRAPHICS_PIPELINE_STATE_DESC* pso)
{
	for (int i = 0; i < BM_COUNT; i++)
	{
		switch (i)
		{
		case BM_NONE:
		{
			D3D12_BLEND_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.AlphaToCoverageEnable = FALSE;
			bd.IndependentBlendEnable = FALSE;
			bd.RenderTarget[0].BlendEnable = TRUE;
			bd.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
			bd.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
			bd.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			bd.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			bd.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			bd.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			bd.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

			pso->BlendState = bd;

			shader->BF[0] = 1.0f;
			shader->BF[1] = 1.0f;
			shader->BF[2] = 1.0f;
			shader->BF[3] = 1.0f;

			break;
		}
		case BM_AVERAGE:
		{
			D3D12_BLEND_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.AlphaToCoverageEnable = FALSE;
			bd.IndependentBlendEnable = TRUE;
			bd.RenderTarget[0].BlendEnable = TRUE;
			bd.RenderTarget[0].SrcBlend = D3D12_BLEND_BLEND_FACTOR;
			bd.RenderTarget[0].DestBlend = D3D12_BLEND_BLEND_FACTOR;
			bd.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			bd.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_BLEND_FACTOR;
			bd.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_BLEND_FACTOR;
			bd.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			bd.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

			pso->BlendState = bd;

			shader->BF[0] = 128.0f * (1.0f / 255.0f);
			shader->BF[1] = 128.0f * (1.0f / 255.0f);
			shader->BF[2] = 128.0f * (1.0f / 255.0f);
			shader->BF[3] = 128.0f * (1.0f / 255.0f);
			break;
		}
		case BM_ADD:
		{
			D3D12_BLEND_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.AlphaToCoverageEnable = FALSE;
			bd.IndependentBlendEnable = TRUE;
			bd.RenderTarget[0].BlendEnable = TRUE;
			bd.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
			bd.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			bd.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			bd.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			bd.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
			bd.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			bd.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

			pso->BlendState = bd;

			shader->BF[0] = 1.0f;
			shader->BF[1] = 1.0f;
			shader->BF[2] = 1.0f;
			shader->BF[3] = 1.0f;
			break;
		}
		case BM_SUBTRACT:
		{
			D3D12_BLEND_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.AlphaToCoverageEnable = FALSE;
			bd.IndependentBlendEnable = TRUE;
			bd.RenderTarget[0].BlendEnable = TRUE;
			bd.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
			bd.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			bd.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
			bd.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			bd.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
			bd.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_REV_SUBTRACT;
			bd.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

			pso->BlendState = bd;

			shader->BF[0] = 1.0f;
			shader->BF[1] = 1.0f;
			shader->BF[2] = 1.0f;
			shader->BF[3] = 1.0f;
			break;
		}
		case BM_ADD_QUATER_SOURCE:
		{
			D3D12_BLEND_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.AlphaToCoverageEnable = FALSE;
			bd.IndependentBlendEnable = TRUE;
			bd.RenderTarget[0].BlendEnable = TRUE;
			bd.RenderTarget[0].SrcBlend = D3D12_BLEND_BLEND_FACTOR;
			bd.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			bd.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			bd.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_BLEND_FACTOR;
			bd.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
			bd.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			bd.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

			pso->BlendState = bd;

			shader->BF[0] = 64.0f * (1.0f / 255.0f);
			shader->BF[1] = 64.0f * (1.0f / 255.0f);
			shader->BF[2] = 64.0f * (1.0f / 255.0f);
			shader->BF[3] = 64.0f * (1.0f / 255.0f);
			break;
		}
		}

		assert(!FAILED(d3ddev->CreateGraphicsPipelineState(pso, IID_PPV_ARGS(&shader->GPS[i]))));
	}
}

void Emulator_DestroyGlobalShaders()
{
	for (int i = 0; i < BM_COUNT; i++)
	{
		if (g_gte_shader_4.GPS[i] != NULL)
		{
			g_gte_shader_4.GPS[i]->Release();
			g_gte_shader_4.GPS[i] = NULL;
		}

		if (g_gte_shader_8.GPS[i] != NULL)
		{
			g_gte_shader_8.GPS[i]->Release();
			g_gte_shader_8.GPS[i] = NULL;
		}

		if (g_gte_shader_16.GPS[i] != NULL)
		{
			g_gte_shader_16.GPS[i]->Release();
			g_gte_shader_16.GPS[i] = NULL;
		}

		if (g_blit_shader.GPS[i] != NULL)
		{
			g_blit_shader.GPS[i]->Release();
			g_blit_shader.GPS[i] = NULL;
		}
	}

	if (g_gte_shader_4.RS != NULL)
	{
		g_gte_shader_4.RS->Release();
		g_gte_shader_4.RS = NULL;
	}

	if (g_gte_shader_8.RS != NULL)
	{
		g_gte_shader_8.RS->Release();
		g_gte_shader_8.RS = NULL;
	}

	if (g_gte_shader_16.RS != NULL)
	{
		g_gte_shader_16.RS->Release();
		g_gte_shader_16.RS = NULL;
	}

	if (g_blit_shader.RS != NULL)
	{
		g_blit_shader.RS->Release();
		g_blit_shader.RS = NULL;
	}
}

void Emulator_SetDefaultRenderTarget()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(renderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, renderTargetDescriptorSize);
	commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, NULL);
}

void Emulator_SetConstantBuffers()
{
	ID3D12DescriptorHeap* ppHeapsCBV[] = { projectionMatrixBufferHeap[frameIndex]};
	commandList->SetDescriptorHeaps(_countof(ppHeapsCBV), ppHeapsCBV);
	commandList->SetGraphicsRootDescriptorTable(2, projectionMatrixBufferHeap[frameIndex]->GetGPUDescriptorHandleForHeapStart());
}

void Emulator_CreateConstantBuffers()
{
	unsigned int projectionMatrixBufferSize = ((sizeof(float) * 16) + 255) & ~255;

	for (int i = 0; i < frameCount; i++)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = 1;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		HRESULT hr = d3ddev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&projectionMatrixBufferHeap[i]));
		assert(SUCCEEDED(hr));
	}

	for (int i = 0; i < frameCount; i++)
	{
		d3ddev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(projectionMatrixBufferSize), D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&projectionMatrixBuffer[i]));

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		ZeroMemory(&cbvDesc, sizeof(cbvDesc));
		cbvDesc.BufferLocation = projectionMatrixBuffer[i]->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = projectionMatrixBufferSize;
		d3ddev->CreateConstantBufferView(&cbvDesc, projectionMatrixBufferHeap[i]->GetCPUDescriptorHandleForHeapStart());
	
		void** pMap = NULL;
		CD3DX12_RANGE readRange(0, 0);
		HRESULT hr = projectionMatrixBuffer[i]->Map(0, &readRange, (void**)(&pMap));
		ZeroMemory(pMap, projectionMatrixBufferSize);
		projectionMatrixBuffer[i]->Unmap(0, NULL);
	}
}

void Emulator_UpdateProjectionConstantBuffer(float* ortho)
{
	void** pMap = NULL;
	CD3DX12_RANGE readRange(0, 0);
	HRESULT hr = projectionMatrixBuffer[frameIndex]->Map(0, &readRange, (void**)(&pMap));
	memcpy(pMap, ortho, (sizeof(float) * 16));
	projectionMatrixBuffer[frameIndex]->Unmap(0, &readRange);
}

void Emulator_EndCommandBuffer()
{
	commandList->Close();

	ID3D12CommandList* ppCommandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(sizeof(ppCommandLists) / sizeof(commandList), ppCommandLists);

	begin_commands_flag = FALSE;
}

void Emulator_EndPass()
{
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	Emulator_EndCommandBuffer();

	if (vram_need_update)
	{
		Emulator_UpdateVRAM();
	}

	begin_pass_flag = FALSE;
}

int Emulator_BeginCommandBuffer()
{
	if (begin_commands_flag)
	{
		return begin_commands_flag;
	}

	Emulator_WaitForPreviousFrame();

	int last_begin_commands_flag = begin_commands_flag;

	HRESULT hr = commandAllocator->Reset();
	assert(!FAILED(hr));
	hr = commandList->Reset(commandAllocator, NULL);
	assert(!FAILED(hr));

	begin_commands_flag = TRUE;

	return last_begin_commands_flag;
}

void Emulator_BeginPass()
{
	Emulator_BeginCommandBuffer();

	if (!begin_pass_flag)
	{
		commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(renderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, renderTargetDescriptorSize);
		commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, NULL);
	}

	begin_pass_flag = TRUE;
}

void Emulator_CreateRasterState(int wireframe)
{
	UNIMPLEMENTED();
}

void Emulator_WaitForPreviousFrame()
{
	const UINT64 fenceV = fenceValue[frameIndex];
	assert(SUCCEEDED(commandQueue->Signal(fence, fenceV)));
	fenceValue[frameIndex]++;

	if (fence->GetCompletedValue() < fenceV)
	{
		assert(SUCCEEDED(fence->SetEventOnCompletion(fenceV, fenceEvent)));
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	frameIndex = swapChain->GetCurrentBackBufferIndex();
}

#elif defined(VULKAN)
void Emulator_CreateRasterState(int wireframe)
{
	g_rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	g_rasterizer.depthClampEnable = VK_FALSE;
	g_rasterizer.rasterizerDiscardEnable = VK_FALSE;
	g_rasterizer.polygonMode = wireframe ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
	g_rasterizer.lineWidth = 1.0f;
	g_rasterizer.cullMode = VK_CULL_MODE_NONE;
	g_rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	g_rasterizer.depthBiasEnable = VK_FALSE;
}

void Emulator_CreatePipelineState(ShaderID& shader, VkPipeline* pipeline, VkPipelineColorBlendStateCreateInfo* colourBlendState)
{
	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	memset(&vertexInputInfo, 0, sizeof(VkPipelineVertexInputStateCreateInfo));

	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(g_attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &g_bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = g_attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly;
	memset(&inputAssembly, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkRect2D scissor;
	scissor.extent.width = windowWidth;
	scissor.extent.height = windowHeight;
	scissor.offset.x = 0;
	scissor.offset.y = 0;

	VkPipelineViewportStateCreateInfo viewportState;
	memset(&viewportState, 0, sizeof(VkPipelineViewportStateCreateInfo));
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &g_viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineMultisampleStateCreateInfo multisampling;
	memset(&multisampling, 0, sizeof(VkPipelineMultisampleStateCreateInfo));

	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineShaderStageCreateInfo shaderStages[] = { shader.VS, shader.PS };

	VkGraphicsPipelineCreateInfo pipelineInfo;
	memset(&pipelineInfo, 0, sizeof(VkGraphicsPipelineCreateInfo));
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &g_rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	if (colourBlendState == NULL)
	{
		pipelineInfo.pColorBlendState = &g_colorBlend;
	}
	else
	{
		pipelineInfo.pColorBlendState = colourBlendState;
	}

	pipelineInfo.layout = shader.PL;
	pipelineInfo.renderPass = render_pass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, pipeline) != VK_SUCCESS)
	{
		eprinterr("Failed to create graphics pipeline\n");
		assert(FALSE);
	}
}

VkCommandBuffer Emulator_BeginSingleTimeCommands() 
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void Emulator_EndSingleTimeCommands(VkCommandBuffer commandBuffer) 
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void Emulator_CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) 
{
	VkCommandBuffer commandBuffer = Emulator_BeginSingleTimeCommands();

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1
	};

	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	Emulator_EndSingleTimeCommands(commandBuffer);
}

void Emulator_TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) 
{
	VkCommandBuffer commandBuffer = Emulator_BeginSingleTimeCommands();

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && (newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL || newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR))
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else 
	{
		eprinterr("Unsupported layout transition!\n");
	}

	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, NULL, 0, NULL, 1, &barrier);

	Emulator_EndSingleTimeCommands(commandBuffer);
}

void Emulator_CreateVulkanBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, NULL, &buffer) != VK_SUCCESS) 
	{
		eprinterr("Failed to create buffer!\n");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = Emulator_FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) 
	{
		eprinterr("Failed to allocate buffer memory!\n");
	}

	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void Emulator_EndCommandBuffer()
{
	if (vkEndCommandBuffer(commandBuffers[currentFrame]) != VK_SUCCESS)
	{
		eprinterr("Failed to end command buffer!\n");
		assert(FALSE);
	}

	begin_commands_flag = FALSE;
}

void Emulator_EndPass()
{
	vkCmdEndRenderPass(commandBuffers[currentFrame]);

	Emulator_EndCommandBuffer();

	if (vram_need_update)
	{
		Emulator_UpdateVRAM();
	}

	begin_pass_flag = FALSE;
}

int Emulator_BeginCommandBuffer()
{
	if (begin_commands_flag)
	{
		return begin_commands_flag;
	}

	int last_begin_commands_flag = begin_commands_flag;

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS)
	{
		eprinterr("Failed to begin recording command buffer!\n");
		assert(FALSE);
	}

	begin_commands_flag = TRUE;

	return last_begin_commands_flag;
}

void Emulator_BeginPass()
{
	if (vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX) != VK_SUCCESS)
	{
		eprinterr("Failed to wait for fences!\n");
		assert(FALSE);
	}
	
	VkResult result = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		eprinterr("Failed to acquire next image!\n");
		assert(FALSE);
	}

	if (vkResetFences(device, 1, &inFlightFences[currentFrame]) != VK_SUCCESS)
	{
		eprinterr("Failed to reset fences!\n");
		assert(FALSE);
	}

	if (vkResetCommandBuffer(commandBuffers[currentFrame], 0) != VK_SUCCESS)
	{
		eprinterr("Failed to reset command buffer!\n");
		assert(FALSE);
	}

	Emulator_BeginCommandBuffer();

	VkExtent2D swapChainExtent;
	swapChainExtent.width = windowWidth;
	swapChainExtent.height = windowHeight;

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = render_pass;
	renderPassInfo.framebuffer = swapchainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, g_activeShader.PL, 0, 1, &descriptorSets[currentFrame][g_activeDescriptor], 0, NULL);
	
	begin_pass_flag = TRUE;
}

VkImageView Emulator_CreateImageView(VkImage image, VkFormat format) 
{
	VkImageViewCreateInfo viewInfo;
	memset(&viewInfo, 0, sizeof(VkImageViewCreateInfo));

	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
	{
		eprinterr("Failed to create image view!");
		assert(FALSE);
	}

	return imageView;
}

void Emulator_CreateConstantBuffers()
{
	VkDeviceSize bufferSize = sizeof(float) * 16;

	uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
		Emulator_CreateVulkanBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void Emulator_UpdateProjectionConstantBuffer(float* ortho)
{
	void* data = NULL;
	vkMapMemory(device, uniformBuffersMemory[currentFrame], 0, sizeof(float) * 16, 0, &data);
	memcpy(data, ortho, sizeof(float) * 16);
	vkUnmapMemory(device, uniformBuffersMemory[currentFrame]);
}

void Emulator_DestroyConstantBuffers()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
		vkDestroyBuffer(device, uniformBuffers[i], NULL);
		vkFreeMemory(device, uniformBuffersMemory[i], NULL);
		uniformBuffers[i] = VK_NULL_HANDLE;
		uniformBuffersMemory[i] = VK_NULL_HANDLE;
	}
}

void Emulator_DestroySyncObjects()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
		vkDestroySemaphore(device, renderFinishedSemaphores[i], NULL);
		vkDestroySemaphore(device, imageAvailableSemaphores[i], NULL);
		vkDestroyFence(device, inFlightFences[i], NULL);

		renderFinishedSemaphores[i] = VK_NULL_HANDLE;
		imageAvailableSemaphores[i] = VK_NULL_HANDLE;
		inFlightFences[i] = VK_NULL_HANDLE;
	}

	renderFinishedSemaphores.clear();
	imageAvailableSemaphores.clear();
	inFlightFences.clear();
}

void Emulator_DestroyVulkanCommandPool()
{
	vkDestroyCommandPool(device, commandPool, NULL);
	commandPool = VK_NULL_HANDLE;
}

void Emulator_DestroyVulkanCommandBuffers()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffers[i]);
	}

	commandBuffers.clear();
}

void Emulator_DestroyDescriptorSetLayout()
{
	for (int i = 0; i < 2; i++)
	{
		vkDestroyDescriptorSetLayout(device, descriptorSetLayout[i], NULL);
	}
}

#endif

void Emulator_HintTouchFontUIButton(short tpage, short clut, short* x, short* y, int buttonIndex)
{
	g_useHintedTouchUIFont = TRUE;
	
	struct TouchButtonVRAM* touchButton = &touchButtonsUI[buttonIndex];

	touchButton->tpage = tpage;
	touchButton->clut = clut;

	for (int i = 0; i < 4; i++)
	{
		touchButton->u[i] = x[i];
		touchButton->v[i] = y[i];
		touchButton->w = 11;
		touchButton->h = 11;
	}
}