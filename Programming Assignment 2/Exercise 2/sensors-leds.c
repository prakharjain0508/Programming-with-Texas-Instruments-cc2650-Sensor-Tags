/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */
/*---------------------------------------------------------------------------*/

// Exercise 2 instructions:
// Implement an application called sensors-leds.c that periodically (e.g. once per second)
// gathers light and temperature readings. After reading the values,
// the mote controls its LEDs as follows:
// - If darkness is detected, the mote turns its green LED on. Otherwise, it turns its green LED off.
// - If the temperature is hot (warmer than room temperature), the mote turns its red LED on.
//   Otherwise, it turns its red LED off.

#include "contiki.h"
#include "sys/ctimer.h"
#include "dev/leds.h"
#include "board-peripherals.h"

#include <stdio.h>
#include <stdint.h>
/*---------------------------------------------------------------------------*/
PROCESS(cc26xx_demo_process, "cc26xx demo process");
AUTOSTART_PROCESSES(&cc26xx_demo_process);
/*---------------------------------------------------------------------------*/

#define SENSOR_READING_PERIOD (CLOCK_SECOND)

static struct ctimer opt_timer, hdc_timer;		//declaration of ctimer structure variable for both light and temperature sensors
/*---------------------------------------------------------------------------*/
static void init_opt_reading(void *not_used);		//prototype of the light function that is called once its corresponding ctimer expires
static void init_hdc_reading(void *not_used);		//prototype of the temperature function that is called once its corresponding ctimer expires
/*---------------------------------------------------------------------------*/
static int
get_hdc_reading()		//this function returns the temperature value at each clock_second
{
  int value;
  clock_time_t next = SENSOR_READING_PERIOD;
 
  value = hdc_1000_sensor.value(HDC_1000_SENSOR_TYPE_TEMP);
  if(value != CC26XX_SENSOR_READING_ERROR) {
    printf("HDC: Temp=%d.%02d C\n", value / 100, value % 100);
  } else {
    printf("HDC: Temp Read Error\n");
  }

  ctimer_set(&hdc_timer, next, init_hdc_reading, NULL);			//setting a ctimer. Expiration of this timer in one clock_second results in calling of function: "init_hdc_reading" 
  return value;
}
/*---------------------------------------------------------------------------*/
static int
get_light_reading()		//this function returns the light intensity value at each clock_second
{
  int value;
  clock_time_t next = SENSOR_READING_PERIOD;

  value = opt_3001_sensor.value(0);
  if(value != CC26XX_SENSOR_READING_ERROR) {
    printf("OPT: Light=%d.%02d lux\n", value / 100, value % 100);
  } else {
    printf("OPT: Light Read Error\n");
  }

  /* The OPT will turn itself off, so we don't need to call its DEACTIVATE */
  ctimer_set(&opt_timer, next, init_opt_reading, NULL);			//setting a ctimer. Expiration of this timer in one clock_second results in calling of function: "init_opt_reading"
  return value;
}
/*---------------------------------------------------------------------------*/
static void
init_opt_reading(void *not_used)
{
  SENSORS_ACTIVATE(opt_3001_sensor);					//turning on the ambient light sensor(opt_3001_sensor)
}

/*---------------------------------------------------------------------------*/
static void
init_hdc_reading(void *not_used)
{
  SENSORS_ACTIVATE(hdc_1000_sensor);					//turning on the temperature sensor(hdc_1000_sensor)
}

/*---------------------------------------------------------------------------*/
static void
init_sensor_readings(void)
{
  SENSORS_ACTIVATE(hdc_1000_sensor);
  SENSORS_ACTIVATE(opt_3001_sensor);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(cc26xx_demo_process, ev, data){
  float light_state, temp_state;				//varibles storing the current sensors values
  PROCESS_BEGIN();
  init_sensor_readings();

  while(1) {

    PROCESS_YIELD();


    if(ev == sensors_event) {
      if(data == &opt_3001_sensor) {
      light_state = get_light_reading();

      if(light_state/100 < 50)			//check if light intensity is above "50 lux"
        leds_on(LEDS_GREEN);				//glow green LED if light intensity is above "50 lux"
      else
        leds_off(LEDS_GREEN);
      }

      if(data == &hdc_1000_sensor) {
        temp_state = get_hdc_reading();
        
      if(temp_state/100 > 27)				//check if temperature is above "27 deg C"
        leds_on(LEDS_RED);				//glow red LED if temperature is above "27 deg C"
      else
        leds_off(LEDS_RED);
      } 

    }

  }
  

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
