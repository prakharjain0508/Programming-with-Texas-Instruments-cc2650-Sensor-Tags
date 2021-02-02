# Network stack and Nullnet
In Contiki the applications use an API to interact with a collection of software called network stack. Contiki-NG'S network stack conceptualize a routing layer, a network layer, a medium access control (MAC) layer, and a radio layer. Nullnet is the simplest network stack in Contiki-NG. Data in nullnet is encapsulated directly in a MAC-layer frame, which can be either broadcast or unicast to neighbors.

# Exercise 1 instructions:
This file creates a simple received packet counter called reception-counter.c that behaves as follows:
- Two motes broadcast a message to each other, once per second with some added randomness.
- Each mote maintains a counter.
- When a mote receives a message, it increments its counter.
- Each mote uses its LEDs to display the two (or three, if you have TelosB/Sky motes) least significant bits of its counter.


## Things to lookout for
* The makefile and the C file are in the same directory
* Within the makefile the path for the contiki-ng directory CONTIKI = ../path takes to the contiki-ng directory/../
* The LEDs blink according to the following order depending on the least significant bits of the counter.

	* LED Green: LSB
	* LED Red: bit next to LSB

1. When counter is  4x, 	(x = 0, 1, 2,...), both LEDs OFF 			(00)
2. When counter is (4x+1), (x = 0, 1, 2,...), Red  OFF, Green ON			(01)
3. When counter is (4x+2), (x = 0, 1, 2,...), Red ON, Green OFF				(10)
4. When counter is (4x+3), (x = 0, 1, 2,...), both LEDs ON				(11)

Each state is maintained till the next packet is received.

The node transmits a message each second with some added randomness between 0 to 0.99 second.
This random interval is achieved by calling 'random_rand()'.
- A random number is generated and its mod 100 is calculated. 
- This gives a number between 0 to 99
- This number is divided by 100 to get a value between 0 to 0.99
- This fraction is added to the default clock interval (which is 1 second)
	
	
