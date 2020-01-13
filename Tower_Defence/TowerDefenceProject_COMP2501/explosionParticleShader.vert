#version 130

// Vertex buffer
in vec2 vertex;//position
in vec2 dir;//direction
in float t;//time
in vec2 uv;//texture

// Attributes forwarded to the fragment shader
out vec4 color_interp;
out vec2 uv_interp;
out float fade;

// Uniform (global) buffer
uniform mat4 transformationMatrix;
uniform float time;
uniform vec2 direction;


void main()
{
	vec4 ppos;
	float acttime;
	float speed = 5.0;
	float gravity = 20;
    ppos = vec4(vertex.x+dir.x*time*speed, vertex.y+dir.y*time*speed, 0.0, 1.0);
    gl_Position = transformationMatrix*ppos;

    color_interp = vec4(uv,1.0,1.0);
	uv_interp = uv;
	fade = 0;
}