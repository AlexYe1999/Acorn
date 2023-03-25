#pragma once

#include "render_pass.hpp"
#include "runtime/function/render/rhi.hpp"
#include "runtime/function/render/render_pass.hpp"

#include <vector>
#include <memory>
#include <string>
#include <variant>

namespace Acorn
{
    class GraphNode
    {
    public:
        GraphNode(char* name)
            : m_name(name)
        {}

        virtual void Init()    = 0;
        virtual void Render()  = 0;
        virtual void Cleanup() = 0;

        const char* GetNodeName() const { return m_name.c_str(); }

    private:
        std::string m_name;
    };

    class ResourceNode : public GraphNode
    {
    public:

    private:

    };


    class PassNode : public GraphNode
    {
    public:

    private:
        std::unique_ptr<RenderPass> m_render_pass;
    };

    class FrameGraph
    {
    public:
        void Init();
        void Render();
        void Cleanup();

    private:
        //using RenderPasses = std::vector<std::unique_ptr<RenderPass>>;

        //RenderPasses m_render_passes;
    };
}