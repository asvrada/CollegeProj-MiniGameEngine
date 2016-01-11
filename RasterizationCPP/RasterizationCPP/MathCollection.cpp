#include "MathCollection.h"

FLOAT3D FLOAT3D::CrossProduct(FLOAT3D b)
{
	FLOAT3D output;
	output.x = y*b.z - z*b.y;
	output.y = z*b.x - x*b.z;
	output.z = x*b.y - y*b.x;
	return output;
}

float FLOAT3D::DotProduct(FLOAT3D b)
{
	return x*b.x + y*b.y + z*b.z;
}

FLOAT3D FLOAT3D::operator+(const FLOAT3D b)
{
	return FLOAT3D(x + b.x, y + b.y, z + b.z);
}

FLOAT3D FLOAT3D::operator-(const FLOAT3D b)
{
	return FLOAT3D(x - b.x, y - b.y, z - b.z);
}
