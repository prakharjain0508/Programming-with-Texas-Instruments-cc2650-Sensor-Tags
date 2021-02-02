/*
 * Copyright (c) 2017, RISE SICS.
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

/**
 * \file
 *         NullNet broadcast example
 * \author
*         Simon Duquennoy <simon.duquennoy@ri.se>
 *
 */

#include "contiki.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "net/nullnet/nullnet.h"
#include <string.h>
#include <stdio.h> /* For printf() */

#include "dev/leds.h"
#include "random.h"

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Configuration */
#define SEND_INTERVAL (CLOCK_SECOND/4)

#if MAC_CONF_WITH_TSCH
#include "net/mac/tsch/tsch.h"
static linkaddr_t coordinator_addr =  {{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};
#endif /* MAC_CONF_WITH_TSCH */

int16_t a[4], avg, new;				//a[] is array to store RSSI of the most recent packets
int flag = 0;

/*---------------------------------------------------------------------------*/
PROCESS(nullnet_example_process, "NullNet broadcast example");
AUTOSTART_PROCESSES(&nullnet_example_process);

/*---------------------------------------------------------------------------*/
void input_callback(const void *data, uint16_t len, const linkaddr_t *src, const linkaddr_t *dest)
{

	//this function executes to deal with the receiver role of the node. i.e. here we examine RSSI of the received packets

  if(len == sizeof(unsigned)) {
    unsigned count;
    memcpy(&count, data, sizeof(count));
    LOG_INFO("Received %u from ", count);			//prints the count received from the neighbor mote
    LOG_INFO_LLADDR(src);					//prints the link layer address of the sending mote
    LOG_INFO_("\n");
    int i;

    
    if(flag==0)
    {
    	for(i=0;i<4;i++)
    	{
		a[i] = (int16_t)packetbuf_attr(PACKETBUF_ATTR_RSSI);
    	}
    	flag = 1;
    }
	
	avg = (a[0]+a[1]+a[2]+a[3])/4;
	new = (int16_t)packetbuf_attr(PACKETBUF_ATTR_RSSI);


	//checking 3 cases to decide which LED/s will turn on

	if(new > avg+1)
	{
		leds_on(LEDS_RED);			
		leds_off(LEDS_GREEN);
	}

	else if(new < avg-1)
	{
		leds_off(LEDS_RED);			
		leds_on(LEDS_GREEN);
	}

	else
	{
		leds_off(LEDS_RED);			
		leds_off(LEDS_GREEN);
	}

	//moving the buffer window and storing new 
	a[0] = a[1];
	a[1] = a[2];
	a[2] = a[3];
	a[3] = new;
	//LOG_INFO("Received avg %d from ", avg);
	//LOG_INFO("Received new %d from ", new);
	//LOG_INFO("Buffer is : %d, %d, %d, %d", a[0],a[1],a[2],a[3]);
   }
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(nullnet_example_process, ev, data)
{
  static struct etimer periodic_timer;
  static unsigned count = 0;
  PROCESS_BEGIN();

#if MAC_CONF_WITH_TSCH
  tsch_set_coordinator(linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr));
#endif /* MAC_CONF_WITH_TSCH */

  /* Initialize NullNet */
  nullnet_buf = (uint8_t *)&count;
  nullnet_len = sizeof(count);
  nullnet_set_input_callback(input_callback);			//function call to process on RSSI of the received packet

  etimer_set(&periodic_timer, SEND_INTERVAL);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    LOG_INFO("Sending %u to ", count);				//prints the count sent to the neighbor mote
   // LOG_INFO("Sending %d to ", new);
   // LOG_INFO("Sending %d to ", avg);			
    LOG_INFO_LLADDR(NULL);
    LOG_INFO_("\n");
    
    memcpy(nullnet_buf, &count, sizeof(count));			//copying the count into the data buffer(nullnet_buf)
    nullnet_len = sizeof(count);				//Assigning the size of the count in bytes to nullnet_len
		
    NETSTACK_NETWORK.output(NULL);				//broadcasts a packet to all the neighbors
    count++;								//incrementing the count
    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
