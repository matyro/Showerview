#version 330 core

in vec2 position;	// from -1 to 1

out vec2 texcoord;

void main(void) 
{
  gl_Position = vec4(position, 0.0, 1.0);

  texcoord = position * 0.5 + vec2(0.5, 0.5);
}
