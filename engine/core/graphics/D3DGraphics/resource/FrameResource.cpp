#include"FrameResource.hpp"

namespace Acorn{

    FrameResource::FrameResource(
        ID3D12Device *device, 
        uint32_t passCount,
        uint32_t objectCount
    ){
        device->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            IID_PPV_ARGS(CmdListAlloc.GetAddressOf())
        );

        PassCB = std::make_unique<UploadBuffer<PassConstant, true>>(device, passCount);
        OjectCB = std::make_unique<UploadBuffer<ObjectConstant, true>>(device, objectCount);

    }
}