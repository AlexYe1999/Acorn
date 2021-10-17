#include"LandAndWave.hlsli"

VSOut main(VSIn VSin){
    VSOut outputData;

    float4 PosW = mul(float4(VSin.PosL, 1.0f), World);
    outputData.PosW = PosW.xyz;
    outputData.NormalW = mul(VSin.NormalL, (float3x3)WorldIT);
    outputData.PosH = mul(PosW, ViewProj);
    outputData.Color = VSin.Color;

    return outputData;
}