This example demonstrates and tests the functionality of the LED HAL, temperature sensor and light sensor. The application periodically(e.g. once per second) gathers light and temperature readings. After reading the values, the motes controls its LEDs as follows:
* If darkness is detected, the mote turns its green LED on. Otherwise, it turns its green LED off.
* If the temperature is hot (warmer than room temperature), the mote turns its red LED on. Otherwise, it turns its red LED off.
In short, the mote uses its LEDs to provide a visual summary of the sensor readings. The thresholds for ambient light sensor and the temperature sensor are set to "50 lux" and "27 deg C" respectively which can be changed according to the surrounding. 


# Supported devices
This example is expected to work off-the-shelf on the following boards:

* All CC13xx/CC26xx devices
* All CC2538 devices

# Things to lookout for
* The makefile and the C file are in the same directory
* Within the makefile the path for the contiki-ng directory CONTIKI = ../path takes to the contiki-ng directory/../
  
	
	
