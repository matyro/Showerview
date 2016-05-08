#version 330 core


uniform sampler2D tex;

in vec2 texcoord;

out vec4 color;


void main(void) 
{
   
	color = texture(tex, texcoord) + vec4(0.0, 0.0, 0, 1.0);
	
}
