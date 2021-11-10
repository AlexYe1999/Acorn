#include"CubeMap.hlsli"

VSOut main(VSIn vIn) {
	VSOut vOut;

    vOut.PosL = vIn.PosL;
	float4 PosW = mul(float4(vIn.PosL, 1.0f), World);
    PosW.xyz += EyePosW;
	vOut.PosH = mul(PosW, ViewProj).xyww;

	return vOut;
}