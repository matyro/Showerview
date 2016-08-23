
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


///////////////////////////////////////////////

#define BATCH_PRIMITIVE_COUNT 256
#define BIN_SIZE 32

//Each work-group (7) processes one batch of primitives at a time, while each workitem (1024) processes that batch for exactly one bin.
// lineIn: line input
// lineOut line Output
// batchCounter: number of processed batches (internal control var)
// batch_count: number of batches to calculate (each batch = 256 lines)
// bin_count: number of bins in x and y direction
__kernel void level1reduce(__global float16* lineIn, __global float16* lineOut, __global int* batchCounter, const int batch_count, const int2 binCount)
{ 
	const unsigned int global_id = get_global_id(0);
	const unsigned int local_id = get_local_id(0);
	const unsigned int local_size = get_local_size(0);

	if (global_id == 0) 
	{
		batchCounter = 0;
	}
	barrier(CLK_GLOBAL_MEM_FENCE);

	__local float16 localBatchStorage[BATCH_PRIMITIVE_COUNT];
	__local unsigned int batchIdx;

	while(true)
	{
		
		// Recieve next (global) batchIdx for this workgroup
		if (local_id == 0)
		{
			batchIdx = atomic_inc(batchCounter);
		}
		barrier(CLK_LOCAL_MEM_FENCE);

		if (batchIdx >= batch_count) {
			return;
		}



		// Load a batch of lines from global memory into local memory
		const unsigned int primitive_id_offset = batchIdx * BATCH_PRIMITIVE_COUNT;

		event_t event = async_work_group_copy(&localBatchStorage[0],
												(__global const float16*)&lineIn[primitive_id_offset],
												BATCH_PRIMITIVE_COUNT, 0);		
		wait_group_events(1, &event);



		// Bin worker part :

		const int number_of_bins = binCount.x * binCount.y;
		// in cases where #bins > #work-items, we need to iterate over all bins (simply offset the bin_idx by the #work-items)
		for (int bin_idx = local_id; bin_idx < number_of_bins; bin_idx = bin_idx + local_size)
		{
			const uint2 bin_location = (uint2)(bin_idx % binCount.x, bin_idx / binCount.x);
			




			// copy queue to global memory (note that some/all implementations have 64-bit loads/stores -> use an ulong4)
			const size_t tmp_offset = (bin_idx * number_of_bins + batchIdx);
			lineOut[tmp_offset] = localBatchStorage[batchIdx];


		}
	}
}




#define smoothEdge  0.004f
#define thickness  0.1f


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


		float dist = originvec_vec_distance(rayDir, lineIn[i].s0123, lineIn[i].s4567 - lineIn[i].s0123);		// Perspective
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

