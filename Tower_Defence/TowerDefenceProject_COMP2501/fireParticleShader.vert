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
	acttime = mod(time + t, 0.5);
	float scale = acttime;
    ppos = vec4(vertex.x+dir.x*acttime*speed + direction.x*gravity*acttime*acttime, vertex.y+dir.y*acttime*speed + direction.y*gravity*acttime*acttime, 0.0, 1.0);
    gl_Position = transformationMatrix*ppos;

    color_interp = vec4(uv,1.0,1.0);
	uv_interp = uv;
	fade = acttime * 2;
}