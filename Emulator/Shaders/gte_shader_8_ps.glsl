#version 330

out vec4 fragColor;

out vec4 v_texcoord;
out vec4 v_color;
out vec4 v_page_clut;

uniform sampler2D s_texture;
uniform sampler2D s_lut;

vec2 VRAM(vec2 uv) { return texture2D(s_texture, uv).rg; }

	mat4 dither = mat4(
			-4.0,  +0.0,  -3.0,  +1.0,
			+2.0,  -2.0,  +3.0,  -1.0,
			-3.0,  +1.0,  -4.0,  +0.0,
			+3.0,  -1.0,  +2.0,  -2.0) / 255.0;
		vec3 DITHER(const ivec2 dc) { 
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
			if(i == dc.x && j == dc.y) {
				return vec3(dither[i][j] * v_texcoord.w); }
				}
			}
		}
void main() {
	vec2 uv = (v_texcoord.xy * vec2(0.5, 1.0) + v_page_clut.xy) * vec2(1.0 / 1024.0, 1.0 / 512.0);
	vec2 comp = VRAM(uv);

	vec2 clut_pos = v_page_clut.zw;
	int index = int(mod(v_texcoord.x, 2.0));
	if(index == 0) { 
		clut_pos.x += comp[0] * 255.0 / 1024.0;
	} else {	
		clut_pos.x += comp[1] * 255.0 / 1024.0;
	}
	vec2 color_rg = VRAM(clut_pos);
	if (color_rg.x + color_rg.y == 0.0) { discard; }
	fragColor = texture2D(s_lut, color_rg);
		fragColor *= v_color;
		ivec2 dc = ivec2(fract(gl_FragCoord.xy / 4.0) * 4.0);
		fragColor.xyz += DITHER(dc);
}