#include"AppFramework.hpp"
#include"GLApp.hpp"

int WINAPI WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR pCmdLine, int nCmdShow
){

    Acorn::Log::Init();
    //Acorn::Log::GetCoreLogger()->warn("Initialized Log!");
    //Acorn::Log::GetClientLogger()->info("Hello!");

    GLApp app(960, 640, "Hello OpenGL");
    AppFramework::Run(&app, hInstance, nCmdShow);

    return 0;
}

