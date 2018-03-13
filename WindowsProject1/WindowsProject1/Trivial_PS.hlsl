Texture2D baseTexture;

SamplerState envFilter;

struct INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 Texture : TEXCOORD0;
    float3 Normals : NORMALS;
    float3 time : TIMER;
    float3 lightDir : LIGHTDIR;
    float4 lightColor : LIGHTCOLOR;
};

float map(float value, float inMin, float inMax, float outMin, float outMax)
{
    float per = (value - inMin) / (inMax - inMin);

    return per * (outMax - outMin) + outMin;
}

float4 main( INPUT inData ) : SV_TARGET
{

    float4 lightColor = (0.956f, 0.921f, 0.258f, 1.0f);
    float3 lightDir = (0.0f, -1.0f, 0.0f);

    float lightRatio = saturate(dot(-lightDir, inData.Normals));

    return (lightRatio * lightColor) * baseTexture.Sample(envFilter, inData.Texture) /* * map(cos(inData.time.b), -1.0f, 1.0f, 0.1f, 1.0f)*/;
    
}