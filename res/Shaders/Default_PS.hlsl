cbuffer FrameCB : register(b1)
{
    float4x4 gViewProj; // optionnel
    float3 gCameraPos;
    uint pad0;

    float3 gLightPos;
    float gLightRange;
    float3 gLightColor;
    float gLightIntensity;
};

Texture2D gTex0 : register(t0);
SamplerState gSamp0 : register(s0);

struct VertexOut
{
    float4 pos : SV_POSITION;
    float3 worldPos : TEXCOORD0;
    float3 worldNrm : TEXCOORD1;
    float2 uv : TEXCOORD2;
};

float4 PSMain(VertexOut i) : SV_TARGET
{
    float3 albedo = gTex0.Sample(gSamp0, i.uv).rgb;

    float3 N = normalize(i.worldNrm);

    float3 ambient = 0.05f * albedo;

    float3 toL = gLightPos - i.worldPos;
    float d = length(toL);

    float3 lighting = 0.0f;
    if (d < gLightRange)
    {
        float3 L = toL / max(d, 1e-4f);
        float NdotL = saturate(dot(N, L));

        float x = saturate(1.0f - d / gLightRange);
        float att = x * x;

        float3 radiance = gLightColor * gLightIntensity;
        lighting = radiance * (NdotL * att);
    }

    float3 color = ambient + albedo * lighting;
    return float4(color, 1.0f);
}