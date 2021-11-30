#include"GLApp.hpp"

GLApp::GLApp(uint16_t width, uint16_t height, std::string name) 
    : App(width, height, name)
{
    m_fAspectRatio = width / height;
}

GLApp::~GLApp(){}

void GLApp::OnInit(){

}

void GLApp::OnUpdate(){

}

void GLApp::OnRender(){

}

void GLApp::OnDestroy(){

}

void GLApp::OnResize(){

}

void GLApp::OnKeyDown(uint16_t){

}

void GLApp::OnKeyUp(uint16_t){

}