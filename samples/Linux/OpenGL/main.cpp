#include"../AppFramework.hpp"
#include"GLApp.hpp"

int main(int argc, char* argv[]){
    GLApp App(960, 480, "Hello game engine !!!");
    return AppFramework::Run(&App, argc, argv);
}