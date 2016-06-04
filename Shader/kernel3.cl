
typedef struct Line
{
	float4 start;
	float4 end;
	float4 col;
} Line;


typedef struct projectedLine
{ 
	float2 start;
	float2 end;
	float2 z;
}


__kernel void tansform(float16 view, float16 proj, __global Line* lines, unsigned int lineCount)
{
	int id = get_global_id(0);

	if (id >= lineCount)
		return;

	float4 start = lines[id].start;
	float4 end = lines[id].end;

	float4 viewStart = (float4)(
		dot(view.s0123, start),
		dot(view.s4567, start),
		dot(view.s89ab, start),
		dot(view.scdef, start));

	lines[id].start = viewStart;

	float4 viewEnd = (float4)(
		dot(view.s0123, end),
		dot(view.s4567, end),
		dot(view.s89ab, end),
		dot(view.scdef, end));

	lines[id].end = viewEnd;


	float lengthStart = dot(proj.scdef, viewStart);	// w part of vector
	float lengthEnd = dot(proj.scdef, viewEnd);		// w part of vector


	lines[id].start = (float4)(
		dot(proj.s0123, viewStart) / lengthStart,
		dot(proj.s4567, viewStart) / lengthStart,
		viewStart.z,
		lengthStart);

	lines[id].end = (float4)(
		dot(proj.s0123, viewEnd) / lengthEnd,
		dot(proj.s4567, viewEnd) / lengthEnd,
		viewEnd.z,
		lengthEnd);


	start = lines[id].start;
	end = lines[id].end;
	printf("1 Start: %f %f %f %f\n", start.x, start.y, start.z, start.w);
	printf("1 End: %f %f %f %f\n", start.x, start.y, start.z, start.w);

	/*if (get_global_id(1) == 0 && get_global_id(2) == 0)
	{
			printf("Coords View Start: %f,%f,%f,%f -> %f,%f,%f,%f\n", start.x, start.y, start.z, start.w, viewStart.x, viewStart.y, viewStart.z, viewStart.w);
			printf("Coords View End: %f,%f,%f,%f -> %f,%f,%f,%f\n", end.x, end.y, end.z, end.w, viewEnd.x, viewEnd.y, viewEnd.z, viewEnd.w);
			printf("Coords Proj Start: %f,%f,%f,%f -> %f,%f,%f,%f\n", start.x, start.y, start.z, start.w, projStart.x, projStart.y, projStart.z, projStart.w);
			printf("Coords Proj End: %f,%f,%f,%f -> %f,%f,%f,%f\n", end.x, end.y, end.z, end.w, projEnd.x, projEnd.y, projEnd.z, projEnd.w);
		}*/
}










#define BIN_DISTANT (sqrt((64.0f * 64.0f) + (64.0f * 64.0f))/512.0f)

#define BIN_X_PIX  128.0f
#define BIN_X2_PIX 64.0f

#define BIN_Y_PIX  128.0f
#define BIN_Y2_PIX 64.0f


#define IMAGE_X 1024.0f
#define IMAGE_Y 1024.0f

#define BIN_CORNER_X 0.0625f		//64.0f / 1024.0f
#define BIN_CORNER_Y 0.0625f		//64.0f / 1024.0f






float pointVecDistance(const float2 p, const float2 sta, const float2 end)
{
	printf("Point to Vec: (%f %f) to (%f %f)(%f %f)\n", p.x, p.y, sta.x, sta.y, end.x, end.y);

	const float2 v = end - sta;
	const float2 w = p - sta;


	const float c1 = dot(v, w);
	const float c2 = dot(v, v);

	if (c1 <= 0)
	{
		return fast_length(p - sta);
	}
	else if (c2 <= c1)
	{
		return fast_length(p - end);
	}
	else
	{
		const float2 P = sta + (c1 / c2) * v;

		return fast_length(p - P);
	}

	return FLT_MAX;
}

bool line_intersect(float4 line1, float4 line2, float2 interPoint)
{
	float2 u0 = line1.xy;			// x00, y00
	float2 v0 = line1.zw - u0;		// x01, y01

	float2 u1 = line2.xy;			// x10, y10
	float2 v1 = line2.zw - u0;		// x11, y11


	float s = (((u0.x - u1.x) * v0.y) - ((u0.y - u1.y) * v0.x)) / ((v1.x * v0.y) - (v0.x * v1.y));
	float t = -(-((u0.x - u1.x) * v1.y) + ((u0.y - u1.y) * v1.x)) / ((v1.x * v0.y) - (v0.x * v1.y));

	interPoint = u0 + s * v0;

	if (s < 0 || t < 0)
	{
		return false;
	}
	else if (s > 1 || t > 1)
	{
		return false;
	}

	return true;
}




