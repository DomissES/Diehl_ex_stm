/*
 * mytime.h
 *
 *  Created on: Jan 22, 2024
 *      Author: domis
 */

#ifndef INC_MYTIME_H_
#define INC_MYTIME_H_

#include <stdbool.h>
#include <stdint.h>

#define GMT_REGION  1

typedef uint32_t timestamp_t;

typedef enum
{
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
}weekday_t;

typedef struct
{
    uint8_t Hour;
    uint8_t Minutes;
    uint8_t Seconds;
    uint8_t Day;
    uint8_t Month;
    uint8_t Year;
}date_t;

bool timeToDate(timestamp_t timestamp, char *date);
bool timeToWeekday(timestamp_t timestamp, weekday_t *day);
bool dateToTime(const date_t *date, timestamp_t *timestamp);
bool dayBetweenTimes(timestamp_t time_A, timestamp_t time_B, uint16_t *days);


#endif /* INC_MYTIME_H_ */
