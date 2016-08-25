


const float vec_vec_distance(float4 A, float4 p1, float4 B, float4 p2)
{	
	float4 diff = B - A;	

	float4 n = cross(p1, p2);
	float4 n2 = cross(p2, n);

	const float x1 = dot(diff, n2) / dot(p1, n2);
	if (x1 < 1.0f) { return FLT_MAX; }

	
	const float4 n1 = cross(p1, n);
	const float x2 = clamp(dot(-diff, n1) / dot(p2, n1), 0.0f, 1.0f);
	
	if(x2 == 0.0f || x2 == 1.0f)
	{
	return fast_length(mad(x2, p2, B) - mad(x1, p1, A));	// mad(a,b,c) = a*b+c in fast  || faster -> FLT_MAX;// 
	}	
	else
	{
		return  fast_length( dot(n / fast_length(n), diff) ) ;
	}		
	

	return FLT_MAX;
}


const float originvec_vec_distance(float4 p1, float4 B, float4 p2)
{
	float4 n = cross(p1, p2);
	float4 n2 = cross(p2, n);

	const float x1 = dot(B, n2) / dot(p1, n2);
	if (x1 < 1.0f) { return FLT_MAX; }


	const float4 n1 = cross(p1, n);
	const float x2 = clamp(dot(-B, n1) / dot(p2, n1), 0.0f, 1.0f);

	if (x2 == 0.0f || x2 == 1.0f)
	{
		return fast_length(mad(x2, p2, B) - (x1* p1) );	// mad(a,b,c) = a*b+c in fast  || faster -> FLT_MAX;// 
	}
	else
	{
		return  fast_length(dot(n / fast_length(n), B));
	}


	return FLT_MAX;
}


const float 2d_vec_point_distance(float2 A, float2 p1, float2 B)
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

bool 2d_line_intersect(float4 line1, float4 line2, float2 interPoint)
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

