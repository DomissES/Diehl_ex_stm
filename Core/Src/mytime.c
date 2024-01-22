/*
 * mytime.c
 *
 *  Created on: Jan 22, 2024
 *      Author: domis
 */



#include "mytime.h"
#include <stdio.h>
#include <stdlib.h>

#define ONE_HOUR    3600
#define ONE_DAY     86400

static const uint8_t dayInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const char *const monthNames[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

//====================== PRIVATE UFNCTIONS ========================

//Calculate how much years is in *timePtr
//substract years*seconds from *timePtr
static uint8_t timeToYear(timestamp_t *timePtr)
{
    const uint32_t oneYearSeconds = 365 * ONE_DAY;
    uint16_t year = 1970;

    while(*timePtr > (365 * ONE_DAY)) //is enough seconds for one year
    {
        uint32_t secondsToSubstract = oneYearSeconds;

        if(((year % 4) == 0) && !((year % 100) == 0)) //divided by 4 and no divided by 100
        {
            secondsToSubstract += ONE_DAY;
        }
        if((year % 400) == 0) //divided by 400
        {
            secondsToSubstract += ONE_DAY;
        }

        *timePtr -= secondsToSubstract;
        year++;
    }

    return year - 1970;
}

//calculate how much months in in *timePtr
//substract months*seconds from *timePtr
static uint8_t timeToMonth(timestamp_t *timePtr)
{
    uint8_t month = 0;
    while(*timePtr > (dayInMonth[month] * ONE_DAY))
    {
        uint32_t secondsToSubstract = dayInMonth[month] * ONE_DAY;
        month++;
        *timePtr -= secondsToSubstract;
    }

    return month;
}



//============== PUBLIC FUNCTIONS ==========================

bool timeToDate(timestamp_t timestamp, char *dateStr)
{
    date_t date;
    int isOk;

    timestamp = timestamp + (GMT_REGION * ONE_HOUR); //correction for GMT

    if(dateStr == NULL) return false;

    date.Year = timeToYear(&timestamp);
    date.Month = timeToMonth(&timestamp);

   //calculate day
    uint8_t day = timestamp / ONE_DAY;
    date.Day = day + 1 ; //days are starting from 1st
    timestamp -= day * ONE_DAY;

    //calculate hour
    uint8_t hour = timestamp / ONE_HOUR;
    date.Hour = hour;
    timestamp -= hour * ONE_HOUR;

    //calculate minute
    uint8_t minute = timestamp / 60;
    date.Minutes = minute;
    timestamp -= minute * 60;

    //seconds
    date.Seconds = timestamp;


    isOk = sprintf(dateStr, "%02d %s %4d %02d:%02d:%02d", date.Day , monthNames[date.Month], date.Year + 1970, date.Hour, date.Minutes, date.Seconds); //we are starting from 1st day

    if(isOk == 0) return false;

    return true;
}

bool timeToWeekday(timestamp_t timestamp, weekday_t *day)
{
	uint32_t current = 3; // 1st Jan 1970 was thursday

	timestamp = timestamp + (GMT_REGION * ONE_HOUR); //correction for GMT

	uint32_t days = timestamp / ONE_DAY;
	current = (current + days)%7;

	if(day == NULL) return false;
	*day = (weekday_t)current;

    return true;
}

bool dateToTime(const date_t *date, timestamp_t *timestamp)
{
    timestamp_t seconds = 0;

    if((date == NULL) || (timestamp == NULL)) return false;

    //year
    uint32_t secondsInYear = 365 * ONE_DAY;

    for(uint16_t i = 1970; i < (date->Year + 1970); i++)
    {
        if(((i % 4) == 0) && !((i % 100) == 0)) //divided by 4 and no divided by 100
        {
            seconds += ONE_DAY;
        }
        if((i % 400) == 0) //divided by 400
        {
            seconds += ONE_DAY;
        }
        seconds += secondsInYear;
    }

    //months
    for(uint8_t i = 0; i < date->Month; i++)
    {
        seconds += dayInMonth[i] * ONE_DAY;
    }

    //days
    seconds += date->Day * ONE_DAY - ONE_DAY; //date.day is counted from 1st
    //hours
    seconds += date->Hour * ONE_HOUR;
    //minutes
    seconds += date->Minutes * 60;
    //seconds
    seconds += date->Seconds;
    *timestamp = seconds;

    return true;
}

bool dayBetweenTimes(timestamp_t time_A, timestamp_t time_B, uint16_t *days)
{
	if(days == NULL) return false;

	uint32_t delta = abs((int)(time_A - time_B));

	*days = delta / ONE_DAY;
    return true;
}
