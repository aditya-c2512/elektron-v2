Texture2D albedo : register(t0);

SamplerState splr;

float4 main(uint trisID : SV_PRIMITIVEID) : SV_TARGET
{
	float2 col;
	col.x = 0;
	col.y = 0;
	return albedo.Sample(splr, col);
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);

}