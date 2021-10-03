#pragma once
#include"d3dx12.h"
#include<wrl.h>
#include<D3D12.h>
#include<cstdint>
#include<string>
#include<fstream>
#include<d3dcompiler.h>

namespace D3DUtil{

    Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(
        ID3D12Device* device, ID3D12GraphicsCommandList* cmdList,
        const void* initData, uint16_t byteSize,
        Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer
    );


    enum class AlignmentType : uint16_t{
        CONSTANT_BUFFER = 256
    };

    template<AlignmentType Alignment = AlignmentType::CONSTANT_BUFFER>
    constexpr uint16_t CalcAlignment(uint16_t byteSize){
        uint16_t alignSize = static_cast<uint16_t>(Alignment);
        return (byteSize + alignSize - 1) & ~(alignSize - 1);
    }

    Microsoft::WRL::ComPtr<ID3DBlob> LoadBinaryShader(const std::string &fileName);

    
}
