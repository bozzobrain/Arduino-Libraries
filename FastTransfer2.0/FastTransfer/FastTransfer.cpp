#include "FastTransfer.h"
#include <dos.h >

//Captures address of receive array, the max data address, the address of the module, true/false if AKNAKs are wanted and the Serial address
void FastTransfer::begin(int * ptr, uint8_t maxSize, uint8_t givenAddress, boolean error, HardwareSerial *theSerial){
	receiveArrayAddress = ptr;
	moduleAddress = givenAddress;
	_serial = theSerial;
	maxDataAddress = maxSize / 2;
	sendStructAddress = (byte*)&ring_buffer;
	AKNAKsend = error;
	alignErrorCounter = 0;

}

//CRC Calculator
uint8_t FastTransfer::CRC8(const uint8_t * data, int len) {
	uint8_t crc = 0x00;
	while (len--) {
		uint8_t extract = *data++;
		for (uint8_t tempI = 8; tempI; tempI--) {
			uint8_t sum = (crc ^ extract) & 0x01;
			crc >>= 1;
        if (sum) {
				crc ^= polynomial;
			}
			extract >>= 1;
		}
	}
	return crc;
}

//Sends out send buffer with a 2 start bytes, where the packet is going, where it came from, the size of the data packet, the data and the crc.
void FastTransfer::sendData(uint8_t whereToSend){

	//calculate the crc
	uint8_t CS = CRC8( sendStructAddress , ring_buffer.count);

	_serial->write(0x06); //start address
	_serial->write(0x85);  //start address
	_serial->write(whereToSend);
	_serial->write(moduleAddress);
	uint8_t frontal_part = 0x00;
	uint8_t back_part = 0x00;
	int counter = ring_buffer.count;

    frontal_part = counter >> 8;
    back_part = counter;
	_serial->write(frontal_part); //length of packet not including the crc
	_serial->write(back_part);


	//send the rest of the packet
	for(int i = 0; i<ring_buffer.count; i++){
		_serial->write(*(sendStructAddress+i));
	}

	//send the crc
	_serial->write(CS);

	//record the sent message data for aknak check later
	crcBufS_put(&crc_buffer, whereToSend, CS, 0);

	// clears the buffer after a sending
	ringBufS_flush(&ring_buffer, 1);

}

