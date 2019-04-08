#include <iostream>
#include <cstring>

#include "model.h"
#include "billing_file.h"
#include "billing_service.h"
#include "card_service.h"

using namespace std;

LoginInfo* loginInfo;
LoginInfo* headLogin;
int cntLogin;
Billing* billing;
Billing* headBilling;
int cntBilling;

/*
 * name: isLogin
 * feat: 判断是否已登录
 * param: string
 * value: bool
 */
bool isLogin(string name) {
    LoginInfo* pInfo = headLogin;
    while (pInfo != NULL) {
        if (name.compare(pInfo->username) == 0) {
            return true;
        }
        pInfo = pInfo->next;
    }
    return false;
}
/*
 * name: doLogin
 * feat: 保存登录信息
 * param: LoginInfo*
 * value: bool
 */
bool doLogin(LoginInfo* info) {
    if (cntLogin == 0) {
        headLogin = loginInfo = info;
    } else {
        loginInfo->next = info;
        loginInfo = loginInfo->next;
    }
    cntLogin++;
    return true;
}
/*
 * name: addBilling
 * feat: 增加消费记录
 * param: LoginInfo*
 * value: void
 */
void addBilling(LoginInfo* info) {
    Billing* bill = new Billing;
    bill->username = info->username;
    bill->timeStart = info->timeLogin;
    bill->timeEnd = info->timeLogin;
    bill->amount = 0;
    bill->isDel = false;
    bill->status = 0;
    bill->next = NULL;
    if (cntBilling == 0) {
        billing = headBilling = bill;
    } else {
        billing->next = bill;
        billing = billing->next;
    }
    cntBilling++;
    return;
}
bool readBilling(Billing* bill) {
    if (cntBilling == 0) {
        billing = headBilling = bill;
    } else {
        billing->next = bill;
        billing = billing->next;
    }
    cntBilling++;
    LoginInfo* info = new LoginInfo;
    info->username = bill->username;
    Card* card = queryCard(info->username);
    info->balance = card->balance;
    info->timeLogin = bill->timeStart;
    info->next = NULL;
    doLogin(info);
    return true;
}
/*
 * name: doLogout
 * feat: 登出
 * param: Card*
 * value: void
 */
void doLogout(string qName)
{
    LoginInfo* info = headLogin;
    if (qName.compare(info->username) == 0) {
        LoginInfo* q = info;
        headLogin = info->next;
        delete(q);
        cntLogin--;
        cout<<cntLogin;
        return;
    }
    while (info->next != NULL) {
        if (qName.compare(info->next->username) == 0) {
            LoginInfo* q = info->next;
            info->next = info->next->next;
            delete(q);
            cntLogin--;
        }
        info = info->next;
    }
    headLogin = NULL;
}

/*
 * name: getBilling
 * feat: 获取订单
 * param: string
 * value: Billing*
 */
Billing* getBilling(string name)
{
    Billing* pBill = headBilling;
    while (pBill != NULL) {
        if (pBill->status != 1 && name.compare(pBill->username) == 0) {
            return pBill;
        }
        pBill = pBill->next;
    }
    return NULL;
}

void saveBill()
{
    cout<<"正在保存消费信息......"<<endl;
    saveToFile(headBilling);
    release();
}

/*
 * name: release
 * feat: 释放空间
 * param: void
 * value: void
 */
void release()
{
    while (headLogin != NULL) {
        LoginInfo* p = headLogin;
        headLogin = headLogin->next;
        delete(p);
    }
    while (headBilling != NULL) {
        Billing* p = headBilling;
        headBilling = headBilling->next;
        delete(p);
    }
}

