/*
 * Project_RTC_LCD.c
 *
 *  Created on: 02-Jun-2026
 *      Author: Subarno Pal
 */

#include <stdio.h>
#include "ds1307.h"

char *get_day_of_week(uint8_t i);
char *time_to_string(RTC_time_t *rtc_time);
char *date_to_string(RTC_date_t *rtc_date);
void number_to_string(uint8_t num, char *buff);

int main(void)
{
	RTC_time_t current_time;
	RTC_date_t current_date;

	printf("RTC test\n");

	if(ds1307_init())
	{
		printf("RTC init has failed\n");
		while(1);
	}

	current_date.date  = 2;
	current_date.day   = TUESDAY;
	current_date.month = 6;
	current_date.year  = 26;

	current_time.hours       = 10;
	current_time.minutes     = 15;
	current_time.seconds     = 41;
	current_time.time_format = TIME_FORMAT_12HR_PM;

	ds1307_set_current_date(&current_date);
	ds1307_set_current_time(&current_time);

	ds1307_get_current_date(&current_date);
	ds1307_get_current_time(&current_time);

	char *am_pm;

	if(current_time.time_format != TIME_FORMAT_24HR)
	{
		am_pm = (current_time.time_format == TIME_FORMAT_12HR_PM) ? "PM" : "AM";

		printf("Current Time : %s %s\n",
				time_to_string(&current_time),
				am_pm);
	}
	else
	{
		printf("Current Time : %s\n",
				time_to_string(&current_time));
	}

	printf("Current Date : %s <%s>\n",
			date_to_string(&current_date),
			get_day_of_week(current_date.day));

	return 0;
}

char *get_day_of_week(uint8_t i)
{
	static char *days[] =
	{
		"Sunday",
		"Monday",
		"Tuesday",
		"Wednesday",
		"Thursday",
		"Friday",
		"Saturday"
	};

	return days[i - 1];
}

/*
 * Returns time in format:
 * hh:mm:ss
 */
char *time_to_string(RTC_time_t *rtc_time)
{
	static char buff[9];

	buff[2] = ':';
	buff[5] = ':';

	number_to_string(rtc_time->hours,   &buff[0]);
	number_to_string(rtc_time->minutes, &buff[3]);
	number_to_string(rtc_time->seconds, &buff[6]);

	buff[8] = '\0';

	return buff;
}

void number_to_string(uint8_t num, char *buff)
{
	if(num < 10)
	{
		buff[0] = '0';
		buff[1] = num + '0';
	}
	else
	{
		buff[0] = (num / 10) + '0';
		buff[1] = (num % 10) + '0';
	}
}

/*
 * Returns date in format:
 * dd/mm/yy
 */
char *date_to_string(RTC_date_t *rtc_date)
{
	static char buff[9];

	buff[2] = '/';
	buff[5] = '/';

	number_to_string(rtc_date->date,  &buff[0]);
	number_to_string(rtc_date->month, &buff[3]);
	number_to_string(rtc_date->year,  &buff[6]);

	buff[8] = '\0';

	return buff;
}
