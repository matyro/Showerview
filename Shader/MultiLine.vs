#version 330 core

layout(location = 0) in vec3 start;
layout(location = 1) in vec3 line;
layout(location = 2) in vec4 color;
layout(location = 3) in float width;

out vec4 vColor;
out vec3 vLine;
out float vWidth;
out mat4 vProjection;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(start.xyz, 1.0);
    vColor = color;
    vLine = line;
	vWidth = width;

	vProjection = projection;
}
 
