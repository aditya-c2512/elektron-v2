Texture2D skyMap : register(t0);

SamplerState splr;

float4 main(float3 cameraPos : Position, float3 n : Normal, float2 texCoords : TextureCoord) : SV_TARGET
{
	return skyMap.Sample(splr, texCoords);
}