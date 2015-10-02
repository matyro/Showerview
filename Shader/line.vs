#version 330 core

layout(location = 0) in  vec3 position;
layout(location = 1) in  vec4 color;

out vec4 vColor;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(position.xyz, 1.0);
    vColor = color;
} 
