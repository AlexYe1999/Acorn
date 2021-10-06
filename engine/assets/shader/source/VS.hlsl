#include"vertex.hlsli"

VertexOut main(VertexIn vIn) {
	VertexOut vOut;
	float4 PosW = mul(float4(vIn.PosL, 1.0f), World);

	vOut.PosH = mul(PosW, ViewProj);
	vOut.Color = vIn.Color;
	return vOut;
}