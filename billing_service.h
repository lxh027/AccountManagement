#pragma once

#include <iostream>
#include <cstring>

#include "model.h"
using namespace std;

bool isLogin(string);
bool doLogin(LoginInfo*);
void addBilling(LoginInfo*);
bool readBilling(Billing*);
Billing* getBilling(string);
void doLogout(string);
void release();
void saveBill();