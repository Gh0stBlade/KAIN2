#include "LIBGPU.H"
#include "EMULATOR_RENDER_GXM.H"
#include "Core/Debug/EMULATOR_LOG.H"
#include "Core/Render/EMULATOR_RENDER_COMMON.H"
#include "Core/Setup/Game/GAME_VERSION.H"
#include <stdio.h>

#if defined(SN_TARGET_PSP2)

#include <string.h>
#include <stdlib.h>
#include <sceconst.h>
#include <libdbg.h>
#include <kernel.h>
#include <display.h>
#include <ctrl.h>
#include <gxm.h>
#include <math.h>
#include <stdio.h>

#include <gxm/program.h>

SceGxmContext* g_context = NULL;
SceGxmShaderPatcher* g_shaderPatcher = NULL;
SceUID g_patcherBufUid;
SceUID g_patcherCombinedUsseUid;

#define DISPLAY_WIDTH				960
#define DISPLAY_HEIGHT				544
#define DISPLAY_STRIDE_IN_PIXELS	1024

#define DISPLAY_COLOR_FORMAT		SCE_GXM_COLOR_FORMAT_A8B8G8R8
#define DISPLAY_PIXEL_FORMAT		SCE_DISPLAY_PIXELFORMAT_A8B8G8R8

#define DISPLAY_BUFFER_COUNT		3

#define DISPLAY_MAX_PENDING_SWAPS	2

#define ALIGN(x, a)					(((x) + ((a) - 1)) & ~((a) - 1))

#define SAMPLE_NAME SHORT_GAME_NAME

//SAMPLE

// Data structure for clear geometry
typedef struct ClearVertex {
	float x;
	float y;
} ClearVertex;

// Data structure for basic geometry
typedef struct BasicVertex {
	float x;
	float y;
	float z;
	uint32_t color;
} BasicVertex;

/*	Data structure to pass through the display queue.  This structure is
	serialized during sceGxmDisplayQueueAddEntry, and is used to pass
	arbitrary data to the display callback function, called from an internal
	thread once the back buffer is ready to be displayed.

	In this example, we only need to pass the base address of the buffer.
*/

// Callback function for displaying a buffer
static void displayCallback(const void *callbackData);

// Callback function to allocate memory for the shader patcher
static void *patcherHostAlloc(void *userData, uint32_t size);

// Callback function to allocate memory for the shader patcher
static void patcherHostFree(void *userData, void *mem);

// Helper function to allocate memory and map it for the GPU
static void *graphicsAlloc(SceKernelMemBlockType type, uint32_t size, uint32_t alignment, uint32_t attribs, SceUID *uid);

// Helper function to free memory mapped to the GPU
static void graphicsFree(SceUID uid);

// Helper function to allocate memory and map it as vertex USSE code for the GPU
static void *vertexUsseAlloc(uint32_t size, SceUID *uid, uint32_t *usseOffset);

// Helper function to free memory mapped as vertex USSE code for the GPU
static void vertexUsseFree(SceUID uid);

// Helper function to allocate memory and map it as fragment USSE code for the GPU
static void *fragmentUsseAlloc(uint32_t size, SceUID *uid, uint32_t *usseOffset);

// Helper function to free memory mapped as fragment USSE code for the GPU
static void fragmentUsseFree(SceUID uid);

// Mark variable as used
#define	UNUSED(a)					(void)(a)

// User main thread parameters
extern const char			sceUserMainThreadName[]		= "libgxm_basic_main_thr";
extern const int			sceUserMainThreadPriority	= SCE_KERNEL_DEFAULT_PRIORITY_USER;
extern const unsigned int	sceUserMainThreadStackSize	= SCE_KERNEL_STACK_SIZE_DEFAULT_USER_MAIN;

// Define a 1MiB heap for this sample
unsigned int	sceLibcHeapSize	= 1*1024*1024;

// Entry point
int main(void)
{
	int err = SCE_OK;
	UNUSED(err);

	/* ---------------------------------------------------------------------
		1. Load optional Razor modules.

		These modules must be loaded before libgxm is initialized.
	   --------------------------------------------------------------------- */

#ifdef ENABLE_RAZOR_HUD
	// Initialize the Razor HUD system.
	// This should be done before the call to sceGxmInitialize().
	err = sceSysmoduleLoadModule( SCE_SYSMODULE_RAZOR_HUD );
	SCE_DBG_ASSERT(err == SCE_OK);
#endif

#ifdef ENABLE_RAZOR_GPU_CAPTURE
	// Initialize the Razor capture system.
	// This should be done before the call to sceGxmInitialize().
	err = sceSysmoduleLoadModule( SCE_SYSMODULE_RAZOR_CAPTURE );
	SCE_DBG_ASSERT(err == SCE_OK);

	// Trigger a capture after 100 frames.
	sceRazorGpuCaptureSetTrigger( 100, "app0:basic.sgx" );
#endif

	/* ---------------------------------------------------------------------
		2. Initialize libgxm

		We specify the default parameter buffer size of 16MiB.

	   --------------------------------------------------------------------- */

	// set up parameters
	SceGxmInitializeParams initializeParams;
	memset(&initializeParams, 0, sizeof(SceGxmInitializeParams));
	initializeParams.flags							= 0;
	initializeParams.displayQueueMaxPendingCount	= DISPLAY_MAX_PENDING_SWAPS;
	initializeParams.displayQueueCallback			= displayCallback;
	initializeParams.displayQueueCallbackDataSize	= sizeof(DisplayData);
	initializeParams.parameterBufferSize			= SCE_GXM_DEFAULT_PARAMETER_BUFFER_SIZE;

	// initialize
	err = sceGxmInitialize(&initializeParams);
	SCE_DBG_ASSERT(err == SCE_OK);

	/* ---------------------------------------------------------------------
		3. Create a libgxm context
		
		Once initialized, we need to create a rendering context to allow to us
		to render scenes on the GPU.  We use the default initialization
		parameters here to set the sizes of the various context ring buffers.
	   --------------------------------------------------------------------- */

	// allocate ring buffer memory using default sizes
	SceUID vdmRingBufferUid;
	void *vdmRingBuffer = graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE,
		4,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&vdmRingBufferUid);
	SceUID vertexRingBufferUid;
	void *vertexRingBuffer = graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE,
		4,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&vertexRingBufferUid);
	SceUID fragmentRingBufferUid;
	void *fragmentRingBuffer = graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE,
		4,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&fragmentRingBufferUid);
	SceUID fragmentUsseRingBufferUid;
	uint32_t fragmentUsseRingBufferOffset;
	void *fragmentUsseRingBuffer = fragmentUsseAlloc(
		SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE,
		&fragmentUsseRingBufferUid,
		&fragmentUsseRingBufferOffset);

	SceGxmContextParams contextParams;
	memset(&contextParams, 0, sizeof(SceGxmContextParams));
	contextParams.hostMem						= malloc(SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE);
	contextParams.hostMemSize					= SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE;
	contextParams.vdmRingBufferMem				= vdmRingBuffer;
	contextParams.vdmRingBufferMemSize			= SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE;
	contextParams.vertexRingBufferMem			= vertexRingBuffer;
	contextParams.vertexRingBufferMemSize		= SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE;
	contextParams.fragmentRingBufferMem			= fragmentRingBuffer;
	contextParams.fragmentRingBufferMemSize		= SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE;
	contextParams.fragmentUsseRingBufferMem		= fragmentUsseRingBuffer;
	contextParams.fragmentUsseRingBufferMemSize	= SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE;
	contextParams.fragmentUsseRingBufferOffset	= fragmentUsseRingBufferOffset;
	
	SceGxmContext *context = NULL;
	err = sceGxmCreateContext(&contextParams, &context);
	SCE_DBG_ASSERT(err == SCE_OK);

	/* ---------------------------------------------------------------------
		4. Create a render target

		Finally we create a render target to describe the geometry of the back
		buffers we will render to.  This object is used purely to schedule
		rendering jobs for the given dimensions, the color surface and
		depth/stencil surface must be allocated separately.
	   --------------------------------------------------------------------- */

	// set up parameters
	SceGxmRenderTargetParams renderTargetParams;
	memset(&renderTargetParams, 0, sizeof(SceGxmRenderTargetParams));
	renderTargetParams.flags				= 0;
	renderTargetParams.width				= DISPLAY_WIDTH;
	renderTargetParams.height				= DISPLAY_HEIGHT;
	renderTargetParams.scenesPerFrame		= 1;
	renderTargetParams.multisampleMode		= SCE_GXM_MULTISAMPLE_NONE;
	renderTargetParams.multisampleLocations	= 0;
	renderTargetParams.driverMemBlock		= SCE_UID_INVALID_UID;
	
	/*	If you would like to allocate the memblock manually, then this code can
		be used.  Change the MANUALLY_ALLOCATE_RT_MEMBLOCK to 1 at the top of
		this file to use this mode in the sample.
	*/
