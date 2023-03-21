/*
 * Clock.h
 *
 *  Created on: Apr 6, 2020
 *      Author: Saadat
 */

#ifndef CLOCK_CLOCK_H_
#define CLOCK_CLOCK_H_


//#include "DS3231.h"
#include "RTClib.h"
#include "Time.h"

extern HardwareSerial Serial2;
template<typename... Args>
void DEBUG_println(Args... args){
	Serial2.println(args...);
}
template<typename... Args>
void DEBUG_printf(Args... args){
	Serial2.printf(args...);
}

class Clock{
public:
	Clock();
	void 		begin();

	void 		SetDate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);
	void 		SetDate(MyTime time);
	void 		GetDate(MyTime& time);
	String 		GetDate();
	String 		GetOnlyClock();

private:
	uint16_t	_year 	= 0;
	uint16_t 		_month 	= 0;
	uint16_t 		_day 	= 0;
	uint16_t 		_hour 	= 0;
	uint16_t 		_minute = 0;
	uint16_t		_second = 0;

	uint16_t	_lastMillis = 0;

	RTC_DS3231 rtc;

	bool	Century = false;
	bool	h12 = false;
	bool	PM = false;

	void	checkAndAmendTimeRanges(MyTime &myTime);

};





#endif /* CLOCK_CLOCK_H_ */
