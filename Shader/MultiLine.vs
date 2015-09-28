#version 330

//uniform mat4 projection;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

//out VertexData
//{
//  vec4 vNormal;
//  vec4 vColor;
//} vertex;

out vec4 vColor;

void main() 
{
    gl_Position = vec4(position.xyz, 1.0);
	vColor = 0.00001 * color;   //vertex.vColor 
}
 
