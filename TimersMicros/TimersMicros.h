/******************************************************************
*  TimersMicros library for use on STACEE and CHRISTEE
*	The basic necessities for time keeping		
*
*		Brought to you by:
*              Zac Kilburn
******************************************************************/
#ifndef TIMERS_MICROS_H
#define TIMERS_MICROS_H
#include "Arduino.h"


class TimersMicros
{	
public:
	TimersMicros();
	TimersMicros(unsigned long l);
	void resetTimer();
	void setInterval(unsigned long l);
	void updateTimer();
	bool timerDone();

private:
	unsigned long currentTime;
	unsigned long startTime;
	unsigned long length;
};
#endif
