#include"Sprite.hlsli"

float4 CalcLighting(
    Material mat, float3 pos, 
    float3 normal, float3 toEye
){
	float3 result = 0.1f;
    result += CalcDirectionalLight(Lights[0], mat, normal, toEye);
	result += CalcPointLight(Lights[1], mat, pos, normal, toEye);
    result += CalcSpotLight(Lights[2], mat, pos, normal, toEye);
    return float4(result, 0.0f);
}

float4 main(GSOut PSin) : SV_Target{
	
	float3 uvw = float3(PSin.TexC, PSin.PrimID%3);
	float4 diffuseAlbedo = DiffuseAlbedo * DiffuseMap.Sample(samAnisotricWrap, uvw);
	
	clip(diffuseAlbedo.a - 0.01f);
	
	float4 ambient = AmbientLight * diffuseAlbedo;
	float3 toEye = EyePosW - PSin.PosW;
	float distance = length(toEye);
	toEye /= distance;
	
	Material mat = { diffuseAlbedo, FresnelR0, 1.0f - Roughness };
	
	PSin.NormalW = normalize(PSin.NormalW);
	float4 direct = CalcLighting(mat, PSin.PosW, PSin.NormalW, toEye);
	float4 litColor = ambient + direct;
    
	float fogAmount = saturate((distance - FogStart) / FogRange);
	litColor = lerp(litColor, FogColor, fogAmount);
	
	litColor.a = DiffuseAlbedo.a;
	return litColor;
}