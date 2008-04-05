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

#include "../global.h"
#include "funge-space.h"
#include "../../lib/libghthash/ght_hash_table.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#if defined(_POSIX_ADVISORY_INFO) && (_POSIX_ADVISORY_INFO != -1)
#  include <fcntl.h>
#endif

#define FUNGESPACEINITIALSIZE 150000

typedef struct _fungeSpace {
	// These two form a rectangle for the program size
	fungePosition     topLeftCorner;
	fungePosition     bottomRightCorner;
	// And this is the hash table.
	ght_hash_table_t *entries;
} fungeSpace;

// Funge-space storage.
static fungeSpace *fspace = NULL;

/**
 * Check if position is in range.
 */
__attribute__((pure, FUNGE_IN_FAST))
static inline bool FungeSpaceInRange(const fungePosition * restrict position)
{
	if ((position->x > fspace->bottomRightCorner.x) || (position->x < fspace->topLeftCorner.x))
		return false;
	if ((position->y > fspace->bottomRightCorner.y) || (position->y < fspace->topLeftCorner.y))
		return false;
	return true;
}

FUNGE_FAST bool
FungeSpaceCreate(void)
{
	fspace = (fungeSpace*)cf_malloc(sizeof(fungeSpace));
	if (!fspace)
		return false;
	fspace->entries = ght_create(FUNGESPACEINITIALSIZE);
	if (!fspace->entries)
		return false;
	ght_set_rehash(fspace->entries, true);

	fspace->topLeftCorner.x = 0;
	fspace->topLeftCorner.y = 0;
	fspace->bottomRightCorner.x = 0;
	fspace->bottomRightCorner.y = 0;
	return true;
}


FUNGE_FAST void
FungeSpaceFree(void)
{
	if (!fspace)
		return;
	ght_finalize(fspace->entries);
	cf_free(fspace);
}

FUNGE_FAST void
FungeSpaceGetBoundRect(fungeRect * restrict rect)
{
	rect->x = fspace->topLeftCorner.x;
	rect->y = fspace->topLeftCorner.y;
	rect->w = fspace->bottomRightCorner.x - fspace->topLeftCorner.x;
	rect->h = fspace->bottomRightCorner.y - fspace->topLeftCorner.y;
}


FUNGE_FAST FUNGEDATATYPE
FungeSpaceGet(const fungePosition * restrict position)
{
	FUNGEDATATYPE *tmp;

	assert(position != NULL);

	tmp = (FUNGEDATATYPE*)ght_get(fspace->entries, position);
	if (!tmp)
		return (FUNGEDATATYPE)' ';
	else
		return *tmp;
}


FUNGE_FAST FUNGEDATATYPE
FungeSpaceGetOff(const fungePosition * restrict position, const fungePosition * restrict offset)
{
	fungePosition tmp;
	FUNGEDATATYPE *result;

	assert(position != NULL);
	assert(offset != NULL);

	tmp.x = position->x + offset->x;
	tmp.y = position->y + offset->y;

	result = (FUNGEDATATYPE*)ght_get(fspace->entries, &tmp);
	if (!result)
		return (FUNGEDATATYPE)' ';
	else
		return *result;
}

FUNGE_FAST static inline void
FungeSpaceSetNoBoundUpdate(FUNGEDATATYPE value, const fungePosition * restrict position)
{
	assert(position != NULL);
	if (value == ' ') {
		ght_remove(fspace->entries, position);
	} else {
		// Reuse cell if it exists
		FUNGEDATATYPE *tmp;
		if ((tmp = (FUNGEDATATYPE*)ght_get(fspace->entries, position)) != NULL) {
			*tmp = value;
		} else {
			if (ght_insert(fspace->entries, value, position) == -1) {
				ght_replace(fspace->entries, value, position);
			}
		}
	}
}

FUNGE_FAST void
FungeSpaceSet(FUNGEDATATYPE value, const fungePosition * restrict position)
{
	assert(position != NULL);
	FungeSpaceSetNoBoundUpdate(value, position);
	if (fspace->bottomRightCorner.y < position->y)
		fspace->bottomRightCorner.y = position->y;
	if (fspace->bottomRightCorner.x < position->x)
		fspace->bottomRightCorner.x = position->x;
	if (fspace->topLeftCorner.y > position->y)
		fspace->topLeftCorner.y = position->y;
	if (fspace->topLeftCorner.x > position->x)
		fspace->topLeftCorner.x = position->x;
}

FUNGE_FAST void
FungeSpaceSetOff(FUNGEDATATYPE value, const fungePosition * restrict position, const fungePosition * restrict offset)
{
	assert(position != NULL);
	assert(offset != NULL);

	FungeSpaceSet(value, VectorCreateRef(position->x + offset->x, position->y + offset->y));
}

FUNGE_FAST void
FungeSpaceWrap(fungePosition * restrict position, const fungeVector * restrict delta)
{
	// Quick and dirty if cardinal.
	if (VectorIsCardinal(delta)) {
		if (position->x < fspace->topLeftCorner.x)
			position->x = fspace->bottomRightCorner.x;
		else if (position->x >= fspace->bottomRightCorner.x)
			position->x = fspace->topLeftCorner.x;

		if (position->y < fspace->topLeftCorner.y)
			position->y = fspace->bottomRightCorner.y;
		else if (position->y >= fspace->bottomRightCorner.y)
			position->y = fspace->topLeftCorner.y;
	} else {
		if (!FungeSpaceInRange(position)) {
			do {
				position->x -= delta->x;
				position->y -= delta->y;
			} while (FungeSpaceInRange(position));
			position->x += delta->x;
			position->y += delta->y;
		}
	}
}

