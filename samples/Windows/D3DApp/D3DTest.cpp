#include"AppFramework.hpp"
#include"D3DApp.hpp"

int WINAPI WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR pCmdLine, int nCmdShow
){
    D3DApp app(960, 640, "Window Test");
    AppFramework::Run(&app, hInstance, nCmdShow);

    return 0;
}

