#version 330

layout(triangles) in;
layout(triangle_strip) out;
layout(max_vertices = 3) out;

uniform mat4 projection;

in VertexData
{
  vec4 vNormal;
  vec4 vColor;
} vertex[];

out vec4 fColor; // Output to fragment shader

void main() 
{

	//vec3 right = vec3(vProjection[0][0][0], vProjection[0][1][0], vProjection[0][2][0]); 
 
 	//vec3 up = vec3(vProjection[0][0][1], vProjection[0][1][1], vProjection[0][2][1]); 
	
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
	
	fColor = vertex[0].vColor;	
	gl_Position = projection * (gl_in[0].gl_Position);
	EmitVertex();
	
	fColor = vertex[0].vColor;
	gl_Position = projection * (gl_in[1].gl_Position);
	EmitVertex();
	
	fColor = vertex[0].vColor;
	gl_Position = projection * (gl_in[2].gl_Position);
	EmitVertex();
		
	EndPrimitive();
}


	
	
	