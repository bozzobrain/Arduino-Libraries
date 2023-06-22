/******************************************************************
*            FastTransfer Arduino Library
*            Brought to you by:
*            Igor Vinograd
*
*            With help from:
*            Zac Kilburn
*
             Expanded By:
*            Rami Nehme
*
*            Original Library "EasyTransfer" written by
*            Bill Porter
*
*            See Readme for other info and version history
*            If you have any questions, comments or suggestions please contact me viniisiggs@gmail.com
*
*
*
*This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
<http://www.gnu.org/licenses/>
*
*This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
*To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or
*send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
******************************************************************/
#ifndef FastTransfer_h
#define FastTransfer_h


//the capital D is so there is no interference with the lower case d of EasyTransfer
#define Details(name) (int*)&name,sizeof(name)

//Not necessary, but just in case.
#if ARDUINO > 22
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "HardwareSerial.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

class FastTransfer {
	public:
	void begin(int *, uint8_t maxSize, uint8_t givenAddress, boolean error, HardwareSerial *theSerial);
	void sendData(uint8_t whereToSend);
	boolean receiveData();
	void ToSend(const int where, const unsigned int what);
	uint8_t AKNAK(uint8_t module);
	unsigned int alignError(void);
	unsigned int CRCError(void);
	unsigned int addressError(void);
	unsigned int dataAddressError(void);
	private:



	HardwareSerial *_serial;
	uint8_t * rx_buffer; //address for temporary storage and parsing buffer
	int rx_array_inx; //index for RX parsing buffer
	int rx_len; //RX packet length according to the packet
	uint8_t calc_CS; //calculated Checksum
	uint8_t moduleAddress; // the address of this module
	uint8_t returnAddress; //the address to send the crc back to
	uint8_t maxDataAddress; //max address allowable
	int * receiveArrayAddress; // this is where the data will go when it is received
	uint8_t * sendStructAddress;  // this is where the data will be sent from
	boolean AKNAKsend; // turns the acknowledged or not acknowledged on/off
	unsigned int alignErrorCounter; //counts the align errors
	unsigned int crcErrorCounter; // counts any failed crcs
	unsigned int addressErrorCounter; // counts every time a wrong address is received
	unsigned int dataAdressErrorCounter; // counts if the received data fall outside of the receive array
	uint8_t rx_address; //RX address received
	#define polynomial 0x8C  //polynomial used to calculate crc
	#define BUFFER_SIZE 600 //ring buffer size
	#define CRC_COUNT 5 // how many AKNAKs are stored
	#define CRC_DEPTH 3  // how many pieces of data are stored with each CRC send event
	#define CRC_BUFFER_SIZE (CRC_COUNT * CRC_DEPTH) //crc buffer size 5 deep and 3 bytes an entry


	struct ringBufS { // this is where the send data is stored before sending
		uint8_t buf_One[BUFFER_SIZE];
		uint8_t buf_Two[BUFFER_SIZE];
		int head;
		int tail;
		int count;
	};


	struct ringBufS ring_buffer;

	union stuff { // this union is used to join and disassemble integers
		unsigned char parts[2];
		unsigned int integer;
	};
	union stuff group;

	struct crcBufS {  // this is where the address where sent to, the sent crc, the status of the AKNAK
		unsigned char buf[CRC_BUFFER_SIZE];
		int head;
	};
	struct crcBufS crc_buffer;

	uint8_t CRC8(const uint8_t * data, int len);
	void sendBuf_put(struct ringBufS *_this, const uint8_t towhere_one, const uint8_t towhere_two , const unsigned int towhat);
	uint8_t ringBufS_get(struct ringBufS* _this);
	void ringBufS_flush(struct ringBufS* _this, const int clearBuffer);
	unsigned int modulo_inc(const unsigned int value, const unsigned int modulus);
	void crcBufS_put(struct crcBufS* _this, uint8_t address, uint8_t oldCRC, uint8_t status);
	void crcBufS_status_put(struct crcBufS* _this, uint8_t time, uint8_t status);
	uint8_t crcBufS_get(struct crcBufS* _this, uint8_t time, uint8_t space);
	void CRCcheck(void);
};



#endif
