
typedef struct Line
{
	float4 start;
	float4 end;	
} Line;

typedef

typedef struct Plane
{ 
	float4 camPos;
	float4 viewDir;
	float4 planeTL;
	float4 planeR;
	float4 planeD;
} Plane;

typedef struct Line2D
{
	float2 start;
	float2 end;
} Line2D;




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









//__constant for
__kernel void writeTexture(__write_only image2d_t image, __const float t, __const float16 mat, __global Line* lines, __global Line* pts,  __const int lineSize)
{
	const int2 coordi = (int2)(get_global_id(0), get_global_id(1));

	const float2 coordf = convert_float2(coordi) / (float2)(get_global_size(0), get_global_size(1));


	int i = get_global_id(0) + (get_global_id(1) * get_global_size(0));

	if( i < lineSize )
	{

		pts[i].start = (float4)(dot(mat.s0123, lines[i].start),
			dot(mat.s4567, lines[i].start),
			dot(mat.s89ab, lines[i].start),
			dot(mat.scdef, lines[i].start));

		pts[i].end = (float4)(dot(mat.s0123, lines[i].end),
			dot(mat.s4567, lines[i].end),
			dot(mat.s89ab, lines[i].end),
			dot(mat.scdef, lines[i].end));
	
	
		if(i == 100)
		{
			printf("Coords: %f,%f,%f,%f -> %f,%f,%f,%f\n", lines[100].start.x, lines[100].start.y, lines[100].start.z, lines[100].start.w, pts[i].start.x, pts[i].start.y, pts[i].start.z, pts[i].start.w);
			//printf("%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n", mat.s0, mat.s1, mat.s2, mat.s3, mat.s4, mat.s5, mat.s6, mat.s7, mat.s8, mat.s9, mat.sa, mat.sb, mat.sc, mat.sd, mat.se, mat.sf);
		}
	}


	float4 color = (float4)(0, 0, 0, 0.0);


	for(int i=0; i<lineSize; i++)
	{
		if(pts[i].start.x == coordi.x && pts[i].start.y == coordi.y)
		{
			color = (float4)(1, 0, 0, 1.0);
		}

		if (pts[i].end.x == coordi.x && pts[i].end.y == coordi.y)
		{
			color = (float4)(1, 0, 0, 1.0);
		}
	
	}

	



	
		
	write_imagef(
		image,
		coordi,
		color
	);
}