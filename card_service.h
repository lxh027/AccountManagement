#pragma once

#include "model.h"

bool addCard(Card*);
Card* queryCard(string);
Card* queryCards(string);
void updateAfterUse(Billing*);
void updateStatus(string);
void init();
void saveCard();
void release(Card*);
bool doAddMoney(MoneyInfo*);
bool doRefundMoney(MoneyInfo*);
bool doAnnualCard(string);

