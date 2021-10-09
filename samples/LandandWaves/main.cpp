#include<Windows.h>

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR pCmdLine,
    int nCmdShow
){

    Win32App *app = Win32App::GetInstance();
    
    try{
        app->InitApp(hInstance, nCmdShow, 500, 500);
    }
    catch(...){
        return 0;
    }

    app->RunApp();

    return 0;
}