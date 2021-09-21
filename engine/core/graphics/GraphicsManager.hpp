#pragma once
#include"IRuntimeModule.hpp"
#include"Vector.hpp"

namespace AcornEngine{

    class GraphicsManager : public IRuntimeModule{
    public:
        static GraphicsManager* GetInstance() {
            return s_pInstance != nullptr ? s_pInstance
                : s_pInstance = new GraphicsManager;
        }

        virtual void Initialize() = 0;
        virtual void Finalize() = 0;
        virtual void Tick() = 0;

        virtual void Clear() = 0;
        virtual void Draw() = 0;

        virtual void DrawLine(const Vector3f& from, const Vector3f& to, const Vector3f& color) = 0;
        virtual void DrawBox(const Vector3f& bbMin, const Vector3f& bbMax, const Vector3f& color) = 0;
        virtual void ClearDebugBuffers();

    protected:
        virtual bool InitializeShaders() = 0;
        virtual void ClearShaders() = 0;
        //virtual void InitializeBuffers(const Scene& scene) = 0;
        virtual void ClearBuffers() = 0 ;

        virtual void InitConstants() = 0;
        virtual void CalculateCameraMatrix() = 0;
        virtual void CalculateLights() = 0;
        virtual void UpdateConstants() = 0;
        virtual void RenderBuffers() = 0;

    protected:
        void CreateCommandObject() = 0;
        void CreateSwapChain() = 0;

    };
}