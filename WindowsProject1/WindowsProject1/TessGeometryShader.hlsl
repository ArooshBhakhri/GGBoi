struct DS_OUTPUT
{
	float4 vPosition  : SV_POSITION;
	float4 Color : COLOR;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
    float4 color : COLOR;
};

[maxvertexcount(3)]
void main(
	triangle DS_OUTPUT input[3], 
	inout TriangleStream< GSOutput > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.pos = input[i].vPosition;
        element.color = input[i].Color;
		output.Append(element);
	}
}