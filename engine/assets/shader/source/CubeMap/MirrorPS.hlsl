#include"CubeMap.hlsli"

float4 main(PSIn pIn) : SV_Target {

	float3 toObj = EyePosW - pIn.PosW;
	float3 r = reflect(-normalize(toObj), pIn.NormalW);
	float4 color = DiffuseMap.Sample(samAnisotricWrap, r);
	
	return color * (DiffuseAlbedo + float4(SchlickFresnel(FresnelR0, pIn.NormalW, r), 1.0f));
}