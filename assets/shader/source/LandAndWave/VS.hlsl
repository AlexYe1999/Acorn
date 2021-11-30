#include"LandAndWave.hlsli"

VSOut main(VSIn VSin){
    VSOut outputData;

    float4 PosW = mul(float4(VSin.PosL, 1.0f), World);
    outputData.PosW = PosW.xyz;
    outputData.NormalW = mul(VSin.NormalL, (float3x3)WorldIT);
    outputData.PosH = mul(PosW, ViewProj);
    
	float4 TexC = mul(float4(VSin.TexC, 0.0f, 1.0f), TexTransform);
	outputData.TexC = mul(TexC, MatTransform).xy;

    return outputData;
}