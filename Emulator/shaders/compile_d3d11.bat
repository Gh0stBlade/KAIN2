SET PATH=%PATH%;C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Utilities\bin\x86

mkdir D3D11

fxc /nologo /T vs_4_0 /O3 /Vn gte_shader_4_vs /Fh D3D11/gte_shader_4_vs.h gte_shader_4_dx11.hlsl /DVERTEX
fxc /nologo /T ps_4_0 /O3 /Vn gte_shader_4_ps /Fh D3D11/gte_shader_4_ps.h gte_shader_4_dx11.hlsl /DPIXEL
fxc /nologo /T vs_4_0 /O3 /Vn gte_shader_8_vs /Fh D3D11/gte_shader_8_vs.h gte_shader_8_dx11.hlsl /DVERTEX
fxc /nologo /T ps_4_0 /O3 /Vn gte_shader_8_ps /Fh D3D11/gte_shader_8_ps.h gte_shader_8_dx11.hlsl /DPIXEL
fxc /nologo /T vs_4_0 /O3 /Vn gte_shader_16_vs /Fh D3D11/gte_shader_16_vs.h gte_shader_16_dx11.hlsl /DVERTEX
fxc /nologo /T ps_4_0 /O3 /Vn gte_shader_16_ps /Fh D3D11/gte_shader_16_ps.h gte_shader_16_dx11.hlsl /DPIXEL
fxc /nologo /T vs_4_0 /O3 /Vn blit_shader_vs /Fh D3D11/blit_shader_vs.h blit_shader_dx11.hlsl /DVERTEX
fxc /nologo /T ps_4_0 /O3 /Vn blit_shader_ps /Fh D3D11/blit_shader_ps.h blit_shader_dx11.hlsl /DPIXEL
