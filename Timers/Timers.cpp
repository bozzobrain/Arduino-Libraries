#include <Timers.h>

//Library constructor
Timers::Timers()
{	
  length=50;
  currentTime=millis();
}
Timers::Timers(unsigned long l)
{	
  length=l;
  currentTime=millis();
}
//External method
void Timers::resetTimer()
{
	startTime=millis();
	currentTime=startTime;
}
void Timers::setInterval(unsigned long l)
{
	length=l;	
}
void Timers::updateTimer()
{
	currentTime=millis();
}
bool Timers::timerDone()
{
     updateTimer();
	if (abs(currentTime-startTime)>length)
	{
		resetTimer();
		return true;
	}
	else return false;
}
