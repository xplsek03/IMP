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

#include "lwip/apps/httpd_opts.h"
#include "lwip/def.h"
#include "lwip/apps/fs.h"
#include <string.h>
#include "fsdata.h"
#include "fsdata.c"

/* CGI GET START */

#include "lwip/apps/httpd.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include <stdio.h>

/* zatim nedoslo k akci */
int action = 0;

int kill = 0; // objevil se jiny  request, zastav polling

// jednotlve sipky - zmacknute
int up_pressed = 0;
int down_pressed = 0;
int left_pressed = 0;
int right_pressed = 0;

/* konkretni handlery */
static const tCGI cgi_handlers[] = {
  {
   "/update.html",
   cgi_server_update
  },
  {
    "/index.html",
    cgi_handler_basic
  },
  {
    "/",
    cgi_handler_basic
  }
};

/* klient poll a server posle jen kdyz ma co poslat */
const char * cgi_server_update(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
	LWIP_ASSERT("check index", iIndex < LWIP_ARRAYSIZE(cgi_handlers));

	while(1) {

		for(int i=0; i < 500; i++); // DELAY

		if(!down_pressed && !(GPIOE->PDIR & BTN_SW3)) {
			down_pressed = 1;
			return "down";
		}
		else if(down_pressed && (GPIOE->PDIR & BTN_SW3)) {
			down_pressed = 0;
			return "down";
		}
		if (!up_pressed && !(GPIOE->PDIR & BTN_SW5)) {
			up_pressed = 1;
			return "up";
		}
		else if(up_pressed && (GPIOE->PDIR & BTN_SW5)) {
			up_pressed = 0;
			return "up";
		}
		else if(!right_pressed && !(GPIOE->PDIR & BTN_SW2)) {
			right_pressed = 1;
			return "right";
		}
		else if(right_pressed && (GPIOE->PDIR & BTN_SW2)) {
			right_pressed = 0;
			return "right";
		}
		else if(!left_pressed && !(GPIOE->PDIR & BTN_SW4)) {
			left_pressed = 1;
			return "left";
		}
		else if(left_pressed && (GPIOE->PDIR & BTN_SW4)) {
			left_pressed = 0;
			return "left";
		}

		// prerus polling a posli preruseni ke klientovi
		else if(!(GPIOE->PDIR & BTN_SW6)) {
			return "pollkill";
		}
	}
	return "/index.html";
}

/* callback zpracovani klientskeho get requestu */
const char * cgi_handler_basic(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {

  LWIP_ASSERT("check index", iIndex < LWIP_ARRAYSIZE(cgi_handlers));

  if (iNumParams == 1) {

    if (!strcmp(pcParam[0], "led")) {
	  if(!strcmp(pcValue[0], "l1")) {
		  GPIOB->PDOR ^= LED_D9;
		  return "valid";
	  }
	  else if (!strcmp(pcValue[0], "l2")) {
		  GPIOB->PDOR ^= LED_D10;
		  return "valid";
	  }
	  else if (!strcmp(pcValue[0], "l3")) {
		  GPIOB->PDOR ^= LED_D11;
		  return "valid";
      }
      else if (!strcmp(pcValue[0], "l4")) {
    	  GPIOB->PDOR ^= LED_D12;
    	  return "valid";
      }
      else return "/index.html";
    }

    else if(!strcmp(pcParam[0], "arrow")) {

      if (!strcmp(pcValue[0], "upon")) { // dal na neho prst a zaroven neni zmacknute
    	  GPIOE->PDOR |= BTN_SW5; // zmen stav na zmacknuto
    	  return "valid"; // DEBUG: index.html
      }
      else if(!strcmp(pcValue[0], "upout")) { // sundal z neho prst
    	  GPIOE->PDOR &= ~(BTN_SW5);
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "downon")) {
    	  GPIOE->PDOR |= BTN_SW3;
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "downout")) {
    	  GPIOE->PDOR &= ~(BTN_SW3);
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "lefton")) {
    	  GPIOE->PDOR |= BTN_SW4;
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "leftout")) {
    	  GPIOE->PDOR &= ~(BTN_SW4);
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "righton")) {
    	  GPIOE->PDOR |= BTN_SW2;
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "rightout")) {
    	  GPIOE->PDOR &= ~(BTN_SW2);
    	  return "valid";
      }
    }

  }
  return "/index.html";
}

/* nastav handler na httpd */
void cgi_ex_init(void) {
  http_set_cgi_handlers(cgi_handlers, LWIP_ARRAYSIZE(cgi_handlers));
}

int fs_open_custom(struct fs_file *file, const char *name) {

/* pokud se ma zobrazit defaultni index */
  if (!strcmp(name, "right") || !strcmp(name, "left") || !strcmp(name, "up") || !strcmp(name, "down") || !strcmp(name, "valid") || !strcmp(name, "pollkill")) {
    /* initialize fs_file correctly */

    memset(file, 0, sizeof(struct fs_file));
    file->pextension = mem_malloc(strlen(name));
    if (file->pextension != NULL) {
      memcpy(file->pextension, name, strlen(name));
      file->data = (const char *)file->pextension;
      file->len = strlen(name);
      file->index = file->len;
      file->flags = FS_FILE_FLAGS_HEADER_PERSISTENT;
      return 1;
    }
  }
  return 0;
}

void fs_close_custom(struct fs_file *file) {
  if (file && file->pextension) {
    mem_free(file->pextension);
    file->pextension = NULL;
  }
}


err_t fs_open(struct fs_file *file, const char *name) {

  const struct fsdata_file *f;

  if ((file == NULL) || (name == NULL)) {
     return ERR_ARG;
  }

  if (fs_open_custom(file, name)) {
    file->is_custom_file = 1;
    return ERR_OK;
  }
  file->is_custom_file = 0;

  for (f = FS_ROOT; f != NULL; f = f->next) {
    if (!strcmp(name, (const char *)f->name)) {
      file->data = (const char *)f->data;
      file->len = f->len;
      file->index = f->len;
      file->pextension = NULL;
      file->flags = f->flags;
      return ERR_OK;
    }
  }
  return ERR_VAL;
}

void fs_close(struct fs_file *file)
{
  if (file->is_custom_file) {
    fs_close_custom(file);
  }
  LWIP_UNUSED_ARG(file);
}

int fs_bytes_left(struct fs_file *file)
{
  return file->len - file->index;
}
