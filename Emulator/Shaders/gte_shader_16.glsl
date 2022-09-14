#version 330
	
#ifdef VERTEX
	#define varying   out
	#define attribute in
	#define texture2D texture
#else
	#define varying     in
	#define texture2D   texture
	out vec4 fragColor;
#endif

