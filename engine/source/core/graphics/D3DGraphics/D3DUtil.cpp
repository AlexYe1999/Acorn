#include"D3DUtil.hpp"

namespace D3DUtil{

    Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(        
        ID3D12Device* device, ID3D12GraphicsCommandList* cmdList,
        const void* initData, uint64_t byteSize,
        Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer
    ){
        Microsoft::WRL::ComPtr<ID3D12Resource> defaultBuffer;

        device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(byteSize),
            D3D12_RESOURCE_STATE_COMMON, nullptr,
            IID_PPV_ARGS(defaultBuffer.GetAddressOf())
        );

        D3D12_SUBRESOURCE_DATA subResourceData = {};
        subResourceData.pData = initData;
        subResourceData.RowPitch = byteSize;
        subResourceData.SlicePitch = byteSize;

        device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(byteSize),
            D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
            IID_PPV_ARGS(uploadBuffer.GetAddressOf())
        );

        cmdList->ResourceBarrier(
            1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
            D3D12_RESOURCE_STATE_COMMON,
            D3D12_RESOURCE_STATE_COPY_DEST)
        );

        UpdateSubresources(
            cmdList, defaultBuffer.Get(), uploadBuffer.Get(),
            0, 0, 1, &subResourceData
        );

        cmdList->ResourceBarrier(
            1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
            D3D12_RESOURCE_STATE_COPY_DEST,
            D3D12_RESOURCE_STATE_GENERIC_READ)
        );

        return defaultBuffer;
    }
    
    Microsoft::WRL::ComPtr<ID3DBlob> LoadBinaryShader(const std::string &fileName){
        std::ifstream fin(fileName, std::ios::binary);

        fin.seekg(0, std::ios::end);
        std::streamsize size = static_cast<std::streamsize>(fin.tellg());
        fin.seekg(0, std::ios::beg);

        Microsoft::WRL::ComPtr<ID3DBlob> blob;
        D3DCreateBlob(size, blob.GetAddressOf());

        fin.read(static_cast<char*>(blob->GetBufferPointer()), size);
        fin.close();

        return blob;
    }

}