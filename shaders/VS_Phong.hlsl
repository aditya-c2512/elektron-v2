cbuffer ModelBuffer
{
    matrix modelTransform;
    matrix mvcTransform;
};

struct VSOut
{
    float3 worldPos : Position;
    float3 n : Normal;
    float4 pos : SV_POSITION;
};

VSOut main( float3 pos : POSITION, float3 n : Normal )
{
    VSOut vs_out;
    vs_out.worldPos = (float3) mul(float4(pos, 1.0f), modelTransform);
    vs_out.n = mul(n, (float3x3) modelTransform);
    vs_out.pos = mul(float4(pos, 1.0f), mvcTransform);
	return vs_out;
}