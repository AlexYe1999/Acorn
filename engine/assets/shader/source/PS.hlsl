#include"vertex.hlsli"

float4 main(VertexOut vIn) : SV_Target {
	return vIn.Color;
}