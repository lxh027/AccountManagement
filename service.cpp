#include <cstring>
#include <ctime>

#include <iostream>
#include <iomanip>


#include "model.h"
#include "menu.h"
#include "card_service.h"
#include "billing_service.h"
#include "service.h"
#include "card_file.h"
#include "billing_file.h"
#include "money_file.h"
#include "tool.h"

void init()
{
    readCardFromFile();
    readBillingFromFile();
}

/*
name:   add
feature:添加卡
param:  void
value:  void
*/
void add()
{
    Card* card = new Card;
    string tmpName;   //临时用户名
    string tmpPwd;    //临时密码
    struct tm* startTime;//临时开卡时间
    struct tm* endTime; //临时截止时间
    cout<<"请输入卡号（长度为3-12）：";
    cin>>tmpName;
    cin.clear();
    cin.sync();
    //判断卡号长度
    if (tmpName.length()>18 || tmpName.length()<3) {
        cout<<"输入卡号长度不符合标准！";
        return;
    }
    //查重
    if (queryCard(tmpName) != NULL) {
        cout<<"账号已存在";
        return;
    }
    //存入
    card->username = tmpName;

    cout<<"请输入密码（长度为3-8）：";
    cin>>tmpPwd;
    cin.clear();
    cin.sync();
    //判断密码长度
    if (tmpPwd.length()>=18) {
        cout<<"输入密码长度不符合标准！";
        return;
    }
    //存入
    card->password = tmpPwd;

    //开卡金额
    cout<<"请输入开卡金额（￥）：";
    cin>>card->balance;
    cin.clear();
    cin.sync();

    card->totalUse = card->balance;
    card->isDel = false;
    card->status = 0;
    card->useCount = 0;
    card->next = NULL;
    //获取时间
    card->timeEnd = card->timeStart = card->timeLastUse = time(NULL);
    //计算截止时间，每张卡有效期为一年
    startTime = localtime(&card->timeStart);
    endTime = localtime(&card->timeEnd);
    endTime->tm_year = startTime->tm_year + 1;
    card->timeEnd = mktime(endTime);
    //输出信息
    cout<<endl;
    cout<<"---------------添加的卡信息如下---------------"<<endl;
    cout<<setw(12)<<"卡号"<<setw(12)<<"密码"<<setw(12)<<"状态"<<setw(12)<<"开卡金额"<<endl;
    cout<<setw(12)<<card->username<<setw(12)<<card->password;
    cout<<setw(12)<<card->status<<setw(12)<<fixed<<setprecision(2)<<card->balance<<endl;
    //添加信息
    if (!addCard(card)) {
        cout<<endl<<"------******-----"<<"添加信息失败"<<"-----******------"<<endl;
    } else {
        cout<<endl<<"------******-----"<<"添加信息成功"<<"-----******------"<<endl;
    }
}
/*
name:   query
feature:查询卡
param:  void
value:  void
*/
void query()
{
    string qName;
    int queryType;
    Card* pCard = NULL;
    char lastTime[30];

    cout<<"请输入想要查询的卡号（长度为3-12）：";
    cin>>qName;
    cin.clear();
    cin.sync();

    cout<<"1. 精确查询 2. 模糊查询"<<endl;
    cin>>queryType;
    cin.clear();
    cin.sync();
    //查询
    if (queryType == 1) {
        //精确查询
        pCard = queryCard(qName);
    } else if (queryType == 2){
        //模糊查询
        pCard = queryCards(qName);
    } else {
        cout<<endl<<"------******-------"<<"请输入正确的编号"<<"-------******------"<<endl;
        return;
    }
    //查询失败
    if (pCard == NULL) {
        cout<<endl<<"------******-------"<<"查询失败"<<"-------******------"<<endl;
        return;
    }
    cout<<endl<<"---------******------------"<<" 卡信息如下"<<"------------******---------"<<endl;
    cout<<setw(10)<<"卡号"<<setw(10)<<"状态"<<setw(10)<<"余额";
    cout<<setw(20)<<"累计金额"<<setw(20)<<"使用次数"<<setw(20)<<"上次使用时间"<<endl;
    //将time_t转化为字符串
    timeToString(pCard->timeLastUse, lastTime);
    //输出信息
    while (pCard != NULL) {
        cout<<setw(10)<<pCard->username<<setw(10)<<pCard->status;
        cout<<setw(10)<<pCard->balance<<setw(10)<<pCard->totalUse;
        cout<<setw(10)<<pCard->useCount<<setw(20)<<lastTime<<endl;
        pCard = pCard->next;
    }
    release(pCard);
}
/*
 * name: login
 * feat: 上机
 * param: void
 * value: void
 */
