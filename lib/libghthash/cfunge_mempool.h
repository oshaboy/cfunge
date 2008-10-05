/* -*- mode: C; coding: utf-8; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * cfunge - a conformant Befunge93/98/08 interpreter in C.
 * Copyright (C) 2008 Arvid Norlander <anmaster AT tele2 DOT se>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at the proxy's option) any later version. Arvid Norlander is a
 * proxy who can decide which future versions of the GNU General Public
 * License can be used.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// This file does not come from libght, however this code is for libght
// exclusively.


/**
 * Mempools are used for allocating funge-space s_hash_entry.
 * Since cfunge is single-threaded they are static, and have no locking.
 *
 * For implementation details see comments in cfunge_mempool.c.
 */

#ifndef _HAD_LIB_CFUNGE_MEMPOOL_H
#define _HAD_LIB_CFUNGE_MEMPOOL_H

#include "../../src/global.h"
#include "../../src/funge-space/funge-space.h"

#include "ght_hash_table.h"

#include <stdbool.h>

typedef struct s_hash_entry memorypool_data;

FUNGE_ATTR_FAST
bool mempool_setup(void);

FUNGE_ATTR_FAST
void mempool_teardown(void);

FUNGE_ATTR_FAST FUNGE_ATTR_WARN_UNUSED FUNGE_ATTR_MALLOC
memorypool_data *mempool_alloc(void);

FUNGE_ATTR_FAST
void mempool_free(memorypool_data *ptr);


#endif