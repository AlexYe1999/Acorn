#include"../AppFramework.hpp"
#include"GLApp.hpp"

int main(int argc, char* argv[]){
    GLApp App(640, 480, "Hello OpenGL !!!");
    return AppFramework::Run(&App, argc, argv);
}