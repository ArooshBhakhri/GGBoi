// Input control point
struct VS_CONTROL_POINT_OUTPUT
{
	float4 vPosition : SV_POSITION;
	// TODO: change/add other stuff
    float4 Color : COLOR;
	float4x4 WVP: WORLD_VIEW_PROJECTION;
};

// Output control point
struct HS_CONTROL_POINT_OUTPUT
{
    float4 vPosition : SV_POSITION;
    float4 Color : COLOR;
	float4x4 WVP: WORLD_VIEW_PROJECTION;
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
};

#define NUM_CONTROL_POINTS 3

// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	// Insert code to compute Output here
    Output.EdgeTessFactor[0] = 
    Output.EdgeTessFactor[1] = 
    Output.EdgeTessFactor[2] = 
	Output.InsideTessFactor = 15; // e.g. could calculate dynamic tessellation factors instead

	return Output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HS_CONTROL_POINT_OUTPUT Output;

	// Insert code to compute Output here
	Output.vPosition = ip[i].vPosition;
    Output.Color = ip[i].Color;
	Output.WVP = ip[i].WVP;

	return Output;
}
