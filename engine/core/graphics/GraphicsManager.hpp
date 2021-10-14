#pragma once
#include"IRuntimeModule.hpp"
#include"Vector.hpp"
#include"Scene.hpp"
#include"Timer.hpp"

namespace Acorn{

    class GraphicsManager : public IRuntimeModule{
    public:
        virtual void Initialize() = 0;
        virtual void Initialize(Scene* const scene, Timer* const timer) = 0;
        virtual void Finalize() = 0;
        virtual void Tick() = 0;

        virtual void Render() = 0;
        virtual void Clear() = 0;

        virtual void DrawLine(const Vector3f& from, const Vector3f& to, const Vector3f& color) = 0;
        virtual void DrawBox(const Vector3f& bbMin, const Vector3f& bbMax, const Vector3f& color) = 0;
        virtual void ClearDebugBuffers() = 0;

    protected:
        virtual void InitializeBuffers() = 0;
        virtual void InitializeConstants() = 0;
        virtual void InitializeShaders() = 0;
        virtual void ClearBuffers() = 0;
        virtual void ClearShaders() = 0;

        virtual void UpdateFrameResource() = 0;
        virtual void UpdateConstants() = 0;
        virtual void RenderBuffers() = 0;

    };
}