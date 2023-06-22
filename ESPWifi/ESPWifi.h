/******************************************************************
*		Brought to you by:
*              Zac Kilburn
******************************************************************/
#ifndef ESP_WIFI_H
#define ESP_WIFI_H
#include "Arduino.h"


class ESPWifi
{	
public:
	ESPWifi();
	//ESPWifi(long l);
	void setupWifi(HardwareSerial *theSerial);
	int checkForWifiMessage(void);
	int getLastCommand(void);
	void setLastCommand(int c);

private:
	HardwareSerial *wifiSerial;
	int lastCommand;
};
#endif
