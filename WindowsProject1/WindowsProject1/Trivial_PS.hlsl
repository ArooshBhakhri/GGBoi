Texture2D baseTexture;

SamplerState envFilter;

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

float map(float value, float inMin, float inMax, float outMin, float outMax)
{
    float per = (value - inMin) / (inMax - inMin);

    return per * (outMax - outMin) + outMin;
}

float4 main( INPUT inData ) : SV_TARGET
{

    float lightRatio = saturate(dot(-dir, inData.Normals));

    lightRatio = saturate(lightRatio + 0.10f);

    float4 final = baseTexture.Sample(envFilter, inData.Texture).rgba;

    clip(final.a - 0.25f);

    return lightRatio * color.rgba * final;
    
}