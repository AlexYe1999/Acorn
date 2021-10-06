#pragma once
#include<cstdint>
#include<memory>
#include<wrl.h>
#include<D3D12.h>
#include"Vector.hpp"
#include"Matrix.hpp"
#include"UploadBuffer.hpp"

namespace Acorn{

    template<typename PassType, typename ObjectType>
    struct FrameResource{
        FrameResource::FrameResource(
            ID3D12Device* device,
            uint32_t passCount,
            uint32_t objectCount
        ){
            device->CreateCommandAllocator(
                D3D12_COMMAND_LIST_TYPE_DIRECT,
                IID_PPV_ARGS(CmdListAlloc.GetAddressOf())
            );

            PassCB = std::make_unique<UploadBuffer<PassType, true>>(device, passCount);
            ObjectCB = std::make_unique<UploadBuffer<ObjectType, true>>(device, objectCount);
        }
        FrameResource(const FrameResource &frame) = delete;
        FrameResource& operator = (const FrameResource& frame) = delete;

        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CmdListAlloc;

        std::unique_ptr<UploadBuffer<PassType, true>> PassCB = nullptr;
        std::unique_ptr<UploadBuffer<ObjectType, true>> ObjectCB = nullptr;

        uint64_t Fence = 0;
    };

    struct PassConstant{
        Matrix4f View;
        Matrix4f InvView;
        Matrix4f Proj;
        Matrix4f InvProj;
        Matrix4f ViewProj;
        Matrix4f InvViewProj;
        Vector3f EyePosW;
        float    pad1;
        Vector2f RenderTargetSize;
        Vector2f InvRenderTargetSize;
        float    NearZ;
        float    FarZ;
        float    TotalTime;
        float    DeltaTime;
        PassConstant() = default;
    }; //size 6*16 + 4 + 4 + 4 = 108*4 = 432

    struct ObjectConstant{
        Matrix4f World;
        ObjectConstant() = default;
    }; // size 16*4 = 64

}