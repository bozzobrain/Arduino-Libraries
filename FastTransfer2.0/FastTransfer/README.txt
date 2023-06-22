*************************************
Fast Transfer Arduino Library v1.0

Brought to you by 
Igor Vinograd   
For qestions, comments, suggestions:  viniisiggs@gmail.com

With help from
Zac Kilburn

Original Library "Easy Transfer" written by 
Bill Porter

*************************************
Library Version History

1.0 Created 
1.1 Added the dataAddressError function. returns the number of times a data address was out of range.
1.2 Fixed mistake with address verification section. 

*************************************

Library Limits
0-255 addressable modules
0-254 data addresses (255 is reserved for AKNAK)
Maximum transmit at one time 64 addressed pieces of data

Protocol as follows

Header(0x06,0x85), AddressWhereToSend, AddressWhereMessageCameFrom, SizeOfMassege, DataAddress, Data, (repeat DataAddress, Data, for more data), CRC

*************************************
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
<http://www.gnu.org/licenses/>

This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or
send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.

*************************************

To install unzip and place in the Arduino library folder. 

Restart the Arduino IDE. To add the library to a project go to "Sketch" -> "Import Library".

To find examples go to "File" -> "Examples" -> "FastTransfer".

See examples on how to use the library. 
