cbuffer MVPBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix proj;
};

cbuffer ShadowBuffer : register(b1)
{
    matrix shadowView;
}

struct VSOut
{
    float3 cameraPos : Position;
    float4 shadowCamPos : ShadowViewPosition;
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
    
    const float4 shadowCamera = mul(float4(pos, 1.0f), model);
    const float4 shadowHomo = mul(shadowCamera, shadowView);
    vs_out.shadowCamPos = shadowHomo * float4(0.5f, -0.5f, 1.0f, 1.0f) + (float4(0.5f, 0.5f, 0.0f, 0.0f) * shadowHomo.w);
    
	return vs_out;
}