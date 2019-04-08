#include <ctime>
#include <cstdio>
#include <string>
using namespace std;

/*
name:   timeToString
feature:时间格式转化字符串
param:  time_t, char*
value:  void
*/
void timeToString(time_t t, char* pBuf)
{
    struct tm* timeInfo;
    timeInfo = localtime(&t);
    strftime(pBuf, 20, "%Y-%m-%d %H:%M", timeInfo);
}
/*
name:   timeToString
feature:字符串转化时间格式
param:  string
value:  time_t
*/
time_t stringToTime(string str)
{
    const char* pTime = str.data();
    struct tm tm1;
    time_t time1;
    sscanf(pTime, "%d-%d-%d %d:%d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday, &tm1.tm_hour, &tm1.tm_min);
    tm1.tm_year -= 1900;
    tm1.tm_mon -= 1;
    tm1.tm_sec = 0;
    tm1.tm_isdst = -1;

    time1 = mktime(&tm1);
    return time1;
}
/* name: timeCalc
 * feat: 计算两时间有多少小时
 * param: struct tm*, struct tm*
 * value: int
 */
int timeCalc(struct tm* start, struct tm* end)
{
    return 1+(end->tm_year-start->tm_year)*8760+(end->tm_mon-start->tm_mon)*30*24+(end->tm_mday-start->tm_mday)+(end->tm_hour-start->tm_hour);
}

