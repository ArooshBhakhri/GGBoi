Texture2D baseTexture;

SamplerState samplerState;

struct input
{
    float4 vPosition : SV_POSITION;
	// TODO: change/add other stuff
    float2 texCoord : TEXCOORD;
};

float4 main(input inData) : SV_TARGET
{
    return baseTexture.Sample(samplerState, inData.texCoord);
}