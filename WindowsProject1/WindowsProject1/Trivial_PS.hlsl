struct INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

float4 main( INPUT gg ) : SV_TARGET
{
	return gg.Color;
}