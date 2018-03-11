Texture2D skyboxTexture;

SamplerState skyboxSRV;

struct inPut
{
    float4 pos : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

float4 main(inPut inData) : SV_TARGET
{
    return skyboxTexture.Sample(skyboxSRV, inData.texCoord.xy);
}