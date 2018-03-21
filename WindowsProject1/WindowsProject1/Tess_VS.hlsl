struct INPUT_VERTEX
{
    float3 coordinate : POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
    float3 normals : NORMALS;
};

struct output
{
    float3 output : WORLDPOS;
};

output main( INPUT_VERTEX input )
{
    output Output;
    Output.output = (input.coordinate);

    return Output;

}