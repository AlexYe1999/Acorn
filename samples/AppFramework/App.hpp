#pragma once
#include<cstdint>
#include<string>
#include<cstdlib>

class App{
public:
    App(uint16_t width, uint16_t height, std::string name) 
        : 
        m_uWidth(width),
        m_uHeight(height),
        m_wstrTitle(name)
    {};
    virtual ~App() {};

    virtual void OnInit() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
    virtual void OnDestroy() = 0;

    virtual void OnResize() = 0;

    virtual void OnKeyDown(uint16_t)   {}
    virtual void OnKeyUp(uint16_t)     {}

    // Accessors.
    uint16_t GetWidth() const { return m_uWidth; }
    uint16_t GetHeight() const { return m_uHeight; }
    const std::string& GetTitle() const { return m_wstrTitle; }

protected:
    uint16_t m_uWidth;
    uint16_t m_uHeight;
    std::string m_wstrTitle;
};