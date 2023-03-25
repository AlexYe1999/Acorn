#pragma once

namespace Acorn
{
    class System
    {
    public:
        ~System() {};

        virtual void InitSystem() = 0;
        virtual void ShutdownSystem() = 0;

        virtual void Tick(const float delta_time) = 0;
    };
}