void login()
{
    Card* card;
    LoginInfo* info = new LoginInfo;
    struct tm* startTime;//上机时间
    string qName;
    string password;
    char loginTime[30];

    cout<<"请输入上机卡号：";
    cin>>qName;
    cin.clear();
    cin.sync();

    card = queryCard(qName);
    if (card == NULL) {
        cout<<endl<<"------******-------"<<"卡不存在"<<"-------******------"<<endl;
        return;
    }
    if (isLogin(qName)) {
        cout<<endl<<"------******-------"<<"已登录"<<"-------******------"<<endl;
        return;
    }
    cout<<" 请输入密码：";
    cin>>password;
    cin.clear();
    cin.sync();

    if (password.compare(card->password) != 0) {
        cout<<endl<<"------******-------"<<"密码错误"<<"-------******------"<<endl;
        return;
    }
    if (card->balance <= 0) {
        cout<<endl<<"------******-------"<<"余额不足"<<"-------******------"<<endl;
        cout<<endl<<"------******-------"<<"余额:￥"<<card->balance<<"-------******------"<<endl;
        return;
    }

    info->username = qName;
    info->balance = card->balance;
    info->timeLogin = time(NULL);
    info->next = NULL;

    if (!doLogin(info)) {
        cout<<endl<<"------******-----"<<"登录失败"<<"-----******------"<<endl;
    } else {
        addBilling(info);
        updateStatus(qName);
        timeToString(info->timeLogin, loginTime);
        cout<<endl<<"------******-----"<<"登录成功"<<"-----******------"<<endl;
        cout<<endl<<"---------******------------"<<"登录信息如下"<<"------------******---------"<<endl;
        cout<<setw(10)<<"卡号"<<setw(10)<<"余额"<<setw(20)<<"登录时间"<<endl;
        cout<<setw(10)<<info->username<<setw(10)<<info->balance;
        cout<<setw(20)<<loginTime<<endl;
    }
}
/*
 * name: logout
 * feat: 登出
 * param: void
 * value: void
 */
void logout() {
    Card* card;
    Billing* bill;
    string qName;
    string password;
    struct tm* startTime;//登录时间
    struct tm* endTime; //登出时间
    int nowTime;
    char pTime[30];


    cout<<"请输入卡号:";
    cin>>qName;
    cin.clear();
    cin.sync();

    card = queryCard(qName);
    if (card == NULL) {
        cout<<endl<<"------******-------"<<"卡不存在"<<"-------******------"<<endl;
        return;
    }
    if (!isLogin(qName)) {
        cout<<endl<<"------******-------"<<"未登录"<<"-------******------"<<endl;
        return;
    }

    cout<<" 请输入密码：";
    cin>>password;
    cin.clear();
    cin.sync();

    if (password.compare(card->password) != 0) {
        cout<<endl<<"------******-------"<<"密码错误"<<"-------******------"<<endl;
        return;
    }

    bill = getBilling(qName);
    bill->timeEnd = time(NULL);
    startTime = localtime(&bill->timeStart);
    endTime = localtime(&bill->timeEnd);
    nowTime = timeCalc(startTime, endTime);
    bill->amount = nowTime*1;

    if (bill->amount > card->balance) {
        cout<<endl<<"------******-------"<<"余额不足"<<"-------******------"<<endl;
        cout<<endl<<"------******-------"<<"余额："<<card->balance<<"消费："<<bill->amount<<"-------******------"<<endl;
        return;
    } else {
        doLogout(qName);
        updateAfterUse(bill);
        card = queryCard(qName);
        cout<<endl<<"---------******------------"<<"上机信息如下"<<"------------******---------"<<endl;
        cout<<setw(10)<<"卡号"<<setw(10)<<"消费"<<setw(10)<<"余额";
        cout<<setw(20)<<"上机时间"<<setw(20)<<"下机时间"<<endl;
        cout<<setw(10)<<bill->username<<setw(10)<<bill->amount<<setw(10)<<card->balance;
        timeToString(bill->timeStart, pTime);
        cout<<setw(20)<<pTime;
        timeToString(bill->timeEnd, pTime);
        cout<<setw(20)<<pTime<<endl;
    }
}
/*
 * name: addMoney
 * feat: 充值
 * param: void
 * value: void
 */
