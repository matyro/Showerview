
typedef struct Line
{
	float4 start;
	float4 direction;	
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




__kernel void transformLines(float16 mat, __global Line* lines, __const int lineSize)
{ 
	const int2 coordi = (int2)(get_global_id(0), get_global_id(1));
	
	float4 st = (float4)(dot(mat.s0123, lines.start),
				dot(mat.s4567, lines.start),
				dot(mat.s89ab, lines.start),
				dot(mat.scdef, lines.start));

	float4 en = (float4)(dot(mat.s0123, lines.end),
		dot(mat.s4567, lines.end),
		dot(mat.s89ab, lines.end),
		dot(mat.scdef, lines.end));


}







//__constant for
__kernel void writeTexture(__write_only image2d_t image, __const float t, __global Line* lines, __const int lineSize)
{
	const int2 coordi = (int2)(get_global_id(0), get_global_id(1));

	const float2 coordf = convert_float2(coordi) / (float2)(get_global_size(0), get_global_size(1));


	float4 color = (float4)(0, 0, 0, 0.0);
	
		
	write_imagef(
		image,
		coordi,
		color
	);
}