







__kernel void transform_perspective(__global float16* lineIn, unsigned int lineCount, float16 view, float16 proj)
{ 
	const int i = get_global_id(0);	
	
	if(i >= lineCount)
	{ 
		return;
	}
	
	float4 start = lineIn[i].s0123;
	float4 end = lineIn[i].s4567;
	float4 col = lineIn[i].s89AB;
	float4 empty = (float4)(0.0, 0.0, 0.0, 0.0);

	//printf("Start: %f,%f,%f   <<\n", start.x, start.y, start.z);


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
	
	//printf("Start transform: %f,%f,%f \n\n", viewStart.x, viewStart.y, viewStart.z);
	
	lineIn[i].s0123 = viewStart;
	lineIn[i].s4567 = viewEnd;
	lineIn[i].s89AB = col;
}
