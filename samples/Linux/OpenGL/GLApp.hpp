#pragma once
#include"IApplication.hpp"

class GLApp : public IApplication{
public:
    GLApp(uint16_t width, uint16_t height, const std::string& name)
        : IApplication(width, height, name)
    {};

    virtual ~GLApp() {};

    virtual void OnInit() override {};
    virtual void OnUpdate() override {};
    virtual void OnRender() override {};
    virtual void OnDestroy() override {};

    virtual void OnResize() override {};

    virtual void OnKeyDown(uint16_t) override {}
    virtual void OnKeyUp(uint16_t)   override {}

    uint16_t GetWidth()  const { return m_uWidth; }
    uint16_t GetHeight() const { return m_uHeight; }
    const std::string& GetTitle() const { return m_strTitle; }

};