boolean FastTransfer::receiveData(){

	//start off by looking for the header bytes. If they were already found in a previous call, skip it.
	if(rx_len == 0){
		//this size check may be redundant due to the size check below, but for now I'll leave it the way it is.
		if(_serial->available() >  4){
			//this will block until a 0x06 is found or buffer size becomes less then 3.
			while(_serial->read() != 0x06) {
				//This will trash any preamble junk in the serial buffer
				//but we need to make sure there is enough in the buffer to process while we trash the rest
				//if the buffer becomes too empty, we will escape and try again on the next call
				alignErrorCounter++;  //increments the counter whenever a byte is trashed
				if(_serial->available() < 3)
				return false;
			}
			if (_serial->read() == 0x85){

				rx_address = _serial->read(); // pulls the address
				returnAddress = _serial->read(); // pulls where the message came from

                while(!_serial->available());
                rx_len += (_serial->read() << 8);
                while(!_serial->available());
                rx_len += (_serial->read());


                // pulls the length
				//make sure the address received is a match for this module if not throw the packet away
				    if (rx_address != moduleAddress) {
					addressErrorCounter++; // increments a counter whenever the wrong address is received
					//if the address does not match the buffer is flushed for the size of
					//the data packet plus one for the CRC
					for (int u = 0; u <= (rx_len + 1) ; u++) {
						_serial->read();
					}

					rx_len = 0; // reset length ORIGINAL
					return false; //ORIGINAL
				}
				// if the address matches the a dynamic buffer is created to store the received data

				rx_buffer = (uint8_t*) malloc(rx_len + 1);
			}
		}
	}
	//we get here if we already found the header bytes, the address matched what we know, and now we are byte aligned.
	if(rx_len != 0){

		//this check is preformed to see if the first data address is a 255, if it is then this packet is an AKNAK
		if (rx_array_inx == 0){
			while( !(_serial->available() >= 1));
			if (255 == _serial->peek()) {
				CRCcheck();
				rx_len = 0;
				rx_array_inx = 0;
				free(rx_buffer);
				return receiveData();
			}
		}


		while(_serial->available() && rx_array_inx <= rx_len){
			rx_buffer[rx_array_inx++] = _serial->read();
		}

		if(rx_len == (rx_array_inx - 1)){
			//seem to have got whole message
			//last uint8_t is CS
			calc_CS = CRC8( rx_buffer, rx_len);



			if(calc_CS == rx_buffer[rx_array_inx - 1]){//CS good

				// reassembles the data and places it into the receive array according to data address.
				for (int r = 0; r < rx_len; r = r + 4) {
					if (rx_buffer[r] < maxDataAddress) {
						group.parts[0] = rx_buffer[r + 2];
						group.parts[1] = rx_buffer[r + 3];
						receiveArrayAddress[(rx_buffer[r])] = group.integer;
						} else {dataAdressErrorCounter++;
					}
				}


				if (AKNAKsend) { // if enabled sends an AK
					uint8_t holder[3];
					holder[0] = 255;
					holder[1] = 1;
					holder[2] = rx_buffer[rx_array_inx-1];
					uint8_t crcHolder = CRC8(holder, 3);
					_serial->write(0x06);
					_serial->write(0x85);
					_serial->write(returnAddress);
					_serial->write(moduleAddress);
					_serial->write(3);
					_serial->write(255);
					_serial->write(1);
					_serial->write(rx_buffer[rx_array_inx-1]);
					_serial->write(crcHolder);
				}



				rx_len = 0;
				rx_array_inx = 0;
				free(rx_buffer);
				return true;
			}

			else{
				crcErrorCounter++; //increments the counter every time a crc fails

				if (AKNAKsend) { // if enabled sends NAK
					uint8_t holder[3];
					holder[0] = 255;
					holder[1] = 2;
					holder[2] = rx_buffer[rx_array_inx-1];
					uint8_t crcHolder = CRC8(holder, 3);
					_serial->write(0x06);
					_serial->write(0x85);
					_serial->write(returnAddress);
					_serial->write(moduleAddress);
					_serial->write(3);
					_serial->write(255);
					_serial->write(2);
					_serial->write(rx_buffer[rx_array_inx-1]);
					_serial->write(crcHolder);
				}

				//failed checksum, need to clear this out
				rx_len = 0;
				rx_array_inx = 0;
				free(rx_buffer);
				return false;
			}
		}
	}

	return false;
}


// populates what info needs sent and to what data address
void FastTransfer::ToSend(int where, unsigned int what) {
    uint8_t Part_One = 0x00;
    uint8_t Part_Two = 0x00;
    if( where < 255)
    {
        Part_Two = where;
    }
    else
    while( where > 255 )
    {
        where -= 255;
        Part_One++;
        Part_Two = where;
    }

	sendBuf_put(&ring_buffer, Part_One, Part_Two , what);
}


// disassembles the data and places it in a buffer to be sent
void FastTransfer::sendBuf_put(struct ringBufS *_this, uint8_t towhere_one, uint8_t towhere_two , unsigned int towhat) {

	group.integer = towhat;


	if (_this->count < (BUFFER_SIZE - 4) && _this->count < 600)  {
		_this->buf_One[_this->head] = towhere_one;
		_this->head = modulo_inc(_this->head, BUFFER_SIZE);
		++_this->count;
		_this->buf_One[_this->head] = towhere_two;
		_this->head = modulo_inc(_this->head, BUFFER_SIZE);
		++_this->count;
		_this->buf_One[_this->head] = group.parts[0];
		_this->head = modulo_inc(_this->head, BUFFER_SIZE);
		++_this->count;
		_this->buf_One[_this->head] = group.parts[1];
		_this->head = modulo_inc(_this->head, BUFFER_SIZE);
		++_this->count;

	}



}


//pulls info out of the send buffer in a first in first out fashion
uint8_t FastTransfer::ringBufS_get(struct ringBufS* _this) {
	unsigned char c;
	if (_this->count > 0   && _this->count > 600)
        {
		c = _this->buf_One[_this->tail];
		_this->tail = modulo_inc(_this->tail, BUFFER_SIZE);
		--_this->count;
		}
    else {
		c = 0;
	}
	return (c);
}


