
typedef struct Line
{
	float4 start;
	float4 direction;
	float4 color;	
} Line;

typedef struct Plane
{ 
	float4 camPos;
	float4 viewDir;
	float4 planeTL;
	float4 planeR;
	float4 planeD;
} Plane;


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
	
	//if(x2 == 0.0f || x2 == 1.0f)
	//{
	return fast_length(mad(x2, p2, B) - mad(x1, p1, A));	// mad(a,b,c) = a*b+c in fast  || faster -> FLT_MAX;// 
	//}	
	//else
	//{
		//return  fast_length( dot(n / fast_length(n), diff) ) ;
	//}		
	

	//return FLT_MAX;
}


#define smoothEdge  0.01f
#define thickness  0.1f

//__constant for
__kernel void writeTexture(__write_only image2d_t image, float t, const Plane cam, __global const Line* lines, const int lineSize)
{
	const int2 coordi = (int2)(get_global_id(0), get_global_id(1));

	const float2 coordf = convert_float2(coordi) / (float2)(get_global_size(0), get_global_size(1));

	const float4 ray = cam.planeTL + (coordf.x * cam.planeR) + (coordf.y * cam.planeD);


	float4 color = (float4)(0, 0, 0, 0.0);
	
	//if(coordi.x == 100 && coordi.y == 100)
	//{
	//	printf("Lines: %i", lineSize);
	//}
	
	

	const float4 rayDir = cam.camPos - ray;	

#pragma unroll
	for(int i=0; i<lineSize; i++)
	{
		/*float st = dot(lines[i].start - cam.camPos, cam.viewDir);
		float en = dot(lines[i].start + lines[i].direction - cam.camPos, cam.viewDir);
		if(st  > 0 && en > 0)
		{ 			
			continue;
		}*/


		const float dist = vec_vec_distance(cam.camPos, rayDir, lines[i].start, lines[i].direction);		// Perspective
		//const float dist = vec_vec_distance(ray, cam.viewDir, lines[i].start, lines[i].direction);			// Orthogonal

		if(dist < thickness)
		{
			color = color + lines[i].color;
		}		
	}	 

	write_imagef(
		image,
		coordi,
		color
	);
}