// ceil(n/256) x 8 x 8 global workgroup size
// 256 x 1 x 1 local workgroup size  -> 256 lines per workgroup
__kernel void sortToBins(__global Line* lines, unsigned int lineCount, __global unsigned int* counter, __global Line* sortedLines)
{	
	int lineID = get_local_id(0);	// 0 to 256

	lineID = lineID + 256 * get_group_id(0);

	if (lineID >= lineCount)
		return;	

	const float4 start = lines[lineID].start;
	const float4 end = lines[lineID].end;

	const int2 bin = (int2)(get_global_id(1), get_global_id(2));



	// bin = 0,1,2,3,4,5,6,7
	// bin - 3.5 = -3.5 -2.5 -1.5 -0.5 +0.5 +1.5 +2.5 +3.5
	// bin / 4.0 = -0.875 -0.625 -0.375 -0.125 ...
	const float2 binCenterCoor = ( convert_float2(bin) - 3.5f ) / 4.0f;

	//printf("2 Start: %f %f %f %f\n", start.x, start.y, start.z, start.w);
	//printf("2 End: %f %f %f %f\n", end.x, end.y, end.z, end.w);
	//printf("Bin: %f %f\n", binCenterCoor.x, binCenterCoor.y);

	if ((start.z <= 1.0f) && (end.z <= 1.0f))
		return;

	float dist = pointVecDistance(binCenterCoor, start.s01, end.s01);

	if(dist < BIN_DISTANT)
	{ 
		int binID = atomic_inc(counter + bin.x + (bin.y * 8));
		sortedLines[binID].start = start;
		sortedLines[binID].end = end;

		//printf("Line: %i,\n Bin: %i, %i\n Dist: %f \n", lineID, bin.x, bin.y, dist );
	}
	
}





__kernel void sortToTile(__global Line* sortedLines, __global unsigned int* counter)
{
	int lineID = get_local_id(0);	// 0 to 256

	lineID = lineID + 256 * get_group_id(0);

	const int2 bin = (int2)(get_global_id(1), get_global_id(2));

	if (lineID >= counter[bin.x + (8 * bin.y)])
		return;

	const float4 start = sortedLines[lineID].start;
	const float4 end = sortedLines[lineID].end;

	 

}











// 1024 x 1024 global size -> pixel level
// 16 x 16 tile size
__kernel void writeTexture(__read_write image2d_t image, __global Line* sortedLines, __global unsigned int* frameCounter)
{
	pointVecDistance((float2)(0.0f, 0.0f),
		(float2)(-1.0f, 0.0f),
		(float2)(0.0f, 1.0f)
		);

	return;

	const int2 bin = (int2)(get_group_id(0) &(~0x07) >> 4, get_group_id(1) &(~0x07) >> 4);
	const float2 binCenterCoor = (convert_float2(bin) - 3.5f) / 4.0f;

	const int2 tile = (int2)(get_group_id(0) & 0x07, get_group_id(1) & 0x07);
	const float2 tileCenterCoor = ((convert_float2(tile) - 3.5f) / (8.0f * 4.0f)) + binCenterCoor;


	const int2 pix = (int2)(get_local_id(0), get_local_id(1));



	
	
	int linesInBin = sortedLines[bin.x + (bin.y * 8)];

	// Test lines of this bin if they hit this tile
	int id = pix.x + (pix.y * 16);

	__local int counter;
	counter = 0;

	__local projectedLine linesInTile[512];	// 12288 Byte | 512 * 6(elements) * 4(byte)

	while(true)
	{
		if (id >= linesInBin)
			break;


		Line tmp = sortedLines[id];

		float dist = pointVecDistance(tileCenterCoor, tmp.start.s01, tmp.end.s01);	//calculate Distance

		if(dist < 0)
		{
			linesInTile[ atomic_inc(&counter) ] = tmp;
		}



		
		id = id + 256;
	}







	//barrier();	// Paint to pixel

	__local float texturePart[16 * 16];	// 1024 Byte

	




}


