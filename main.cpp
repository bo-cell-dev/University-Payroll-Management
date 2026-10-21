#include "Menus.h"
#include <iostream> 
#ifdef _WIN32
#include <Windows.h>
#endif

int main() {
#ifdef _WIN32
    std::locale::global(std::locale(".UTF8"));
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#else
    std::locale::global(std::locale(""));
#endif
    entranceMenu();
    return 0;
}

