#pragma once

#include <ctime>
#include <string>
using namespace std;

void timeToString(time_t, char*);
time_t stringToTime(string);
int timeCalc(struct tm*, struct tm*);
