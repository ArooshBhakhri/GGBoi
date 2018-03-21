cbuffer cbGridGS
{
    float4x4 VP;
};

struct GSInput
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

struct GSOutput
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

[maxvertexcount(3)]
void main(
    triangle GSInput input[3], 
	inout TriangleStream< GSOutput > output
)
{

    GSOutput verts[6];

    verts[0].Pos = input[0].Pos;
    verts[1].Pos = input[1].Pos;
    verts[2].Pos = input[2].Pos;

    verts[3].Pos = input[0].Pos;

    verts[4].Pos = input[1].Pos;
    verts[4].Pos.y += 0.5f;

    verts[5].Pos = input[2].Pos;

    for (uint i = 0; i < 3; ++i)
    {
        verts[i].Pos = mul(VP, verts[i].Pos);
        verts[i].Color = input[i].Color;
        output.Append(verts[i]);
    }

    for (uint j = 3; j < 6; ++j)
    {
        verts[j].Pos = mul(VP, verts[j].Pos);
        verts[j].Color = float4(0.0f, 1.0f, 0.0f, 1.0f);
        output.Append(verts[j]);
    }

    //verts[3].Pos = input[0].Pos;
    //verts[3].Pos.x -= 0.5f;

    //verts[5].Pos = verts[0].Pos;
    //verts[5].Pos.y += 1.0f;

    //verts[4].Pos = input[1].Pos;

    //for (uint i = 0; i < 2; ++i)
    //{
    //    GSOutput element;
        
    //    element.Pos = mul(VP, input[i].Pos);

    //    element.Color = input[i].Color;

    //    output.Append(element);
    //}

}