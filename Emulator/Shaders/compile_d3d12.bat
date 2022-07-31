SET PATH=%PATH%;C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0\x64

mkdir D3D12

dxc /nologo /T vs_6_0 /O3 /Vn gte_shader_4_vs /Fh D3D12/gte_shader_4_vs.h gte_shader_4.hlsl /DVERTEX /DD3D12
dxc /nologo /T ps_6_0 /O3 /Vn gte_shader_4_ps /Fh D3D12/gte_shader_4_ps.h gte_shader_4.hlsl /DPIXEL /DD3D12
dxc /nologo /T vs_6_0 /O3 /Vn gte_shader_8_vs /Fh D3D12/gte_shader_8_vs.h gte_shader_8.hlsl /DVERTEX /DD3D12
dxc /nologo /T ps_6_0 /O3 /Vn gte_shader_8_ps /Fh D3D12/gte_shader_8_ps.h gte_shader_8.hlsl /DPIXEL /DD3D12
dxc /nologo /T vs_6_0 /O3 /Vn gte_shader_16_vs /Fh D3D12/gte_shader_16_vs.h gte_shader_16.hlsl /DVERTEX /DD3D12
dxc /nologo /T ps_6_0 /O3 /Vn gte_shader_16_ps /Fh D3D12/gte_shader_16_ps.h gte_shader_16.hlsl /DPIXEL /DD3D12
dxc /nologo /T vs_6_0 /O3 /Vn blit_shader_vs /Fh D3D12/blit_shader_vs.h blit_shader.hlsl /DVERTEX /DD3D12
dxc /nologo /T ps_6_0 /O3 /Vn blit_shader_ps /Fh D3D12/blit_shader_ps.h blit_shader.hlsl /DPIXEL /DD3D12
