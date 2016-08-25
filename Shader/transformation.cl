
// rotate and move 

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

