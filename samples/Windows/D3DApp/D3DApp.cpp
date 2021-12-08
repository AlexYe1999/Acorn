#include"D3DApp.hpp"

D3DApp::D3DApp(uint16_t width, uint16_t height, std::string name) 
    : 
    App(width, height, name),
    m_pGraphicsMgr(nullptr)
{
    m_fAspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

D3DApp::~D3DApp(){}

void D3DApp::OnInit(){
    m_pGraphicsMgr = new D3D12GraphicsManager;
}

void D3DApp::OnUpdate(){

}

void D3DApp::OnRender(){

}

void D3DApp::OnDestroy(){

}

void D3DApp::OnResize(){

}

void D3DApp::OnKeyDown(uint16_t){

}

void D3DApp::OnKeyUp(uint16_t){

}