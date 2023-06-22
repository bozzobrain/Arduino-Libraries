#include <Arduino.h>
#include <ESPWifi.h>

ESPWifi::ESPWifi()
{
}

void ESPWifi::setupWifi(HardwareSerial *theSerial)
{
	lastCommand=-1;
	wifiSerial=theSerial;
  //Serial.println("AT\r\n");
  //Serial.println("AT+CIPMUX=1\r\n");
  //Serial.println("AT+CIPSERVER=1,80\r\n");
  wifiSerial->println("AT\r\n");
  delay(1000);
  while(wifiSerial->available())
  {
    Serial.print((char)wifiSerial->read());
  }
  wifiSerial->println("AT+CIPMUX=1\r\n");
  delay(1000);
  while(wifiSerial->available())
  {
    Serial.print((char)wifiSerial->read());
  }
  wifiSerial->println("AT+CIPSERVER=1,80\r\n");
  delay(1000);
  while(wifiSerial->available())
  {
    Serial.print((char)wifiSerial->read());
  }
}


int ESPWifi::checkForWifiMessage(void)
{
  int sizeOfMessage=0;
  char popped;
  if(wifiSerial->available()>9)
  while(wifiSerial->available())
  {
    popped = wifiSerial->read();
    if(popped == '+')
    {        
      popped = wifiSerial->read();   
      if(popped == 'I')
      {        
        popped = wifiSerial->read();     
        if(popped  == 'P')
        {
          popped = wifiSerial->read();     
          if(popped  == 'D')
          {
            popped = wifiSerial->read();     
            if(popped  == ',')
           {
              popped = wifiSerial->read();    
              if(popped  == '0')
             {
                popped = wifiSerial->read();     
                if(popped  == ',')
                {
                  popped = wifiSerial->read();     
                  if(popped  == '1')
                  {
                    popped = wifiSerial->read();     
                    if(popped  == ':')
                    {
                      popped = wifiSerial->read();
                      switch(popped)
                      {   
					    case '-':
                          Serial.println("message -1");
						  lastCommand=-1;
						  return -1;
                        break;
                        case '0':
                          Serial.println("message 0");
						  lastCommand=0;
						  return 0;
                        break;
                        case '1':
                          Serial.println("message 1");
						  lastCommand=1;
						  return 1;
                        break;
                        case '2':
                          Serial.println("message 2");
						  lastCommand=2;
						  return 2;
                        break;
                        case '3':
                          Serial.println("message 3");
						  lastCommand=3;
						  return 3;
                        break;
                        case '4':
                          Serial.println("message 4");
						  lastCommand=4;
						  return 4;
                        break;
                        case '8':
                          Serial.println("message 8");
						  lastCommand=8;
						  return 8;
                        break;
                      }
                    }
                  }
                }
             }
           }
          }
        }
      }
    }
  }
  return lastCommand;
}

void ESPWifi::setLastCommand(int c)
{
	lastCommand=c;
}

int ESPWifi::getLastCommand(void)
{
	return lastCommand;
}