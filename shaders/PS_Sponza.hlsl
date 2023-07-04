cbuffer LightBuffer
{
    float3 lightPos;
    float3 ambient_color;
    float3 diffuse_color;
    float intensity_diffuse;
};

Texture2D albedoMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D metallicMap : register(t2);
Texture2D shadowMap : register(t3);

sampler splr;
sampler shadowSplr;

static const float att_const = 1.0f;
static const float att_lin = 0.0014;
static const float att_quad = 0.0007f;
static const float PI = 3.14159f;
static float3 f;

float ggx_G(float a, float3 n, float3 l, float3 v)
{
    float k = pow(a + 1, 2) / 8.0f;
    float Gl = 1.0f / (dot(n, l) * (1 - k) + k);
    float Gv = 1.0f / (dot(n, v) * (1 - k) + k);
    return Gl * Gv;
}
float3 ggx_F(float3 f0, float voh)
{
    float3 f1 = (f0 + (1.0f - f0) * pow(2.0f, (-5.55473 * voh - 6.98316) * voh));
    return f1;
}
float ggx_D(float a, float noh)
{
    float d = a / (noh * noh * (a * a - 1.0f) + 1.0f);
    return d * d / PI;
}
float3 ggx_specular(float3 n, float3 l, float3 v, float2 texCoords)
{
    const float4 metalRoughness = metallicMap.Sample(splr, texCoords);
    const float roughness = metalRoughness.g;
    const float metallic = metalRoughness.b;
    
    float3 h = normalize((v + l) / 2.0f);
    
    float3 f0 = float3(0.04f, 0.04f, 0.04f);
    f0 = lerp(f0, albedoMap.Sample(splr, texCoords).rgb, metallic);
    f = ggx_F(f0, max(dot(v, h), 0.0f));
    
    float3 specular = ggx_D(roughness * roughness, dot(n, h)) * f * ggx_G(roughness * roughness, n, l, v) / (4 * dot(n, l) * dot(n, v) + 0.00001f);

    return specular;
}

float4 main(float3 cameraPos : Position, float4 shadowCamPos : ShadowViewPosition, float2 texCoords : TextureCoord, float3x3 tbn : TBN_Normal) : SV_TARGET
{
    float3 c_diff;
    float3 c_spec;
    
    shadowCamPos.xyz = shadowCamPos.xyz / shadowCamPos.w;
    
    float3 n = normalMap.Sample(splr, texCoords).rgb;
    n = normalize(2.0f * n - 1.0f);
    n = mul(n, tbn);
    
    float3 l = lightPos - cameraPos;
    float lightDist = length(l);
    float3 lightDir = normalize(l);
    float NoL = max(dot(n, lightDir), 0.0f);
    float3 att = 1.0f / (lightDist * lightDist);
    
    if (shadowMap.Sample(shadowSplr, shadowCamPos.xy).r > shadowCamPos.z)
    {
        const float metallic = metallicMap.Sample(splr, texCoords).b;
        float kD = (1.0f - f) * (1 - metallic);
        c_diff = kD / PI * intensity_diffuse * albedoMap.Sample(splr, texCoords).rgb;
        c_spec = ggx_specular(n, lightDir, normalize(-cameraPos), texCoords);
    }
    else
    {
        c_diff = c_spec = float3(0.0f, 0.0f, 0.0f);
    }
    float3 final = saturate(NoL * att * (c_diff + c_spec));
    //final = pow(final / (final + 1.0f), 1.0f / 2.2f);
    return float4(final, 1.0f);
}