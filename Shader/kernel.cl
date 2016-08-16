
#include "../Shader/math.cl"


typedef struct Line3D
{
	float4 start;
	float4 end;
	float4 col;
} Line3D;

typedef struct Line2D
{
	float2 start;
	float2 end;
	float4 col;
} Line2D;



__kernel void transform(__global float16* lineIn, unsigned int lineCount, float16 view, float16 proj)
{ 
	const int i = get_global_id(0);	

	if(i > lineCount)
	{ 
		return;
	}
		
	float4 start = lineIn[i].s0123;
	float4 end = lineIn[i].s4567;
	float4 col = lineIn[i].s89AB;
	float4 empty = (float4)(0.0, 0.0, 0.0, 0.0);


	float4 viewStart = (float4)(start.x + view.s3, start.y + view.s7, start.z + view.sB, 0.0);
		
	viewStart = (float4)(dot(view.s0123, viewStart),
						dot(view.s4567, viewStart),
						dot(view.s89ab, viewStart),
						dot(view.scdef, viewStart));

	float4 viewEnd = (float4)(end.x + view.s3, end.y + view.s7, end.z + view.sB, 0.0);

	viewEnd = (float4)(dot(view.s0123, viewEnd),
		dot(view.s4567, viewEnd),
		dot(view.s89ab, viewEnd),
		dot(view.scdef, viewEnd));
	
	
	lineIn[i].s0123 = viewStart;
	lineIn[i].s4567 = viewEnd;
	lineIn[i].s89AB = col;
}



#define smoothEdge  0.02f
#define thickness  0.3f


//cam: m_vec3CamPos, m_vec3TopLeft, m_vec3Right, m_vec3Down
//Line3D: start, end, color, none
__kernel void writeTexture(__read_write image2d_t image, __global float16* lineIn, unsigned int Line3DCount)
{
	const int2 coordi = (int2)(get_global_id(0), get_global_id(1));
	const int2 sizei = (int2)(get_global_size(0), get_global_size(1));


	const float2 coordf = (convert_float2(coordi) - (convert_float2(sizei) / 2.0f)) / convert_float2(sizei);	
	
	const float4 rayDir = (float4)(coordf.x * tan(M_PI / 4.0), coordf.y * (1.0/1.0) * tan(M_PI / 4.0), -1.0, 0.0 );

	float4 color = (float4)(0.0, 0.0, 0.0, 0.0);


	for(int i=0; i<Line3DCount; i++)
	{
		/*const float4 viewDir = cam.s4567 + cam.s4567 + (0.5 * cam.s89AB) + (0.5 * cam.sCDEF);
		float st = dot(Line3Ds[i].s0123 - cam.s0123, viewDir);
		float en = dot(Line3Ds[i].s4567 - cam.s0123, viewDir);
		if(st  > 0 && en > 0)
		{
			continue;
		}*/


		float dist = vec_vec_distance((float4)(0.0f, 0.0f, 0.0f, 0.0f), rayDir, lineIn[i].s0123, lineIn[i].s4567 - lineIn[i].s0123);		// Perspective
																														//const float dist = vec_vec_distance(ray, cam.viewDir, Line3Ds[i].start, Line3Ds[i].direction);			// Orthogonal

		if (dist < thickness)
		{
			color = color + lineIn[i].s89AB;
		}
		else if(dist < thickness + smoothEdge)
		{
			dist = dist - thickness;
			color = color + ((smoothEdge - dist)/smoothEdge) * lineIn[i].s89AB;
		}

	}


	write_imagef(
		image,
		coordi,
		color
	);
}

