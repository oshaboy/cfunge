/*
 * cfunge08 - a conformant Befunge93/98/08 interpreter in C.
 * Copyright (C) 2008 Arvid Norlander <anmaster AT tele2 DOT se>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../../global.h"
#include "funge-space.h"

#include <string.h>
#include <stdio.h>

struct _fungeSpace {
	FUNGEDATATYPE entries[25][80];
};

static inline bool fungeSpaceInRange(const fungePosition * position)
{
	if ((position->x > 79) || (position->x < 0))
		return false;
	if ((position->y > 24) || (position->y < 0))
		return false;
	return true;
}

fungeSpace*
fungeSpaceCreate(void)
{
	fungeSpace * tmp = cf_malloc(sizeof(fungeSpace));
	for (int y = 0; y < 25; y++)
		for (int x = 0; x < 80; x++)
			tmp->entries[y][x]=(FUNGEDATATYPE)' ';
	return tmp;
}


void
fungeSpaceFree(fungeSpace * me)
{
	cf_free(me);
}


FUNGEDATATYPE
fungeSpaceGet(fungeSpace * me, const fungePosition * position)
{
	if (!fungeSpaceInRange(position))
		position = fungeSpaceWrap(me, position);
	// Thanks to Zaba for suggesting this code
	return me->entries[position->y][position->x];
}


void
fungeSpaceSet(fungeSpace * me, FUNGEDATATYPE value, const fungePosition * position)
{
	if (!fungeSpaceInRange(position))
		position = fungeSpaceWrap(me, position);
	me->entries[position->y][position->x] = value;
}


fungePosition *
fungeSpaceWrap(__attribute__((unused)) fungeSpace * me, const fungePosition * position)
{
	fungePosition *tmp = cf_malloc(sizeof(position));
	// Fix this for less than -80
	if (position->x < 0)
		tmp->x = 80 + position->x;
	else
		tmp->x = position->x % 80;

	if (position->y < 0)
		tmp->y = 25 + position->y;
	else
		tmp->y = position->y % 25;

	return tmp;
}

void
fungeSpaceWrapInPlace(__attribute__((unused)) fungeSpace * me, fungePosition * position)
{
	// Fix this for less than -80
	if (position->x < 0)
		position->x = 80 + position->x;
	else
		position->x = position->x % 80;

	if (position->y < 0)
		position->y = 25 + position->y;
	else
		position->y = position->y % 25;
}


bool
fungeSpaceLoad(fungeSpace * me, const char * filename)
{
	FILE * file;
	char * line;
	// Row in fungespace
	int    y = 0;
	int    x = 0;

	file = fopen(filename, "r");
	if (file == NULL)
		return false;

	line = cf_malloc(81 * sizeof(char));

	while ((y < 25) && (fgets(line, 81, file) != NULL)) {
		for (size_t i = 0; i < (strlen(line) + 1); i++) {
			if (line[i] == '\0') {
				break;
			} else if (line[i] == '\n') {
				x = 0;
				y++;
				continue;
			}
			if (i < 80)
				me->entries[y][x] = (FUNGEDATATYPE)line[i];
			x++;
		}
	}

	return true;
}