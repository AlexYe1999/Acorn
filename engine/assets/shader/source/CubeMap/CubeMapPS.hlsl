#include"CubeMap.hlsli"

float4 main(VSOut PSIn) : SV_Target {
	return DiffuseMap.Sample(samAnisotricWrap, PSIn.PosL);
}