cbuffer MVPBuffer
{
    matrix model;
    matrix view;
    matrix proj;
};

struct VSOut
{
    float3 cameraPos : Position;
    float3 n : Normal;
    float2 texCoords : TextureCoord;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float3 n : Normal, float2 texCoords : TextureCoord)
{
    VSOut vs_out;
    vs_out.cameraPos = (float3) mul(float4(pos, 1.0f), mul(model, view));
    vs_out.n = mul(n, (float3x3) mul(model, view));
    vs_out.texCoords = texCoords;
    vs_out.pos = mul(float4(pos, 1.0f), mul(mul(model, view), proj));
    return vs_out;
}