/******************************************************************
*  Timers library for use on STACEE and CHRISTEE
*	The basic necessities for time keeping		
*
*		Brought to you by:
*              Zac Kilburn
******************************************************************/
#ifndef Timers_h
#define Timers_h
#include "Arduino.h"


class Timers
{	
public:
	Timers();
	Timers(unsigned long l);
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
