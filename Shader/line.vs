#version 330 core

layout(location = 0) in  vec3 in_Pos;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(in_Pos.xyz, 1.0);
} 
