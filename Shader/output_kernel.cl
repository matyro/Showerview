
#include "../Shader/math.cl"

#include "../Shader/transformation.cl"



#define BATCH_PRIMITIVE_COUNT 256
#define BIN_SIZE 32

#define smoothEdge  0.08f
#define thickness  0.01f

//cam: m_vec3CamPos, m_vec3TopLeft, m_vec3Right, m_vec3Down
//Line3D: start, end, color, none
__kernel void writeTexture(__global float* image, __global float16* lineIn, unsigned int Line3DCount)
{
	const int2 coordi = (int2)(get_global_id(0), get_global_id(1));
	const int2 sizei = (int2)(get_global_size(0), get_global_size(1));

	const float2 coordf = convert_float2(coordi);
	const float2 sizef = convert_float2(sizei);

	const float2 pixel = (float2)( (2.0*(coordf.x / sizef.x)) - 1.0, (2.0*(coordf.y / sizef.y)) - 1.0);

	//const float2 coordf = (convert_float2(coordi) - (convert_float2(sizei) / 2.0f)) / (convert_float2(sizei) / 2.0);	
	
	float4 color = (float4)(1.0, 1.0, 1.0, 0.0);


	for(int i=0; i<Line3DCount; i++)
	{
		/*const float4 viewDir = cam.s4567 + cam.s4567 + (0.5 * cam.s89AB) + (0.5 * cam.sCDEF);
		float st = dot(Line3Ds[i].s0123 - cam.s0123, viewDir);
		float en = dot(Line3Ds[i].s4567 - cam.s0123, viewDir);
		if(st  > 0 && en > 0)
		{
			continue;
		}*/


		//float dist = originvec_vec_distance(rayDir, lineIn[i].s0123, lineIn[i].s4567 - lineIn[i].s0123);		// Perspective


		float4 pixelPos = (float4)(pixel.x * 100.0, pixel.y * 100.0, 0.0, 0.0);
		float4 viewDir = (float4)(0.0, 0.0, -1.0, 0.0);
		float dist = vec_vec_distance(pixelPos, viewDir, lineIn[i].s0123, lineIn[i].s4567 - lineIn[i].s0123);			// Orthogonal

		if (dist < thickness)
		{
			color = color - lineIn[i].s89AB;
		}
		else if(dist < thickness + smoothEdge)
		{
			dist = dist - thickness;
			color = color - ((smoothEdge - dist)/smoothEdge) * lineIn[i].s89AB;
		}

	}

	color = clamp(color, 0.0, 1.0);

	const int position = coordi.x * sizei.y + coordi.y ;
	image[4 * position + 0] = color.x;
	image[4 * position + 1] = color.y;
	image[4 * position + 2] = color.z;
	image[4 * position + 3] = 0.0;
}

