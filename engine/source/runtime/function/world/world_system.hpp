#pragma once

#include "runtime/function/framework/ecs/system.hpp"

namespace Acorn
{
    class WorldSystem : public System
    {
    public:
        WorldSystem();
        
        void InitSystem() override;
        void ShutdownSystem() override;

        void Tick(const float delta_time) override;

    protected:

    };
}