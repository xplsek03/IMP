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

/* je tu aby cgi handlery vedely co je callback funkce */
static const char *cgi_handler_basic(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

/* defaultni sablona index.html, ktera se bude postupne prepisovat, ale vzdycky zustane ulozena posledni verze */

const char generated_html[] = "";

/* konkretni handlery */
static const tCGI cgi_handlers[] = {
  {
    "/index.html",
    cgi_handler_basic
  },
  {
	"/",
	cgi_handler_basic
  }
};

/* callback zpracovani get requestu */
static const char * cgi_handler_basic(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {

  LWIP_ASSERT("check index", iIndex < LWIP_ARRAYSIZE(cgi_handlers));

  if (iNumParams == 1) {
    if (!strcmp(pcParam[0], "led")) {

      if (!strcmp(pcValue[0], "l1")) {
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "l2")) {
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "l3")) {
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "l4")) {
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "l5")) {
    	  return "valid";
      }
      else return "/404.html";
    }

    else if (!strcmp(pcParam[0], "arrow")) {

      if (!strcmp(pcValue[0], "upon")) {
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "upout")) {
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "downon")) {
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "downout")) {
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "lefton")) {
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "leftout")) {
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "righton")) {
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "rightout")) {
    	  return "valid";
      }
      else return "/404.html";
    }

    else if (!strcmp(pcParam[0], "bzz")) {

      if (!strcmp(pcValue[0], "on")) {
    	  return "valid";
      }
      else if (!strcmp(pcValue[0], "off")) {
    	  return "valid";
      }
      else return "/404.html";
    }

  }
  else if(!iNumParams) {
	  // zjisti stav co tam je systemove a nastav ho
	  return "/index.html";
  }

  return "/404.html";
}

/* nastav handler na httpd */
void cgi_ex_init(void) {
  http_set_cgi_handlers(cgi_handlers, LWIP_ARRAYSIZE(cgi_handlers));
}

/* GET END */

int fs_open_custom(struct fs_file *file, const char *name) {

/* pokud se ma zobrazit defaultni index */
  if (!strcmp(name, "valid")) {
    /* initialize fs_file correctly */

    memset(file, 0, sizeof(struct fs_file));
    file->pextension = mem_malloc(sizeof(generated_html));
    if (file->pextension != NULL) {
      /* instead of doing memcpy, you would generate e.g. a JSON here */
      memcpy(file->pextension, generated_html, sizeof(generated_html));
      file->data = (const char *)file->pextension;
      file->len = sizeof(generated_html) - 1; /* don't send the trailing 0 */
      file->index = file->len;
      /* allow persisteng connections */
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

#if LWIP_HTTPD_FS_ASYNC_READ
u8_t fs_canread_custom(struct fs_file *file);
u8_t fs_wait_read_custom(struct fs_file *file, fs_wait_cb callback_fn, void *callback_arg);
int fs_read_async_custom(struct fs_file *file, char *buffer, int count, fs_wait_cb callback_fn, void *callback_arg);
#else /* LWIP_HTTPD_FS_ASYNC_READ */
int fs_read_custom(struct fs_file *file, char *buffer, int count);
#endif /* LWIP_HTTPD_FS_ASYNC_READ */

/*-----------------------------------------------------------------------------------*/
err_t fs_open(struct fs_file *file, const char *name) {
  const struct fsdata_file *f;

  if ((file == NULL) || (name == NULL)) {
     return ERR_ARG;
  }

#if LWIP_HTTPD_CUSTOM_FILES
  if (fs_open_custom(file, name)) {
    file->is_custom_file = 1;
    return ERR_OK;
  }
  file->is_custom_file = 0;
#endif

  for (f = FS_ROOT; f != NULL; f = f->next) {
    if (!strcmp(name, (const char *)f->name)) {
      file->data = (const char *)f->data;
      file->len = f->len;
      file->index = f->len;
      file->pextension = NULL;
      file->flags = f->flags;
#if LWIP_HTTPD_FILE_STATE
      file->state = fs_state_init(file, name);
#endif
      return ERR_OK;
    }
  }

  /* file not found */
  return ERR_VAL;
}

/*-----------------------------------------------------------------------------------*/
void fs_close(struct fs_file *file)
{
#if LWIP_HTTPD_CUSTOM_FILES
  if (file->is_custom_file) {
    fs_close_custom(file);
  }
#endif /* LWIP_HTTPD_CUSTOM_FILES */
#if LWIP_HTTPD_FILE_STATE
  fs_state_free(file, file->state);
#endif /* #if LWIP_HTTPD_FILE_STATE */
  LWIP_UNUSED_ARG(file);
}
/*-----------------------------------------------------------------------------------*/
#if LWIP_HTTPD_DYNAMIC_FILE_READ
#if LWIP_HTTPD_FS_ASYNC_READ
int
fs_read_async(struct fs_file *file, char *buffer, int count, fs_wait_cb callback_fn, void *callback_arg)
#else /* LWIP_HTTPD_FS_ASYNC_READ */
int
fs_read(struct fs_file *file, char *buffer, int count)
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
{
  int read;
  if(file->index == file->len) {
    return FS_READ_EOF;
  }
#if LWIP_HTTPD_FS_ASYNC_READ
  LWIP_UNUSED_ARG(callback_fn);
  LWIP_UNUSED_ARG(callback_arg);
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
#if LWIP_HTTPD_CUSTOM_FILES
  if (file->is_custom_file) {
#if LWIP_HTTPD_FS_ASYNC_READ
    return fs_read_async_custom(file, buffer, count, callback_fn, callback_arg);
#else /* LWIP_HTTPD_FS_ASYNC_READ */
    return fs_read_custom(file, buffer, count);
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
  }
#endif /* LWIP_HTTPD_CUSTOM_FILES */

  read = file->len - file->index;
  if(read > count) {
    read = count;
  }

  MEMCPY(buffer, (file->data + file->index), read);
  file->index += read;

  return(read);
}
#endif /* LWIP_HTTPD_DYNAMIC_FILE_READ */
/*-----------------------------------------------------------------------------------*/
#if LWIP_HTTPD_FS_ASYNC_READ
int
fs_is_file_ready(struct fs_file *file, fs_wait_cb callback_fn, void *callback_arg)
{
  if (file != NULL) {
#if LWIP_HTTPD_FS_ASYNC_READ
#if LWIP_HTTPD_CUSTOM_FILES
    if (!fs_canread_custom(file)) {
      if (fs_wait_read_custom(file, callback_fn, callback_arg)) {
        return 0;
      }
    }
#else /* LWIP_HTTPD_CUSTOM_FILES */
    LWIP_UNUSED_ARG(callback_fn);
    LWIP_UNUSED_ARG(callback_arg);
#endif /* LWIP_HTTPD_CUSTOM_FILES */
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
  }
  return 1;
}
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
/*-----------------------------------------------------------------------------------*/
int
fs_bytes_left(struct fs_file *file)
{
  return file->len - file->index;
}
