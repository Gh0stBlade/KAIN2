#version 330

out vec4 fragColor;

varying vec4 v_texcoord;
varying vec4 v_color;
varying vec4 v_page_clut;

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
	
uniform sampler2D s_texture;
uniform sampler2D s_lut;
	
vec2 VRAM(vec2 uv) { return texture(s_texture, uv).rg; }
	
void main() {
	vec2 uv = (v_texcoord.xy * vec2(0.25, 1.0) + v_page_clut.xy) * vec2(1.0 / 1024.0, 1.0 / 512.0);
	vec2 comp = VRAM(uv);
	int index = int(fract(v_texcoord.x / 4.0 + 0.0001) * 4.0);

	float v = 0.0;
	if(index / 2 == 0) { 
		v = comp[0] * (255.0 / 16.0);
	} else {	
		v = comp[1] * (255.0 / 16.0);
	}
	float f = floor(v);

	vec2 c = vec2( (v - f) * 16.0, f );

	vec2 clut_pos = v_page_clut.zw;
	clut_pos.x += mix(c[0], c[1], fract(float(index) / 2.0) * 2.0) / 1024.0;
	vec2 color_rg = VRAM(clut_pos);
	if (color_rg.x + color_rg.y == 0.0) { discard; }
	fragColor = texture(s_lut, color_rg);
	fragColor *= v_color;
	ivec2 dc = ivec2(fract(gl_FragCoord.xy / 4.0) * 4.0);
	fragColor.xyz += DITHER(dc);
}