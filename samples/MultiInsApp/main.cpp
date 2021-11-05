#include"MultiInsApp.hpp"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR pCmdLine,
    int nCmdShow
){

    MultiInsApp& app = MultiInsApp::GetInstance();
    try{
        app.InitApp(hInstance, nCmdShow,
            700, 700, "MultiInsApp", "MultiInsApp");
    }
    catch(...){
        return 0;
    }

    app.RunApp();

    return 0;
}