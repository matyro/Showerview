
typedef struct Line
{
	float4 start;
	float4 end;	
	float4 col;
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
__kernel void writeTexture(__write_only image2d_t image, __const float4 mov, __const float16 rot, __const float16 proj, __global Line* lines, __global Line* pts,  __const int lineSize, __const int frames)
{
	const int2 coordi = (int2)(get_global_id(0), get_global_id(1));

	const float2 coordf = (2 * convert_float2(coordi) / (float2)(get_global_size(0), get_global_size(1))) - (float2)(1.0f,1.0f);
	

	int i = get_global_id(0) + (get_global_id(1) * get_global_size(0));

	if( i < lineSize )
	{
		float4 tmpS = lines[i].start + mov;
		float4 tmpE = lines[i].end + mov;

		tmpS = (float4)(
			dot(rot.s0123, tmpS),
			dot(rot.s4567, tmpS),
			dot(rot.s89ab, tmpS),
			dot(rot.scdef, tmpS));

		tmpE = (float4)(
			dot(rot.s0123, tmpE),
			dot(rot.s4567, tmpE),
			dot(rot.s89ab, tmpE),
			dot(rot.scdef, tmpE));


		tmpS = (float4)(
			dot(proj.s0123, tmpS),
			dot(proj.s4567, tmpS),
			dot(proj.s89ab, tmpS),
			dot(proj.scdef, tmpS));

		tmpE = (float4)(
			dot(proj.s0123, tmpE),
			dot(proj.s4567, tmpE),
			dot(proj.s89ab, tmpE),
			dot(proj.scdef, tmpE));
		
		


		pts[i].start = tmpS / tmpS.w;
		pts[i].end = tmpE / tmpE.w;
	

	
	
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
		if(pts[i].start.z <= 0 && pts[i].end.z <= 0)
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
		float multi = length(pts[i].start.s012);

		//multi = max(0.0f, multi);

		if(dist/multi < 0.5f)
		{
			//color = color * lines[i].col;
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