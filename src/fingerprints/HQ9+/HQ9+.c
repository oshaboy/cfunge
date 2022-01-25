/* -*- mode: C; coding: utf-8; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * cfunge - A standard-conforming Befunge93/98/109 interpreter in C.
 * Copyright (C) 2008-2013 Arvid Norlander <VorpalBlade AT users.noreply.github.com>
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



#include "../../funge-space/funge-space.h"
#include "HQ9+.h"
#include <stdio.h>
int accumulator;
static void finger_HQ9P_hello(instructionPointer * ip)
{
	(void) ip;
	puts("Hello, World!");
}

static void finger_HQ9P_quine(instructionPointer * ip)
{
	(void)ip;
	fungeRect rect;
	funge_vector vect;
	funge_cell cell;
	fungespace_get_bounds_rect(&rect);
	//printf("rect.h=%d rect.y=%d\n", rect.h, rect.y);
	for (vect.y=rect.y; vect.y<=rect.y+rect.h; vect.y++){
		for (vect.x=rect.x; vect.x<=rect.x+rect.w; vect.x++){
			 cell = fungespace_get(&vect);
			 putchar((int)cell);
		}
		printf("\n");
	}
}
static void finger_HQ9P_bottles(instructionPointer * ip)
{
	(void) ip;
	int bottles=99;
	while (bottles>0){
		printf("%d Bottle%s of beer on the wall\n",bottles, bottles==1?"s":"");
		printf("%d Bottle%s of beer\n",bottles, bottles==1?"s":"");
		printf("Take one down, pass it around\n");
		bottles--;
		if (bottles==0){
			printf("There are no more bottles of beer on the wall\n\n");
		} else{
			printf("%d Bottle%s of beer on the wall\n\n",bottles, bottles==1?"":"s");
		}
	}
	printf("There are no more bottles of beer on the wall\n");
	printf("There are no more bottles of beer\n");
	printf("Go to the store, buy some more\n");
	printf("99 Bottles of beer on the wall\n\n");
}

static void finger_HQ9P_plus(instructionPointer * ip)
{
	(void) ip;
	accumulator++;
}

bool finger_HQ9P_load(instructionPointer * ip)
{
	manager_add_opcode(HQ9P, 'H', hello);
	manager_add_opcode(HQ9P, 'Q', quine);
	manager_add_opcode(HQ9P, 'N', bottles);
	manager_add_opcode(HQ9P, 'P', plus);
	accumulator=0;
	return true;
}
