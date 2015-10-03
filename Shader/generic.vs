#version 330 core

layout(location = 0) in  vec3 position;
layout(location = 1) in  vec4 color;

out vec4 vColor;

uniform mat4 projectionView;
uniform mat4 model;

void main()
{
    gl_Position = projectionView * model * vec4(position.xyz, 1.0);
    vColor = color;
} 
