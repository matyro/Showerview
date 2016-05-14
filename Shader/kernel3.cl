
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









//__constant for
__kernel void writeTexture(__read_write image2d_t image, float16 view, float16 proj, __global Line* lines, int lineSize, int frames)
{
	

	int lineID = get_global_id(0);
	int2 coordi = (int2)(get_global_id(1) , get_global_id(2) );//(int2)(get_global_id(1) * 8, get_global_id(2) * 8);
	//coordi = coordi + (int2)(get_local_id(1), get_local_id(2));

	__local float4 start;
	__local float4 end;

	__local float4 viewStart;
	__local float4 viewEnd;

	__local float4 projStart;
	__local float4 projEnd;

	if (lineID < lineSize && (get_local_id(1) == 0) && (get_local_id(2) == 0))
	{
		start = lines[lineID].start;
		end = lines[lineID].end;

		viewStart = (float4)(
			dot(view.s0123, start),
			dot(view.s4567, start),
			dot(view.s89ab, start),
			dot(view.scdef, start));

		viewEnd = (float4)(
			dot(view.s0123, end),
			dot(view.s4567, end),
			dot(view.s89ab, end),
			dot(view.scdef, end));

		projStart = (float4)(
			dot(proj.s0123, viewStart),
			dot(proj.s4567, viewStart),
			dot(proj.s89ab, viewStart),
			dot(proj.scdef, viewStart));

		projEnd = (float4)(
			dot(proj.s0123, viewEnd),
			dot(proj.s4567, viewEnd),
			dot(proj.s89ab, viewEnd),
			dot(proj.scdef, viewEnd));

		projStart = projStart / projStart.w;	
		projEnd = projEnd / projEnd.w;

		if(get_global_id(1) == 0 && get_global_id(2) == 0)
		{
			//printf("Coords View Start: %f,%f,%f,%f -> %f,%f,%f,%f\n", start.x, start.y, start.z, start.w, viewStart.x, viewStart.y, viewStart.z, viewStart.w);
			//printf("Coords View End: %f,%f,%f,%f -> %f,%f,%f,%f\n", end.x, end.y, end.z, end.w, viewEnd.x, viewEnd.y, viewEnd.z, viewEnd.w);
			//printf("Coords Proj Start: %f,%f,%f,%f -> %f,%f,%f,%f\n", start.x, start.y, start.z, start.w, projStart.x, projStart.y, projStart.z, projStart.w);
			//printf("Coords Proj End: %f,%f,%f,%f -> %f,%f,%f,%f\n", end.x, end.y, end.z, end.w, projEnd.x, projEnd.y, projEnd.z, projEnd.w);
		}
		

	}

	barrier(CLK_LOCAL_MEM_FENCE);

		
	
	if (viewStart.z > 0 && viewEnd.z > 0)
	{
		//float4 imageSegment = read_imagef(image, coordi);
		float4 imageSegment = (float4)(0.0f, 0.0f, 0.0f, 0.0f);

		float2 coordf = (2 * convert_float2(coordi) / (float2)(get_global_size(1), get_global_size(2))) - (float2)(1.0f, 1.0f);

		float2 st = projStart.xy;
		float2 en = projEnd.xy;

		float2 v = en - st;
		float2 w = coordf - st;


		// bestimme distanz

		float dist = FLT_MAX;
		float multi = 0;

		float c1 = dot(v, w);
		float c2 = dot(v, v);

		if (c1 <= 0)
		{
			dist = length(coordf - st);
			multi = length(viewStart);
		}
		else if (c2 <= c1)
		{
			dist = length(coordf - en);
			multi = length(viewEnd);
		}
		else
		{
			float2 P = st + (c1 / c2) * v;

			multi = length(viewStart + ((viewEnd - viewStart) * (c1 / c2)));
			dist = length(coordf - P);
		}

		

		if (dist*multi < 0.2f)
		{
			imageSegment.x = 1.0f;
			imageSegment.y = imageSegment.y + (1.0f / get_global_size(0));
			write_imagef(image, coordi, imageSegment);
		}
		else
		{
			write_imagef(image, coordi, (float4)(0.2f, 0.1f, 0.2f, 1.0f));
		}


		if (get_global_id(1) == 512 && get_global_id(2) == 512)
		{			
			write_imagef(image, coordi, (float4)(0.0f, 1.0f, 0.0f, 1.0f));
		}
	}


	return;
}