mkdir Vulkan

glslangValidator -D --sub vert 0 --ssb vert 2 --stb vert 4 --vn gte_shader_4_vs -S vert -e main -o Vulkan/gte_shader_4_vs.h --define-macro VERTEX -V gte_shader_4.hlsl
glslangValidator -D --sub frag 0 --ssb frag 2 --stb frag 4 --vn gte_shader_4_ps -S frag -e main -o Vulkan/gte_shader_4_ps.h --define-macro PIXEL -V gte_shader_4.hlsl

glslangValidator -D --sub vert 0 --ssb vert 2 --stb vert 4 --vn gte_shader_8_vs -S vert -e main -o Vulkan/gte_shader_8_vs.h --define-macro VERTEX -V gte_shader_8.hlsl
glslangValidator -D --sub frag 0 --ssb frag 2 --stb frag 4 --vn gte_shader_8_ps -S frag -e main -o Vulkan/gte_shader_8_ps.h --define-macro PIXEL -V gte_shader_8.hlsl

glslangValidator -D --sub vert 0 --ssb vert 2 --stb vert 4 --vn gte_shader_16_vs -S vert -e main -o Vulkan/gte_shader_16_vs.h --define-macro VERTEX -V gte_shader_16.hlsl
glslangValidator -D --sub frag 0 --ssb frag 2 --stb frag 4 --vn gte_shader_16_ps -S frag -e main -o Vulkan/gte_shader_16_ps.h --define-macro PIXEL -V gte_shader_16.hlsl

glslangValidator -D --sub vert 0 --ssb vert 2 --stb vert 4 --vn blit_shader_vs -S vert -e main -o Vulkan/blit_shader_vs.h --define-macro VERTEX -V blit_shader.hlsl
glslangValidator -D --sub frag 0 --ssb frag 2 --stb frag 4 --vn blit_shader_ps -S frag -e main -o Vulkan/blit_shader_ps.h --define-macro PIXEL -V blit_shader.hlsl