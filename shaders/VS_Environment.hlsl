cbuffer ModelBuffer
{
    matrix model;
    matrix modelView;
    matrix modelViewProj;
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
    vs_out.cameraPos = (float3) mul(float4(pos, 1.0f), modelView);
    vs_out.n = mul(n, (float3x3) modelView);
    vs_out.texCoords = texCoords;
    vs_out.pos = mul(float4(pos, 1.0f), modelViewProj);
    return vs_out;
}