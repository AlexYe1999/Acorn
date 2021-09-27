#include"vertex.hlsli"

VertexOut main(VertexIn vIn) {
	VertexOut vOut;
	vOut.Color = vIn.Color;
	vOut.PosH = mul(float4(vIn.PosL, 1.0f), gWorldViewProj);
	return vOut;
}