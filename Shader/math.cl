


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
