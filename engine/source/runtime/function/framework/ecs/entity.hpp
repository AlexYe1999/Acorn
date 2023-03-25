#pragma once
#include "entt/entt.hpp"

#include <memory>

namespace Acorn
{
    class Entity
    {
    public:
        Entity(const entt::entity& entity)
            : m_entity{ entity }
        {}

        Entity(entt::entity&& entity)
            : m_entity{ std::move(entity) }
        {}

    protected:
        entt::entity m_entity;
    };
    using EntityPtr = std::unique_ptr<Entity>;
}