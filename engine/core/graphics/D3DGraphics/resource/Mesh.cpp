#include"Mesh.hpp"
namespace Acorn{

    D3D12_VERTEX_BUFFER_VIEW Mesh::VertexBufferView() const{
        D3D12_VERTEX_BUFFER_VIEW vbv;

        vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
        vbv.StrideInBytes = VertexBufferByteSize;
        vbv.SizeInBytes = VertexByteStride;
        return vbv;
    }

    D3D12_INDEX_BUFFER_VIEW Mesh::IndexBufferView() const{
        D3D12_INDEX_BUFFER_VIEW ibv;
        ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
        ibv.Format = IndexFormat;
        ibv.SizeInBytes = IndexBufferByteSize;
        return ibv;
    }

    void Mesh::ReleaseUploaders(){
        VertexBufferUploader = nullptr;
        IndexBufferUploader  = nullptr;
    }

}