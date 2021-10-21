#include"TextureApp.hpp"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR pCmdLine,
    int nCmdShow
){

    TextureApp& app = TextureApp::GetInstance();
    try{
        app.InitApp(hInstance, nCmdShow,
            700, 700, "LandandWaves", "Land and Waves");
    }
    catch(...){
        return 0;
    }

    app.RunApp();

    return 0;
}