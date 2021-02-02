# Exercise 2 instructions:
In a new directory, based on the rpl-udp example, create a temperature data collection application with three
motes. Specifically:
- There should be two .c files: temp-client.c and temp-server.c.
- One mote will run temp-server.c, and the other two motes will run temp-client.c.
- The mote with temp-server.c installed should act as a RPL root and listen for UDP messages. It should
print the temperatures and sender IP addresses from incoming UDP messages to the serial port.
- The motes with temp-client.c installed should, once per second, send a UDP message to the server. The
content of the message should be an updated temperature reading from the mote's temperature sensor.

# Supported devices
This example is expected to work off-the-shelf on the following boards:

* All CC13xx/CC26xx devices
* All CC2538 devices

## Make sure
* The makefile and the C file are in the same directory
* Within the makefile the path for the contiki-ng directory CONTIKI = ../path takes to the contiki-ng directory/../
* The server and client are in the vicinity
	
	
