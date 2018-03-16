struct INPUT_VERTEX
{
    float3 coordinate : POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
    float3 normals : NORMALS;
};

struct OUTPUT_VERTEX
{
    float3 Pos : WORLDPOS;
    float4 Color : COLOR;
};

OUTPUT_VERTEX main(INPUT_VERTEX fromVertexBuffer)
{
    OUTPUT_VERTEX output;

    output.Pos = fromVertexBuffer.color;
    output.Color = fromVertexBuffer.color;

	return output;
}