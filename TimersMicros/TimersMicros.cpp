#include <TimersMicros.h>

//Library constructor
TimersMicros::TimersMicros()
{	
  length=50;
  currentTime=micros();
}
TimersMicros::TimersMicros(unsigned long l)
{	
  length=l;
  currentTime=micros();
}
//External method
void TimersMicros::resetTimer()
{
	startTime=micros();
	currentTime=startTime;
}
void TimersMicros::setInterval(unsigned long l)
{
	length=l;	
}
void TimersMicros::updateTimer()
{
	currentTime=micros();
}
bool TimersMicros::timerDone()
{
     updateTimer();
	if (abs(currentTime-startTime)>length)
	{
		resetTimer();
		return true;
	}
	else return false;
}
