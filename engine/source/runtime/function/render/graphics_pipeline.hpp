#pragma once

#include <memory>

namespace Acorn
{
    class GraphicsPipeline
    {
    public:
        GraphicsPipeline() = default;
        virtual ~GraphicsPipeline() = default;
    };
    using GraphicsPipelinePtr = std::unique_ptr<GraphicsPipeline>;
}