/*
 * Clock.cpp
 *
 *  Created on: Apr 6, 2020
 *      Author: Saadat
 */

#include "Clock.h"

#include <Wire.h>
//portMUX_TYPE  i2cMux = portMUX_INITIALIZER_UNLOCKED;
TwoWire wire(PB9, PB8);

Clock::Clock(){
}

void Clock::begin(){
	rtc.begin(&wire);
	if (rtc.lostPower()) {
		DEBUG_println("[Clock] RTC lost power, let's set the time!");
		// When time needs to be set on a new device, or after a power loss, the
		// following line sets the RTC to the date & time this sketch was compiled
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	}
}

String Clock::GetOnlyClock(){
	char myTime[] = "                    ";
	DateTime dateTime = rtc.now();
	sprintf(myTime, "%02d:%02d:%02d", dateTime.hour(), dateTime.minute(), dateTime.second());
	return String(myTime);
}

void Clock::checkAndAmendTimeRanges(MyTime &time){
	if(time.year >= 2040)
		time.year = 2020;
	if(time.month > 12)
		time.month = 1;
	if(time.day > 31)
		time.day = 1;
	if(time.hour > 23)
		time.hour = 0;
	if(time.minute > 59)
		time.minute = 0;
	if(time.second > 59)
		time.second = 0;
}

void Clock::SetDate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute){
	DateTime dateTime = DateTime(year, month, day, hour, minute, 0);
	rtc.adjust(dateTime);
}
void Clock::SetDate(MyTime time){
	checkAndAmendTimeRanges(time);
	DateTime dateTime = DateTime(time.year, time.month, time.day, time.hour, time.minute, time.second);
	rtc.adjust(dateTime);
}

void Clock::GetDate(MyTime& myTime){
	DateTime dateTime = rtc.now();
//	DEBUG_printf("Time from rtc: %02d:%02d:%02d", dateTime.hour(), dateTime.minute(), dateTime.second());
	myTime.year = dateTime.year();
	myTime.month = dateTime.month();
	myTime.day = dateTime.day();
	myTime.hour = dateTime.hour();
	myTime.minute = dateTime.minute();
	myTime.second = dateTime.second();
	checkAndAmendTimeRanges(myTime);
}
String Clock::GetDate(){
	char myTimeStr[] = "                    ";
	MyTime myTime;
	GetDate(myTime);
	checkAndAmendTimeRanges(myTime);
	sprintf(myTimeStr, "%02d-%02d-%02d %02d:%02d:%02d", myTime.year, myTime.month, myTime.day,
			myTime.hour, myTime.minute, myTime.second);
	return String(myTimeStr);
}

//----------------------------------------------------------------

