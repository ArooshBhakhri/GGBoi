struct GSOutput
{
	float4 pos : SV_POSITION;
};

[maxvertexcount(3)]
void main(
	triangle float4 input[3] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.pos = input[i];
		output.Append(element);
	}

    GSOutput elements[3];

    elements[0].pos = input[0];
    elements[0].pos.x += -0.5f;

    elements[1].pos = input[0];
    elements[1].pos.y += 1.0f;

    elements[2].pos = input[0];
    elements[2].pos.x += 0.5f;

    output.Append(elements[0]);
    output.Append(elements[1]);
    output.Append(elements[2]);

}