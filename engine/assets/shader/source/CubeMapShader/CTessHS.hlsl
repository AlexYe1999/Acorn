#include"CTess.hlsli"

PatchTess ConstantHS(
    InputPatch<HSIn, 4> patch,
    uint patchID : SV_PrimitiveID
){
    
	float3 centerL = 0.25f * (patch[0].PosL
        + patch[1].PosL + patch[2].PosL + patch[3].PosL);
	float3 centerW = mul(float4(centerL, 1.0f), World).xyz;
    
	float d = distance(centerW, EyePosW);
	float d0 = 20;
	float d1 = 2000;
	float tess = 64.0f * saturate((d1-d) / (d1-d0));
    
    PatchTess pt;
	pt.EdgesTess[0] = tess;
	pt.EdgesTess[1] = tess;
	pt.EdgesTess[2] = tess;
	pt.EdgesTess[3] = tess;

	pt.InsideTess[0] = tess;
	pt.InsideTess[1] = tess;
    return pt;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
DSIn main(
    InputPatch<HSIn, 4> p, 
    uint i : SV_OutputControlPointID,
    uint patch : SV_PrimitiveID
){
    return p[i];
}

