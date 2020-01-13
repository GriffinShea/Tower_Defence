// Source code of vertex shader
#version 130

// Vertex buffer
in vec2 vertex;//position
in vec3 color;//color before texture
in vec2 uv;//texture

// Uniform (global) buffer
uniform mat4 transformationMatrix;

// Attributes forwarded to the fragment shader
out vec4 color_interp;
out vec2 uv_interp;
out float fade;

void main()
{
	vec4 vertexPos = vec4(vertex, 0.0, 1.0);
    gl_Position = transformationMatrix * vertexPos;
	
    color_interp = vec4(color, 1.0);
	uv_interp = uv;
	fade = 0;
}