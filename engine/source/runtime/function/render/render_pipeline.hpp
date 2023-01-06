#pragma once

#include <vector>

namespace Acorn
{
    class RenderPipeline
    {
    public:
        RenderPipeline(char const* vert_path, char const* frag_path);

    private:
        static std::vector<char> ReadFile(char const* file_path);

        void CreateGraphicsPipeline(char const* vert_path, char const* frag_path);
    };
}