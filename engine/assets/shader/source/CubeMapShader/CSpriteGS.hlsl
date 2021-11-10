#include"CSprite.hlsli"

[maxvertexcount(24)]
void main(
    point GSIn GSin[1], 
    uint primID : SV_PrimitiveID,
    inout TriangleStream<PSIn> CubeMapStream
){
    float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 look = float3(0.0f, 50.0f, 0.0f) - GSin[0].PosW;
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

    PSIn outputData;

    for(int i = 0; i < 6; i++){
        outputData.RTIndex = i;

        for(int j = 0; j < 4; j++){
			outputData.PosH = mul(vertex[j], gCubeViewProjs[i]);

            outputData.PosW = vertex[j].xyz;
            outputData.NormalW = look;
            outputData.TexC = texC[j];
            outputData.PrimID = primID;
            CubeMapStream.Append(outputData);
        }

        CubeMapStream.RestartStrip();
    }

}