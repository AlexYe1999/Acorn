#pragma once
#include"IApplication.hpp"
#include<cstring>

#include<X11/Xlib.h>
#include<xcb/xcb.h>

#include<GL/gl.h> 
#include<GL/glx.h> 

class AppFramework{
public:
    static int Run(IApplication* pApp, int argc, char* argv[]);
    static IApplication* GetApp()  { return m_pApp; }

private:
    static inline IApplication* m_pApp = nullptr;
};