#if MANUALLY_ALLOCATE_RT_MEMBLOCK
	{
		// compute memblock size
		uint32_t driverMemSize;
		sceGxmGetRenderTargetMemSize(&renderTargetParams, &driverMemSize);

		// allocate driver memory
		renderTargetParams.driverMemBlock = sceKernelAllocMemBlock(
			"SampleRT", 
			SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, 
			driverMemSize, 
			NULL);
	}
#endif

	// create the render target
	SceGxmRenderTarget *renderTarget;
	err = sceGxmCreateRenderTarget(&renderTargetParams, &renderTarget);
	SCE_DBG_ASSERT(err == SCE_OK);

	/* ---------------------------------------------------------------------
		5. Allocate display buffers and sync objects

		We will allocate our back buffers in CDRAM, and create a color
		surface for each of them.

		To allow display operations done by the CPU to be synchronized with
		rendering done by the GPU, we also create a SceGxmSyncObject for each
		display buffer.  This sync object will be used with each scene that
		renders to that buffer and when queueing display flips that involve
		that buffer (either flipping from or to).
	   --------------------------------------------------------------------- */

	// allocate memory and sync objects for display buffers
	void *displayBufferData[DISPLAY_BUFFER_COUNT];
	SceUID displayBufferUid[DISPLAY_BUFFER_COUNT];
	SceGxmColorSurface displaySurface[DISPLAY_BUFFER_COUNT];
	SceGxmSyncObject *displayBufferSync[DISPLAY_BUFFER_COUNT];
	for (uint32_t i = 0; i < DISPLAY_BUFFER_COUNT; ++i) {
		// allocate memory with large (1MiB) alignment to ensure physical contiguity
		displayBufferData[i] = graphicsAlloc(
			SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA,
			ALIGN(4*DISPLAY_STRIDE_IN_PIXELS*DISPLAY_HEIGHT, 1*1024*1024),
			SCE_GXM_COLOR_SURFACE_ALIGNMENT,
			SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE,
			&displayBufferUid[i]);

		// memset the buffer to black
		for (uint32_t y = 0; y < DISPLAY_HEIGHT; ++y) {
			uint32_t *row = (uint32_t *)displayBufferData[i] + y*DISPLAY_STRIDE_IN_PIXELS;
			for (uint32_t x = 0; x < DISPLAY_WIDTH; ++x) {
				row[x] = 0xff000000;
			}
		}

		// initialize a color surface for this display buffer
		err = sceGxmColorSurfaceInit(
			&displaySurface[i],
			DISPLAY_COLOR_FORMAT,
			SCE_GXM_COLOR_SURFACE_LINEAR,
			SCE_GXM_COLOR_SURFACE_SCALE_NONE,
			SCE_GXM_OUTPUT_REGISTER_SIZE_32BIT,
			DISPLAY_WIDTH,
			DISPLAY_HEIGHT,
			DISPLAY_STRIDE_IN_PIXELS,
			displayBufferData[i]);
		SCE_DBG_ASSERT(err == SCE_OK);

		// create a sync object that we will associate with this buffer
		err = sceGxmSyncObjectCreate(&displayBufferSync[i]);
		SCE_DBG_ASSERT(err == SCE_OK);
	}

	/* ---------------------------------------------------------------------
		6. Allocate a depth buffer

		Note that since this sample renders in a strictly back-to-front order,
		a depth buffer is not strictly required.  However, since it is usually
		necessary to create one to handle partial renders, we will create one
		now.  Note that we do not enable force load or store, so this depth
		buffer will not actually be read or written by the GPU when this
		sample is executed, so will have zero performance impact.
	   --------------------------------------------------------------------- */

	// compute the memory footprint of the depth buffer
	const uint32_t alignedWidth = ALIGN(DISPLAY_WIDTH, SCE_GXM_TILE_SIZEX);
	const uint32_t alignedHeight = ALIGN(DISPLAY_HEIGHT, SCE_GXM_TILE_SIZEY);
	uint32_t sampleCount = alignedWidth*alignedHeight;
	uint32_t depthStrideInSamples = alignedWidth;

	// allocate it
	SceUID depthBufferUid;
	void *depthBufferData = graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		4*sampleCount,
		SCE_GXM_DEPTHSTENCIL_SURFACE_ALIGNMENT,
		SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE,
		&depthBufferUid);

	// create the SceGxmDepthStencilSurface structure
	SceGxmDepthStencilSurface depthSurface;
	err = sceGxmDepthStencilSurfaceInit(
		&depthSurface,
		SCE_GXM_DEPTH_STENCIL_FORMAT_S8D24,
		SCE_GXM_DEPTH_STENCIL_SURFACE_TILED,
		depthStrideInSamples,
		depthBufferData,
		NULL);
	SCE_DBG_ASSERT(err == SCE_OK);

	/* ---------------------------------------------------------------------
		7. Create a shader patcher and register programs

		A shader patcher object is required to produce vertex and fragment
		programs from the shader compiler output.  First we create a shader
		patcher instance, using callback functions to allow it to allocate
		and free host memory for internal state.

		We will use the shader patcher's internal heap to handle buffer
		memory and USSE memory for the final programs.  To do this, we
		leave the callback functions as NULL, but provide static memory
		blocks.

		In order to create vertex and fragment programs for a particular
		shader, the compiler output must first be registered to obtain an ID
		for that shader.  Within a single ID, vertex and fragment programs
		are reference counted and could be shared if created with identical
		parameters.  To maximise this sharing, programs should only be
		registered with the shader patcher once if possible, so we will do
		this now.
	   --------------------------------------------------------------------- */

	// set buffer sizes for this sample
	const uint32_t patcherBufferSize		= 64*1024;
	const uint32_t patcherVertexUsseSize 	= 64*1024;
	const uint32_t patcherFragmentUsseSize 	= 64*1024;
	
	// allocate memory for buffers and USSE code
	SceUID patcherBufferUid;
	void *patcherBuffer = graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		patcherBufferSize,
		4, 
		SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE,
		&patcherBufferUid);
	SceUID patcherVertexUsseUid;
	uint32_t patcherVertexUsseOffset;
	void *patcherVertexUsse = vertexUsseAlloc(
		patcherVertexUsseSize,
		&patcherVertexUsseUid,
		&patcherVertexUsseOffset);
	SceUID patcherFragmentUsseUid;
	uint32_t patcherFragmentUsseOffset;
	void *patcherFragmentUsse = fragmentUsseAlloc(
		patcherFragmentUsseSize,
		&patcherFragmentUsseUid,
		&patcherFragmentUsseOffset);

	// create a shader patcher
	SceGxmShaderPatcherParams patcherParams;
	memset(&patcherParams, 0, sizeof(SceGxmShaderPatcherParams));
	patcherParams.userData					= NULL;
	patcherParams.hostAllocCallback			= &patcherHostAlloc;
	patcherParams.hostFreeCallback			= &patcherHostFree;
	patcherParams.bufferAllocCallback		= NULL;
	patcherParams.bufferFreeCallback		= NULL;
	patcherParams.bufferMem					= patcherBuffer;
	patcherParams.bufferMemSize				= patcherBufferSize;
	patcherParams.vertexUsseAllocCallback	= NULL;
	patcherParams.vertexUsseFreeCallback	= NULL;
	patcherParams.vertexUsseMem				= patcherVertexUsse;
	patcherParams.vertexUsseMemSize			= patcherVertexUsseSize;
	patcherParams.vertexUsseOffset			= patcherVertexUsseOffset;
	patcherParams.fragmentUsseAllocCallback	= NULL;
	patcherParams.fragmentUsseFreeCallback	= NULL;
	patcherParams.fragmentUsseMem			= patcherFragmentUsse;
	patcherParams.fragmentUsseMemSize		= patcherFragmentUsseSize;
	patcherParams.fragmentUsseOffset		= patcherFragmentUsseOffset;

	SceGxmShaderPatcher *shaderPatcher = NULL;
	err = sceGxmShaderPatcherCreate(&patcherParams, &shaderPatcher);
	SCE_DBG_ASSERT(err == SCE_OK);

	// register programs with the patcher
	SceGxmShaderPatcherId clearVertexProgramId;
	SceGxmShaderPatcherId clearFragmentProgramId;
	SceGxmShaderPatcherId basicVertexProgramId;
	SceGxmShaderPatcherId basicFragmentProgramId;
	/*err = sceGxmShaderPatcherRegisterProgram(shaderPatcher, &_binary_clear_v_gxp_start, &clearVertexProgramId);
	SCE_DBG_ASSERT(err == SCE_OK);
	err = sceGxmShaderPatcherRegisterProgram(shaderPatcher, &_binary_clear_f_gxp_start, &clearFragmentProgramId);
	SCE_DBG_ASSERT(err == SCE_OK);
	err = sceGxmShaderPatcherRegisterProgram(shaderPatcher, &_binary_basic_v_gxp_start, &basicVertexProgramId);
	SCE_DBG_ASSERT(err == SCE_OK);
	err = sceGxmShaderPatcherRegisterProgram(shaderPatcher, &_binary_basic_f_gxp_start, &basicFragmentProgramId);
	SCE_DBG_ASSERT(err == SCE_OK);*/

	/* ---------------------------------------------------------------------
		8. Create the programs and data for the clear

		On SGX hardware, vertex programs must perform the unpack operations
		on vertex data, so we must define our vertex formats in order to
		create the vertex program.  Similarly, fragment programs must be
		specialized based on how they output their pixels and MSAA mode.

		We define the clear geometry vertex format here and create the vertex
		and fragment program.

		The clear vertex and index data is static, we allocate and write the
		data here.
	   --------------------------------------------------------------------- */

	// get attributes by name to create vertex format bindings
	const SceGxmProgram *clearProgram = sceGxmShaderPatcherGetProgramFromId(clearVertexProgramId);
	SCE_DBG_ASSERT(clearProgram);
	const SceGxmProgramParameter *paramClearPositionAttribute = sceGxmProgramFindParameterByName(clearProgram, "aPosition");
	SCE_DBG_ASSERT(paramClearPositionAttribute && (sceGxmProgramParameterGetCategory(paramClearPositionAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));

	// create clear vertex format
	SceGxmVertexAttribute clearVertexAttributes[1];
	SceGxmVertexStream clearVertexStreams[1];
	clearVertexAttributes[0].streamIndex = 0;
	clearVertexAttributes[0].offset = 0;
	clearVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
	clearVertexAttributes[0].componentCount = 2;
	clearVertexAttributes[0].regIndex = sceGxmProgramParameterGetResourceIndex(paramClearPositionAttribute);
	clearVertexStreams[0].stride = sizeof(ClearVertex);
	clearVertexStreams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;

	// create sclear programs
	SceGxmVertexProgram *clearVertexProgram = NULL;
	SceGxmFragmentProgram *clearFragmentProgram = NULL;
	err = sceGxmShaderPatcherCreateVertexProgram(
		shaderPatcher,
		clearVertexProgramId,
		clearVertexAttributes,
		1,
		clearVertexStreams,
		1,
		&clearVertexProgram);
	SCE_DBG_ASSERT(err == SCE_OK);
	err = sceGxmShaderPatcherCreateFragmentProgram(
		shaderPatcher,
		clearFragmentProgramId,
		SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4,
		SCE_GXM_MULTISAMPLE_NONE,
		NULL,
		sceGxmShaderPatcherGetProgramFromId(clearVertexProgramId),
		&clearFragmentProgram);
	SCE_DBG_ASSERT(err == SCE_OK);

	// create the clear triangle vertex/index data
	SceUID clearVerticesUid;
	SceUID clearIndicesUid;
	

	uint16_t *const clearIndices = (uint16_t *)graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		3*sizeof(uint16_t),
		2,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&clearIndicesUid);

	/* ---------------------------------------------------------------------
		9. Create the programs and data for the spinning triangle

		We define the spinning triangle vertex format here and create the
		vertex and fragment program.

		The vertex and index data is static, we allocate and write the data
		here too.
	   --------------------------------------------------------------------- */

	// get attributes by name to create vertex format bindings
	// first retrieve the underlying program to extract binding information
	const SceGxmProgram *basicProgram = sceGxmShaderPatcherGetProgramFromId(basicVertexProgramId);
	SCE_DBG_ASSERT(basicProgram);
	const SceGxmProgramParameter *paramBasicPositionAttribute = sceGxmProgramFindParameterByName(basicProgram, "aPosition");
	SCE_DBG_ASSERT(paramBasicPositionAttribute && (sceGxmProgramParameterGetCategory(paramBasicPositionAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));
	const SceGxmProgramParameter *paramBasicColorAttribute = sceGxmProgramFindParameterByName(basicProgram, "aColor");
	SCE_DBG_ASSERT(paramBasicColorAttribute && (sceGxmProgramParameterGetCategory(paramBasicColorAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));

	// create shaded triangle vertex format
	SceGxmVertexAttribute basicVertexAttributes[2];
	SceGxmVertexStream basicVertexStreams[1];
	basicVertexAttributes[0].streamIndex = 0;
	basicVertexAttributes[0].offset = 0;
	basicVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
	basicVertexAttributes[0].componentCount = 3;
	basicVertexAttributes[0].regIndex = sceGxmProgramParameterGetResourceIndex(paramBasicPositionAttribute);
	basicVertexAttributes[1].streamIndex = 0;
	basicVertexAttributes[1].offset = 12;
	basicVertexAttributes[1].format = SCE_GXM_ATTRIBUTE_FORMAT_U8N;
	basicVertexAttributes[1].componentCount = 4;
	basicVertexAttributes[1].regIndex = sceGxmProgramParameterGetResourceIndex(paramBasicColorAttribute);
	basicVertexStreams[0].stride = sizeof(BasicVertex);
	basicVertexStreams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;

	// create shaded triangle shaders
	SceGxmVertexProgram *basicVertexProgram = NULL;
	SceGxmFragmentProgram *basicFragmentProgram = NULL;
	err = sceGxmShaderPatcherCreateVertexProgram(
		shaderPatcher,
		basicVertexProgramId,
		basicVertexAttributes,
		2,
		basicVertexStreams,
		1,
		&basicVertexProgram);
	SCE_DBG_ASSERT(err == SCE_OK);
	err = sceGxmShaderPatcherCreateFragmentProgram(
		shaderPatcher,
		basicFragmentProgramId,
		SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4,
		SCE_GXM_MULTISAMPLE_NONE,
		NULL,
		sceGxmShaderPatcherGetProgramFromId(basicVertexProgramId),
		&basicFragmentProgram);
	SCE_DBG_ASSERT(err == SCE_OK);

	// find vertex uniforms by name and cache parameter information
	const SceGxmProgramParameter *wvpParam = sceGxmProgramFindParameterByName(basicProgram, "wvp");
	SCE_DBG_ASSERT(wvpParam && (sceGxmProgramParameterGetCategory(wvpParam) == SCE_GXM_PARAMETER_CATEGORY_UNIFORM));

	// create shaded triangle vertex/index data
	SceUID basicVerticesUid;
	SceUID basicIndiceUid;
	BasicVertex *const basicVertices = (BasicVertex *)graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		3*sizeof(BasicVertex),
		4,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&basicVerticesUid);
	uint16_t *const basicIndices = (uint16_t *)graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		3*sizeof(uint16_t),
		2,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&basicIndiceUid);

	basicVertices[0].x = -0.5f;
	basicVertices[0].y = -0.5f;
	basicVertices[0].z = 0.0f;
	basicVertices[0].color = 0xff0000ff;
	basicVertices[1].x = 0.5f;
	basicVertices[1].y = -0.5f;
	basicVertices[1].z = 0.0f;
	basicVertices[1].color = 0xff00ff00;
	basicVertices[2].x = -0.5f;
	basicVertices[2].y = 0.5f;
	basicVertices[2].z = 0.0f;
	basicVertices[2].color = 0xffff0000;

	basicIndices[0] = 0;
	basicIndices[1] = 1;
	basicIndices[2] = 2;

	/* ---------------------------------------------------------------------
		10. Start the main loop

		Now that everything has been initialized, we can start the main
		rendering loop of the sample.
	   --------------------------------------------------------------------- */

	// initialize controller data
	SceCtrlData ctrlData;
	memset(&ctrlData, 0, sizeof(ctrlData));

	// message for SDK sample auto test
	printf("## api_libgxm/basic: INIT SUCCEEDED ##\n");

	// loop until exit
	uint32_t backBufferIndex = 0;
	uint32_t frontBufferIndex = 0;
	float rotationAngle = 0.0f;
	bool quit = false;
	while (!quit) {
		/* -----------------------------------------------------------------
			11. Update step

			Firstly, we check the control data for quit.

			Next, we perform the update step for this sample.  We advance the
			triangle angle by a fixed amount and update its matrix data.
		   ----------------------------------------------------------------- */

		// check control data
		sceCtrlReadBufferPositive(0, &ctrlData, 1);

		// update triangle angle
		rotationAngle += SCE_MATH_TWOPI/60.0f;
		if (rotationAngle > SCE_MATH_TWOPI)
			rotationAngle -= SCE_MATH_TWOPI;

		// set up a 4x4 matrix for a rotation
		float aspectRatio = (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT;

		float s = sin(rotationAngle);
		float c = cos(rotationAngle);

		float wvpData[16];
		wvpData[ 0] = c/aspectRatio;
		wvpData[ 1] = s;
		wvpData[ 2] = 0.0f;
		wvpData[ 3] = 0.0f;

		wvpData[ 4] = -s/aspectRatio;
		wvpData[ 5] = c;
		wvpData[ 6] = 0.0f;
		wvpData[ 7] = 0.0f;

		wvpData[ 8] = 0.0f;
		wvpData[ 9] = 0.0f;
		wvpData[10] = 1.0f;
		wvpData[11] = 0.0f;

		wvpData[12] = 0.0f;
		wvpData[13] = 0.0f;
		wvpData[14] = 0.0f;
		wvpData[15] = 1.0f;

		/* -----------------------------------------------------------------
			12. Rendering step

			This sample renders a single scene containing the two triangles,
			the clear triangle followed by the spinning triangle.  Before
			any drawing can take place, a scene must be started.  We render
			to the back buffer, so it is also important to use a sync object
			to ensure that these rendering operations are synchronized with
			display operations.

			The clear triangle shaders do not declare any uniform variables,
			so this may be rendered immediately after setting the vertex and
			fragment program.

			The spinning triangle vertex program declares a matrix parameter,
			so this forms part of the vertex default uniform buffer and must
			be written before the triangle can be drawn.

			Once both triangles have been drawn the scene can be ended, which
			submits it for rendering on the GPU.
		   ----------------------------------------------------------------- */

		// start rendering to the main render target
		sceGxmBeginScene(
			context, 
			0,
			renderTarget,
			NULL,
			NULL,
			displayBufferSync[backBufferIndex],
			&displaySurface[backBufferIndex],
			&depthSurface);

		// set clear shaders
		sceGxmSetVertexProgram(context, clearVertexProgram);
		sceGxmSetFragmentProgram(context, clearFragmentProgram);

		// draw the clear triangle
		

		// render the rotating triangle
		sceGxmSetVertexProgram(context, basicVertexProgram);
		sceGxmSetFragmentProgram(context, basicFragmentProgram);

		// set the vertex program constants
		void *vertexDefaultBuffer;
		sceGxmReserveVertexDefaultUniformBuffer(context, &vertexDefaultBuffer);
		sceGxmSetUniformDataF(vertexDefaultBuffer, wvpParam, 0, 16, wvpData);

		// draw the spinning triangle
		sceGxmSetVertexStream(context, 0, basicVertices);
		sceGxmDraw(context, SCE_GXM_PRIMITIVE_TRIANGLES, SCE_GXM_INDEX_FORMAT_U16, basicIndices, 3);

		// end the scene on the main render target, submitting rendering work to the GPU
		sceGxmEndScene(context, NULL, NULL);

		// PA heartbeat to notify end of frame
		sceGxmPadHeartbeat(&displaySurface[backBufferIndex], displayBufferSync[backBufferIndex]);

		/* -----------------------------------------------------------------
			13. Flip operation

			Now we have finished submitting rendering work for this frame it
			is time to submit a flip operation.  As part of specifying this
			flip operation we must provide the sync objects for both the old
			buffer and the new buffer.  This is to allow synchronization both
			ways: to not flip until rendering is complete, but also to ensure
			that future rendering to these buffers does not start until the
			flip operation is complete.

			The callback function will be called from an internal thread once
			queued GPU operations involving the sync objects is complete.
			Assuming we have not reached our maximum number of queued frames,
			this function returns immediately.

			Once we have queued our flip, we manually cycle through our back
			buffers before starting the next frame.
		   ----------------------------------------------------------------- */

		// queue the display swap for this frame
		DisplayData displayData;
		displayData.address = displayBufferData[backBufferIndex];
		sceGxmDisplayQueueAddEntry(
			displayBufferSync[frontBufferIndex],	// front buffer is OLD buffer
			displayBufferSync[backBufferIndex],		// back buffer is NEW buffer
			&displayData);

		// update buffer indices
		frontBufferIndex = backBufferIndex;
		backBufferIndex = (backBufferIndex + 1) % DISPLAY_BUFFER_COUNT;
	}

	/* ---------------------------------------------------------------------
		14. Wait for rendering to complete and shut down

		Since there could be many queued operations not yet completed it is
		important to wait until the GPU has finished before destroying
		resources.  We do this by calling sceGxmFinish for our rendering
		context.

		Once the GPU is finished, we release all our programs, deallocate
		all our memory, destroy all object and finally terminate libgxm.
	   --------------------------------------------------------------------- */

	// wait until rendering is done
	sceGxmFinish(context);

	// clean up allocations
	sceGxmShaderPatcherReleaseFragmentProgram(shaderPatcher, basicFragmentProgram);
	sceGxmShaderPatcherReleaseVertexProgram(shaderPatcher, basicVertexProgram);
	sceGxmShaderPatcherReleaseFragmentProgram(shaderPatcher, clearFragmentProgram);
	sceGxmShaderPatcherReleaseVertexProgram(shaderPatcher, clearVertexProgram);
	graphicsFree(basicIndiceUid);
	graphicsFree(basicVerticesUid);
	graphicsFree(clearIndicesUid);
	//graphicsFree(clearVerticesUid);

	// wait until display queue is finished before deallocating display buffers
	err = sceGxmDisplayQueueFinish();
	SCE_DBG_ASSERT(err == SCE_OK);

	// clean up display queue
	graphicsFree(depthBufferUid);
	for (uint32_t i = 0; i < DISPLAY_BUFFER_COUNT; ++i) {
		// clear the buffer then deallocate
		memset(displayBufferData[i], 0, DISPLAY_HEIGHT*DISPLAY_STRIDE_IN_PIXELS*4);
		graphicsFree(displayBufferUid[i]);

		// destroy the sync object
		sceGxmSyncObjectDestroy(displayBufferSync[i]);
	}

	// unregister programs and destroy shader patcher
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, basicFragmentProgramId);
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, basicVertexProgramId);
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, clearFragmentProgramId);
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, clearVertexProgramId);
	sceGxmShaderPatcherDestroy(shaderPatcher);
	fragmentUsseFree(patcherFragmentUsseUid);
	vertexUsseFree(patcherVertexUsseUid);
	graphicsFree(patcherBufferUid);

	// destroy the render target
	sceGxmDestroyRenderTarget(renderTarget);
