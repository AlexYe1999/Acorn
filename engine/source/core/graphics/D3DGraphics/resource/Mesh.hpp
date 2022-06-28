#pragma once
#include<cstdint>
#include<string>
#include<unordered_map>
#include<wrl.h>
#include<D3D12.h>
#include<directxcollision.h>
namespace Acorn{

    struct SubMesh{
        uint16_t IndexCount = 0;
        uint16_t StartIndexLocation  = 0;
        uint32_t StartVertexLocation = 0;
        DirectX::BoundingBox BBox;
    };

    struct Mesh{

        D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const;
        D3D12_INDEX_BUFFER_VIEW IndexBufferView() const;
        void ReleaseUploaders();

        std::string Name;
        std::unordered_map<std::string, SubMesh> SubMesh;

        DXGI_FORMAT IndexFormat       = DXGI_FORMAT_R16_UINT;
        uint16_t VertexByteStride     = 0;
        uint32_t IndexBufferByteSize  = 0;
        uint32_t VertexBufferByteSize = 0;

        Microsoft::WRL::ComPtr<ID3DBlob> VertexBufferCPU = nullptr;
        Microsoft::WRL::ComPtr<ID3DBlob> IndexBufferCPU  = nullptr;

        Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;
        Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferGPU  = nullptr;

        Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
        Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader  = nullptr;

        Mesh(std::string name) : Name(std::move(name)){};
    };



}