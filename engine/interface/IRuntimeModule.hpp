#pragma once

namespace AcornEngine{

    class IRuntimeModule{
    public:
        IRuntimeModule() : m_bIsInit(false) {}
        
        virtual void Initialize() = 0;
        virtual void Finalize() = 0;

        virtual void Tick() = 0;

    protected:
        bool m_bIsInit;
    };

}