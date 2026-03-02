cbuffer FrameCB : register(b1)
{
    float3 gCameraPos;
    uint gLightCount;
};

Texture2D gTex0 : register(t0);
SamplerState gSamp0 : register(s0);

struct Light
{
    float3 position;
    float range;
    float3 color;
    float intensity;
    float3 direction;
    float spotCosAngle; // non utilisé ici (spot plus tard)
    uint type;
    uint3 pad;
};

// t1 = buffer structuré de lights
StructuredBuffer<Light> gLights : register(t1);

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

    // Un petit ambient pour voir quelque chose même hors lumière
    float3 ambient = 0.05f * albedo;

    float3 lighting = 0.0f;

    [loop]
    for (uint li = 0; li < gLightCount; ++li)
    {
        Light Lgt = gLights[li];

        // type 0 = point light
        if (Lgt.type == 0)
        {
            float3 toL = Lgt.position - i.worldPos;
            float d = length(toL);

            if (d < Lgt.range)
            {
                float3 L = toL / max(d, 1e-4f);
                float NdotL = saturate(dot(N, L));

                // attenuation simple (smooth-ish)
                float x = saturate(1.0f - (d / Lgt.range));
                float att = x * x;

                float3 radiance = Lgt.color * Lgt.intensity;

                lighting += radiance * (NdotL * att);
            }
        }
    }

    // Emissive (pour l’instant 0 ; tu peux lier une texture emissive t2 plus tard)
    float3 emissive = 0.0f;

    float3 color = ambient + (albedo * lighting) + emissive;

    return float4(color, 1.0f);
}