void addMoney()
{
    string qName;
    string password;
    double money;
    Card* card;
    MoneyInfo moneyInfo;

    cout<<"请输入充值卡号：";
    cin>>qName;
    cin.clear();
    cin.sync();

    card = queryCard(qName);
    if (card == NULL) {
        cout<<endl<<"------******-------"<<"卡不存在"<<"-------******------"<<endl;
        return;
    }
    cout<<"请输入密码：";
    cin>>password;
    cin.clear();
    cin.sync();

    if (password.compare(card->password) != 0) {
        cout<<endl<<"------******-------"<<"密码错误"<<"-------******------"<<endl;
        return;
    }
    cout<<"请输入充值金额（￥）：";
    cin>>money;
    cin.clear();
    cin.sync();

    moneyInfo.username = qName;
    moneyInfo.money = money;

    if (doAddMoney(&moneyInfo)) {
        addMoneyInfo(moneyInfo, 0);
        cout<<"----*----充值信息如下----*----"<<endl;
        cout<<setw(12)<<" 卡号"<<setw(12)<<"充值金额"<<setw(12)<<"余额"<<endl;
        cout<<setw(12)<<moneyInfo.username<<setw(12)<<moneyInfo.money<<setw(12)<<fixed<<setprecision(1)<<moneyInfo.balance;
        cout<<endl<<endl;
    } else {
        cout<<"----*----充值失败----*----"<<endl;
    }
}
/*
 * name: refundMoney
 * feat: 退费
 * param: void
 * value: void
 */
void refundMoney()
{
    string qName;
    string password;
    double money;
    Card* card;
    MoneyInfo moneyInfo;

    cout<<"请输入退费卡号：";
    cin>>qName;
    cin.clear();
    cin.sync();

    card = queryCard(qName);
    if (card == NULL) {
        cout<<endl<<"------******-------"<<"卡不存在"<<"-------******------"<<endl;
        return;
    }
    cout<<"请输入密码：";
    cin>>password;
    cin.clear();
    cin.sync();

    if (password.compare(card->password) != 0) {
        cout<<endl<<"------******-------"<<"密码错误"<<"-------******------"<<endl;
        return;
    }
    cout<<"请输入退费金额（￥）：";
    cin>>money;
    cin.clear();
    cin.sync();

    moneyInfo.username = qName;
    moneyInfo.money = money;

    if (doRefundMoney(&moneyInfo)) {
        addMoneyInfo(moneyInfo, 1);
        cout<<"----*----退费信息如下----*----"<<endl;
        cout<<setw(12)<<" 卡号"<<setw(12)<<"退费金额"<<setw(12)<<"余额"<<endl;
        cout<<setw(12)<<moneyInfo.username<<setw(12)<<moneyInfo.money<<setw(12)<<fixed<<setprecision(1)<<moneyInfo.balance;
        cout<<endl<<endl;
    } else {
        cout<<"----*----退费失败----*----"<<endl;
    }
}
/*
 * name: refundMoney
 * feat: 退费
 * param: void
 * value: void
 */
void annualCard()
{
    string qName;
    string password;
    Card* card;

    cout<<"请输入注销卡号：";
    cin>>qName;
    cin.clear();
    cin.sync();

    card = queryCard(qName);
    if (card == NULL) {
        cout<<endl<<"------******-------"<<"卡不存在"<<"-------******------"<<endl;
        return;
    }
    if (card->status == 1) {
        cout<<endl<<"------******-------"<<"正在上机"<<"-------******------"<<endl;
        return;
    }
    if (card->status == 2) {
        cout<<endl<<"------******-------"<<"此卡已注销"<<"-------******------"<<endl;
        return;
    }
    cout<<"请输入密码：";
    cin>>password;
    cin.clear();
    cin.sync();

    if (password.compare(card->password) != 0) {
        cout<<endl<<"------******-------"<<"密码错误"<<"-------******------"<<endl;
        return;
    }

    if (doAnnualCard(qName)) {
        cout<<"----*----注销信息如下----*----"<<endl;
        cout<<setw(12)<<"卡号"<<setw(12)<<"退款金额"<<endl;
        cout<<setw(12)<<qName<<setw(12)<<card->balance;
        cout<<endl<<endl;
    } else {
        cout<<"----*----退费失败----*----"<<endl;
    }
}
/*
 * name: save
 * feat: 保存信息
 * param: void
 * value: void
 */
void save()
{
    saveBill();
    saveCard();
    saveMoneyInfo();
    system("pause");
}