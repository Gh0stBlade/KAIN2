#version 330
				
out vec4 fragColor;

varying vec4 v_texcoord;

uniform sampler2D s_texture;
uniform sampler2D s_lut;
vec2 VRAM(vec2 uv) { return texture2D(s_texture, uv).rg; }

void main() {
		vec2 color_rg = VRAM(v_texcoord.xy);
		fragColor = texture2D(s_lut, color_rg);
}