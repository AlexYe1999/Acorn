#include"CLandAndWave.hlsli"

[maxvertexcount(18)]
void main(
    triangle GSIn input[3],
    inout TriangleStream<PSIn> CubeMapStream
){
    for(int i = 0; i < 6; i++){
        PSIn outputData;
		outputData.RTIndex = i;

        for(int j = 0; j < 3; j++){
			outputData.PosH = mul(float4(input[j].PosW, 1.0f), gCubeViewProjs[i]);

            outputData.PosW = input[j].PosW;
            outputData.NormalW = input[j].NormalW;
            outputData.TexC = input[j].TexC;
            CubeMapStream.Append(outputData);   
        }

        CubeMapStream.RestartStrip();
    }
}