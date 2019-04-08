#include <cstring>
#include <ctime>

#include <iostream>
#include <iomanip>

#include "service.h"
#include "card_service.h"

using namespace std;
/*
name:   showMenu
feature:输出菜单
param:  void
value:  void   
*/
void showMenu()
{
    cout<<"\n--------菜单--------\n";
    cout<<"1.\t添加卡\n";
    cout<<"2.\t查询卡\n";
    cout<<"3.\t上机\n";
    cout<<"4.\t下机\n";
    cout<<"5.\t充值\n";
    cout<<"6.\t退费\n";
    cout<<"7.\t查询统计\n";
    cout<<"8.\t注销卡\n";
    cout<<"0.\t退出\n";
    cout<<"请选择菜单编号：";
}
/*
name:   option
feature:选择菜单
param:  void
value： bool   
*/
bool option()
{
    int opSelector;
    cin.clear();
    cin>>opSelector;
    switch (opSelector)
    {
        case 1: cout<<"--------添加卡--------\n\n"; add(); break;
        case 2: cout<<"--------查询卡--------\n\n"; query(); break;
        case 3: cout<<"--------上机--------\n\n"; login(); break;
        case 4: cout<<"--------下机--------\n\n"; logout(); break;
        case 5: cout<<"--------充值--------\n\n"; addMoney(); break;
        case 6: cout<<"--------退费--------\n\n"; refundMoney(); break;
        case 7: cout<<"--------查询统计--------\n\n"; break;
        case 8: cout<<"--------注销卡--------\n\n"; annualCard(); break;
        case 0: cout<<"--------退出--------\n\n"; save(); break;
        default: cout<<"编号不存在！请重新选择！\n\n"; break;
    }
    return !opSelector;
}

