#pragma once
#include"IRuntimeModule.hpp"
#include<cstdint>
#include<bitset>

namespace Acorn{

    template<uint8_t KeyNum>
    class IKeyModule : public IRuntimeModule{
    public:
        IKeyModule() : IRuntimeModule(){}

        virtual void Initialize() = 0;
        virtual void Finalize() = 0;
        virtual void Tick() = 0;

        virtual void KeyDown(const uint8_t key){
            m_bKeyBitSet[key] = true;    
        }

        virtual void KeyUp(const uint8_t key){
            m_bKeyBitSet[key] = false;
        }

        virtual bool IsKeyDown(const uint8_t key) const{
            return m_bKeyBitSet[key];
        }

    protected:
        std::bitset<KeyNum> m_bKeyBitSet;
    };

}