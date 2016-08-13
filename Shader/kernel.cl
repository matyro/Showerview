
typedef struct Line
{
	float4 start;
	float4 end;
	float4 col;
} Line;


//#define OWN_MATH 

#ifdef OWN_MATH
#define vectorLength( vector ) \
   (half_sqrt( (vector).x*(vector).x + (vector).y*(vector).y + (vector).z*(vector).z ))
#else
#define vectorLength( vector ) \
   fast_length(vector)
#endif

#define normalizeVector( v ) \
   v /= vectorLength( v );

#ifdef OWN_MATH
float dotProduct(float4 v1, float4 v2)
{
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}
#else
#define dotProduct( v1, v2 )\
   dot(v1,v2)
#endif 

#ifdef OWN_MATH
float4 crossProduct(float4 v1, float4 v2)
{
	return (float4)(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x, 0);
}
#else
#define crossProduct( v1, v2 )\
   cross(v1,v2)
#endif



const float vec_vec_distance(float4 A, float4 p1, float4 B, float4 p2)
{	
	float4 diff = B - A;	

	float4 n = crossProduct(p1, p2);
	float4 n2 = crossProduct(p2, n);

	const float x1 = dotProduct(diff, n2) / dotProduct(p1, n2);
	if (x1 < 1.0f) { return FLT_MAX; }

	
	const float4 n1 = crossProduct(p1, n);
	const float x2 = clamp(dotProduct(-diff, n1) / dotProduct(p2, n1), 0.0f, 1.0f);
	
	if(x2 == 0.0f || x2 == 1.0f)
	{
	return fast_length(mad(x2, p2, B) - mad(x1, p1, A));	// mad(a,b,c) = a*b+c in fast  || faster -> FLT_MAX;// 
	}	
	else
	{
		return  fast_length( dot(n / fast_length(n), diff) ) ;
	}		
	

	return FLT_MAX;
}


#define smoothEdge  0.02f
#define thickness  0.3f


//cam: m_vec3CamPos, m_vec3TopLeft, m_vec3Right, m_vec3Down
__kernel void writeTexture(__read_write image2d_t image, __global float16* lines, unsigned int lineCount, unsigned int lineSize, float16 cam)
{
	const int2 coordi = (int2)(get_global_id(0), get_global_id(1));
	const float2 coordf = convert_float2(coordi) / (float2)(get_global_size(0), get_global_size(1));



	const float4 ray = cam.s4567 + (coordf.x * cam.s89AB) + (coordf.y * cam.sCDEF);
	const float4 rayDir = cam.s0123 - ray;

	float4 color = (float4)(0.0, 0.0, 0.0, 0.0);

	/*if(get_global_id(0) == 0 &&  get_global_id(1) == 0)
	{
		printf("CamPos: %f,%f,%f,%f\n"	, cam.s0, cam.s1, cam.s2, cam.s3);
		printf("TopLeft: %f,%f,%f,%f\n"	, cam.s4, cam.s5, cam.s6, cam.s7);
		printf("Right: %f,%f,%f,%f\n"	, cam.s8, cam.s9, cam.sA, cam.sB);
		printf("Down: %f,%f,%f,%f\n"	, cam.sC, cam.sD, cam.sE, cam.sF);
	}*/


	for(int i=0; i<lineCount; i++)
	{
		
		//float st = dot(lines[i].start - cam.camPos, cam.viewDir);
		//float en = dot(lines[i].start + lines[i].direction - cam.camPos, cam.viewDir);
		//if(st  > 0 && en > 0)
		//{
			//continue;
		//}


		float dist = vec_vec_distance(cam.s0123, rayDir, lines[i].s0123, lines[i].s4567 - lines[i].s0123);		// Perspective
																														//const float dist = vec_vec_distance(ray, cam.viewDir, lines[i].start, lines[i].direction);			// Orthogonal

		if (dist < thickness)
		{
			color = color + lines[i].s89AB;
		}
		else if(dist < thickness + smoothEdge)
		{
			dist = dist - thickness;
			color = color + ((smoothEdge - dist)/smoothEdge) * lines[i].s89AB;
		}

	}


	write_imagef(
		image,
		coordi,
		color
	);
}


void bla(__read_write image2d_t image, __global float16* lines, unsigned int lineSize, float16 cam)
{
	const int2 coordi = (int2)(get_global_id(0), get_global_id(1));
	const float2 coordf = convert_float2(coordi) / (float2)(get_global_size(0), get_global_size(1));



	const float4 ray = cam.s4567 + (coordf.x * cam.s89AB) + (coordf.y * cam.sCDEF);
	const float4 rayDir = cam.s0123 - ray;

	float4 color = (float4)(0.0, 0.0, 0.0, 0.0);


	int i = get_global_id(2);
	{/*
	 float st = dot(lines[i].start - cam.camPos, cam.viewDir);
	 float en = dot(lines[i].start + lines[i].direction - cam.camPos, cam.viewDir);
	 if(st  > 0 && en > 0)
	 {
	 continue;
	 }*/


		const float dist = vec_vec_distance(cam.s0123, rayDir, lines[i].s0123, lines[i].s4567 - lines[i].s0123);		// Perspective
																														//const float dist = vec_vec_distance(ray, cam.viewDir, lines[i].start, lines[i].direction);			// Orthogonal

		if (dist < thickness)
		{
			color = color + lines[i].s89AB;
		}
	}

	write_imagef(
		image,
		coordi,
		color
	);
}
