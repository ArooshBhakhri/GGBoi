Texture2D baseTexture;

Texture2D baseTexture1;

SamplerState samplerState;

cbuffer cBufferPS
{
    float3 dir;
    float4 color;
};

struct INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 Texture : TEXCOORD0;
    float3 Normals : NORMALS;
    float3 time : TIMER;
};

float4 main(INPUT inData) : SV_TARGET
{
    
    float4 tex1 = baseTexture.Sample(samplerState, inData.Texture);

    float4 tex2 = baseTexture1.Sample(samplerState, inData.Texture) * 0.30f;

    float lightRatio = saturate(dot(-dir, inData.Normals));

    lightRatio = saturate(lightRatio + 0.10f);

    return tex1 * tex2 * lightRatio * color;
}