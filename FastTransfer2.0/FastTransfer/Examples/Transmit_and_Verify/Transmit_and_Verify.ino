#include "FastTransfer.h"

//declare a class of Fast transfer Called Send.
FastTransfer Send;

//this array will hold the received data by data address.
//for example receiveArray ={address 0, adress 1, address 2}
// this will send signed integers, to send unsigned integers
//declare the array as unsigned int receiveArray[];
int receiveArray[3];

unsigned long pastTime; //stores time
int reply; // stores status of message

int align;  // number of align errors
int address; // number of address fails
int crc; // number of crc fails
int data; // number of data address errors


void setup() {
  // open serial channel
  Serial.begin(9600);
  // start FastTransfer. Place the receiveArray in Details to give FastTransfer its information.
  // the 3 is the given module address. range 0-255. if this module receives a message for another module it will be discarded
  // false turns off the automatic acknowledged or not acknowledged reply
  // &Serial gives the class and address for the serial
  Send.begin(Details(receiveArray), 3, false, &Serial);
  // saves the current time
  pastTime = 0;

  //stores the current errors
  align = Send.alignError();
  address = Send.addressError();
  crc = Send.CRCError();
  reply = Send.AKNAK(4);
  data = Send.dataAddressError();


}

void loop() {


  Send.receiveData(); // even if not receiving actual data the receiveData must be invoke to get the acknowledged or not acknowledged

  if ((millis() - pastTime) > 2000) {//transmits every two seconds
    pastTime = millis();

    // populate the send buffer. the fist digit is the data address. the second digit is the data.
    //any address that is on the receive side larger than the size of the struct will be discarded.
    // the send buffer is flushed every time the data is sent.
    // maximum data address is 254. 255 is reserved.
    // maximum size to transmit is 64 integers of data, addresses don't count. 
    Send.ToSend(0, -5);
    Send.ToSend(1, 1247);

    // send data to address 4
    Send.sendData(4);

  }

// checks if a reply was received from module 4
  if (reply != Send.AKNAK(4)) {
    reply = Send.AKNAK(4);
    if (reply == 1) {
      Serial.println("Message receive verified");
    } else if (reply == 0) {
      Serial.println("Nothing received yet");
    } else if (reply == 2) {
      Serial.println("Message send failed");
    } else if (reply == 4) {
      Serial.println("No message was sent to this address");
    }
  }


// align errors are common if debugging with serial commands
//  if (align != Send.alignError()) {
//    align = Send.alignError();
//    Serial.print("Number of align errors ");
//    Serial.print(align);
//    Serial.println();
//  }

  if (address != Send.addressError()) {
    address = Send.addressError();
    Serial.print("Number of address errors ");
    Serial.print(address);
    Serial.println();
  }

  if (crc != Send.CRCError()) {
    crc = Send.CRCError();
    Serial.print("Number of CRC errors ");
    Serial.print(crc);
    Serial.println();
  }
  
  if (data != Send.dataAddressError()) {
    data = Send.dataAddressError();
    Serial.print("Number of data address errors ");
    Serial.print(data);
    Serial.println();
  }

}
