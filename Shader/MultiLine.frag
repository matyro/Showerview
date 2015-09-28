#version 330

in  vec4 vColor;
out vec4 outColor;
 
void main(void) {
	outColor = vec4(0.9,0,0, 0.5) + vColor;
}  
