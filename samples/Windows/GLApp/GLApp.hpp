#pragma once
#include"App.hpp"
#include"stdafx.hpp"

class __declspec(dllexport) GLApp : public App{
public:
    GLApp(uint16_t width, uint16_t height, std::string name);
    virtual ~GLApp();

    virtual void OnInit() override;
    virtual void OnUpdate() override;
    virtual void OnRender() override;
    virtual void OnDestroy() override;

    virtual void OnResize() override;

    virtual void OnKeyDown(uint16_t) override;
    virtual void OnKeyUp(uint16_t) override;
    float GetAspectRatio() const{ return m_fAspectRatio; }

protected:

    float m_fAspectRatio;

};
