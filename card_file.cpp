#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include "json/json.h"

#include "global.h"
#include "tool.h"
#include "model.h"
#include "card_service.h"
#include "card_file.h"
using namespace std;

/*
name:   init
feature:读入数据
param:  void
value:  void
*/
void readCardFromFile()
{
    ifstream ifs;
    ifs.open(CardPath);
    assert(ifs.is_open());
    Json::Reader loader;
    Json::Value root;
    if (!loader.parse(ifs, root, false)) {
        printf("数据库为空");
        return;
    }
    int cnt = root.size()-1;
    for (int info = 0; info <= cnt; info++) {
        Card* card = new Card;
        card->username = root[info]["username"].asString();
        card->password = root[info]["password"].asString();
        card->status = root[info]["status"].asInt();
        card->totalUse = root[info]["totalUse"].asDouble();
        card->useCount = root[info]["useCount"].asInt();
        card->balance = root[info]["balance"].asDouble();
        card->isDel = root[info]["isDel"].asBool();
        string tmpTime;
        tmpTime = root[info]["timeStart"].asString();
        card->timeStart = stringToTime(tmpTime.data());
        tmpTime = root[info]["timeEnd"].asString();
        card->timeEnd = stringToTime(tmpTime.data());
        tmpTime = root[info]["timeLastUse"].asString();
        card->timeLastUse = stringToTime(tmpTime.data());
        card->next = NULL;
        if (!addCard(card)) {
            cout<<"导入数据失败"<<endl;
            return;
        }
    }
    cout<<"导入数据成功"<<endl;
    ifs.close();
}
/*
name:   init
feature:保存数据
param:  Card*
value:  void
*/
void saveToFile(Card* cards)
{
    Json::Value root;
    Json::StreamWriterBuilder builder;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    int i = 0;
    while (cards != NULL) {
        Json::Value card;
        card["username"] = cards->username;
        card["password"] = cards->password;
        card["status"] = cards->status;
        card["totalUse"] = cards->totalUse;
        card["useCount"] = cards->useCount;
        card["balance"] = cards->balance;
        card["isDel"] = cards->isDel;
        char sTime[20], eTime[20], lTime[20];
        timeToString(cards->timeStart, sTime);
        timeToString(cards->timeEnd, eTime);
        timeToString(cards->timeLastUse, lTime);
        card["timeStart"] = (string)sTime;
        card["timeEnd"] = (string)eTime;
        card["timeLastUse"] = (string)lTime;
        root[i++] = card;
        cards = cards->next;
    }
    ofstream ofl(CardPath, ios_base::out);
    if (!ofl.is_open()) {
        cout<<"文件无法打开，保存错误"<<endl;
        return;
    }
    writer->write(root, &ofl);
    ofl.close();
}