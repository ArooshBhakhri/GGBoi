cbuffer cbGrid
{
    float4x4 WVP;
    float4x4 World;
    float3 time;
};

struct OUTPUT_VERTEX
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

struct INPUT_VERTEX
{
    float3 coordinate : POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
    float3 normals : NORMALS;
};

OUTPUT_VERTEX main(INPUT_VERTEX fromVertexBuffer)
{
    OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX) 0;
    sendToRasterizer.Pos.w = 1;

    sendToRasterizer.Pos.xyz = fromVertexBuffer.coordinate.xyz;

    sendToRasterizer.Pos = mul(World, sendToRasterizer.Pos);

    sendToRasterizer.Color = fromVertexBuffer.color;

    return sendToRasterizer;
}