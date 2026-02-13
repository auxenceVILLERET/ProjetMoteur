cbuffer ObjectCB : register(b0)
{
    float4x4 gWorldViewProj;
};

Texture2D gTex0 : register(t0);

SamplerState gSamp0 : register(s0);

struct VertexIn
{
    float3 pos : POSITION;
    float2 uv  : TEXCOORD0;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VertexOut VSMain(VertexIn v)
{
    VertexOut o;
    o.pos = mul(gWorldViewProj, float4(v.pos, 1.0));
    o.uv = v.uv;
    return o;
}

float4 PSMain(VertexOut i) : SV_TARGET
{
    return gTex0.Sample(gSamp0, i.uv);
}
