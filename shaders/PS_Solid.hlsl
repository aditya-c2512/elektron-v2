cbuffer CBuf
{
    float3 color;
};

float4 main() : SV_Target
{
    return float4(color, 1.0f);
}