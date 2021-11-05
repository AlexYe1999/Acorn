#pragma once
#include<cstdint>
#include<memory>
#include<wrl.h>
#include<D3D12.h>
#include"Vertex.hpp"
#include"UploadBuffer.hpp"

namespace Acorn{

    template<
        typename PassType, typename ObjectType, typename MaterialType
    >
    struct FrameResource{
        FrameResource::FrameResource(
            ID3D12Device* device,
            uint32_t passCount,
            uint32_t objectCount,
            uint32_t materialCount
        ){
            device->CreateCommandAllocator(
                D3D12_COMMAND_LIST_TYPE_DIRECT,
                IID_PPV_ARGS(CmdListAlloc.GetAddressOf())
            );

            PassCB = std::make_unique<UploadBuffer<PassType, true>>(device, passCount);
            ObjectCB = std::make_unique<UploadBuffer<ObjectType, false>>(device, objectCount);
            MaterialCB = std::make_unique<UploadBuffer<MaterialType, false>>(device, materialCount);
        }

        FrameResource(const FrameResource &frame) = delete;
        FrameResource& operator = (const FrameResource& frame) = delete;

        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CmdListAlloc;

        std::unique_ptr<UploadBuffer<PassType, true>> PassCB = nullptr;
        std::unique_ptr<UploadBuffer<ObjectType, false>> ObjectCB = nullptr;
        std::unique_ptr<UploadBuffer<MaterialType, false>> MaterialCB = nullptr;

        uint64_t Fence = 0;
    };

}