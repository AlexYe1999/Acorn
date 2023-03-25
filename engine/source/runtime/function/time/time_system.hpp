#pragma once

namespace Acorn
{
    class TimeSystem
    {
    public:
        virtual void InitSystem() = 0;
        virtual void ShutdownSystem() = 0;

    };

}