
float Average(float4 arr)
{
	float sum = arr.x + arr.y + arr.z + arr.w;
	return sum * 0.25;
}
float Average(float3 arr)
{
	float sum = arr.x + arr.y + arr.z;
	return sum * 0.33333333333333331;
}
float Average(float2 arr)
{
	float sum = arr.x + arr.y;
	return sum * 0.5;
}