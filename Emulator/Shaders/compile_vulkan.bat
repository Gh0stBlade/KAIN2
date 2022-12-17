mkdir Vulkan

glslangValidator -D -DD3D12 --sub vert 0 --stb frag 2 --ssb frag 4 --spirv-val --vn gte_shader_4_vs -S vert -e main -o Vulkan/gte_shader_4_vs.h --define-macro VERTEX -V gte_shader_4.hlsl
glslangValidator -D --spirv-dis -DD3D12 --sub vert 0 --stb frag 2 --ssb frag 4 --spirv-val --vn gte_shader_4_ps -S frag -e main -o Vulkan/gte_shader_4_ps.h --define-macro PIXEL -V gte_shader_4.hlsl

glslangValidator -D -DD3D12 --sub vert 0 --stb frag 2 --ssb frag 4 --spirv-val --vn gte_shader_8_vs -S vert -e main -o Vulkan/gte_shader_8_vs.h --define-macro VERTEX -V gte_shader_8.hlsl
glslangValidator -D -DD3D12 --sub vert 0 --stb frag 2 --ssb frag 4 --spirv-val --vn gte_shader_8_ps -S frag -e main -o Vulkan/gte_shader_8_ps.h --define-macro PIXEL -V gte_shader_8.hlsl

glslangValidator -D -DD3D12 --sub vert 0 --stb frag 2 --ssb frag 4 --spirv-val --vn gte_shader_16_vs -S vert -e main -o Vulkan/gte_shader_16_vs.h --define-macro VERTEX -V gte_shader_16.hlsl
glslangValidator -D -DD3D12 --sub vert 0 --stb frag 2 --ssb frag 4 --spirv-val --vn gte_shader_16_ps -S frag -e main -o Vulkan/gte_shader_16_ps.h --define-macro PIXEL -V gte_shader_16.hlsl

glslangValidator -D -DD3D12 --sub vert 0 --stb frag 2 --ssb frag 4 --spirv-val --vn blit_shader_vs -S vert -e main -o Vulkan/blit_shader_vs.h --define-macro VERTEX -V blit_shader.hlsl
glslangValidator -D -DD3D12 --sub vert 0 --stb frag 2 --ssb frag 4 --spirv-val --vn blit_shader_ps -S frag -e main -o Vulkan/blit_shader_ps.h --define-macro PIXEL -V blit_shader.hlsl