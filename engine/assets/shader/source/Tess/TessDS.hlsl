#include"Tess.hlsli"

[domain("quad")]
PS main(
    PatchTess patchTess, 
    float2 uv : SV_DomainLocation,
    const OutputPatch<DS, 4> quad
){
	DS interData;
	float3 pos;
	float3 v1 = lerp(quad[0].PosL, quad[1].PosL, uv.x);
	float3 v2 = lerp(quad[2].PosL, quad[3].PosL, uv.x);
	pos = lerp(v1, v2, uv.y);
	pos.y = 0.3f * (pos.z * sin(0.1f * pos.x) + pos.x * cos(0.1f * pos.z));

	interData.PosL = pos;

	interData.NormalL = float3(
        -0.03f * pos.z * cos(0.1f * pos.x) - 0.3f * cos(0.1f * pos.z),
        1.0f,
        -0.3f * sin(0.1f * pos.x) + 0.03f * pos.x * sin(0.1f * pos.z)
    );
	interData.NormalL = normalize(interData.NormalL);
    
	float2 t1 = lerp(quad[0].TexC, quad[1].TexC, uv.x);
	float2 t2 = lerp(quad[2].TexC, quad[3].TexC, uv.x);
	interData.TexC = lerp(t1, t2, uv.y);
    
    PS outputData;

    float4 PosW = mul(float4(interData.PosL, 1.0f), World);
    outputData.PosW = PosW.xyz;
    outputData.NormalW = mul(interData.NormalL, (float3x3)WorldIT);
    outputData.PosH = mul(PosW, ViewProj);
    
	float4 TexC = mul(float4(interData.TexC, 0.0f, 1.0f), TexTransform);
	outputData.TexC = mul(TexC, MatTransform).xy;

    return outputData;
}