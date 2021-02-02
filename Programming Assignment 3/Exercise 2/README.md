# RPL over 6LoWPAN
Nullnet only performs link-local broadcast annd unicast, and therefore only handles one hop. For routing over multiple hops, Contiki-NG provides an implementation pf RPL (Routing Protocol for Low-Power and Lossy Networks), an IETF standard. Implemented a temperature data collection application with three motes where:
- There are two .c files: temp-client.c and temp-server.c.
- One mote runs temp-server.c, and the other two motes run temp-client.c.
- The mote with temp-server.c installed act as a RPL root and listen for UDP messages. It prints the temperatures and sender IP addresses 
from incoming UDP messages to the serial port.
- The motes with temp-client.c installed send a UDP message to the server once per second. The
content of the message is an updated temperature reading from the mote's temperature sensor.

# Supported devices
This example is expected to work off-the-shelf on the following boards:

* All CC13xx/CC26xx devices
* All CC2538 devices

## Things to lookout for
* The makefile and the C file are in the same directory
* Within the makefile the path for the contiki-ng directory CONTIKI = ../path takes to the contiki-ng directory/../
* The server and client are in the vicinity
	
	
