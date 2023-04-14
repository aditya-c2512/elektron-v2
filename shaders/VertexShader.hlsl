struct VS_OUT
{
	float3 color : COLOR;
	float4 pos : SV_POSITION;
	
};

cbuffer ConstantBuffer
{
	row_major matrix transform;
};

VS_OUT main( float2 pos : POSITION, float3 color : COLOR )
{
	VS_OUT vsOut;
	vsOut.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
	vsOut.color = color;

	return vsOut;
}