struct PointLight
{
    float3 lightPos;
    float3 ambient_color;
    float3 diffuse_color;
    float intensity_diffuse;
};

cbuffer LightBuffer : register(b0)
{
    PointLight pointLights[4];
};

static const float att_const = 1.0f;
static const float att_lin = 0.0014;
static const float att_quad = 0.00007f;
static float3 f;

Texture2D albedoMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D metalRoughnessMap : register(t2);
Texture2D emissiveMap : register(t3);
Texture2D aoMap : register(t4);
TextureCube skyCube : register(t5);
sampler splr;

static const float PI = 3.14159f;

float ggx_G(float a, float3 n, float3 l, float3 v)
{
    float k = pow(a + 1, 2) / 8.0f;
    float Gl = 1.0f / (dot(n, l) * (1 - k) + k);
    float Gv = 1.0f / (dot(n, v) * (1 - k) + k);
    return Gl * Gv;
}
float3 ggx_F(float3 f0, float voh)
{
    float3 f = (f0 + (1.0f - f0) * pow(2.0f, (-5.55473 * voh - 6.98316) * voh));
    return f;
}
float ggx_D(float a, float noh)
{
    float d = a / (noh * noh * (a * a - 1.0f) + 1.0f);
    return d * d / PI;
}
float3 ggx_specular(float3 n, float3 l, float3 v, float2 texCoords)
{
    const float4 metalRoughness = metalRoughnessMap.Sample(splr, texCoords);
    const float roughness = metalRoughness.g;
    const float metallic = metalRoughness.b;
    
    float3 h = normalize((v + l) / 2);
    
    float3 f0 = float3(0.09f, 0.09f, 0.09f);
    f0 = lerp(f0, albedoMap.Sample(splr, texCoords).rgb, metallic);
    f = ggx_F(f0, max(dot(v, h), 0.0f));
    
    float3 specular = max(dot(l, n), 0.0f) * ggx_D(roughness * roughness, dot(n, h)) * f * ggx_G(roughness * roughness, n, l, v) / (4 * dot(n, l) * dot(n, v) + 0.0001f);

    return specular;
}

float4 main(float3 cameraPos : Position, float3 dn : Normal, float2 texCoords : TextureCoord, float3x3 tbn : TBN_Normal) : SV_TARGET
{
    float3 final = float3(0, 0, 0);
    float3 n = normalMap.Sample(splr, texCoords).rgb;
    n = normalize(2.0f * n - 1.0f);
    n = mul(n, tbn);
    
    const float metallic = metalRoughnessMap.Sample(splr, texCoords).b;
    
    for (int i = 0; i < 4; i++)
    {
        const float3 vl = pointLights[i].lightPos - cameraPos;
        const float dist_vl = length(vl);
        const float3 dir_vl = vl / dist_vl;

        const float att = 1.0f / (att_const + att_quad * dist_vl * dist_vl);
    
        const float3 specular = att * ggx_specular(n, dir_vl, -cameraPos, texCoords);
    
        float3 kD = (1.0f - f) * (1 - metallic);
    
        final += (kD/PI * albedoMap.Sample(splr, texCoords).rgb + specular) * att * pointLights[i].diffuse_color * pointLights[i].intensity_diffuse * max(dot(dir_vl,n),0);
    }
    const float3 r = reflect(cameraPos, n);
    const float3 skyReflection = skyCube.Sample(splr, r).rgb;
    
    final += emissiveMap.Sample(splr, texCoords).rgb + skyReflection * metallic / 10;
    final = final / (final + float3(1.0f, 1.0f, 1.0f));
    final = pow(final, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
    
    return float4(final, 1.0f);
}