#if MANUALLY_ALLOCATE_RT_MEMBLOCK
	sceKernelFreeMemBlock(renderTargetParams.driverMemBlock);
#endif

	// destroy the context
	sceGxmDestroyContext(context);
	fragmentUsseFree(fragmentUsseRingBufferUid);
	graphicsFree(fragmentRingBufferUid);
	graphicsFree(vertexRingBufferUid);
	graphicsFree(vdmRingBufferUid);
	free(contextParams.hostMem);

	// terminate libgxm
	sceGxmTerminate();

	/* ---------------------------------------------------------------------
		15. Unload optional Razor modules.

		These must be unloaded after terminating libgxm.
	   --------------------------------------------------------------------- */
#ifdef ENABLE_RAZOR_GPU_CAPTURE
	// Terminate Razor capture.
	// This should be done after the call to sceGxmTerminate().
	sceSysmoduleUnloadModule( SCE_SYSMODULE_RAZOR_CAPTURE );
#endif

#ifdef ENABLE_RAZOR_HUD
	// Terminate Razor HUD.
	// This should be done after the call to sceGxmTerminate().
	sceSysmoduleUnloadModule( SCE_SYSMODULE_RAZOR_HUD );
#endif

	// message for SDK sample auto test
	printf("## api_libgxm/basic: FINISHED ##\n");
	return SCE_OK;
}

