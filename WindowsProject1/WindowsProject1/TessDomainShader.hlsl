struct DS_OUTPUT
{
	float4 vPosition  : SV_POSITION;
	// TODO: change/add other stuff
    float4 Color : COLOR;
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

[domain("tri")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT Output;
    
    Output.vPosition = (patch[0].vPosition * domain.x + patch[1].vPosition * domain.y + patch[2].vPosition * domain.z, 1);

    Output.vPosition = mul(Output.vPosition, patch[0].WVP);

    Output.Color = float4(patch[0].Color * domain.x + patch[1].Color * domain.y + patch[2].Color * domain.z);

	return Output;
}
