
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

	const float2 coordf = (2 * convert_float2(coordi) / (float2)(get_global_size(0), get_global_size(1))) - (float2)(1.0f,1.0f);
	

	int i = get_global_id(0) + (get_global_id(1) * get_global_size(0));

	if( i < lineSize )
	{
		const float4 tmpS = lines[i].start;
		const float4 tmpE = lines[i].end;

		pts[i].start = (float4)(
			(mat.s0 * tmpS.s0) + (mat.s1 * tmpS.s1) + (mat.s2 * tmpS.s2)  + (mat.s3 * tmpS.s3),
			(mat.s4 * tmpS.s0) + (mat.s5 * tmpS.s1) + (mat.s6 * tmpS.s2) + (mat.s7 * tmpS.s3),
			(mat.s8 * tmpS.s0) + (mat.s9 * tmpS.s1) + (mat.sa * tmpS.s2) + (mat.sb * tmpS.s3),
			(mat.sc * tmpS.s0) + (mat.sd * tmpS.s1) + (mat.se * tmpS.s2) + (mat.sf * tmpS.s3));

		pts[i].end = (float4)(
			(mat.s0 * tmpE.s0) + (mat.s1 * tmpE.s1) + (mat.s2 * tmpE.s2) + (mat.s3 * tmpE.s3),
			(mat.s4 * tmpE.s0) + (mat.s5 * tmpE.s1) + (mat.s6 * tmpE.s2) + (mat.s7 * tmpE.s3),
			(mat.s8 * tmpE.s0) + (mat.s9 * tmpE.s1) + (mat.sa * tmpE.s2) + (mat.sb * tmpE.s3),
			(mat.sc * tmpE.s0) + (mat.sd * tmpE.s1) + (mat.se * tmpE.s2) + (mat.sf * tmpE.s3));

		
		pts[i].start = pts[i].start / pts[i].start.z;
		pts[i].end = pts[i].end / pts[i].end.z;

	
	
		if(i == 0)
		{
			//printf("Coords Start: %f,%f,%f,%f -> %f,%f,%f,%f\n", lines[i].start.x, lines[i].start.y, lines[i].start.z, lines[i].start.w, pts[i].start.x, pts[i].start.y, pts[i].start.z, pts[i].start.w);
			//printf("Coords End: %f,%f,%f,%f -> %f,%f,%f,%f\n", lines[i].end.x, lines[i].end.y, lines[i].end.z, lines[i].end.w, pts[i].end.x, pts[i].end.y, pts[i].end.z, pts[i].end.w);

			//printf("%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n", mat.s0, mat.s1, mat.s2, mat.s3, mat.s4, mat.s5, mat.s6, mat.s7, mat.s8, mat.s9, mat.sa, mat.sb, mat.sc, mat.sd, mat.se, mat.sf);
		}
	}

	barrier( CLK_GLOBAL_MEM_FENCE );

	float counter = 0;
	float4 color = (float4)(0, 0, 0, 0.0);


	for(int i=0; i<lineSize; i++)
	{
		if(pts[i].start.w <= 0 && pts[i].end.w <= 0)
		{
			continue;
		}

		float2 st = pts[i].start.xy;
		float2 en = pts[i].end.xy;
		
		/*float2 v = en - st;		
		float2 w = coordf - st;


		// bestimme distanz

		float dist = FLT_MAX;
		float multi = 0;

		float c1 = dot(v, w);
		float c2 = dot(v, v);

		if(c1 <= 0)
		{ 
			dist = length(coordf - st);
			multi = pts[i].start.w;
		}
		else if(c2 <= 0)
		{
			dist = length(coordf - en);
			multi = pts[i].end.w;
		}
		else
		{ 
			float2 P = st + (c1 / c2) * v;

			multi = pts[i].start.w + ((pts[i].end.w - pts[i].start.w) * (c1 / c2));
			dist = length(coordf - P);
		}

		if (get_global_id(0) == 400 && get_global_id(1) == 400)
		{ 			
			//printf("Coords Start: %f,%f,%f,%f -> %f,%f,%f,%f\n", lines[i].start.x, lines[i].start.y, lines[i].start.z, lines[i].start.w, pts[i].start.x, pts[i].start.y, pts[i].start.z, pts[i].start.w);
			//printf("Coords End: %f,%f,%f,%f -> %f,%f,%f,%f\n", lines[i].end.x, lines[i].end.y, lines[i].end.z, lines[i].end.w, pts[i].end.x, pts[i].end.y, pts[i].end.z, pts[i].end.w);


			//printf("Dist %i: %f\n",i,dist);
			//printf("Multi %i: %f\n", i, multi);

			color = (float4)(1.0, 0, 1.0f, 1.0);
			counter = 0;
		}
		*/

		float dist = min( length(coordf - st), length(coordf - en));
		float multi = pts[i].start.w;

		//multi = max(0.0f, multi);

		if(dist/multi < 0.5f)
		{
			counter = counter + 1;
		}
	}

	if(counter > 0)
	{ 
		color = (float4)(1.0, counter / lineSize, 0, 1.0);
	}
	
		
	write_imagef(
		image,
		coordi,
		color
	);
}