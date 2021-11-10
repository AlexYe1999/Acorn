#include"CSprite.hlsli"

GSIn main(VSIn VSin){
    GSIn outputData;
    outputData.PosW = VSin.PosW;
	outputData.Size = VSin.Size;
    return outputData;
}