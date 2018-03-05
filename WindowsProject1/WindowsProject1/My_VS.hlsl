cbuffer cbPerObject
{
    float4x4 WVP;
};

cbuffer THIS_IS_VRAM : register(b0)
{
    float4 constantColor;
    float3 constantOffset;
    float2 padding;
};

struct OUTPUT_VERTEX
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

struct INPUT_VERTEX
{
    float3 coordinate : POSITION;
};

OUTPUT_VERTEX main(INPUT_VERTEX fromVertexBuffer)
{
    OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX) 0;
    sendToRasterizer.Pos.w = 1;

    sendToRasterizer.Pos.xyz = fromVertexBuffer.coordinate.xyz;

	// TODO : PART 4 STEP 4
    sendToRasterizer.Pos.xyz += constantOffset;

	// TODO : PART 3 STEP 7
    sendToRasterizer.Color = constantColor;
	// END PART 3

    return sendToRasterizer;
}