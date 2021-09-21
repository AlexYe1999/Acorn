#pragma once
#include"IRuntimeModule.hpp"

namespace AcornEngine{

    class Timer : public IRuntimeModule{
    public:
        virtual void Initialize() override;
        virtual void Finalize() override;
        virtual void Tick() override;

    };

}