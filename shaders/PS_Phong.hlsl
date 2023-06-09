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

static const float PI = 3.14159f;

Texture2D albedoMap;
Texture2D specularMap;
Texture2D skyMap : register(t2); // Testing direct equirectangular map sampling for IBL. Switch to TextureCube
sampler splr;

float4 main(float3 cameraPos : Position, float3 n : Normal, float2 texCoords : TextureCoord) : SV_TARGET
{
	const float3 vl = lightPos - cameraPos;
	const float dist_vl = length(vl);
	const float3 dir_vl = vl / dist_vl;

    const float att = 1.0f / (att_const + att_lin * dist_vl + att_quad * (dist_vl * dist_vl));

    const float3 diffuse = diffuse_color * intensity_diffuse * att * max(0.0f, dot(dir_vl, n));
    
    const float3 w = n * dot(vl, n);
    const float3 r = w * 2.0f - vl;
    
    const float4 specularMapSample = specularMap.Sample(splr, texCoords);
    const float3 specularColorIntensity = specularMapSample.rgb;
    const float specularColorPower = pow(2.0f, specularMapSample.a * 13.0f);
    const float3 specular = att * specularColorIntensity * pow(max(0.0f, dot(normalize(-r), normalize(cameraPos))), specularColorPower);
    
    return float4(saturate((diffuse + ambient_color) * albedoMap.Sample(splr, texCoords).rgb + specular * 10.0f), 1.0f);
    
    // Opposite normals sample same texel. Figure out the proper mapping technique OR Switch to Cubemaps
    //float3 r1 = normalize(reflect(-cameraPos, n));
    //float theta = atan2(r1.z, r1.x) / (2 * 3.14159f);
    //float phi = acos(-r1.y) / 3.14159f;
    //float2 uvs = float2(theta + 0.5f, phi);
    //return skyMap.Sample(splr, uvs);

}