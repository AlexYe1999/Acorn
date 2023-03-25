#pragma once

#include "struct.hpp"

#include <memory>
#include <cstdint>
#include <cstdlib>

namespace Acorn
{
    class Texture
    {
    public:
        Texture(size_t width, size_t height, COLOR format)
            : m_width(width)
            , m_height(height)
            , m_format(format)
        {}

        virtual void SetParam(size_t index)             = 0;
        virtual void CopyToTexture(std::byte* buffer)   = 0;
        virtual void CopyFromTexture(std::byte* buffer) = 0;

    private:
        size_t m_width  { 0 };
        size_t m_height { 0 };
        COLOR  m_format { COLOR::NONE };
    };
    using TexturePtr = std::unique_ptr<Texture>;
}