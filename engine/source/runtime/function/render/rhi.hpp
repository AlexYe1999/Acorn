#pragma once

namespace Acorn
{

    class RHI
    {
    public:
        virtual ~RHI() = 0;

        virtual void Initialize() = 0;
        virtual void Shutdown()   = 0;
    };
}
