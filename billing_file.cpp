#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include "json/json.h"

#include "global.h"
#include "tool.h"
#include "model.h"
#include "billing_service.h"
#include "billing_file.h"
using namespace std;

/*
 * name: saveToFile
 * feat: 保存信息
 * param: Billing*
 * value: void
 */
void saveToFile(Billing* bills)
{
    loadUnreadRecord();
    Json::Value root;
    Json::StreamWriterBuilder builder;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    int i = 0;
    while (bills != NULL) {
        Json::Value bill;
        bill["username"] = bills->username;
        bill["amount"] = (int)(bills->amount*10)/10.0;
        char sTime[20], eTime[20];
        timeToString(bills->timeStart, sTime);
        timeToString(bills->timeEnd, eTime);
        bill["timeStart"] = (string)sTime;
        bill["timeEnd"] = (string)eTime;
        bill["status"] = bills->status;
        bill["isDel"] = bills->isDel;
        root[i++] = bill;
        bills = bills->next;
    }
    ofstream ofl(BillPath, ios_base::out);
    if (!ofl.is_open()) {
        cout<<"文件无法打开，保存错误"<<endl;
        return;
    }
    writer->write(root, &ofl);
    ofl.close();
}
/*
 * name: readFromFile
 * feat: 读取记录
 * param: void
 * value: void
 */
void readBillingFromFile()
{
    ifstream ifs;
    ifs.open(BillPath);
    assert(ifs.is_open());
    Json::Reader loader;
    Json::Value root;
    if (!loader.parse(ifs, root, false)) {
        printf("记录数据库为空");
        return;
    }
    int cnt = root.size()-1;
    for (int info = 0; info <= cnt; info++) {
        Billing* bill = new Billing;
        bill->username = root[info]["username"].asString();
        bill->amount = root[info]["amount"].asDouble();
        bill->status = root[info]["status"].asInt();
        bill->isDel = root[info]["isDel"].asBool();
        if (bill->status == 1) continue;
        string tmpTime;
        tmpTime = root[info]["timeStart"].asString();
        bill->timeStart = stringToTime(tmpTime.data());
        tmpTime = root[info]["timeEnd"].asString();
        bill->timeEnd = stringToTime(tmpTime.data());
        bill->next = NULL;
        if (!readBilling(bill)) {
            cout<<"导入数据失败"<<endl;
            return;
        }
    }
    cout<<"导入数据成功"<<endl;
    ifs.close();
}
/* name: loadUnreadRecord
 * feat: 存入前检查上次保存的记录
 * param: void
 * value: void
 */
void loadUnreadRecord()
{
    ifstream ifs;
    ifs.open(BillPath);
    assert(ifs.is_open());
    Json::Reader loader;
    Json::Value root;
    loader.parse(ifs, root, false);
    int cnt = root.size()-1;
    for (int info = 0; info <= cnt; info++) {
        Billing* bill = new Billing;
        bill->username = root[info]["username"].asString();
        bill->amount = root[info]["amount"].asDouble();
        bill->status = root[info]["status"].asInt();
        bill->isDel = root[info]["isDel"].asBool();
        if (bill->status != 1 || bill->isDel) continue;
        bill->isDel = true;
        string tmpTime;
        tmpTime = root[info]["timeStart"].asString();
        bill->timeStart = stringToTime(tmpTime.data());
        tmpTime = root[info]["timeEnd"].asString();
        bill->timeEnd = stringToTime(tmpTime.data());
        bill->next = NULL;
        readBilling(bill);
    }
    ifs.close();
}