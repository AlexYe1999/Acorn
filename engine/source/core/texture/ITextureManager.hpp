#pragma once
#include"IRuntimeModule.hpp"

namespace Acorn{
    
    class ITextureManager : public IRuntimeModule{
    public:
        ITextureManager() : IRuntimeModule {}
        
        virtual void Initialize() = 0;
        virtual void Finalize() = 0;

        virtual void Tick() = 0;


    }


}