SET PATH=%PATH%;%NINTENDO_SDK_ROOT%\Tools\Graphics\GraphicsTools\

mkdir NVN

ShaderConverter32.exe -o NVN/gte_shader_4_vs.bnsh -s Glsl -c Binary -a Gl --vertex-shader gte_shader_4_vs.glsl --separable --reflection
ShaderConverter32.exe -o NVN/gte_shader_4_ps.bnsh -s Glsl -c Binary -a Gl --pixel-shader gte_shader_4_ps.glsl --separable --reflection

ShaderConverter32.exe -o NVN/gte_shader_8_vs.bnsh -s Glsl -c Binary -a Gl --vertex-shader gte_shader_8_vs.glsl --separable --reflection
ShaderConverter32.exe -o NVN/gte_shader_8_ps.bnsh -s Glsl -c Binary -a Gl --pixel-shader gte_shader_8_ps.glsl --separable --reflection

ShaderConverter32.exe -o NVN/gte_shader_16_vs.bnsh -s Glsl -c Binary -a Gl --vertex-shader gte_shader_16_vs.glsl --separable --reflection
ShaderConverter32.exe -o NVN/gte_shader_16_ps.bnsh -s Glsl -c Binary -a Gl --pixel-shader gte_shader_16_ps.glsl --separable --reflection

ShaderConverter32.exe -o NVN/blit_shader_vs.bnsh -s Glsl -c Binary -a Gl --vertex-shader blit_shader_vs.glsl --separable --reflection
ShaderConverter32.exe -o NVN/blit_shader_ps.bnsh -s Glsl -c Binary -a Gl --pixel-shader blit_shader_ps.glsl --separable --reflection