#include"CCubeMap.hlsli"

float4 main(PSIn PSIn) : SV_Target {
	return DiffuseMap.Sample(samAnisotricWrap, PSIn.PosL);
}