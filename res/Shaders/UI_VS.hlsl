cbuffer UICB : register(b0)
{
    float4 gTint; // rgba, optionnel (1,1,1,1)
};

Texture2D gTex0 : register(t0);

SamplerState gSamp0 : register(s0);

struct VertexIn
{
    float2 pos : POSITION; // NDC [-1..1]
    float2 uv : TEXCOORD0;
    uint col : COLOR0; // R8G8B8A8_UNORM packé (optionnel)
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 col : COLOR0;
};

float4 UnpackRGBA8(uint c)
{
    float4 o;
    o.x = (c & 255) / 255.0;
    o.y = ((c >> 8) & 255) / 255.0;
    o.z = ((c >> 16) & 255) / 255.0;
    o.w = ((c >> 24) & 255) / 255.0;
    return o;
}

VertexOut VSMain(VertexIn v)
{
    VertexOut o;
    o.pos = float4(v.pos, 0.0, 1.0);
    o.uv = v.uv;
    o.col = UnpackRGBA8(v.col);
    return o;
}