#include <cstring>
#include <iostream>
#include "model.h"
#include "card_file.h"
#include "card_service.h"
#include "billing_service.h"

using namespace std;

Card* cards;
Card* head;
int cnt=0;

/*
name:   addCard
feature:加入卡
param:  Card
value:  int
*/
bool addCard(Card* card)
{
    /*cards->username = card->username;
    cards->password = card->password;
    cards->status = card->status;
    cards->timeEnd = card->timeEnd;
    cards->timeStart = card->timeStart;
    cards->timeLastUse = card->timeLastUse;
    cards->useCount = card->useCount;
    cards->isDel = card->isDel;
    cards->totalUse = card->totalUse;
    cards->balance = card->balance;*/
    if (cnt == 0) {
        cards = head = card;
    } else {
        cards->next = card;
        cards = cards->next;
    }
    cnt++;
    return true;
}
/*
 * name: queryCard
 * feat: 实现精确查询
 * param: string
 * value Card*
 */
Card* queryCard(string pName)
{
    Card *pCard = head;
    while (pCard != NULL) {
        string str_tmp = pCard->username;
        if (str_tmp.compare(pName) == 0) {
            if (pCard->isDel == 1) {
                continue;
            }
            Card *ansCard = new Card;
            *ansCard = *pCard;
            ansCard->next = NULL;
            return ansCard;
        }
        pCard = pCard->next;
    }
    return NULL;
}
/*
 * name: queryCards
 * feat: 实现模糊查询
 * param: string
 * value: Card*
 */
Card* queryCards(string pName)
{
    Card *ansCards = NULL;
    Card *pCard = head;
    while (pCard != NULL) {
        string str_tmp = pCard->username;
        if (str_tmp.find(pName) != -1) {
            if (pCard->isDel == 1) {
                continue;
            }
            Card *p = new Card;
            *p = *pCard;
            p->next = ansCards;
            ansCards = p;
        }
        pCard = pCard->next;
    }
    return ansCards;
}
/*
 * name: updateAfterUse
 * feat: 下机后更新数据库
 * param: Billing*
 * value: void
 */
void updateAfterUse(Billing* bill)
{
    Card *pCard = head;
    while (pCard != NULL) {
        string str_tmp = pCard->username;
        if (str_tmp.compare(bill->username) == 0) {
            pCard->timeLastUse = bill->timeStart;
            pCard->useCount++;
            pCard->status = 0;
            pCard->totalUse += bill->amount;
            pCard->balance  -= bill->amount;
            bill->status = 1;
            return;
        }
        pCard = pCard->next;
    }
}
/*
 * name:updateStatus
 * feat:更新状态
 * param：string
 * value: void
 */
void updateStatus(string name)
{
    Card *pCard = head;
    while (pCard != NULL) {
        string str_tmp = pCard->username;
        if (str_tmp.compare(name) == 0) {
            pCard->status = 1;
            return;
        }
        pCard = pCard->next;
    }
}
/*
 * name: doAddMoney
 * feat: 充值
 * param: MoneyInfo*
 * value: bool
 */
bool doAddMoney(MoneyInfo* moneyInfo)
{
    Card *pCard = head;
    while (pCard != NULL) {
        string str_tmp = pCard->username;
        if (str_tmp.compare(moneyInfo->username) == 0) {
            if (pCard->status != 1 && pCard->status != 0) {
                cout<<"----*----卡未被使用----*-----"<<endl;
                return false;
            }
            pCard->balance += moneyInfo->money;
            moneyInfo->balance = pCard->balance;
            return true;
        }
        pCard = pCard->next;
    }
    return false;
}

bool doRefundMoney(MoneyInfo* moneyInfo)
{
    Card *pCard = head;
    while (pCard != NULL) {
        string str_tmp = pCard->username;
        if (str_tmp.compare(moneyInfo->username) == 0) {
            if (pCard->status != 1 && pCard->status != 0) {
                cout<<"----*----卡未被使用----*-----"<<endl;
                return false;
            }
            if (pCard->balance<moneyInfo->money) {
                cout<<"----*----余额不足----*-----"<<endl;
                return false;
            }
            pCard->balance -= moneyInfo->money;
            moneyInfo->balance = pCard->balance;
            return true;
        }
        pCard = pCard->next;
    }
    return false;
}

bool doAnnualCard(string name)
{
    Card *pCard = head;
    while (pCard != NULL) {
        string str_tmp = pCard->username;
        if (str_tmp.compare(name) == 0) {
            pCard->status = 2;
            pCard->isDel = 1;
            pCard->balance = 0;
            pCard->timeLastUse = time(NULL);
            return true;
        }
        pCard = pCard->next;
    }
    return false;
}

/*
 * name: init
 * feat: 初始化数据
 * param: void
 * value: void
 */
void initCard()
{
    cout<<"正在完成初始化......\n";
    readCardFromFile();
}
/*
 * name: save
 * feat: 保存到文件
 * param: void
 * value: void
 */
void saveCard()
{
    cout<<"正在保存卡信息......\n";
    saveToFile(head);
    release(head);
}
/* name: release
 * feat: 释放内存
 * param:Card*
 * value: void
 */
void release(Card* pCard)
{
    while (pCard != NULL) {
        Card *tmp = pCard;
        pCard = pCard->next;
        delete(tmp);
    }
}



