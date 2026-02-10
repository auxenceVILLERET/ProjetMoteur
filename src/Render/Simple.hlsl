cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
};

struct VertexIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VertexOut VSMain(VertexIn v)
{
    VertexOut o;
    o.pos = mul(float4(v.pos, 1.0), gWorldViewProj);
    o.color = v.color;
    return o;
}

float4 PSMain(VertexOut i) : SV_Target
{
    return i.color;
}
