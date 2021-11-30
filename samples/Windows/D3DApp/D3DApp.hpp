#pragma once
#include"stdafx.hpp"

class D3DApp : public App{
public:
    D3DApp(uint16_t width, uint16_t height, std::string name);
    virtual ~D3DApp();

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

protected:
    std::unique_ptr<Acorn::GraphicsManager> m_pGraphicsMgr;

};

