cbuffer LightBuffer
{
    float3 lightPos;
    float3 ambient_color;
    float3 diffuse_color;
    float intensity_diffuse;
    float att_const;
    float att_lin;
    float att_quad;
};

Texture2D albedoMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D metalRoughnessMap : register(t2);
Texture2D emissiveMap : register(t3);
Texture2D aoMap : register(t4);
TextureCube skyCube : register(t5);
sampler splr;

static const float PI = 3.14159f;

float3 burley_diffuse()
{
    return diffuse_color * intensity_diffuse/PI;
}
float ggx_G(float a, float3 n, float3 l, float3 v)
{
    float k = pow(a + 1, 2) / 8.0f;
    float Gl = 1.0f / (dot(n, l) * (1 - k) + k);
    float Gv = 1.0f / (dot(n, v) * (1 - k) + k);
    return Gl * Gv;
}
float ggx_F(float f0, float voh)
{
    float f = (f0 + (1.0f - f0) * pow(2.0f, (-5.55473 * voh - 6.98316) * voh));
    return f;
}
float ggx_D(float a, float noh)
{
    float d = a / (noh * noh * (a * a - 1.0f) + 1.0f);
    return d * d/PI;
}
float3 ggx_specular(float3 n, float3 l, float3 v, float2 texCoords)
{
    const float4 metalRoughness = metalRoughnessMap.Sample(splr, texCoords);
    const float roughness = metalRoughness.g;
    const float metallic = metalRoughness.b;
    
    float3 h = normalize(v + l);
    
    float3 specular = max(dot(l, n), 0) * ggx_D(roughness * roughness, dot(n, h)) * ggx_F(0.9f, dot(v, h)) * ggx_G(roughness * roughness, n, l, v) / (4 * dot(n, l) * dot(n, v));

    return specular;
}

float4 main(float3 cameraPos : Position, float3 dn : Normal, float2 texCoords : TextureCoord, float3x3 tbn : TBN_Normal) : SV_TARGET
{
    float3 n = normalMap.Sample(splr, texCoords).rgb;
    n = normalize(2.0f * n - 1.0f);
    n = mul(n, tbn);
    
    const float3 vl = lightPos - cameraPos;
    const float dist_vl = length(vl);
    const float3 dir_vl = vl / dist_vl;

    const float att = 1.0f / (att_const + att_lin * dist_vl + att_quad * (dist_vl * dist_vl));

    const float3 diffuse = burley_diffuse() * att * max(0.0f, dot(dir_vl, n));
    
    const float3 specular = att * ggx_specular(n, dir_vl, -cameraPos, texCoords);
    
    const float metallic = metalRoughnessMap.Sample(splr, texCoords).b;
    
    const float3 r = reflect(cameraPos, n);
    const float3 skyReflection = skyCube.Sample(splr, r).rgb;
    
    const float3 final = saturate(diffuse * albedoMap.Sample(splr, texCoords).rgb * (1.0f - metallic) + emissiveMap.Sample(splr, texCoords).rgb + specular + skyReflection*metallic/10.0f);
    
    return float4(final, 1.0f);
}