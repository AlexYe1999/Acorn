#include"Sprite.hlsli"

[maxvertexcount(4)]
void main(
    point VSOut GSin[1], 
    uint primID : SV_PrimitiveID,
    inout TriangleStream<GSOut> triStream
){
    float3 up = float3(0.0f, 1.0f, 0.0f);
    float3 look = EyePosW - GSin[0].PosW;
    look.y = 0.0f;
    look = normalize(look);
    float3 right = cross(up, look);

    float2 halfSize = 0.5f * GSin[0].Size;
    float4 vertex[4] = {
        float4(GSin[0].PosW + halfSize.x*right - halfSize.y*up, 1.0f),
        float4(GSin[0].PosW + halfSize.x*right + halfSize.y*up, 1.0f),
        float4(GSin[0].PosW - halfSize.x*right - halfSize.y*up, 1.0f),
        float4(GSin[0].PosW - halfSize.x*right + halfSize.y*up, 1.0f)
    };

    float2 texC[4] = {
        float2(0.0f, 1.0f),
        float2(0.0f, 0.0f),
        float2(1.0f, 1.0f),
        float2(1.0f, 0.0f),
    };

    GSOut OutputData;

    for(int i = 0; i < 4; i++){
        OutputData.PosH = mul(vertex[i], ViewProj);
        OutputData.PosW = vertex[i].xyz;
        OutputData.NormalW = look;
        OutputData.TexC = texC[i];
        OutputData.PrimID = primID;
        triStream.Append(OutputData);
    }
}