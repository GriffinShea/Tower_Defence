// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;
in float fade;

out vec4 FragColor;

uniform sampler2D onetex;
uniform float fireRedness;//used for fire particles

void main()
{
	vec4 color = texture2D(onetex, uv_interp);
	if (fade == 0) {//the sprite renderer (rshader) passes 0 into fade so render this as a sprite if fade is 0
		FragColor = vec4(color.r,color.g,color.b,color.a);
		//FragColor = color_interp;
		
	} else {//otherwise, render the particle with colour dependant on fade (function of time)
		FragColor = vec4(fade * fireRedness,1 - fade,(2-fireRedness)/2,fade);
	}
    if(color.a < 1)
	{
		discard;
	}
//	 FragColor = color_interp;
}