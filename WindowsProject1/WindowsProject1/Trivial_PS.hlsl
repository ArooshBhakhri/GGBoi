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

float4 main( INPUT gg ) : SV_TARGET
{
    float lightRatio = clamp(dot(-1 * gg.lightDir, gg.Normals), 0, 1);

    //return /*lightRatio * gg.lightColor * */baseTexture.Sample(envFilter, gg.Texture) * map(cos(gg.time.b), -1.0f, 1.0f, 0.1f, 1.0f);
    return gg.Color;
}