#pragma once

#include "ecs/system.hpp"
#include "ecs/registry.hpp"

namespace Acorn
{

    class EntitySystem : public System
    {
    public:
        ~EntitySystem() {};

        virtual void InitSystem() override;
        virtual void ShutdownSystem() override;

        virtual void Tick(const float delta_time) override;

    protected:
        std::unique_ptr<Registry> m_registry;
    };

}