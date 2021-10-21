
struct Light{
	float3 Strength;
	float  FalloffStart;
	float3 Direction;
	float  FalloffEnd;
	float3 Position;
	float  SpotPower;
};

struct Material{
    float4 DiffuseAlbedo;
    float3 FresnelR0;
	float  Shininess;
};

cbuffer PassCB : register(b0) {
	float4x4 View;
	float4x4 InvView;
	float4x4 Proj;
	float4x4 InvProj;
	float4x4 ViewProj;
	float4x4 InvViewProj;
	float3 EyePosW;
	float pad1;
	float2 RenderTargetSize;
	float2 InvRenderTargetSize;
	float Nearz;
	float Farz;
	float TotalTime;
	float DeltaTime;
    float4 AmbientLight;
	Light Lights[3];
};

cbuffer ObjectCB : register(b1){
	float4x4 World;
	float4x4 WorldIT;
	float4x4 TexTransform;
};

cbuffer MatCB : register(b2){
	float4 DiffuseAlbedo;
    float3 FresnelR0;
    float  Roughness;
    float4x4 MatTransform;
};

Texture2D DiffuseMap : register(t0);

SamplerState samPointWrap : register(s0);
SamplerState samPointClamp : register(s1);
SamplerState samLinearWrap : register(s2);
SamplerState samLinearClamp : register(s3);
SamplerState samAnisotricWrap : register(s4);
SamplerState samAnisotricClamp : register(s5);

struct VSIn{
	float3 PosL  : POSITION;
	float3 NormalL : NORMAL;
	float2 TexC : TEXCOORD;
};

struct VSOut{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float2 TexC : TEXCOORD;
};

float CalcAttenuation(float d, float falloffStart, float falloffEnd){
	return saturate((d - falloffStart) / (falloffEnd - falloffStart));
}

float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec){
    
    float cos = saturate(dot(normal, lightVec));

	return R0 + (1.0f - R0) * pow(1 - cos, 5.0f);
}

float3 BlinnPhong(
    float3 lightStrength, float3 lightVec,
    float3 normal, float3 toEye, Material mat
){

	const float m = mat.Shininess * 256.0f;
	float3 halfVec = normalize(toEye + lightVec);

	float roughnessFactor = (m+8.0f) * pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
	float3 fresnelFactor = SchlickFresnel(mat.FresnelR0, halfVec, lightVec);

	float3 specAlbedo = roughnessFactor * fresnelFactor;

	specAlbedo = specAlbedo / (specAlbedo + 1.0f);

	return (mat.DiffuseAlbedo.rgb + specAlbedo) * lightStrength;
}

float3 CalcDirectionalLight(Light light, Material mat, float3 normal, float3 toEye){
    float3 lightVec = -light.Direction;

    float ndol = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = light.Strength * ndol;

	return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}

float3 CalcPointLight(Light light, Material mat, float3 pos, float3 normal, float3 toEye){
    float3 lightVec = light.Position - pos;

    float d = length(lightVec);

    if(d > light.FalloffEnd){
        return 0.0f;
    }

    lightVec /= d;
    float ndol = max(dot(lightVec, normal), 0.0f);

    float3 lightStrength = light.Strength * ndol;
    float att = CalcAttenuation(d, light.FalloffStart, light.FalloffEnd);
	lightStrength *= att;

	return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}

float3 CalcSpotLight(Light light, Material mat, float3 pos, float3 normal, float3 toEye){
	float3 lightVec = light.Position - pos;

	float d = length(lightVec);

	if (d > light.FalloffEnd)
		return 0.0f;

	lightVec /= d;

	float ndotl = max(dot(lightVec, normal), 0.0f);
	float3 lightStrength = light.Strength * ndotl;

	float att = CalcAttenuation(d, light.FalloffStart, light.FalloffEnd);
	lightStrength *= att;

	float spotFactor = max(dot(-lightVec, light.Direction), 0.0f);
	spotFactor = pow(spotFactor, light.SpotPower);
	lightStrength *= spotFactor;

	return lightStrength;

}