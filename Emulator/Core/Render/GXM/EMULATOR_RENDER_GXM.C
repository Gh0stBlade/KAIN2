#include "LIBGPU.H"
#include "EMULATOR_RENDER_GXM.H"
#include "Core/Debug/EMULATOR_LOG.H"
#include "Core/Render/EMULATOR_RENDER_COMMON.H"
#include <stdio.h>

#if defined(GXM)

extern void Emulator_DoPollEvent();
extern void Emulator_WaitForTimestep(int count);
extern void Emulator_GenerateCommonTextures();
extern void Emulator_CreateGlobalShaders();
extern void Emulator_DestroyTextures();
extern void Emulator_DestroyGlobalShaders();
extern void Emulator_CreateVertexBuffer();

const char* renderBackendName = "OpenGL 3.3";

unsigned int dynamic_vertex_buffer;
unsigned int dynamic_vertex_array;

unsigned int u_Projection;

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

void Shader_CheckShaderStatus(unsigned int shader)
{
	char info[1024];
}

void Shader_CheckProgramStatus(unsigned int program)
{
	char info[1024];
	
}

ShaderID Shader_Compile(const char* source)
{
	const char* GLSL_HEADER_VERT =
		"#version 330\n"
		"#define VERTEX\n"
		"#define varying   out\n"
		"#define attribute in\n"
		"#define texture2D texture\n";

	const char* GLSL_HEADER_FRAG =
		"#version 330\n"
		"#define varying     in\n"
		"#define texture2D   texture\n"
		"out vec4 fragColor;\n";

	const char* vs_list[] = { GLSL_HEADER_VERT, source };
	const char* fs_list[] = { GLSL_HEADER_FRAG, source };

	return 0;
}

void Emulator_DestroyVertexBuffer()
{
	dynamic_vertex_buffer = 0;
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
	

	g_gte_shader_4 = 0;
	g_gte_shader_8 = 0;
	g_gte_shader_16 = 0;
	g_blit_shader = 0;
}

int Emulator_InitialiseGXMContext(char* windowName)
{

	return TRUE;
}

void Emulator_CreateGlobalShaders()
{
	g_gte_shader_4 = Shader_Compile(gte_shader_4);
	g_gte_shader_8 = Shader_Compile(gte_shader_8);
	g_gte_shader_16 = Shader_Compile(gte_shader_16);
	g_blit_shader = Shader_Compile(blit_shader);

	//u_Projection = glGetUniformLocation(g_gte_shader_4, "Projection");
}

void Emulator_GenerateCommonTextures()
{
	unsigned int pixelData = 0xFFFFFFFF;

	
}

void Emulator_CreateVertexBuffer()///@TODO OGLES
{
	
}

int Emulator_CreateCommonResources()
{
	memset(vram, 0, VRAM_WIDTH * VRAM_HEIGHT * sizeof(unsigned short));
	
	Emulator_GenerateCommonTextures();
	
	Emulator_CreateGlobalShaders();

	Emulator_CreateVertexBuffer();

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
	float z = (znear + zfar) / (znear - zfar);

	float ortho[16] = {
		a, 0, 0, 0,
		0, b, 0, 0,
		0, 0, c, 0,
		x, y, z, 1
	};
	
	///@TODO this generates an error sometimes
	//eprinterr("GL Error: %x\n", glGetError());

	//glUniformMatrix4fv(u_Projection, 1, GL_FALSE, ortho);

	//eprinterr("GL Error: %x\n", glGetError());
}

void Emulator_SetShader(const ShaderID shader)
{
	//glUseProgram(shader);
	
	//eprinterr("GL_Error: %x\n", glGetError());
	Emulator_Ortho2D(0.0f, activeDispEnv.disp.w, activeDispEnv.disp.h, 0.0f, 0.0f, 1.0f);
	//eprinterr("GL_Error: %x\n", glGetError());
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

#if defined(SINGLE_THREADED)
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