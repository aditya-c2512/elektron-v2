cbuffer MVPBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix proj;
};

struct VSOut
{
    float3 cameraPos : Position;
    float2 texCoords : TextureCoord;
    float3x3 tbn : TBN_Normal;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float3 n : Normal, float2 texCoords : TextureCoord, float3 tangent : Tangent, float3 bitangent : Bitangent)
{
    VSOut vs_out;
    vs_out.cameraPos = (float3) mul(float4(pos, 1.0f), mul(model, view));
    vs_out.texCoords = texCoords;
    vs_out.pos = mul(float4(pos, 1.0f), mul(mul(model, view), proj));
    
    float3 T = normalize(mul(tangent, (float3x3) mul(model, view)));
    float3 B = normalize(mul(bitangent, (float3x3) mul(model, view)));
    float3 N = normalize(mul(n, (float3x3) mul(model, view)));
    vs_out.tbn = float3x3(T, B, N);
    
	return vs_out;
}