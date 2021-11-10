#include"Camera.hpp"

namespace Acorn{

    Camera::Camera() 
        : 
        m_bIsViewDirty(true),
        m_fNearZ(1.0f), m_fFarZ(1000.0f),
        m_fAspect(1.0f), m_fFov(0.25f * 3.1415926535f),
        m_fNearWindowHeight(0.0f),
        m_fFarWindowHeight(0.0f),
        m_vUp(0.0f, 1.0f, 0.0f),
        m_vRight(1.0f, 0.0f, 0.0f),
        m_vLookAt(0.0f, 0.0f, 1.0f),
        m_vPosition(0.0f, 0.0f, 0.0f)
    {
        SetLens(1.0f, 1000.0f, 1.0f, XMConvertToRadians(60));
    }

    void Camera::SetLens(
        const float zNear, const float zFar,
        const float aspect, const float fov
    ){
        m_fNearZ = zNear;
        m_fFarZ = zFar;
        m_fFov = fov;
        m_fAspect = aspect;

        m_fNearWindowHeight = 2.0f * m_fNearZ * tanf(0.5f * m_fFov);
        m_fFarWindowHeight = 2.0f * m_fFarZ * tanf(0.5f * m_fFov);

        m_mProjMatrix = XMMatrixPerspectiveFovLH(m_fFov, m_fAspect, m_fNearZ, m_fFarZ);
    }

    void Camera::Walk(Vector3f direction, const float length){
        direction = m_vRight*direction.x 
            + m_vUp*direction.y + m_vLookAt*direction.z;
        XMVECTOR l = XMVectorReplicate(length);
        XMVECTOR d = XMLoadFloat3(&direction);
        XMVECTOR p = XMLoadFloat3(&m_vPosition);
        DirectX::XMStoreFloat3(&m_vPosition, XMVectorMultiplyAdd(l, d, p));

        m_bIsViewDirty = true;
    }

    void Camera::SetPosition(const Vector3f& position){
        m_vPosition = position;
        m_bIsViewDirty = true;
    }

    const Vector3f& Camera::GetPosition() const{
        return m_vPosition;
    }

    void Camera::LookAt(const Vector3f& position, const Vector3f& lookAt, const Vector3f& up){
        
        XMVECTOR Position = XMLoadFloat3(&position);
        XMVECTOR LookAt = XMLoadFloat3(&lookAt);
        XMVECTOR Up = XMLoadFloat3(&up);

        LookAt = XMVector3Normalize(XMVectorSubtract(LookAt, Position));
        XMVECTOR Right = XMVector3Normalize(XMVector3Cross(Up, LookAt));
        Up = XMVector3Cross(LookAt, Right);

        m_vPosition = Position;
        m_vLookAt   = LookAt;
        m_vRight    = Right;
        m_vUp       = Up;

        m_bIsViewDirty = true;
    }

    const Vector3f& Camera::GetLookAt() const{
        return m_vLookAt;
    }

    void Camera::Pitch(float angle){
        XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_vRight), angle);

        m_vUp =  XMVector3TransformNormal(XMLoadFloat3(&m_vUp), R);
        m_vLookAt = XMVector3TransformNormal(XMLoadFloat3(&m_vLookAt), R);

        m_bIsViewDirty = true;
    }

    void Camera::RotateY(float angle){
        XMMATRIX R = XMMatrixRotationY(angle);

        m_vLookAt = XMVector3TransformNormal(XMLoadFloat3(&m_vLookAt), R);
        m_vRight  = XMVector3TransformNormal(XMLoadFloat3(&m_vRight), R);
        m_vUp     = XMVector3TransformNormal(XMLoadFloat3(&m_vUp), R);

        m_bIsViewDirty = true;
    }

    const Matrix4f& Camera::GetViewMatrix(){
        if(m_bIsViewDirty){
            CalcViewMatrix();
            m_bIsViewDirty = false;
        }
        return m_mViewMatrix;
    }

    const Matrix4f& Camera::GetProjMatrix() const{
        return m_mProjMatrix;
    }

    void Camera::CalcViewMatrix(){
        m_mViewMatrix = XMMatrixLookAtLH(
            XMLoadFloat3(&m_vPosition),
            XMLoadFloat3(&(m_vPosition + m_vLookAt)),
            XMLoadFloat3(&m_vUp)
        );
    }

}