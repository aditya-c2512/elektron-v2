Texture2D albedo : register(t0);

SamplerState splr;

float4 main(uint trisID : SV_PRIMITIVEID) : SV_TARGET
{
	float2 col;
	col.x = 0;
	col.y = 0;
	return albedo.Sample(splr, col);

}