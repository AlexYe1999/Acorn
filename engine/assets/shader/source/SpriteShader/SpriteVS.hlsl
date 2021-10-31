#include"Sprite.hlsli"

VSOut main(VSIn VSin){
    VSOut outputData;
    outputData.PosW = VSin.PosW;
	outputData.Size = VSin.Size;
    return outputData;
}