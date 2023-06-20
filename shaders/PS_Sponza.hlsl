Texture2D albedoMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D metallicMap : register(t2);

sampler splr;

float4 main(float3 cameraPos : Position, float3 normal : Normal, float2 texCoords : TextureCoord, float3x3 tbn : TBN_Normal, float3 lightDir : LightDir) : SV_TARGET
{
    float3 n = normalMap.Sample(splr, texCoords).rgb;
    n = normalize(2.0f * n - 1.0f);
    n = mul(n, tbn);
    
    float NoL = max(dot(n, normalize(lightDir)), 0.0f);
    float3 c_diff = NoL * albedoMap.Sample(splr, texCoords).rgb;
    float3 c_spec = float3(0, 0, 0);
    float3 final = c_diff + c_spec;
    final = pow(final / (final + 1.0f), 1.0f / 2.2f);
	return float4(c_diff, 1.0f);
}