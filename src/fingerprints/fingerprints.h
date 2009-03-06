/* -*- mode: C; coding: utf-8; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * cfunge - A standard-conforming Befunge93/98/109 interpreter in C.
 * Copyright (C) 2008-2009 Arvid Norlander <anmaster AT tele2 DOT se>
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
// WARNING THIS FILE IS AUTO GENERATED DON'T CHANGE
// It was generated by tools/gen_fprint_list.sh
// Rerun the tool to update this list
#ifndef MANAGER_INTERNAL
#  error "This is for use in manager.c only"
#endif

#ifndef FUNGE_HAD_SRC_FINGERPRINTS_FINGERPRINTS_H
#define FUNGE_HAD_SRC_FINGERPRINTS_FINGERPRINTS_H

#include "../global.h"

#include <stdbool.h>
#include <stdint.h>

#include "3DSP/3DSP.h"
#include "BASE/BASE.h"
#include "CPLI/CPLI.h"
#include "DATE/DATE.h"
#include "DIRF/DIRF.h"
#include "FILE/FILE.h"
#include "FING/FING.h"
#include "FIXP/FIXP.h"
#include "FPDP/FPDP.h"
#include "FPSP/FPSP.h"
#include "FRTH/FRTH.h"
#include "HRTI/HRTI.h"
#include "INDV/INDV.h"
#include "JSTR/JSTR.h"
#include "MODU/MODU.h"
#include "NCRS/NCRS.h"
#include "NULL/NULL.h"
#include "ORTH/ORTH.h"
#include "PERL/PERL.h"
#include "REFC/REFC.h"
#include "REXP/REXP.h"
#include "ROMA/ROMA.h"
#include "SCKE/SCKE.h"
#include "SOCK/SOCK.h"
#include "STRN/STRN.h"
#include "SUBR/SUBR.h"
#include "TERM/TERM.h"
#include "TIME/TIME.h"
#include "TOYS/TOYS.h"
#include "TURT/TURT.h"

typedef struct s_ImplementedFingerprintEntry {
	const funge_cell         fprint;   /**< Fingerprint. */
	const char            * uri;      /**< URI, used for Funge-109. */
	const fingerprintLoader loader;   /**< Loader function pointer. */
	const char            * opcodes;  /**< Sorted string with all implemented opcodes. */
	const char            * url;      /**< URL, used to show links for more info about fingerprints. */
	const bool              safe;     /**< If true, this fingerprint is safe in sandbox mode. */
} ImplementedFingerprintEntry;

