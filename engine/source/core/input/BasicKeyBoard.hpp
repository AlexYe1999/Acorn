#pragma once
#include "IKeyModule.hpp"

namespace Acorn{

    class BasicKeyBoard : public IKeyModule<256>{
    public:
        virtual void Initialize() override {};
        virtual void Finalize() override {};
        virtual void Tick() override {};
    };

}