/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Duda I/O
 *  --------
 *  Copyright (C) 2012-2014, Eduardo Silva P. <edsiper@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef DUDA_EVENT_H
#define DUDA_EVENT_H

#include "MKPlugin.h"
#include "duda.h"
#include "duda_objects.h"

#define DUDA_EVENT_READ             MK_EPOLL_READ
#define DUDA_EVENT_WRITE            MK_EPOLL_WRITE
#define DUDA_EVENT_RW               MK_EPOLL_RW
#define DUDA_EVENT_SLEEP            MK_EPOLL_SLEEP
#define DUDA_EVENT_WAKEUP           MK_EPOLL_WAKEUP
#define DUDA_EVENT_LEVEL_TRIGGERED  MK_EPOLL_LEVEL_TRIGGERED
#define DUDA_EVENT_EDGE_TRIGGERED   MK_EPOLL_EDGE_TRIGGERED

/* Return values for a given callback through events interface */
#define DUDA_EVENT_OWNED            MK_PLUGIN_RET_EVENT_OWNED
#define DUDA_EVENT_CLOSE            MK_PLUGIN_RET_EVENT_CLOSE
#define DUDA_EVENT_CONTINUE         MK_PLUGIN_RET_EVENT_CONTINUE

/* Thread key to map the event lists per worker */
pthread_key_t duda_events_list;

struct duda_event_signal_channel {
    int fd_r;
    int fd_w;
    struct mk_list _head;
};

struct mk_list duda_event_signals_list;

struct duda_event_handler {
    int sockfd;
    int mode;
    int behavior;

    int (*cb_on_read) (int, void *);
    int (*cb_on_write) (int, void *);
    int (*cb_on_error) (int, void *);
    int (*cb_on_close) (int, void *);
    int (*cb_on_timeout) (int, void *);

    void *cb_data;
    struct mk_list _head;
};

struct duda_api_event {
    int (*add) (int,
                int, int,
                int (*cb_on_read) (int, void *),
                int (*cb_on_write) (int, void *),
                int (*cb_on_error) (int, void *),
                int (*cb_on_close) (int, void *),
                int (*cb_on_timeout) (int, void *),
                void *);
    struct duda_event_handler *(*lookup) (int);
    int (*mode) (int, int, int);
    int (*delete) (int);
    int (*signal) (uint64_t);
    int (*create_signal_fd) ();
};

/* Export an API object */
struct duda_api_event *duda_event_object();

/* Register a new event into Duda events handler */
int duda_event_add(int sockfd,
                   int mode, int behavior,
                   int (*cb_on_read) (int sockfd,  void *data),
                   int (*cb_on_write) (int sockfd, void *data),
                   int (*cb_on_error) (int sockfd, void *data),
                   int (*cb_on_close) (int sockfd, void *data),
                   int (*cb_on_timeout) (int sockfd, void *data),
                   void *cb_data);

/* Lookup a specific event_handler through it socket descriptor */
struct duda_event_handler *duda_event_lookup(int sockfd);

/* Change the file descriptor mode and behavior */
int duda_event_mode(int sockfd, int mode, int behavior);

/* Delete an event_handler from the thread list */
int duda_event_delete(int sockfd);

/* Emit a signal to all workers */
int duda_event_signal(uint64_t val);
int duda_event_create_signal_fd();


static inline void duda_event_set_signal_callback(void (*func) (int, uint64_t))
{
    _setup.event_signal_cb = func;
}


/* internal functions */
int duda_event_fd_read(int fd, void *data);

#endif
