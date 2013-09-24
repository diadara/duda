/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Duda I/O
 *  --------
 *  Copyright (C) 2012-2013, Eduardo Silva P. <edsiper@gmail.com>
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

#include "duda_api.h"
#include "duda_package.h"
#include "unqlite.h"
#include "kv.h"

/*
 * Initialize the Key-Value store engine in memory
 */
int kv_init(unqlite **conx)
{
    return unqlite_open(conx, ":mem:", UNQLITE_OPEN_IN_MEMORY);
}

struct duda_api_kv *get_kv_api()
{
    struct duda_api_kv *kv;

    /* Alloc object */
    kv = malloc(sizeof(struct duda_api_kv));
    kv->init           = kv_init;
    kv->store          = unqlite_kv_store;
    kv->store_fmt      = unqlite_kv_store_fmt;
    kv->append         = unqlite_kv_append;
    kv->append_fmt     = unqlite_kv_append_fmt;
    kv->fetch          = unqlite_kv_fetch;
    kv->fetch_callback = unqlite_kv_fetch_callback;
    kv->delete         = unqlite_kv_delete;

    /* Cursor / Iterator */
    kv->cursor_init    = unqlite_kv_cursor_init;
    kv->cursor_release = unqlite_kv_cursor_release;
    kv->cursor_reset   = unqlite_kv_cursor_reset;
    kv->cursor_valid   = unqlite_kv_cursor_valid_entry;
    kv->cursor_first   = unqlite_kv_cursor_first_entry;
    kv->cursor_last    = unqlite_kv_cursor_last_entry;
    kv->cursor_next    = unqlite_kv_cursor_next_entry;
    kv->cursor_prev    = unqlite_kv_cursor_prev_entry;
    kv->cursor_key     = unqlite_kv_cursor_key;
    kv->cursor_data    = unqlite_kv_cursor_data;

    return kv;
}

duda_package_t *duda_package_main()
{
    duda_package_t *dpkg;

    /* Package object */
    dpkg = monkey->mem_alloc(sizeof(duda_package_t));
    dpkg->name    = "kv";
    dpkg->version = "0.1";
    dpkg->api     = get_kv_api();

    return dpkg;
}
