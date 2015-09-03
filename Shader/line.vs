#version 330 core

layout(location = 0) in  vec4 in_Pos;
layout(location = 1) in  vec4 in_Col;
out vec4 ex_Col;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(in_Pos.xyz, 1.0);
    ex_Col = in_Col;
} 
