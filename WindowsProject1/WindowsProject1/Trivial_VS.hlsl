cbuffer cbPerObject
{
    float4x4 WVP;
};

struct OUTPUT_VERTEX
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 Texture : TEXCOORD;
};

struct INPUT_VERTEX
{
    float3 coordinate : POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
};

OUTPUT_VERTEX main(INPUT_VERTEX fromVertexBuffer)
{
    OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX) 0;
    sendToRasterizer.Pos.w = 1;

    sendToRasterizer.Pos.xyz = fromVertexBuffer.coordinate.xyz;

    sendToRasterizer.Pos = mul(WVP, sendToRasterizer.Pos);

	// TODO : PART 3 STEP 7
    sendToRasterizer.Color = fromVertexBuffer.color;
	// END PART 3

    sendToRasterizer.Texture = fromVertexBuffer.texCoord;

    return sendToRasterizer;
}