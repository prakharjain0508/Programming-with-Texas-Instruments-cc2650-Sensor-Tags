# LED HAL Example
This example demonstrates and tests the functionality of the LED HAL with 
the ctimer instead of the etimer used in the default program. The operation remais the same.
You can use it to:

* Understand the logic of the LED HAL.
* Test your implementation of arch-specific LED HAL components if you are
developing a new port.

This example assumes a device with at least 1 LED.

# Supported devices
This example is expected to work off-the-shelf on the following boards:

* All CC13xx/CC26xx devices
* All CC2538 devices

# Make sure
* The makefile and the leds-example.c are in the same directory
* Within the makefile the path for the contiki-ng directory CONTIKI = ../path takes to the contiki-ng directory/../