#include"LandAndWave.hlsli"

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

float4 main(VSOut PSin) : SV_Target {

    float3 toEye = normalize(EyePosW-PSin.PosW);
	float4 diffuseAlbedo = DiffuseAlbedo * DiffuseMap.Sample(samAnisotricWrap, PSin.TexC);
	
	clip(diffuseAlbedo.a - 0.01f);
	
	float4 ambient = AmbientLight * diffuseAlbedo;
  
    PSin.NormalW = normalize(PSin.NormalW);
    
	Material mat = { diffuseAlbedo, FresnelR0, 1.0f - Roughness };
	float4 direct = CalcLighting(mat, PSin.PosW, PSin.NormalW, toEye);
	float4 litColor = ambient + direct;
    
	litColor.a = DiffuseAlbedo.a;
	return litColor;
}

