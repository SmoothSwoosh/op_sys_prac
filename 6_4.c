#include <stdio.h>
#include <time.h>
#include <sys/types.h>

enum
{
    SECONDS_IN_DAY = 86400,
    YEAR_OFFSET = 1900,
    THURSDAY = 4,
    SECOND_THURSDAY = 2,
    FOURTH_THURSDAY = 4
};

int
main(int argc, char *argv[])
{
    int year = 0; //input year
    time_t cur_time = 0; //current time 
    int thursdays_cnt = 0; //counter for thursdays in month
    int cur_month = 0; //current month
    struct tm date = { }; //structure to take presentation 
    
    scanf("%d", &year); //take year
    
    //set date to 1th January of input year
    date.tm_year = year - YEAR_OFFSET; //set year with offset
    date.tm_mday = 1; //set day to 1th
    date.tm_mon = 0; //set month to January
    cur_time = mktime(&date); //get time_t presentation
    localtime_r(&cur_time, &date); //get struct presentation
    cur_month = date.tm_mon;
    
    while (date.tm_year != (year - YEAR_OFFSET + 1)) {
        if (date.tm_wday == THURSDAY && (++thursdays_cnt) && !(date.tm_mday % 3 == 0) 
                && (thursdays_cnt == SECOND_THURSDAY || thursdays_cnt == FOURTH_THURSDAY)) {
            printf("%d %d\n", date.tm_mon + 1, date.tm_mday);
        }
        cur_time += SECONDS_IN_DAY;
        localtime_r(&cur_time, &date);
        if (date.tm_mon != cur_month) {
            cur_month = date.tm_mon;
            thursdays_cnt = 0;
        }
    }
    
    return 0;
}  