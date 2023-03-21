/*
 * Clock.h
 *
 *  Created on: Apr 6, 2020
 *      Author: Saadat
 */

#ifndef CLOCK_TIME_H_
#define CLOCK_TIME_H_

#include <stdint.h>
#include <Arduino.h>

typedef struct{
	uint16_t 		hour;
	uint16_t 		minute;
	uint16_t 		second;

	uint16_t		year;
	uint16_t		month;
	uint16_t		day;
}__attribute__((__packed__)) TimeStruct;

class MyTime{
public:
	uint16_t 		hour 		= 0;
	uint16_t 		minute		= 0;
	uint16_t 		second		= 0;

	uint16_t		year 		= 1;
	uint16_t		month		= 1;
	uint16_t		day			= 1;

	MyTime operator + (int seconds){
		hour += (((second + seconds)+(minute * 60)) / 3600);
		day += (hour/24);
		if(month <= 6){
			if(day > 31){
				day %= 31;
				month ++;
			}
		}
		else if(month <= 11){
			if(day > 30){
				day %= 30;
				month ++;
			}
		}
		else{
			if(year%4 == 3){
				if(day > 30){
					day %= 30;
					month ++;
				}
			}
			else{
				if(day > 29){
					day %= 29;
					month ++;
				}
			}
		}
		if(month > 12){
			month = 1;
			year ++;
		}
		hour %= 24;
		minute += ((second + seconds) / 60);
		minute %= 60;
		second += seconds;
		second %= 60;

		MyTime ret;
		ret.hour = hour;
		ret.minute = minute;
		ret.second = second;
		ret.year = year;
		ret.month = month;
		ret.day = day;

		return ret;
	}

	uint64_t getSeconds(MyTime time){
		//TODO this value is not exact, but it will do the '-' operation correctly
		uint64_t 	seconds = time.second + (time.minute*60) + (time.hour * 3600) + (time.day * 24 * 3600) + (time.month * 31 * 24 * 3600) + (time.year * 12 * 31 * 24 * 3600);
		return seconds;
	}

	MyTime fromTimeStruct(TimeStruct timeStruct){
		second = timeStruct.second;
		minute = timeStruct.minute;
		hour = timeStruct.hour;
		day = timeStruct.day;
		month = timeStruct.month;
		year = timeStruct.year;
		MyTime myTime = *this;
		return myTime;
	}

	// Get the difference of time in seconds (only considered seconds, minutes and hours)
	int32_t operator - (MyTime time){
		MyTime thisTime;
		thisTime.year = year;
		thisTime.month = month;
		thisTime.day = day;
		thisTime.hour = hour;
		thisTime.minute = minute;
		thisTime.second = second;
		int32_t diff = 0;
//		if(time.hour == 0 and thisTime.hour == 23)
//			time.hour = 24;
//		if(time.hour == 23 and thisTime.hour == 0)
//			thisTime.hour = 24;
		diff = getSeconds(thisTime) - getSeconds(time);

		return diff;
	}

	void operator = (MyTime time){
		hour = time.hour;
		minute = time.minute;
		second = time.second;
		year = time.year;
		month = time.month;
		day = time.day;
	}

	String toString() {
	    char s[25];// = new char[25];
	    sprintf(s,
	             "%04d-%02d-%02d %02d:%02d:%02d",
	              this->year, this->month, this->day, this->hour, this->minute, this->second);
	    return String(s);
	}

	String onlyTimeToString() {
	    char s[25];// = new char[25];
	    sprintf(s,
	             "%02d:%02d:%02d",
	              this->hour, this->minute, this->second);
	    return String(s);
	}

	uint32_t getTotalMinutes(){
		return ((uint32_t) (this->hour * 60) + this->minute);
	}

};



#endif /* CLOCK_CLOCK_H_ */
