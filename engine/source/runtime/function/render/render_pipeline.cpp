#include "render_pipeline.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

namespace Acorn
{
    std::vector<char> RenderPipeline::ReadFile(char const* file_path)
    {
        std::ifstream file(file_path, std::ios::ate | std::ios::binary);
    
        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file : " + std::string(file_path));
        }

        size_t file_size = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(file_size);

        file.seekg(0);
        file.read(buffer.data(), file_size);
        file.close();

        return buffer;
    }

    RenderPipeline::RenderPipeline(char const* vert_path, char const* frag_path)
    {
        CreateGraphicsPipeline(vert_path, frag_path);
    }

    void RenderPipeline::CreateGraphicsPipeline(char const* vert_path, char const* frag_path)
    {

        auto vert_code = ReadFile(vert_path);
        auto frag_code = ReadFile(frag_path);

        std::cout << "vert shader code size : " << vert_code.size() << "\n";
        std::cout << "frag shader code size : " << frag_code.size() << "\n";
    }

}