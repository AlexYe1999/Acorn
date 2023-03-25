#pragma once

#include "runtime/function/render/struct.hpp"

#include <memory>

namespace Acorn
{
    class Shader
    {
    public:
        Shader(ShaderDescriptor const& descriptor) : m_descriptor(descriptor) {}
        virtual ~Shader() = default;

    protected:
        ShaderDescriptor m_descriptor;
    };

    using ShaderPtr = std::unique_ptr<Shader>;
} 