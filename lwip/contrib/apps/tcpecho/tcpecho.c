/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#include "tcpecho.h"

#include "lwip/opt.h"

#if LWIP_NETCONN

#include "lwip/sys.h"
#include "lwip/api.h"

#define PORT	7

#define configIP_ADDR0_Serv 192
#define configIP_ADDR1_Serv 168
#define configIP_ADDR2_Serv 100
#define configIP_ADDR3_Serv 58

/*-----------------------------------------------------------------------------------*/
static void
tcpecho_thread(void *arg)
{
	struct netconn *conn, *newconn;
	err_t err;
	LWIP_UNUSED_ARG(arg);
	ip4_addr_t netif_ipaddr_Server;

	/* Create a new connection identifier. */
	/* Bind connection to well known port number 7. */
	conn = netconn_new(NETCONN_TCP);
	//netconn_bind(conn, IP_ADDR_ANY, 7);

	LWIP_ERROR("tcpecho: invalid conn", (conn != NULL), return;);

	/* Tell connection to go into listening mode. */
	//netconn_listen(conn);
	IP4_ADDR(&netif_ipaddr_Server, configIP_ADDR0_Serv, configIP_ADDR1_Serv, configIP_ADDR2_Serv, configIP_ADDR3_Serv);

	struct netbuf *buf;
	void *data;
	u16_t len;

	u8_t myData[] = {"Hello World!"};

	data = (void*)myData;
	len = strlen(myData) + 1;

	while (1)
	{
		conn = netconn_new(NETCONN_TCP);
		err = netconn_connect(conn, &netif_ipaddr_Server, PORT);

		if(err == ERR_OK)
		{
			printf("Conectado \n");

			printf("Enviando datos. Longitud: %d caracteres\n", len-1);

			err = netconn_write(conn, data, len, NETCONN_COPY);
			printf("Enviado \"%s\"\n", myData);

			err = netconn_recv(conn, &buf);
			netbuf_data(buf, &data, &len);
			printf("Recibido \"%s\"\n", data);


			netconn_close(conn);
			netconn_delete(conn);

			vTaskDelay(20000);
		}
		else
		{
			printf("Error al conectar: %d\n", err);

			break;
		}



	}
}
/*-----------------------------------------------------------------------------------*/
void
tcpecho_init(void)
{
  sys_thread_new("tcpecho_thread", tcpecho_thread, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
}
/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
