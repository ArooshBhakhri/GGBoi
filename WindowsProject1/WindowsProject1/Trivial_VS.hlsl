cbuffer cbPerObject
{
    float4x4 WVP;
    float3 time;
    float3 lightDir;
    float4 lightColor;
};

struct OUTPUT_VERTEX
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 Texture : TEXCOORD;
    float3 Normals : NORMALS;
    float3 time : TIMER;
    float3 lightDir : LIGHTDIR;
    float4 lightColor : LIGHTCOLOR;
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

	// TODO : PART 3 STEP 7
    sendToRasterizer.Color = fromVertexBuffer.color;
	// END PART 3

    sendToRasterizer.Texture = fromVertexBuffer.texCoord;

    sendToRasterizer.time = time;

    sendToRasterizer.Normals = fromVertexBuffer.normals;

    sendToRasterizer.lightColor = lightColor;

    sendToRasterizer.lightDir = lightDir;

    return sendToRasterizer;
}