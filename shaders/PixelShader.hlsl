cbuffer ConstantBuffer
{
	float4 face_colors[6];
};

float4 main(uint trisID : SV_PRIMITIVEID) : SV_TARGET
{
	return face_colors[trisID % 6];
}