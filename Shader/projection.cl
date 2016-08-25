// Raytracing part:

// deformt perspective projection 
// line through flat screen -> angle difference on edge is bigger then center

inline float4 projection_perspective_deform(const int2 coord, const int2 size)
{ 
	const float2 coordf = (convert_float2(coord) - (convert_float2(size) / 2.0f)) / convert_float2(size);

	const float4 rayDir = (float4)(coordf.x * tan(M_PI / 4.0), coordf.y * (1.0 / 1.0) * tan(M_PI / 4.0), -1.0, 0.0);

	return rayDir;
}


// perspective projection 
// line through round screen -> angle difference is the same everywhere
inline float4 projection_perspective(const int2 coord, const int2 size)
{
	const float2 coordf = (convert_float2(coord) - (convert_float2(size) / 2.0f)) / convert_float2(size);

	const float angleX = M_PI;
	const float angleY = angleX * (size.y / size.x);

	// tan(alpha) * a = g
	const float4 rayDir = (float4)(	1.0f * tan(coordf.x * angleX),
									1.0f * tan(coordf.y * angleY),
									-1.0, 0.0);

	return rayDir;
}










