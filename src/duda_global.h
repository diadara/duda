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

#ifndef DUDA_GLOBAL_H
#define DUDA_GLOBAL_H

#include "MKPlugin.h"
#include <pthread.h>

typedef struct {
    pthread_key_t key;          /* Pthread key unique identifier */
    void *(*callback) (void *); /* Return the value assigned to the global variable */
    void *data;                 /* the optional data passed to the callback */
    struct mk_list _head;
} duda_global_t;

struct duda_global_dist_t {
    duda_global_t *key;
    void *(*callback) ();

    struct mk_list _head;
};

#define DUDA_GLOBAL_EXCEPTION "You can only define globals inside duda_init() or duda_package_main()"

/* Global data (thread scope) */
struct duda_api_global {
    void  (*init) (duda_global_t *, void *(*callback)(void *), void *data);
    int   (*set)   (duda_global_t, const void *);
    void *(*get)   (duda_global_t);
};

/* This list FIXME! */
struct mk_list duda_global_pkg;

int duda_global_set(duda_global_t key, const void *data);
void *duda_global_get(duda_global_t key);
struct duda_api_global *duda_global_object();

#endif
