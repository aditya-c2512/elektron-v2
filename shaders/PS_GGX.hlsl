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

Texture2D albedoMap;
Texture2D specularMap;
TextureCube skyCube;
sampler splr;

static const float PI = 3.14159f;

float3 burley_diffuse()
{
    return diffuse_color * intensity_diffuse / PI;
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
    return d * d;
}
float3 ggx_specular(float3 n, float3 l, float3 v, float2 texCoords)
{
    const float4 specular_color = specularMap.Sample(splr, texCoords);
    const float3 specular_intensity = specular_color.rgb;
    const float specular_power = specular_color.a;
    
    float3 h = normalize(v + l);
    float3 specular = specular_intensity * max(dot(l, n), 0) * ggx_D(specular_power * specular_power, dot(n, h)) * ggx_F(0.9f, dot(l, h)) * ggx_G(specular_power * specular_power, n, l, v);

    return specular;
}

float4 main(float3 cameraPos : Position, float3 n : Normal, float2 texCoords : TextureCoord) : SV_TARGET
{
    const float3 vl = lightPos - cameraPos;
    const float dist_vl = length(vl);
    const float3 dir_vl = vl / dist_vl;

    const float att = 1.0f / (att_const + att_lin * dist_vl + att_quad * (dist_vl * dist_vl));

    const float3 diffuse = burley_diffuse() * att * max(0.0f, dot(dir_vl, n));
    
    float3 r = normalize(reflect(cameraPos, n));
    float4 reflection = skyCube.Sample(splr, r);
    const float3 specular = att * ggx_specular(n,dir_vl,-cameraPos,texCoords);
    
    const float3 final = saturate((diffuse + ambient_color) * albedoMap.Sample(splr, texCoords).rgb + reflection.xyz * specular);
    
    return float4(final, 1.0f);
}