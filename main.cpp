#include <iostream>
#include "menu.h"
#include "service.h"
using namespace std;

int main()
{
    cout<<"\n欢迎使用计费管理系统\n";
    init();
    bool isExit = false;
    while (!isExit) {
        showMenu();
        isExit = option();
    }
    return 0;
}
