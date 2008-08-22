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
/**
 * @file
 * Definition of, and functions for, a Funge vector.
 */
#ifndef _HAD_SRC_VECTOR_H
#define _HAD_SRC_VECTOR_H

#include "global.h"
#include <stdbool.h>

/// A vector in Funge-Space
typedef struct s_fungeVector {
	fungeCell x; ///< You should be able to guess what this is.
	fungeCell y; ///< You should be able to guess what this is.
} fungeVector;

/// Useful to create a vector in a list of parameter for example.
/// The vector is created on the stack.
/// @param a This should be the x value.
/// @param b This should be the y value.
/// @return A pointer to a vector allocated on the stack, so no need to free it.
#define VectorCreateRef(a, b) (& (fungeVector) { .x = (a), .y = (b) })

/**
 * Checks if vector is cardinal (as in ^>v<).
 * @param v The vector to check.
 */
FUNGE_ATTR_PURE FUNGE_ATTR_FAST
bool VectorIsCardinal(const fungeVector * v);

#endif