// Implemented fingerprints
// NOTE: Keep sorted!
// Also note that this table is processed by scripts, so keep the .loader and
// .opcodes entries on the same line! As well as in current format.
static const ImplementedFingerprintEntry ImplementedFingerprints[] = {
	// 3DSP - 3D space manipulation extension
	{ .fprint = 0x33445350, .uri = NULL, .loader = &finger_3DSP_load, .opcodes = "ABCDLMNPRSTUVXYZ",
	  .url = "http://www.rcfunge98.com/rcsfingers.html#3DSP", .safe = true },
	// BASE - I/O for numbers in other bases
	{ .fprint = 0x42415345, .uri = NULL, .loader = &finger_BASE_load, .opcodes = "BHINO",
	  .url = "http://www.rcfunge98.com/rcsfingers.html#BASE", .safe = true },
	// CPLI - Complex Integer extension
	{ .fprint = 0x43504c49, .uri = NULL, .loader = &finger_CPLI_load, .opcodes = "ADMOSV",
	  .url = "http://www.rcfunge98.com/rcsfingers.html", .safe = true },
	// DATE - Date Functions
	{ .fprint = 0x44415445, .uri = NULL, .loader = &finger_DATE_load, .opcodes = "ACDJTWY",
	  .url = "http://rcfunge98.com/rcsfingers.html#DATE", .safe = true },
	// DIRF - Directory functions extension
	{ .fprint = 0x44495246, .uri = NULL, .loader = &finger_DIRF_load, .opcodes = "CMR",
	  .url = "http://www.rcfunge98.com/rcsfingers.html", .safe = false },
	// FILE - File I/O functions
	{ .fprint = 0x46494c45, .uri = NULL, .loader = &finger_FILE_load, .opcodes = "CDGLOPRSW",
	  .url = "http://www.rcfunge98.com/rcsfingers.html", .safe = false },
	// FING - Operate on single fingerprint semantics
	{ .fprint = 0x46494e47, .uri = NULL, .loader = &finger_FING_load, .opcodes = "XYZ",
	  .url = "http://www.rcfunge98.com/rcsfingers.html#FING", .safe = true },
	// FIXP - Some useful math functions
	{ .fprint = 0x46495850, .uri = NULL, .loader = &finger_FIXP_load, .opcodes = "ABCDIJNOPQRSTUVX",
	  .url = "http://www.rcfunge98.com/rcsfingers.html", .safe = true },
	// FPDP - Double precision floating point
	{ .fprint = 0x46504450, .uri = NULL, .loader = &finger_FPDP_load, .opcodes = "ABCDEFGHIKLMNPQRSTVXY",
	  .url = "http://www.rcfunge98.com/rcsfingers.html", .safe = true },
	// FPSP - Single precision floating point
	{ .fprint = 0x46505350, .uri = NULL, .loader = &finger_FPSP_load, .opcodes = "ABCDEFGHIKLMNPQRSTVXY",
	  .url = "http://www.rcfunge98.com/rcsfingers.html", .safe = true },
	// FRTH - Some common forth commands
	{ .fprint = 0x46525448, .uri = NULL, .loader = &finger_FRTH_load, .opcodes = "DLOPR",
	  .url = "http://www.rcfunge98.com/rcsfingers.html", .safe = true },
	// HRTI - High-Resolution Timer Interface
	{ .fprint = 0x48525449, .uri = NULL, .loader = &finger_HRTI_load, .opcodes = "EGMST",
	  .url = "http://catseye.tc/projects/funge98/library/HRTI.html", .safe = true },
	// INDV - Pointer functions
	{ .fprint = 0x494e4456, .uri = NULL, .loader = &finger_INDV_load, .opcodes = "GPVW",
	  .url = "http://www.rcfunge98.com/rcsfingers.html", .safe = true },
	// JSTR - Read and write strings in Funge-Space
	{ .fprint = 0x4a535452, .uri = NULL, .loader = &finger_JSTR_load, .opcodes = "GP",
	  .url = "http://www.imaginaryrobots.net/projects/funge/myexts.txt", .safe = true },
	// MODU - Modulo Arithmetic
	{ .fprint = 0x4d4f4455, .uri = NULL, .loader = &finger_MODU_load, .opcodes = "MRU",
	  .url = "http://catseye.tc/projects/funge98/library/MODU.html", .safe = true },
#if defined(HAVE_NCURSES)
	// NCRS - ncurses extension
	{ .fprint = 0x4e435253, .uri = NULL, .loader = &finger_NCRS_load, .opcodes = "BCEGIKMNPRSU",
	  .url = "http://www.imaginaryrobots.net/projects/funge/myexts.txt", .safe = true },
#endif
	// NULL - Null Fingerprint
	{ .fprint = 0x4e554c4c, .uri = NULL, .loader = &finger_NULL_load, .opcodes = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	  .url = "http://catseye.tc/projects/funge98/library/NULL.html", .safe = true },
	// ORTH - Orthogonal Easement Library
	{ .fprint = 0x4f525448, .uri = NULL, .loader = &finger_ORTH_load, .opcodes = "AEGOPSVWXYZ",
	  .url = "http://catseye.tc/projects/funge98/library/ORTH.html", .safe = true },
	// PERL - Generic Interface to the Perl Language
	{ .fprint = 0x5045524c, .uri = NULL, .loader = &finger_PERL_load, .opcodes = "EIS",
	  .url = "http://catseye.tc/projects/funge98/library/PERL.html", .safe = false },
	// REFC - Referenced Cells Extension
	{ .fprint = 0x52454643, .uri = NULL, .loader = &finger_REFC_load, .opcodes = "DR",
	  .url = "http://catseye.tc/projects/funge98/library/REFC.html", .safe = true },
	// REXP - Regular Expression Matching
	{ .fprint = 0x52455850, .uri = NULL, .loader = &finger_REXP_load, .opcodes = "CEF",
	  .url = "http://www.rcfunge98.com/rcsfingers.html#REXP", .safe = true },
	// ROMA - Roman Numerals
	{ .fprint = 0x524f4d41, .uri = NULL, .loader = &finger_ROMA_load, .opcodes = "CDILMVX",
	  .url = "http://catseye.tc/projects/funge98/library/ROMA.html", .safe = true },
	// SCKE - TCP/IP async socket and dns resolving extension
	{ .fprint = 0x53434b45, .uri = NULL, .loader = &finger_SCKE_load, .opcodes = "HP",
	  .url = "http://glfunge98.sourceforge.net/", .safe = false },
	// SOCK - TCP/IP socket extension
	{ .fprint = 0x534f434b, .uri = NULL, .loader = &finger_SOCK_load, .opcodes = "ABCIKLORSW",
	  .url = "http://www.rcfunge98.com/rcsfingers.html#SOCK", .safe = false },
	// STRN - String functions
	{ .fprint = 0x5354524e, .uri = NULL, .loader = &finger_STRN_load, .opcodes = "ACDFGILMNPRSV",
	  .url = "http://www.rcfunge98.com/rcsfingers.html", .safe = true },
	// SUBR - Subroutine extension
	{ .fprint = 0x53554252, .uri = NULL, .loader = &finger_SUBR_load, .opcodes = "ACJOR",
	  .url = "http://www.rcfunge98.com/rcsfingers.html", .safe = true },
#if defined(HAVE_NCURSES)
	// TERM - Terminal control functions
	{ .fprint = 0x5445524d, .uri = NULL, .loader = &finger_TERM_load, .opcodes = "CDGHLSU",
	  .url = "http://www.rcfunge98.com/rcsfingers.html", .safe = true },
#endif
	// TIME - Time and Date functions
	{ .fprint = 0x54494d45, .uri = NULL, .loader = &finger_TIME_load, .opcodes = "DFGHLMOSWY",
	  .url = "http://www.rcfunge98.com/rcsfingers.html", .safe = true },
	// TOYS - Funge-98 Standard Toys
	{ .fprint = 0x544f5953, .uri = NULL, .loader = &finger_TOYS_load, .opcodes = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	  .url = "http://catseye.tc/projects/funge98/library/TOYS.html", .safe = true },
	// TURT - Simple Turtle Graphics Library
	{ .fprint = 0x54555254, .uri = NULL, .loader = &finger_TURT_load, .opcodes = "ABCDEFHILNPQRTU",
	  .url = "http://catseye.tc/projects/funge98/library/TURT.html", .safe = true },
};

#endif
