#include"Multiins.hlsli"

VSOut main(VSIn VSin, uint instanceID : SV_InstanceID) {

	InstanceData instanceData = gInstanceData[instanceID];
	
	float4x4 matTramform = gMaterialData[instanceData.MaterialIndex].MatTransform;
	float4 PosW = mul(float4(VSin.PosL, 1.0f), instanceData.World);
	float4 TexC = mul(float4(VSin.TexC, 0.0f, 1.0f), instanceData.TexTransform);
	
	VSOut outputData;
	outputData.MatIndex = instanceData.MaterialIndex;
    outputData.PosW = PosW.xyz;
	outputData.NormalW = mul(VSin.NormalL, (float3x3)instanceData.WorldIT);
    outputData.PosH = mul(PosW, ViewProj);
	outputData.TexC = mul(TexC, matTramform).xy;

    return outputData;
}