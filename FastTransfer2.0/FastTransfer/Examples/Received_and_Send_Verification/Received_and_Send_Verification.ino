#include "FastTransfer.h"

//declare a class of Fast transfer Called Receive.
FastTransfer Receive;

// this array will hold the received data by data address.
// for example receiveArray ={address 0, adress 1, address 2}
// this will send signed integers, to send unsigned integers
// declare the array as unsigned int receiveArray[];
int receiveArray[3];



int align;  // number of align errors
int address; // number of address fails
int crc; // number of crc fails
int data; // number of data address errors

void setup() {
  // open serial channel
  Serial.begin(9600);
  // start FastTransfer. Place the receiveArray in Details to give FastTransfer its information.
  // the 4 is the given module address. range 0-255. if this module receives a message for another module it will be discarded
  // true turns on the automatic acknowledged or not acknowledged reply
  // &Serial gives the class an address for the serial
  Receive.begin(Details(receiveArray), 4, true, &Serial);

  //stores the current errors
  align = Receive.alignError();
  address = Receive.addressError();
  crc = Receive.CRCError();
  data = Receive.dataAddressError();

}

void loop() {


  Receive.receiveData(); // when ever a packet with the correct address is received the verification is sent automatically



  // align errors are common if debugging with serial commands
  //  if (align != Receive.alignError()) {
  //    align = Receive.alignError();
  //    Serial.print("Number of align errors ");
  //    Serial.print(align);
  //    Serial.println();
  //  }

  if (address != Receive.addressError()) {
    address = Receive.addressError();
    Serial.print("Number of address fails ");
    Serial.print(address);
    Serial.println();
  }

  if (crc != Receive.CRCError()) {
    crc = Receive.CRCError();
    Serial.print("Number of CRC fails ");
    Serial.print(crc);
    Serial.println();
  }
  
    if (data != Receive.dataAddressError()) {
    data = Receive.dataAddressError();
    Serial.print("Number of data address errors ");
    Serial.print(data);
    Serial.println();
  }

}
