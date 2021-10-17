#pragma once
#include<cstdint>
#include<memory>
#include<wrl.h>
#include<D3D12.h>
#include"Vertex.hpp"
#include"UploadBuffer.hpp"

namespace Acorn{

    template<
        typename PassType, typename ObjectType,
        typename MaterialType, typename VertexType>
    struct FrameResource{
        FrameResource::FrameResource(
            ID3D12Device* device,
            uint32_t passCount,
            uint32_t objectCount,
            uint32_t materialCount = 0,
            uint32_t VertexCount = 0
        ){
            device->CreateCommandAllocator(
                D3D12_COMMAND_LIST_TYPE_DIRECT,
                IID_PPV_ARGS(CmdListAlloc.GetAddressOf())
            );

            PassCB = std::make_unique<UploadBuffer<PassType, true>>(device, passCount);
            ObjectCB = std::make_unique<UploadBuffer<ObjectType, true>>(device, objectCount);
            MaterialCB = std::make_unique<UploadBuffer<MaterialType, true>>(device, materialCount);
            DynamicVB = std::make_unique<UploadBuffer<VertexType, false>>(device, VertexCount);
        }

        FrameResource(const FrameResource &frame) = delete;
        FrameResource& operator = (const FrameResource& frame) = delete;

        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CmdListAlloc;

        std::unique_ptr<UploadBuffer<PassType, true>> PassCB = nullptr;
        std::unique_ptr<UploadBuffer<ObjectType, true>> ObjectCB = nullptr;
        std::unique_ptr<UploadBuffer<MaterialType, true>> MaterialCB = nullptr;
        std::unique_ptr<UploadBuffer<VertexPNC, false>> DynamicVB = nullptr;

        uint64_t Fence = 0;
    };

}