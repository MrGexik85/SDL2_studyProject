#include <iostream>
#include "Application.h"

#define main SDL_main

using namespace std;


int main(int argc, char *argv[])
{
    Application app;
    return app.Exec();
}