void displayCallback(const void *callbackData)
{
	SceDisplayFrameBuf framebuf;
	int err = SCE_OK;
	UNUSED(err);

	// Cast the parameters back
	const DisplayData *displayData = (const DisplayData *)callbackData;

	// Swap to the new buffer on the next VSYNC
	memset(&framebuf, 0x00, sizeof(SceDisplayFrameBuf));
	framebuf.size        = sizeof(SceDisplayFrameBuf);
	framebuf.base        = displayData->address;
	framebuf.pitch       = DISPLAY_STRIDE_IN_PIXELS;
	framebuf.pixelformat = DISPLAY_PIXEL_FORMAT;
	framebuf.width       = DISPLAY_WIDTH;
	framebuf.height      = DISPLAY_HEIGHT;
	err = sceDisplaySetFrameBuf(&framebuf, SCE_DISPLAY_UPDATETIMING_NEXTVSYNC);
	SCE_DBG_ASSERT(err == SCE_OK);

	// Block this callback until the swap has occurred and the old buffer
	// is no longer displayed
	err = sceDisplayWaitVblankStart();
	SCE_DBG_ASSERT(err == SCE_OK);
}

static void *patcherHostAlloc(void *userData, uint32_t size)
{
	UNUSED(userData);
	return malloc(size);
}

