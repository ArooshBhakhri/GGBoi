Texture2D skyboxTexture;

SamplerState skyboxSRV;

struct inPut
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 Texture : TEXCOORD0;
    float3 Normals : NORMALS;
    float3 time : TIMER;
    float3 lightDir : LIGHTDIR;
    float4 lightColor : LIGHTCOLOR;
};

float4 main(inPut inData) : SV_TARGET
{
    return skyboxTexture.Sample(skyboxSRV, inData.Texture);
}