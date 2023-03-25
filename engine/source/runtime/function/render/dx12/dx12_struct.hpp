#pragma once

#include "runtime/function/render/struct.hpp"

#include <stdexcept>
#include <d3d12.h>

namespace Acorn
{
    namespace Dx12Enum
    {
        D3D12_COMMAND_LIST_TYPE CommandListType(COMMAND_LIST_TYPE type)
        {
            switch (type)
            {
                case Acorn::COMMAND_LIST_TYPE::GRAPHICS:
                {
                    return D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
                }
                default:
                {
                    throw std::runtime_error("invalid command list type !");
                    return D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
                }
            }
        }
    }

    namespace Dx12Struct
    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC Dx12GraphicsPipelineStateDesc(GraphicsPipelineStateDescriptor desc)
        {
            D3D12_GRAPHICS_PIPELINE_STATE_DESC dx12_desc;

            return dx12_desc;
        }
    }

    namespace Dx12Dscriptor
    {
        
    };
} 