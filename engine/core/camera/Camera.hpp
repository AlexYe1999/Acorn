#pragma once
#include"Vector.hpp"
#include"Matrix.hpp"
#include<cstdint>
#include<utility>
#include<DirectXCollision.h>
namespace Acorn{
    
    class Camera{
    public:
        Camera();
        
        void SetLens(const float zNear, const float zFar,
            const float m_fAspect,const float m_fFovY);

        void Walk(Vector3f direction, const float length);

        void SetPosition(const Vector3f& position);
        const Vector3f& GetPosition() const;
        
        void LookAt(const Vector3f& position, const Vector3f& lookAt, const Vector3f& up);
        const Vector3f& GetLookAt() const;

        void Pitch(float angle);
        void RotateY(float angle);

        const Matrix4f& GetViewMatrix();
        const Matrix4f& GetProjMatrix() const;

        const DirectX::BoundingFrustum& GetFrustum(){ return m_Frustum; };
    private:
        void CalcViewMatrix();

    private:
        bool m_bIsViewDirty;
        float m_fNearZ = 0.0f;
        float m_fFarZ  = 0.0f;
        float m_fAspect = 0.0f;
        float m_fFov    = 0.0f;
        float m_fNearWindowHeight = 0.0f;
        float m_fFarWindowHeight  = 0.0f;

        Vector3f m_vUp;
        Vector3f m_vRight;
        Vector3f m_vLookAt;
        Vector3f m_vPosition;

        Matrix4f m_mViewMatrix;
        Matrix4f m_mProjMatrix;

        DirectX::BoundingFrustum m_Frustum;
    };


}