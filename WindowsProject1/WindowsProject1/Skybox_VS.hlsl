cbuffer cbPerObject
{
    float4x4 WVP;
    float3 time;
    float3 lightDir;
    float4 lightColor;
};

struct outPut
{
    float4 pos : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

struct INPUT_VERTEX
{
    float3 coordinate : POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
    float3 normals : NORMALS;
};

outPut main( INPUT_VERTEX input )
{

    outPut outdata = (outPut) 0;

    outdata.pos = mul(float4(input.coordinate, 1.0f), WVP).xyzw;

    outdata.texCoord = input.coordinate;

	return outdata;
}