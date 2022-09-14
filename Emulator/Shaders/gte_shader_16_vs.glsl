#version 330

out vec4 v_texcoord;
out vec4 v_color;

out gl_PerVertex
{
	vec4 gl_Position;
};

in vec4 a_position;
in vec4 a_texcoord; // uv, color multiplier, dither
in vec4 a_color;
in float a_z;
in float a_w;

layout(std140) uniform Mat
{
   	mat4 Projection;
};

void main() {
	vec2 page;
	page.x = fract(a_position.z / 16.0) * 1024.0;
	page.y = floor(a_position.z / 16.0) * 256.0;;
	v_texcoord = a_texcoord;
	v_texcoord.xy += page;
	v_texcoord.xy *= vec2(1.0 / 1024.0, 1.0 / 512.0);
	v_color = a_color;
	v_color.xyz *= a_texcoord.z;
	gl_Position = Projection * vec4(a_position.xy, 0.0, 1.0);
#if defined(PGXP)
	gl_Position.z = a_z;
	gl_Position *= a_w;
#endif
}