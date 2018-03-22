Texture2D heightMap;

SamplerState samplerState;

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
    float2 texCoord : TEXCOORD;
};

output main( INPUT_VERTEX input )
{
    output Output;
    Output.output = (input.coordinate);

    float4 colors = heightMap.SampleLevel(samplerState, input.texCoord, 0);

    Output.output.y = colors.x * 80.0f;

    Output.texCoord = input.texCoord;

    return Output;

}