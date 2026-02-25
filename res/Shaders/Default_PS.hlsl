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

float4 PSMain(VertexOut i) : SV_TARGET
{
    float4 tex = gTex0.Sample(gSamp0, i.uv);
    return tex;
}
