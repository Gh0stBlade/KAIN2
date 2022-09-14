#version 330

out vec4 v_texcoord;

out gl_PerVertex
{
	vec4 gl_Position;
};

in vec4 a_position;
in vec4 a_texcoord;

void main() {
	v_texcoord = a_texcoord * vec4(8.0 / 1024.0, 8.0 / 512.0, 0.0, 0.0);
	gl_Position = vec4(a_position.xy, 0.0, 1.0);
}