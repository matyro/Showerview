#version 330

layout(lines) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

uniform mat4 projectionView;
uniform vec3 camPos;

in VertexData
{  
  vec4 vColor;
  float vWidth;
} vertex[];

out vec4 fColor; // Output to fragment shader

void main() 
{	
	vec4 cameraPosition = (inverse(projectionView))[3];
	
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

	vec4 smallDirection0 =  vec4( -0.45 * vertex[0].vWidth * normalize(cross(gl_in[0].gl_Position.xyz - camPos,  gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz)), 0.0) ;
	vec4 smallDirection1 =  vec4( -0.45 * vertex[1].vWidth * normalize(cross(gl_in[1].gl_Position.xyz - camPos,  gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz)), 0.0) ;
	vec4 bigDirection0 = 1.1 * smallDirection0;
	vec4 bigDirection1 = 1.1 * smallDirection1;


	//fColor = vec4(vertex[0].vColor.xyz, 0.0);	
	//gl_Position = projectionView * (gl_in[0].gl_Position - bigDirection0);
	//EmitVertex();

	//fColor = vec4(vertex[1].vColor.xyz, 0.0);
	//gl_Position = projectionView * (gl_in[1].gl_Position - bigDirection1);
	//EmitVertex();
	


	fColor = vertex[0].vColor;	
	gl_Position = projectionView * (gl_in[0].gl_Position - smallDirection0);
	EmitVertex();

	fColor = vertex[1].vColor;	
	gl_Position = projectionView * (gl_in[1].gl_Position - smallDirection1);
	EmitVertex();



	fColor = vertex[0].vColor;	
	gl_Position = projectionView * (gl_in[0].gl_Position + smallDirection0);
	EmitVertex();

	fColor = vertex[1].vColor;	
	gl_Position = projectionView * (gl_in[1].gl_Position + smallDirection1);
	EmitVertex();



	//fColor = vec4(vertex[0].vColor.xyz, 0.0);	
	//gl_Position = projectionView * (gl_in[0].gl_Position + bigDirection0);
	//EmitVertex();

	//fColor = vec4(vertex[1].vColor.xyz, 0.0);
	//gl_Position = projectionView * (gl_in[1].gl_Position + bigDirection1);
	//EmitVertex();
	
		
	EndPrimitive();
}


	
	
	