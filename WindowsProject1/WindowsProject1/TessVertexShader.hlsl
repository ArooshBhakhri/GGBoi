cbuffer cbPerObject
{
	float4x4 WVP;
	float4x4 World;
	float3 time;
	float3 lightDir;
	float4 lightColor;
};

struct OUTPUT_VERTEX
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
	float4x4 WVP: WORLD_VIEW_PROJECTION;
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
    OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;
    sendToRasterizer.Pos.w = 1;

    sendToRasterizer.Pos.xyz = fromVertexBuffer.coordinate.xyz;
    
    sendToRasterizer.Color = fromVertexBuffer.color;

	sendToRasterizer.WVP = WVP;

    return sendToRasterizer;
}