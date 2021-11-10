#include"CubeMap.hlsli"

PSIn main(VSIn vIn) {
	PSIn vOut;

	float4 PosW = mul(float4(vIn.PosL, 1.0f), World);
    vOut.PosW = PosW.xyz;
	vOut.PosH = mul(PosW, ViewProj);
    vOut.NormalW = normalize(mul(vIn.NormalL, (float3x3)WorldIT));
	return vOut;
}