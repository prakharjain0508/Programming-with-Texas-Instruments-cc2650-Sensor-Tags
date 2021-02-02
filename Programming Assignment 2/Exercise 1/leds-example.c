/*
 * Copyright (c) 2017, George Oikonomou - http://www.spd.gr
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*---------------------------------------------------------------------------*/

// Exercise 1 instructions:
// The leds-example.c application from PA1 uses etimer.
// Modify this application by removing the etimer and using a ctimer instead.
// The end result (the sequence of blinking LEDs) should be the same.


#include "contiki.h"
#include "dev/leds.h"
#include "sys/ctimer.h"				//header file for c-timer

#include <stdio.h>
/*---------------------------------------------------------------------------*/
static struct ctimer ct;			//declaration of ctimer structure variable
static uint8_t counter;
static void my_callback_func(void * data);	//prototype of the function that is called once the ctimer expires
/*---------------------------------------------------------------------------*/
PROCESS(leds_example, "LED HAL Example");
AUTOSTART_PROCESSES(&leds_example);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(leds_example, ev, data)
{
  PROCESS_BEGIN();
  counter = 0;
  ctimer_set(&ct, CLOCK_SECOND, my_callback_func, NULL);		//setting a ctimer. Expiration of this timer in one clock_second results in calling of function: "my_callback_func"
  PROCESS_END();
}

void my_callback_func(void * data){
      if((counter & 7) == 0) {
        leds_set(LEDS_ALL);
      } else if((counter & 7) == 1) {
        leds_off(LEDS_ALL);
      } else if((counter & 7) == 2) {
        leds_on(LEDS_ALL);
      } else if((counter & 7) == 3) {
        leds_toggle(LEDS_ALL);
#if !LEDS_LEGACY_API
      } else if((counter & 7) == 4) {
        leds_single_on(LEDS_LED1);
      } else if((counter & 7) == 5) {
        leds_single_off(LEDS_LED1);
      } else if((counter & 7) == 6) {
        leds_single_toggle(LEDS_LED1);
#endif /* LEDS_LEGACY_API */
      } else if((counter & 7) == 7) {
        leds_toggle(LEDS_ALL);
      }
      counter++;
      ctimer_reset(&ct);			//resetting the ctimer
    }
/*---------------------------------------------------------------------------*/

