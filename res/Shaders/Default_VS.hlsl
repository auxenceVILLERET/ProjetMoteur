cbuffer ObjectCB : register(b0)
{
    float4x4 gWorldViewProj;
};

struct VertexIn
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float3 worldPos : TEXCOORD0;
    float3 worldNrm : TEXCOORD1;
    float2 uv : TEXCOORD2;
};

VertexOut VSMain(VertexIn v)
{
    VertexOut o;
    o.pos = mul(float4(v.pos, 1.0f), gWorldViewProj);

    // Pour l’instant : on passe position objet + normal objet
    o.worldPos = v.pos;
    o.worldNrm = v.normal;
    o.uv = v.uv;
    return o;
}