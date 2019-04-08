#pragma once

#include <string>
#include <ctime>
using namespace std;

typedef struct Card
{
    string username;  //用户名
    string password;  //密码
    int status;         //卡状态（0-未上机；1-上机中；2-已注销；3-失效）
    time_t timeStart;   //开卡时间
    time_t timeEnd;     //截止时间
    time_t timeLastUse; //最后使用时间
    double totalUse;    //累计使用金额
    int useCount;       //使用次数
    double balance; //余额
    bool isDel;         //删除标识（0-未删除；1-已删除）
    Card* next;
} Card;

typedef struct LoginInfo
{
    string username;    //用户名
    time_t timeLogin;   //上机时间
    double balance;      //上机时余额
    LoginInfo* next;
} LoginInfo;

typedef struct Billing
{
    string username;    //用户名
    time_t timeStart;
    time_t timeEnd;
    double amount;
    int status;     //消费状态（0-未结算 1-已结算）
    bool isDel;
    Billing* next;
} Billing;

typedef struct MoneyInfo
{
    string username;
    double money;
    double balance;
} MoneyInfo;

typedef struct Money{
    MoneyInfo moneyInfo;
    string type;
    Money* next;
} Money;
