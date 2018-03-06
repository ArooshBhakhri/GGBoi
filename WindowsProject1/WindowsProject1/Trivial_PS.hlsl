Texture2D baseTexture;

SamplerState envFilter;

struct INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 Texture : TEXCOORD0;
};

float4 main( INPUT gg ) : SV_TARGET
{
    
    return baseTexture.Sample(envFilter, gg.Texture);
	//return gg.Color;
}