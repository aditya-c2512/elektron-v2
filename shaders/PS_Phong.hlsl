cbuffer LightBuffer
{
	float3 lightPos;
};

static const float3 ambient = { 0.15f,0.15f,0.15f };
static const float3 diffuse_color = { 0.604f,0.0f,1.0f };
static const float intensity_diffuse = 1.0f;
static const float att_const = 1.0f;
static const float att_lin = 1.0f;
static const float att_quad = 1.0f;

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_TARGET
{
	const float3 vl = lightPos - worldPos;
	const float dist_vl = length(vl);
	const float3 dir_vl = vl / dist_vl;

	const float att = att_const + att_lin * dist_vl + att_quad * (dist_vl * dist_vl);

    const float3 diffuse = diffuse_color * intensity_diffuse * att * max(0.0f, dot(dir_vl, n));
	
    return float4(saturate(diffuse + ambient), 1.0f);

}