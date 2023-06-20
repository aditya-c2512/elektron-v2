Texture2D skyMap : register(t0);

SamplerState splr;

float4 main(float3 cameraPos : Position, float3 n : Normal, float2 texCoords : TextureCoord) : SV_TARGET
{
	float3 final = skyMap.Sample(splr, texCoords).rgb;
    final = final / (final + float3(1.0f, 1.0f, 1.0f));
    final = pow(final, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
    return float4(final, 1.0f);
}