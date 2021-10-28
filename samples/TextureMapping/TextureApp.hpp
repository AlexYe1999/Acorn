#pragma once
#include"Timer.hpp"
#include"Mesh.hpp"
#include"Scene.hpp"
#include"Point.hpp"
#include"Vector.hpp"
#include"Vertex.hpp"
#include"BasicMouse.hpp"
#include"AppFramework.hpp"
#include"GeometryGenerator.hpp"
#include"D3D12GraphicsManager.hpp"
#include"Waves.h"
#include<memory>
#include<iostream>
#include<sstream>
#include<unordered_map>
#include<DirectXMath.h>

class TextureApp : public AppFramework<TextureApp>{
public:
    TextureApp();
    ~TextureApp(){}

    virtual void InitApp(const HINSTANCE appInstance, int nCmdShow, 
        const int16_t width, const int16_t height,
        const std::string& wndClassName,
        const std::string& wndText
    );
    virtual void RunApp() override;
    virtual LRESULT MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

protected:
    std::unique_ptr<Acorn::Scene> m_pScene;
    std::unique_ptr<Waves> m_pWaves;

    Acorn::Timer m_Timer;
    Acorn::BasicMouse m_Mouse;

    std::unique_ptr<Acorn::D3D12GraphicsManager> m_pGraphicsManager;

protected:
    virtual void BuildScene();
    virtual void CreateMesh();
    virtual void CreateTexture();
    virtual void CreateMaterial();
    virtual void CreateRenderItem();

protected:
    void UpdateInput();
    void UpdateScene();
    
protected:
    void AnimateMaterial();

};