struct INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

float map(float value, float inMin, float inMax, float outMin, float outMax)
{
    float per = (value - inMin) / (inMax - inMin);

    return per * (outMax - outMin) + outMin;
}

float4 main(INPUT inData) : SV_TARGET
{

    return inData.Color;
}