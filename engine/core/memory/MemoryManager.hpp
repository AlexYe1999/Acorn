#pragma once
#include"Allocator.hpp"
#include"IRuntimeModule.hpp"
#include<malloc.h>
namespace Acorn {
    class MemoryManager : IRuntimeModule{
    public:
        template<typename T, typename... Arguments>
        T* New(Arguments... parameters){
            return new (Allocate(sizeof(T))) T(parameters...);
        }

        template<typename T>
        void Delete(T *p){
            reinterpret_cast<T*>(p)->~T();
            Free(p, sizeof(T));
        }

    public:
        virtual ~MemoryManager() {}

        virtual void Initialize() override;
        virtual void Finalize() override;

        void* Allocate(size_t size);
        void  Free(void* p, size_t size);
    private:
        static size_t*      m_pBlockSizeLookup;
        static Allocator*   m_pAllocators;
    private:
        static Allocator* LookUpAllocator(size_t size);
    };
}