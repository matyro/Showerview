#version 330

layout(location = 0) in  vec3 position;
layout(location = 1) in  float width;
layout(location = 2) in  vec4 color;

out VertexData
{  
  vec4 vColor;
  float vWidth;
} vertex;



void main()
{
    gl_Position = vec4(position.xyz, 1.0);
    vertex.vColor = color;
	vertex.vWidth = width;
} 


