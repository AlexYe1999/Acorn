#pragma once
#include"IApplication.hpp"
#include<xcb/xcb.h>
#include<cstring>

class AppFramework{
public:
    static int Run(IApplication* pApp, int argc, char* argv[]);
    static IApplication* GetApp()  { return m_pApp; }

private:
    static inline IApplication* m_pApp = nullptr;
};