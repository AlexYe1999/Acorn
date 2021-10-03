
cbuffer PassCB : register(b0){
	float4x4 gWorldViewProj;
}

cbuffer objCB : register(b1) {
	float4x4 View;
	float4x4 InvView;
	float4x4 Proj;
	float4x4 InvProj;
	float4x4 ViewProj;
	float4x4 InvViewProj;
	float3 EyePosW;
	float  pad1;
	float2 RenderTargetSize;
	float2 InvRenderTargetSize;
	float Nearz;
	float Farz;
	float TotalTime;
	float DeltaTime;
}

struct VertexIn{
	float3 PosL  : POSITION;
	float4 Color : COLOR;
};

struct VertexOut{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
};