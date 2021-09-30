#pragma once
#include<cstdint>
#include<memory>
#include<wrl.h>
#include<D3D12.h>
#include"Vector.hpp"
#include"Matrix.hpp"
#include"UploadBuffer.hpp"

namespace Acorn{
    struct PassConstant; //size 36
    struct ObjectConstant; // size 4

    struct FrameResource{
        FrameResource(ID3D12Device *device, uint32_t passCount, uint32_t objectCount);
        FrameResource(const FrameResource &frame) = delete;
        FrameResource& operator = (const FrameResource& frame) = delete;

        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CmdListAlloc;

        std::unique_ptr<UploadBuffer<PassConstant, true>> PassCB = nullptr;
        std::unique_ptr<UploadBuffer<ObjectConstant, true>> OjectCB = nullptr;

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
        float    Nearz;
        float    Farz;
        float    TotalTime;
        float    DeltaTime;
    }; //size 36

    struct ObjectConstant{
        Matrix4f World;
    }; // size 4

}