//flushes the send buffer to get it ready for new data
void FastTransfer::ringBufS_flush(struct ringBufS* _this, const int clearBuffer) {
	_this->count = 0;
	_this->head = 0;
	_this->tail = 0;
	if (clearBuffer) {
		memset(_this->buf_One, 0, sizeof (_this->buf_One));
		memset(_this->buf_Two, 0, sizeof (_this->buf_Two));
	}
}


// increments counters for the buffer functions
unsigned int FastTransfer::modulo_inc(const unsigned int value, const unsigned int modulus) {
	unsigned int my_value = value + 1;
	if (my_value >= modulus) {
		my_value = 0;
	}
	return (my_value);
}


//searches the buffer for the status of a message that was sent
uint8_t FastTransfer::AKNAK(uint8_t module) {
	for (int r = 0; r < CRC_COUNT; r++) {
		if (module == crcBufS_get(&crc_buffer, r, 0)) {
			return crcBufS_get(&crc_buffer, r, 2);
		}
	}
	return 4;
}


//returns align error
unsigned int FastTransfer::alignError(void) {
	return alignErrorCounter;
}


//returns CRC error
unsigned int FastTransfer::CRCError(void) {
	return crcErrorCounter;
}


//returns address error
unsigned int FastTransfer::addressError(void) {
	return addressErrorCounter;
}

unsigned int FastTransfer::dataAddressError(void){
	return dataAdressErrorCounter;
}

// after a packet is sent records the info of that packet
void FastTransfer::crcBufS_put(struct crcBufS* _this, uint8_t address, uint8_t oldCRC, uint8_t status) {
	_this->buf[_this->head] = address;
	_this->head++;
	_this->buf[_this->head] = oldCRC;
	_this->head++;
	_this->buf[_this->head] = status;
	_this->head++;
	if (_this->head >= CRC_BUFFER_SIZE) {
		_this->head = 0;
	}
}


// after a Ak or NAK is received that status is stored
void FastTransfer::crcBufS_status_put(struct crcBufS* _this, uint8_t time, uint8_t status) {
	if (time >= CRC_COUNT) {
		time = CRC_COUNT - 1;
	}
	time = time + 1;
	int wantedTime = time * 3;
	if (wantedTime > _this->head) {
		wantedTime = (CRC_BUFFER_SIZE) - (wantedTime - _this->head);
		_this->buf[(wantedTime + 2)] = status;
		} else {
		_this->buf[(_this->head - wantedTime) + 2] = status;
	}
}


// pulls data from the AKNAK buffer
uint8_t FastTransfer::crcBufS_get(struct crcBufS* _this, uint8_t time, uint8_t space) {
	if (time >= CRC_COUNT) {
		time = CRC_COUNT - 1;
	}
	if (space >= CRC_DEPTH) {
		space = CRC_DEPTH - 1;
	}
	time = time + 1;
	int wantedTime = time * 3;
	if (wantedTime > _this->head) {
		wantedTime = (CRC_BUFFER_SIZE) - (wantedTime - _this->head);
		return (_this->buf[(wantedTime + space)]);
		} else {
		return (_this->buf[(_this->head - wantedTime) + space]);
	}
}


//when an AK or NAK is received this compares it to the buffer and records the status
void FastTransfer::CRCcheck(void) {

	while (!(_serial->available() > 3)); // trap makes sure that there are enough bytes in the buffer for the AKNAK check

	uint8_t arrayHolder[3];
	arrayHolder[0] = _serial->read();
	arrayHolder[1] = _serial->read();
	arrayHolder[2] = _serial->read();
	uint8_t SentCRC = _serial->read();
	uint8_t calculatedCRC = CRC8(arrayHolder, 3);


	if(SentCRC == calculatedCRC){

		for (int rt = 0; rt < CRC_COUNT; rt++) {
			if (returnAddress == crcBufS_get(&crc_buffer, rt, 0)) {
				if (arrayHolder[2] == crcBufS_get(&crc_buffer, rt, 1)) {
					if (arrayHolder[1] == 1) {
						crcBufS_status_put(&crc_buffer, rt, 1);
						break;
						} else if (arrayHolder[1] == 2) {
						crcBufS_status_put(&crc_buffer, rt, 2);
						break;
					}
				}
			}
		}
		}else{ crcErrorCounter++;} //increments the counter every time a crc fails
	}
