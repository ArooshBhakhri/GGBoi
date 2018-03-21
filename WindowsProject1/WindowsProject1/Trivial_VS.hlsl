cbuffer cbPerObject
{
    float4x4 WVP;
    float4x4 World;
    float3 time;
};

struct OUTPUT_VERTEX
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 Texture : TEXCOORD;
    float3 Normals : NORMALS;
    float3 time : TIMER;
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

    sendToRasterizer.Pos = mul(WVP, sendToRasterizer.Pos);
    
    sendToRasterizer.Color = fromVertexBuffer.color;

    sendToRasterizer.Texture = fromVertexBuffer.texCoord;

    sendToRasterizer.time = time;

    sendToRasterizer.Normals = fromVertexBuffer.normals;

    return sendToRasterizer;
}