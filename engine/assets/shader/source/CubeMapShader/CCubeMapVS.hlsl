#include"CCubeMap.hlsli"

GSIn main(VSIn vIn) {
	GSIn vOut;

    vOut.PosL = vIn.PosL;
	vOut.PosW = mul(float4(vIn.PosL, 1.0f), World);
	vOut.PosW.xyz += float3(0.0f, 50.0f, 0.0f);

	return vOut;
}