static void patcherHostFree(void *userData, void *mem)
{
	UNUSED(userData);
	free(mem);
}

static void *graphicsAlloc(SceKernelMemBlockType type, uint32_t size, uint32_t alignment, uint32_t attribs, SceUID *uid)
{
	int err = SCE_OK;
	UNUSED(err);

	/*	Since we are using sceKernelAllocMemBlock directly, we cannot directly
		use the alignment parameter.  Instead, we must allocate the size to the
		minimum for this memblock type, and just assert that this will cover
		our desired alignment.

		Developers using their own heaps should be able to use the alignment
		parameter directly for more minimal padding.
	*/
	if (type == SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA) {
		// CDRAM memblocks must be 256KiB aligned
		SCE_DBG_ASSERT(alignment <= 256*1024);
		size = ALIGN(size, 256*1024);
	} else {
		// LPDDR memblocks must be 4KiB aligned
		SCE_DBG_ASSERT(alignment <= 4*1024);
		size = ALIGN(size, 4*1024);
	}
	UNUSED(alignment);

	// allocate some memory
	*uid = sceKernelAllocMemBlock("basic", type, size, NULL);
	SCE_DBG_ASSERT(*uid >= SCE_OK);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(*uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// map for the GPU
	err = sceGxmMapMemory(mem, size, attribs);
	SCE_DBG_ASSERT(err == SCE_OK);

	// done
	return mem;
}

static void graphicsFree(SceUID uid)
{
	int err = SCE_OK;
	UNUSED(err);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// unmap memory
	err = sceGxmUnmapMemory(mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// free the memory block
	err = sceKernelFreeMemBlock(uid);
	SCE_DBG_ASSERT(err == SCE_OK);
}

static void *vertexUsseAlloc(uint32_t size, SceUID *uid, uint32_t *usseOffset)
{
	int err = SCE_OK;
	UNUSED(err);

	// align to memblock alignment for LPDDR
	size = ALIGN(size, 4096);
	
	// allocate some memory
	*uid = sceKernelAllocMemBlock("basic", SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, size, NULL);
	SCE_DBG_ASSERT(*uid >= SCE_OK);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(*uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// map as vertex USSE code for the GPU
	err = sceGxmMapVertexUsseMemory(mem, size, usseOffset);
	SCE_DBG_ASSERT(err == SCE_OK);

	// done
	return mem;
}

static void vertexUsseFree(SceUID uid)
{
	int err = SCE_OK;
	UNUSED(err);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// unmap memory
	err = sceGxmUnmapVertexUsseMemory(mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// free the memory block
	err = sceKernelFreeMemBlock(uid);
	SCE_DBG_ASSERT(err == SCE_OK);
}

static void *fragmentUsseAlloc(uint32_t size, SceUID *uid, uint32_t *usseOffset)
{
	int err = SCE_OK;
	UNUSED(err);

	// align to memblock alignment for LPDDR
	size = ALIGN(size, 4096);
	
	// allocate some memory
	*uid = sceKernelAllocMemBlock("basic", SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, size, NULL);
	SCE_DBG_ASSERT(*uid >= SCE_OK);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(*uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// map as fragment USSE code for the GPU
	err = sceGxmMapFragmentUsseMemory(mem, size, usseOffset);
	SCE_DBG_ASSERT(err == SCE_OK);

	// done
	return mem;
}

static void fragmentUsseFree(SceUID uid)
{
	int err = SCE_OK;
	UNUSED(err);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// unmap memory
	err = sceGxmUnmapFragmentUsseMemory(mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// free the memory block
	err = sceKernelFreeMemBlock(uid);
	SCE_DBG_ASSERT(err == SCE_OK);
}

//END SAMPLE

extern void Emulator_DoPollEvent();
extern void Emulator_WaitForTimestep(int count);
extern void Emulator_GenerateCommonTextures();
extern void Emulator_CreateGlobalShaders();
extern void Emulator_DestroyTextures();
extern void Emulator_DestroyGlobalShaders();
extern void Emulator_CreateVertexBuffer();

const char* renderBackendName = "GXM";

SceUID dynamic_vertex_buffer_id;
struct Vertex* dynamic_vertex_buffer;
unsigned int dynamic_vertex_array;

void* g_contextHost;

int g_overrideWidth = -1;
int g_overrideHeight = -1;

SceUID g_vdmRingBufUid;
SceUID g_vertexRingBufUid;
SceUID g_fragmentRingBufUid;
SceUID g_fragmentUsseRingBufUid;
SceUInt32 ringbufFragmentUsseOffset;

void* vramTextureBuff = NULL;
void* whiteTextureBuff = NULL;
void* rg8lutTextureBuff = NULL;

SceGxmTexture vramTextureCtl;
SceGxmTexture whiteTextureCtl;
SceGxmTexture rg8lutTextureCtl;

#define	UNUSED(a)					(void)(a)

const SceGxmProgramParameter* u_Projection;

ShaderID Shader_Compile_Internal(const SceGxmProgram* source_vs, const SceGxmProgram* source_fs, int gte_shader)
{
	ShaderID shader;
	int err = SCE_OK;

#define OFFSETOF(T, E)     ((size_t)&(((T*)0)->E))

	if(gte_shader)
	{
		err = sceGxmShaderPatcherRegisterProgram(g_shaderPatcher, source_vs, &shader.VSID);
		SCE_DBG_ASSERT(err == SCE_OK);
		err =sceGxmShaderPatcherRegisterProgram(g_shaderPatcher, source_fs, &shader.FSID);
		SCE_DBG_ASSERT(err == SCE_OK);

		shader.PRG = sceGxmShaderPatcherGetProgramFromId(shader.VSID);
		SCE_DBG_ASSERT(shader.PRG);
	
		const SceGxmProgramParameter* paramPositionAttribute = sceGxmProgramFindParameterByName(shader.PRG, "In.a_position");
		SCE_DBG_ASSERT(paramPositionAttribute && (sceGxmProgramParameterGetCategory(paramPositionAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));
	
		const SceGxmProgramParameter* paramTexcoordAttribute = sceGxmProgramFindParameterByName(shader.PRG, "In.a_texcoord");
		SCE_DBG_ASSERT(paramTexcoordAttribute && (sceGxmProgramParameterGetCategory(paramTexcoordAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));
	
		const SceGxmProgramParameter* paramColorAttribute = sceGxmProgramFindParameterByName(shader.PRG, "In.a_color");
		SCE_DBG_ASSERT(paramColorAttribute && (sceGxmProgramParameterGetCategory(paramColorAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));

		SceGxmVertexAttribute basicVertexAttributes[3];
		SceGxmVertexStream basicVertexStreams[1];
		basicVertexAttributes[0].streamIndex = 0;
		basicVertexAttributes[0].offset = OFFSETOF(struct Vertex, x);
	#if defined(PGXP)
		basicVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
	#else
		basicVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_S16;
	#endif
		basicVertexAttributes[0].componentCount = 4;
		basicVertexAttributes[0].regIndex = sceGxmProgramParameterGetResourceIndex(paramPositionAttribute);
	
		basicVertexAttributes[1].streamIndex = 0;
		basicVertexAttributes[1].offset = OFFSETOF(struct Vertex, u);
		basicVertexAttributes[1].format = SCE_GXM_ATTRIBUTE_FORMAT_U8;
		basicVertexAttributes[1].componentCount = 2;
		basicVertexAttributes[1].regIndex = sceGxmProgramParameterGetResourceIndex(paramTexcoordAttribute);
	
		basicVertexAttributes[2].streamIndex = 0;
		basicVertexAttributes[2].offset = OFFSETOF(struct Vertex, r);
		basicVertexAttributes[2].format = SCE_GXM_ATTRIBUTE_FORMAT_U8N;
		basicVertexAttributes[2].componentCount = 4;
	
		basicVertexAttributes[2].regIndex = sceGxmProgramParameterGetResourceIndex(paramColorAttribute);
	
		basicVertexStreams[0].stride = sizeof(struct Vertex);
		basicVertexStreams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;
	
		err = sceGxmShaderPatcherCreateVertexProgram(g_shaderPatcher, shader.VSID, basicVertexAttributes, 3, basicVertexStreams, 1, &shader.VP);
		SCE_DBG_ASSERT(err == SCE_OK);

		err = sceGxmShaderPatcherCreateFragmentProgram(g_shaderPatcher, shader.FSID, SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4, SCE_GXM_MULTISAMPLE_NONE, NULL, shader.PRG, &shader.FP);
		SCE_DBG_ASSERT(err == SCE_OK);
	}
	else
	{
		err = sceGxmShaderPatcherRegisterProgram(g_shaderPatcher, source_vs, &shader.VSID);
		SCE_DBG_ASSERT(err == SCE_OK);
		err =sceGxmShaderPatcherRegisterProgram(g_shaderPatcher, source_fs, &shader.FSID);
		SCE_DBG_ASSERT(err == SCE_OK);

		shader.PRG = sceGxmShaderPatcherGetProgramFromId(shader.VSID);
		SCE_DBG_ASSERT(shader.PRG);
	
		const SceGxmProgramParameter* paramPositionAttribute = sceGxmProgramFindParameterByName(shader.PRG, "In.a_position");
		SCE_DBG_ASSERT(paramPositionAttribute && (sceGxmProgramParameterGetCategory(paramPositionAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));
	
		const SceGxmProgramParameter* paramTexcoordAttribute = sceGxmProgramFindParameterByName(shader.PRG, "In.a_texcoord");
		SCE_DBG_ASSERT(paramTexcoordAttribute && (sceGxmProgramParameterGetCategory(paramTexcoordAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));

		SceGxmVertexAttribute basicVertexAttributes[2];
		SceGxmVertexStream basicVertexStreams[1];
		basicVertexAttributes[0].streamIndex = 0;
		basicVertexAttributes[0].offset = OFFSETOF(struct Vertex, x);
#if defined(PGXP)
		basicVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
#else
		basicVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_S16;
#endif
		basicVertexAttributes[0].componentCount = 4;
		basicVertexAttributes[0].regIndex = sceGxmProgramParameterGetResourceIndex(paramPositionAttribute);
	
		basicVertexAttributes[1].streamIndex = 0;
		basicVertexAttributes[1].offset = OFFSETOF(struct Vertex, u);
		basicVertexAttributes[1].format = SCE_GXM_ATTRIBUTE_FORMAT_U8;
		basicVertexAttributes[1].componentCount = 2;
		basicVertexAttributes[1].regIndex = sceGxmProgramParameterGetResourceIndex(paramTexcoordAttribute);
	
		basicVertexStreams[0].stride = sizeof(struct Vertex);
		basicVertexStreams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;
	
		err = sceGxmShaderPatcherCreateVertexProgram(g_shaderPatcher, shader.VSID, basicVertexAttributes, 2, basicVertexStreams, 1, &shader.VP);
		SCE_DBG_ASSERT(err == SCE_OK);

		err = sceGxmShaderPatcherCreateFragmentProgram(g_shaderPatcher, shader.FSID, SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4, SCE_GXM_MULTISAMPLE_NONE, NULL, shader.PRG, &shader.FP);
		SCE_DBG_ASSERT(err == SCE_OK);
	}


	return shader;
}

void Emulator_DestroyVertexBuffer()
{
	dynamic_vertex_buffer = NULL;
	graphicsFree(dynamic_vertex_buffer_id);

	dynamic_vertex_array = 0;
}

void Emulator_ResetDevice()
{
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

	vramTexture = 0;
	rg8lutTexture = 0;
	whiteTexture = 0;
}

void Emulator_DestroyGlobalShaders()
{
	//g_gte_shader_4 = 0;
	//g_gte_shader_8 = 0;
	//g_gte_shader_16 = 0;
	//g_blit_shader = 0;
}

void Emulator_DisplayCallback(const void *data)
{

}

int Emulator_InitialiseGXMContext(char* windowName)
{
	int err = SCE_OK;
	UNUSED(err);

	/* ---------------------------------------------------------------------
		1. Load optional Razor modules.

		These modules must be loaded before libgxm is initialized.
	   --------------------------------------------------------------------- */

#ifdef ENABLE_RAZOR_HUD
	// Initialize the Razor HUD system.
	// This should be done before the call to sceGxmInitialize().
	err = sceSysmoduleLoadModule( SCE_SYSMODULE_RAZOR_HUD );
	SCE_DBG_ASSERT(err == SCE_OK);
#endif

#ifdef ENABLE_RAZOR_GPU_CAPTURE
	// Initialize the Razor capture system.
	// This should be done before the call to sceGxmInitialize().
	err = sceSysmoduleLoadModule( SCE_SYSMODULE_RAZOR_CAPTURE );
	SCE_DBG_ASSERT(err == SCE_OK);

	// Trigger a capture after 100 frames.
	sceRazorGpuCaptureSetTrigger( 100, "app0:basic.sgx" );
#endif

	/* ---------------------------------------------------------------------
		2. Initialize libgxm

		We specify the default parameter buffer size of 16MiB.

	   --------------------------------------------------------------------- */

	// set up parameters
	SceGxmInitializeParams initializeParams;
	memset(&initializeParams, 0, sizeof(SceGxmInitializeParams));
	initializeParams.flags							= 0;
	initializeParams.displayQueueMaxPendingCount	= DISPLAY_MAX_PENDING_SWAPS;
	initializeParams.displayQueueCallback			= displayCallback;
	initializeParams.displayQueueCallbackDataSize	= sizeof(DisplayData);
	initializeParams.parameterBufferSize			= SCE_GXM_DEFAULT_PARAMETER_BUFFER_SIZE;

	// initialize
	err = sceGxmInitialize(&initializeParams);
	SCE_DBG_ASSERT(err == SCE_OK);

	/* ---------------------------------------------------------------------
		3. Create a libgxm context
		
		Once initialized, we need to create a rendering context to allow to us
		to render scenes on the GPU.  We use the default initialization
		parameters here to set the sizes of the various context ring buffers.
	   --------------------------------------------------------------------- */

	// allocate ring buffer memory using default sizes
	SceUID vdmRingBufferUid;
	void *vdmRingBuffer = graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE,
		4,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&vdmRingBufferUid);
	SceUID vertexRingBufferUid;
	void *vertexRingBuffer = graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE,
		4,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&vertexRingBufferUid);
	SceUID fragmentRingBufferUid;
	void *fragmentRingBuffer = graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE,
		4,
		SCE_GXM_MEMORY_ATTRIB_READ,
		&fragmentRingBufferUid);
	SceUID fragmentUsseRingBufferUid;
	uint32_t fragmentUsseRingBufferOffset;
	void *fragmentUsseRingBuffer = fragmentUsseAlloc(
		SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE,
		&fragmentUsseRingBufferUid,
		&fragmentUsseRingBufferOffset);

	SceGxmContextParams contextParams;
	memset(&contextParams, 0, sizeof(SceGxmContextParams));
	contextParams.hostMem						= malloc(SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE);
	contextParams.hostMemSize					= SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE;
	contextParams.vdmRingBufferMem				= vdmRingBuffer;
	contextParams.vdmRingBufferMemSize			= SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE;
	contextParams.vertexRingBufferMem			= vertexRingBuffer;
	contextParams.vertexRingBufferMemSize		= SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE;
	contextParams.fragmentRingBufferMem			= fragmentRingBuffer;
	contextParams.fragmentRingBufferMemSize		= SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE;
	contextParams.fragmentUsseRingBufferMem		= fragmentUsseRingBuffer;
	contextParams.fragmentUsseRingBufferMemSize	= SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE;
	contextParams.fragmentUsseRingBufferOffset	= fragmentUsseRingBufferOffset;
	
	err = sceGxmCreateContext(&contextParams, &g_context);
	SCE_DBG_ASSERT(err == SCE_OK);

	/* ---------------------------------------------------------------------
		4. Create a render target

		Finally we create a render target to describe the geometry of the back
		buffers we will render to.  This object is used purely to schedule
		rendering jobs for the given dimensions, the color surface and
		depth/stencil surface must be allocated separately.
	   --------------------------------------------------------------------- */

	// set up parameters
	SceGxmRenderTargetParams renderTargetParams;
	memset(&renderTargetParams, 0, sizeof(SceGxmRenderTargetParams));
	renderTargetParams.flags				= 0;
	renderTargetParams.width				= DISPLAY_WIDTH;
	renderTargetParams.height				= DISPLAY_HEIGHT;
	renderTargetParams.scenesPerFrame		= 1;
	renderTargetParams.multisampleMode		= SCE_GXM_MULTISAMPLE_NONE;
	renderTargetParams.multisampleLocations	= 0;
	renderTargetParams.driverMemBlock		= SCE_UID_INVALID_UID;
	
	/*	If you would like to allocate the memblock manually, then this code can
		be used.  Change the MANUALLY_ALLOCATE_RT_MEMBLOCK to 1 at the top of
		this file to use this mode in the sample.
	*/
#if MANUALLY_ALLOCATE_RT_MEMBLOCK
	{
		// compute memblock size
		uint32_t driverMemSize;
		sceGxmGetRenderTargetMemSize(&renderTargetParams, &driverMemSize);

		// allocate driver memory
		renderTargetParams.driverMemBlock = sceKernelAllocMemBlock(
			"SampleRT", 
			SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, 
			driverMemSize, 
			NULL);
	}
#endif

	// create the render target
	SceGxmRenderTarget *renderTarget;
	err = sceGxmCreateRenderTarget(&renderTargetParams, &renderTarget);
	SCE_DBG_ASSERT(err == SCE_OK);

	/* ---------------------------------------------------------------------
		5. Allocate display buffers and sync objects

		We will allocate our back buffers in CDRAM, and create a color
		surface for each of them.

		To allow display operations done by the CPU to be synchronized with
		rendering done by the GPU, we also create a SceGxmSyncObject for each
		display buffer.  This sync object will be used with each scene that
		renders to that buffer and when queueing display flips that involve
		that buffer (either flipping from or to).
	   --------------------------------------------------------------------- */

	// allocate memory and sync objects for display buffers
	void *displayBufferData[DISPLAY_BUFFER_COUNT];
	SceUID displayBufferUid[DISPLAY_BUFFER_COUNT];
	SceGxmColorSurface displaySurface[DISPLAY_BUFFER_COUNT];
	SceGxmSyncObject *displayBufferSync[DISPLAY_BUFFER_COUNT];
	for (uint32_t i = 0; i < DISPLAY_BUFFER_COUNT; ++i) {
		// allocate memory with large (1MiB) alignment to ensure physical contiguity
		displayBufferData[i] = graphicsAlloc(
			SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA,
			ALIGN(4*DISPLAY_STRIDE_IN_PIXELS*DISPLAY_HEIGHT, 1*1024*1024),
			SCE_GXM_COLOR_SURFACE_ALIGNMENT,
			SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE,
			&displayBufferUid[i]);

		// memset the buffer to black
		for (uint32_t y = 0; y < DISPLAY_HEIGHT; ++y) {
			uint32_t *row = (uint32_t *)displayBufferData[i] + y*DISPLAY_STRIDE_IN_PIXELS;
			for (uint32_t x = 0; x < DISPLAY_WIDTH; ++x) {
				row[x] = 0xff000000;
			}
		}

		// initialize a color surface for this display buffer
		err = sceGxmColorSurfaceInit(
			&displaySurface[i],
			DISPLAY_COLOR_FORMAT,
			SCE_GXM_COLOR_SURFACE_LINEAR,
			SCE_GXM_COLOR_SURFACE_SCALE_NONE,
			SCE_GXM_OUTPUT_REGISTER_SIZE_32BIT,
			DISPLAY_WIDTH,
			DISPLAY_HEIGHT,
			DISPLAY_STRIDE_IN_PIXELS,
			displayBufferData[i]);
		SCE_DBG_ASSERT(err == SCE_OK);

		// create a sync object that we will associate with this buffer
		err = sceGxmSyncObjectCreate(&displayBufferSync[i]);
		SCE_DBG_ASSERT(err == SCE_OK);
	}

	/* ---------------------------------------------------------------------
		6. Allocate a depth buffer

		Note that since this sample renders in a strictly back-to-front order,
		a depth buffer is not strictly required.  However, since it is usually
		necessary to create one to handle partial renders, we will create one
		now.  Note that we do not enable force load or store, so this depth
		buffer will not actually be read or written by the GPU when this
		sample is executed, so will have zero performance impact.
	   --------------------------------------------------------------------- */

	// compute the memory footprint of the depth buffer
	const uint32_t alignedWidth = ALIGN(DISPLAY_WIDTH, SCE_GXM_TILE_SIZEX);
	const uint32_t alignedHeight = ALIGN(DISPLAY_HEIGHT, SCE_GXM_TILE_SIZEY);
	uint32_t sampleCount = alignedWidth*alignedHeight;
	uint32_t depthStrideInSamples = alignedWidth;

	// allocate it
	SceUID depthBufferUid;
	void *depthBufferData = graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		4*sampleCount,
		SCE_GXM_DEPTHSTENCIL_SURFACE_ALIGNMENT,
		SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE,
		&depthBufferUid);

	// create the SceGxmDepthStencilSurface structure
	SceGxmDepthStencilSurface depthSurface;
	err = sceGxmDepthStencilSurfaceInit(
		&depthSurface,
		SCE_GXM_DEPTH_STENCIL_FORMAT_S8D24,
		SCE_GXM_DEPTH_STENCIL_SURFACE_TILED,
		depthStrideInSamples,
		depthBufferData,
		NULL);
	SCE_DBG_ASSERT(err == SCE_OK);

	/* ---------------------------------------------------------------------
		7. Create a shader patcher and register programs

		A shader patcher object is required to produce vertex and fragment
		programs from the shader compiler output.  First we create a shader
		patcher instance, using callback functions to allow it to allocate
		and free host memory for internal state.

		We will use the shader patcher's internal heap to handle buffer
		memory and USSE memory for the final programs.  To do this, we
		leave the callback functions as NULL, but provide static memory
		blocks.

		In order to create vertex and fragment programs for a particular
		shader, the compiler output must first be registered to obtain an ID
		for that shader.  Within a single ID, vertex and fragment programs
		are reference counted and could be shared if created with identical
		parameters.  To maximise this sharing, programs should only be
		registered with the shader patcher once if possible, so we will do
		this now.
	   --------------------------------------------------------------------- */

	// set buffer sizes for this sample
	const uint32_t patcherBufferSize		= 64*1024;
	const uint32_t patcherVertexUsseSize 	= 64*1024;
	const uint32_t patcherFragmentUsseSize 	= 64*1024;
	
	// allocate memory for buffers and USSE code
	SceUID patcherBufferUid;
	void *patcherBuffer = graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		patcherBufferSize,
		4, 
		SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE,
		&patcherBufferUid);
	SceUID patcherVertexUsseUid;
	uint32_t patcherVertexUsseOffset;
	void *patcherVertexUsse = vertexUsseAlloc(
		patcherVertexUsseSize,
		&patcherVertexUsseUid,
		&patcherVertexUsseOffset);
	SceUID patcherFragmentUsseUid;
	uint32_t patcherFragmentUsseOffset;
	void *patcherFragmentUsse = fragmentUsseAlloc(
		patcherFragmentUsseSize,
		&patcherFragmentUsseUid,
		&patcherFragmentUsseOffset);

	// create a shader patcher
	SceGxmShaderPatcherParams patcherParams;
	memset(&patcherParams, 0, sizeof(SceGxmShaderPatcherParams));
	patcherParams.userData					= NULL;
	patcherParams.hostAllocCallback			= &patcherHostAlloc;
	patcherParams.hostFreeCallback			= &patcherHostFree;
	patcherParams.bufferAllocCallback		= NULL;
	patcherParams.bufferFreeCallback		= NULL;
	patcherParams.bufferMem					= patcherBuffer;
	patcherParams.bufferMemSize				= patcherBufferSize;
	patcherParams.vertexUsseAllocCallback	= NULL;
	patcherParams.vertexUsseFreeCallback	= NULL;
	patcherParams.vertexUsseMem				= patcherVertexUsse;
	patcherParams.vertexUsseMemSize			= patcherVertexUsseSize;
	patcherParams.vertexUsseOffset			= patcherVertexUsseOffset;
	patcherParams.fragmentUsseAllocCallback	= NULL;
	patcherParams.fragmentUsseFreeCallback	= NULL;
	patcherParams.fragmentUsseMem			= patcherFragmentUsse;
	patcherParams.fragmentUsseMemSize		= patcherFragmentUsseSize;
	patcherParams.fragmentUsseOffset		= patcherFragmentUsseOffset;

	err = sceGxmShaderPatcherCreate(&patcherParams, &g_shaderPatcher);
	SCE_DBG_ASSERT(err == SCE_OK);

	return TRUE;
}

void Emulator_CreateGlobalShaders()
{
	g_gte_shader_4 = Shader_Compile(gte_shader_4, 1);
	g_gte_shader_8 = Shader_Compile(gte_shader_8, 1);
	g_gte_shader_16 = Shader_Compile(gte_shader_16, 1);
	g_blit_shader = Shader_Compile(blit_shader, 0);
}

void Emulator_GenerateCommonTextures()
{
#if 0
	unsigned int pixelData = 0xFFFFFFFF;

	whiteTextureBuff = Emulator_GAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA, 1 * 1 * sizeof(unsigned int), SCE_GXM_TEXTURE_ALIGNMENT, SCE_GXM_MEMORY_ATTRIB_READ, &whiteTexture);
	memcpy(whiteTextureBuff, &pixelData, 1 * 1 * sizeof(unsigned int));
	sceGxmTextureInitLinear(&whiteTextureCtl, whiteTextureBuff, SCE_GXM_TEXTURE_FORMAT_A8R8G8B8, 1, 1, 1);
	sceGxmTextureSetMinFilter(&whiteTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMagFilter(&whiteTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMipFilter(&whiteTextureCtl, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);

	rg8lutTextureBuff = Emulator_GAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA, LUT_WIDTH * LUT_HEIGHT * sizeof(unsigned int), SCE_GXM_TEXTURE_ALIGNMENT, SCE_GXM_MEMORY_ATTRIB_READ, &rg8lutTexture);
	memcpy(rg8lutTextureBuff, Emulator_GenerateRG8LUT(), LUT_WIDTH * LUT_HEIGHT * sizeof(unsigned int));
	sceGxmTextureInitLinear(&rg8lutTextureCtl, rg8lutTextureBuff, SCE_GXM_TEXTURE_FORMAT_A8R8G8B8, LUT_WIDTH, LUT_HEIGHT, 1);
	sceGxmTextureSetMinFilter(&rg8lutTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMagFilter(&rg8lutTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMipFilter(&rg8lutTextureCtl, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);
	
	vramTextureBuff = Emulator_GAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA, VRAM_WIDTH * VRAM_HEIGHT * sizeof(unsigned int), SCE_GXM_TEXTURE_ALIGNMENT, SCE_GXM_MEMORY_ATTRIB_READ, &vramTexture);
	sceGxmTextureInitLinear(&vramTextureCtl, vramTextureBuff, SCE_GXM_TEXTURE_FORMAT_A8L8, LUT_WIDTH, LUT_HEIGHT, 1);
	sceGxmTextureSetMinFilter(&vramTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMagFilter(&vramTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMipFilter(&vramTextureCtl, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);
#endif
}

void Emulator_CreateVertexBuffer()///@TODO OGLES
{
	dynamic_vertex_buffer = (struct Vertex*)graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, sizeof(struct Vertex) * MAX_NUM_POLY_BUFFER_VERTICES, 4, SCE_GXM_MEMORY_ATTRIB_READ, &dynamic_vertex_buffer_id);
	sceGxmSetVertexStream(g_context, 0, dynamic_vertex_buffer);
}

int Emulator_CreateCommonResources()
{
	memset(vram, 0, VRAM_WIDTH * VRAM_HEIGHT * sizeof(unsigned short));

	Emulator_GenerateCommonTextures();

	Emulator_CreateGlobalShaders();

	///glDisable(GL_DEPTH_TEST);
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
	
	void* vertexDefaultBuffer = NULL;
	sceGxmReserveVertexDefaultUniformBuffer(g_context, &vertexDefaultBuffer);
	printf("SETTING BUFF: %d!\n", (int)vertexDefaultBuffer);

	sceGxmSetUniformDataF(vertexDefaultBuffer, u_Projection, 0, 16, ortho);
}

void Emulator_SetShader(const ShaderID shader)
{
	printf("SETTING SHADER!\n");
	
	u_Projection = sceGxmProgramFindParameterByName(shader.PRG, "Projection");
	SCE_DBG_ASSERT(u_Projection && (sceGxmProgramParameterGetCategory(u_Projection) == SCE_GXM_PARAMETER_CATEGORY_UNIFORM));

	sceGxmSetVertexProgram(g_context, shader.VP);
	sceGxmSetFragmentProgram(g_context, shader.FP);

	printf("u_Projection: %d, prog: %d\n", (int)u_Projection, (int)shader.PRG);
	Emulator_Ortho2D(0.0f, activeDispEnv.disp.w, activeDispEnv.disp.h, 0.0f, 0.0f, 1.0f);
}

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

	if (g_lastBoundTexture[0] == texture && g_lastBoundTexture[1] == rg8lutTexture) {
		//return;
	}

	g_lastBoundTexture[0] = texture;
	g_lastBoundTexture[1] = rg8lutTexture;
}

void Emulator_SetTextureAndShader(TextureID texture, ShaderID shader)
{
	Emulator_SetShader(shader);

	if (g_texturelessMode) {
		texture = whiteTexture;
	}

	if (g_lastBoundTexture[0] == texture && g_lastBoundTexture[1] == rg8lutTexture) {
		//return;
	}

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, rg8lutTexture);

	//g_lastBoundTexture[0] = texture;
	//g_lastBoundTexture[1] = rg8lutTexture;
}

void Emulator_DestroyTexture(TextureID texture)
{
	
}

void Emulator_Clear(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b)
{
	
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
	short* fb = (short*)malloc(w * h * sizeof(short));

	int* data = (int*)malloc(w * h * sizeof(int));
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

	free(data);

#undef FLIP_Y

	free(fb);

	vram_need_update = TRUE;
}

void Emulator_UpdateVRAM()
{
	if (!vram_need_update) {
		return;
	}
	vram_need_update = FALSE;
}

void Emulator_SetWireframe(int enable)
{
	
}

void Emulator_SetBlendMode(enum BlendMode blendMode)
{
	if (g_PreviousBlendMode == blendMode)
	{
		return;
	}

	if (g_PreviousBlendMode == BM_NONE)
	{
		
	}

	g_PreviousBlendMode = blendMode;
}

void Emulator_DrawTriangles(int start_vertex, int triangles)
{
	if (triangles <= 0)
		return;

	sceGxmDraw(g_context, SCE_GXM_PRIMITIVE_TRIANGLES, SCE_GXM_INDEX_FORMAT_U16, dynamic_vertex_buffer, 3);
}

void Emulator_UpdateVertexBuffer(const struct Vertex* vertices, int num_vertices)
{
	eassert(num_vertices <= MAX_NUM_POLY_BUFFER_VERTICES);

	if (num_vertices <= 0)
		return;

	vbo_was_dirty_flag = TRUE;
}

void Emulator_SetViewPort(int x, int y, int width, int height)
{
	float offset_x = (float)activeDispEnv.screen.x;
	float offset_y = (float)activeDispEnv.screen.y;

}

void Emulator_SwapWindow()
{
	unsigned int timer = 1;

#if defined(SINGLE_THREADED_AUDIO)
	Emulator_CounterWrapper(0, &timer);
#endif

	Emulator_WaitForTimestep(1);
}

void Emulator_WaitForTimestep(int count)
{
#if 0
	if (g_swapInterval > 0)
	{
		int delta = g_swapTime + FIXED_TIME_STEP * count - SDL_GetTicks();

		if (delta > 0) {
			SDL_Delay(delta);
		}
	}

	g_swapTime = SDL_GetTicks();
#endif
}

void Emulator_SetRenderTarget(const RenderTargetID frameBufferObject)
{
}

#endif