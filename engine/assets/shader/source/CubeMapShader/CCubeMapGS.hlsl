#include"CCubeMap.hlsli"

[maxvertexcount(18)]
void main(
    triangle GSIn inputData[3],
    inout TriangleStream<PSIn> CubeMapStream 
){
    
    for(int i = 0; i < 6; i++){
        PSIn outputData;
		outputData.RTIndex = i;
        
        for(int j = 0; j < 3; j++){
			outputData.PosH = mul(float4(inputData[j].PosW, 1.0f), gCubeViewProjs[i]).xyww;
            outputData.PosL = inputData[j].PosL;

            CubeMapStream.Append(outputData);
        }
	    
        CubeMapStream.RestartStrip();
    }
}