#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include "json/json.h"

#include "global.h"
#include "tool.h"
#include "model.h"
#include "money_file.h"

Money* moneyHead = NULL;
Money* tail = moneyHead;

void addMoneyInfo(MoneyInfo moneyInfo, int type)
{
    Money* pMoney = new Money;
    pMoney->moneyInfo = moneyInfo;
    if (type == 0) {
        pMoney->type = "add";
    } else {
        pMoney->type = "refund";
    }
    pMoney->next = NULL;
    if (moneyHead == NULL) {
        tail = moneyHead = pMoney;
    } else {
        tail->next = pMoney;
        tail = pMoney;
    }
}

void saveMoneyInfo()
{
    Json::Value root;
    Json::StreamWriterBuilder builder;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    int i = 0;
    while (moneyHead != NULL) {
        Json::Value money;
        money["username"] = moneyHead->moneyInfo.username;
        money["balance"] = moneyHead->moneyInfo.balance;
        money["money"] = moneyHead->moneyInfo.money;
        money["type"] = moneyHead->type;
        root[i++] = money;
        Money* p = moneyHead;
        moneyHead = moneyHead->next;
        delete(p);
    }
    ofstream ofl(MoneyPath, ios_base::out);
    if (!ofl.is_open()) {
        cout<<"文件无法打开，保存错误"<<endl;
        return;
    }
    writer->write(root, &ofl);
    ofl.close();
}