#ifndef NDEBUG
/*************
 * Debugging *
 *************/


// For use with call in gdb
void FungeSpaceDump(void) __attribute__((unused));

void FungeSpaceDump(void)
{
	if (!fspace)
		return;
	fprintf(stderr, "Fungespace follows:\n");
	for (FUNGEVECTORTYPE y = 0; y <= fspace->bottomRightCorner.y; y++) {
		for (FUNGEVECTORTYPE x = 0; x <= fspace->bottomRightCorner.x; x++)
			fprintf(stderr, "%c", (char)FungeSpaceGet(VectorCreateRef(x, y)));
		fprintf(stderr, "\n");
	}
	fputs("\n", stderr);
}

#endif


__attribute__((nonnull, always_inline, warn_unused_result, FUNGE_IN_FAST))
static inline FILE * FungeSpaceOpenFile(const char * restrict filename)
{
	FILE * file;

	assert(filename != NULL);

	file = fopen(filename, "r");
	if (!file) {
		return NULL;
	} else {
#if defined(_POSIX_ADVISORY_INFO) && (_POSIX_ADVISORY_INFO != -1)
		// Microoptimizing! Remove this if it bothers you.
		{
			int fd = fileno(file);
			posix_fadvise(fd, 0, 0, POSIX_FADV_WILLNEED);
			posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL);
		}
#endif
		return file;
	}
}

FUNGE_FAST bool
FungeSpaceLoad(const char * restrict filename)
{
	FILE * file;
	char * line = NULL;
	size_t linelen = 0;
	bool noendingnewline = true;
	// Row in fungespace
	FUNGEVECTORTYPE y = 0;
	FUNGEVECTORTYPE x = 0;
	assert(filename != NULL);

	file = FungeSpaceOpenFile(filename);
	if (!file)
		return false;

	while (cf_getline(&line, &linelen, file) != -1) {
		for (size_t i = 0; i < (linelen + 1); i++) {
			if (line[i] == '\0') {
				if (fspace->bottomRightCorner.x < x)
					fspace->bottomRightCorner.x = x;
				break;
			} else if (line[i] == '\r' && line[i+1] == '\n') {
				if (fspace->bottomRightCorner.x < x)
					fspace->bottomRightCorner.x = x;
				x = 0;
				y++;
				i++;
				noendingnewline = false;
				continue;
			} else if (line[i] == '\n' || line[i] == '\r') {
				if (fspace->bottomRightCorner.x < x)
					fspace->bottomRightCorner.x = x;
				x = 0;
				y++;
				noendingnewline = false;
				continue;
			}
			FungeSpaceSetNoBoundUpdate((FUNGEDATATYPE)line[i], VectorCreateRef(x, y));
			x++;
			noendingnewline = true;
		}
	}
	if (noendingnewline) y++;
	if (fspace->bottomRightCorner.y < y)
		fspace->bottomRightCorner.y = y;
	fclose(file);
	if (line != NULL)
		cf_free(line);
	return true;
}

FUNGE_FAST bool
FungeSpaceLoadAtOffset(const char          * restrict filename,
                       const fungePosition * restrict offset,
                       fungeVector         * restrict size,
                       bool binary)
{
	FILE * file;
	char * line = NULL;
	size_t linelen = 0;

	FUNGEVECTORTYPE y = 0;
	FUNGEVECTORTYPE x = 0;
	assert(filename != NULL);
	assert(offset != NULL);
	assert(size != NULL);

	file = FungeSpaceOpenFile(filename);
	if (!file)
		return false;

	size->x = 0;
	size->y = 0;

	while (cf_getline(&line, &linelen, file) != -1) {
		for (size_t i = 0; i < (linelen + 1); i++) {
			if (line[i] == '\0') {
				break;
			} else if (!binary && (line[i] == '\r') && (line[i+1] == '\n')) {
				if (x > size->x) size->x = x;
				x = 0;
				y++;
				i++;
				continue;
			} else if (!binary && (line[i] == '\n' || line[i] == '\r')) {
				if (x > size->x) size->x = x;
				x = 0;
				y++;
				continue;
			}
			if (line[i] != ' ')
				FungeSpaceSetOff((FUNGEDATATYPE)line[i], VectorCreateRef(x, y), offset);
			x++;
		}
	}
	if (x > size->x) size->x = x;
	if (y > size->y) size->y = y;
	fclose(file);
	if (line != NULL)
		cf_free(line);
	return true;
}

FUNGE_FAST bool
FungeSpaceSaveToFile(const char          * restrict filename,
                     const fungePosition * restrict offset,
                     const fungeVector   * restrict size,
                     bool textfile)
{
	FILE * file;

	FUNGEDATATYPE value;

	FUNGEVECTORTYPE maxy = offset->y + size->y;
	FUNGEVECTORTYPE maxx = offset->x + size->x;

	assert(filename != NULL);
	assert(offset != NULL);
	assert(size != NULL);

	file = fopen(filename, "w");
	if (!file)
		return false;
	// Microoptimizing! Remove this if it bothers you.
#if defined(_POSIX_ADVISORY_INFO) && (_POSIX_ADVISORY_INFO != -1)
	posix_fallocate(fileno(file), 0, size->y * size->x);
#endif

	// TODO textfile mode
	for (FUNGEVECTORTYPE y = offset->y; y < maxy; y++) {
		for (FUNGEVECTORTYPE x = offset->x; x < maxx; x++) {
			value = FungeSpaceGet(VectorCreateRef(x, y));
			fputc(value, file);
		}
		fputc('\n', file);
	}

	fclose(file);
	return true;
}