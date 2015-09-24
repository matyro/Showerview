#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 vColor[]; // Output from vertex shader for each vertex
in vec3 vLine[];
in float vWidth[];
in mat4 vProjection[];


out vec4 fColor; // Output to fragment shader

void main() {

	vec3 right = vec3(vProjection[0][0][0], vProjection[0][1][0], vProjection[0][2][0]); 
 
 	vec3 up = vec3(vProjection[0][0][1], vProjection[0][1][1], vProjection[0][2][1]); 
	
	 	//2____4__pos__6____8
		//|\   |\      |\   |
		//||   | \     ||   |
		//| \  |  \    | \  | y
		//| |  |   \   | |  |	 __	z
		//|  \ |    \  |  \ |   /|
		//|  | |     \ |  | |  /
		//|___\| _____\|___\| /
		//1    3  dir  5    7
		// x
	
	fColor = vColor[0];	
	gl_Position = gl_in[0].gl_Position + vec4(1.0,1.0,0,0);
	EmitVertex();
	
	fColor = vColor[0];	
	gl_Position = gl_in[0].gl_Position + vec4(-1.0,1.0,0,0);
	EmitVertex();
	
	fColor = vColor[0];	
	gl_Position = gl_in[0].gl_Position + vec4(-1.0,-1.0,0,0);
	EmitVertex();
		
	EndPrimitive();
}


	
	/*	
	//Point 1:
	fColor = vec4(vColor[0].xyz, 0.0);
	gl_Position = vec4(gl_in[0].gl_Position.xyz + vDir[0] - (2.0 * vWidth[0] * right), 1.0);
	EmitVertex();	// Transfers Vertex with current fColor to fragment Shader
	
	//Point 2:
	fColor = vec4(vColor[0].xyz, 0.0);
	gl_Position = vec4(gl_in[0].gl_Position.xyz - (2.0 * vWidth[0] * right), 1.0);
	EmitVertex();	// Transfers Vertex with current fColor to fragment Shader
	
	//Point 3:
	fColor = vColor[0];
	gl_Position = vec4(gl_in[0].gl_Position.xyz + vDir[0] - (vWidth[0] * right), 1.0);
	EmitVertex();	// Transfers Vertex with current fColor to fragment Shader
	
	//Point 4:
	fColor = vColor[0];
	gl_Position = vec4(gl_in[0].gl_Position.xyz - (vWidth[0] * right), 1.0);
	EmitVertex();	// Transfers Vertex with current fColor to fragment Shader
	
	//Point 5:
	fColor = vColor[0];
	gl_Position = vec4(gl_in[0].gl_Position.xyz + vDir[0] + (vWidth[0] * right), 1.0);
	EmitVertex();	// Transfers Vertex with current fColor to fragment Shader
	
	//Point 6:
	fColor = vColor[0];
	gl_Position = vec4(gl_in[0].gl_Position.xyz + (vWidth[0] * right), 1.0);
	EmitVertex();	// Transfers Vertex with current fColor to fragment Shader
	
	//Point 7:
	fColor = vec4(vColor[0].xyz, 0.0);
	gl_Position = vec4(gl_in[0].gl_Position.xyz + vDir[0] + (2.0 * vWidth[0] * right), 1.0);
	EmitVertex();	// Transfers Vertex with current fColor to fragment Shader
	
	//Point 8:
	fColor = vec4(vColor[0].xyz, 0.0);
	gl_Position = vec4(gl_in[0].gl_Position.xyz + (2.0 * vWidth[0] * right), 1.0);
	EmitVertex();	// Transfers Vertex with current fColor to fragment Shader
	*/
	