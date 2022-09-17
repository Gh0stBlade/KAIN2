#version 330

varying vec4 v_texcoord;
varying vec4 v_color;
varying vec4 v_page_clut;

out gl_PerVertex
{
	vec4 gl_Position;
};
	
attribute vec4 a_position;
attribute vec4 a_texcoord; // uv, color multiplier, dither
attribute vec4 a_color;
attribute float a_z;
attribute float a_w;

layout(std140) uniform Mat
{
	mat4 Projection;
};

void main() {
	v_texcoord = a_texcoord;
	v_color = a_color;
	v_color.xyz *= a_texcoord.z;
	v_page_clut.x = fract(a_position.z / 16.0) * 1024.0;
	v_page_clut.y = floor(a_position.z / 16.0) * 256.0;
	v_page_clut.z = fract(a_position.w / 64.0);
	v_page_clut.w = floor(a_position.w / 64.0) / 512.0;
	gl_Position = Projection * vec4(a_position.xy, 0.0, 1.0);
#if defined(PGXP)
	gl_Position.z = a_z;
	gl_Position *= a_w;
#endif
}