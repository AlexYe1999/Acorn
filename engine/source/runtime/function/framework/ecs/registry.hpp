#pragma once
#include "entity.hpp"

namespace Acorn
{
    class Registry final
    {
    public:
        Registry(){};
        Registry(const Registry&) = delete;
        Registry(Registry&&) = delete;
        


    protected:
        entt::registry m_registry{};
    };
} 