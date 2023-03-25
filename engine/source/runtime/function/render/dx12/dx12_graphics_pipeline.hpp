#pragma once

#include "runtime/function/render/struct.hpp"
#include "runtime/function/render/graphics_pipeline.hpp"

#include <d3d12.h>

namespace Acorn
{
    class Dx12GraphicsPipeline : public GraphicsPipeline
    {
    public:
        Dx12GraphicsPipeline(GraphicsPipelineStateDescriptor const& descriptor, const ID3D12Device8* device);
        ~Dx12GraphicsPipeline();
    };
}