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

#ifndef LWIP_HDR_APPS_FS_H
#define LWIP_HDR_APPS_FS_H

#include "httpd_opts.h"
#include "lwip/err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FS_READ_EOF     -1
#define FS_READ_DELAYED -2

#define FS_FILE_FLAGS_HEADER_INCLUDED     0x01
#define FS_FILE_FLAGS_HEADER_PERSISTENT   0x02

struct fs_file {
  const char *data;
  int len;
  int index;
  void *pextension;

  u8_t flags;
  u8_t is_custom_file;
};

err_t fs_open(struct fs_file *file, const char *name);
void fs_close(struct fs_file *file);
int fs_bytes_left(struct fs_file *file);

const char *cgi_handler_basic(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char *cgi_server_update(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

// PINY
#define GPIO_PIN_MASK 0x1Fu
#define GPIO_PIN(x) (((1)<<(x & GPIO_PIN_MASK)))
#define LED_D9  0x20      	// Port B, bit 5
#define LED_D10 0x10      	// Port B, bit 4
#define LED_D11 0x8       	// Port B, bit 3
#define LED_D12 0x4       	// Port B, bit 2

#define BTN_SW2 0x400     	// Port E, bit 10 Right
#define BTN_SW3 0x1000    	// Port E, bit 12 Down
#define BTN_SW4 0x8000000 	// Port E, bit 27 Left
#define BTN_SW5 0x4000000 	// Port E, bit 26 Up
#define BTN_SW6 0x800		// PORT E, bit 11 preruseni pollingu

extern int action;			// doslo k nejake akci
extern int up_pressed; 		// tlacitko je zmacknute
extern int down_pressed; 	// tlacitko je zmacknute
extern int left_pressed; 	// tlacitko je zmacknute
extern int right_pressed; 	// tlacitko je zmacknute

/* funkce pro ovladani souboru s cgi GET */
void cgi_ex_init(void);


#ifdef __cplusplus
}
#endif

#endif /* LWIP_HDR_APPS_FS_H */
