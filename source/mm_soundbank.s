/****************************************************************************
 *                                                          __              *
 *                ____ ___  ____ __  ______ ___  ____  ____/ /              *
 *               / __ `__ \/ __ `/ |/ / __ `__ \/ __ \/ __  /               *
 *              / / / / / / /_/ />  </ / / / / / /_/ / /_/ /                *
 *             /_/ /_/ /_/\__,_/_/|_/_/ /_/ /_/\____/\__,_/                 *
 *                                                                          *
 *                          Soundbank Interface                             *
 *                                                                          *
 *         Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)         *
 *                                                                          *
 * Permission to use, copy, modify, and/or distribute this software for any *
 * purpose with or without fee is hereby granted, provided that the above   *
 * copyright notice and this permission notice appear in all copies.        *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES *
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF         *
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR  *
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES   *
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN    *
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF  *
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.           *
 ****************************************************************************/

// THIS FILE WAS EXTRAORDINARILY EDITED BY fincs
// The mmInitDefault* functions have been rewritten in C, because the official
// implementations horribly sucked (look at mm_stuff.c for more details).

#include "mp_defs.inc"

//---------------------------------------------------------------------
	.TEXT
	.THUMB
	.ALIGN 2
//---------------------------------------------------------------------

/**********************************************************************
 * mmSetCustomSoundBankHandler
 *
 * Setup a custom soundbank interface
 **********************************************************************/
						.global mmSetCustomSoundBankHandler
						.thumb_func
mmSetCustomSoundBankHandler:

	ldr	r1,=mmcbMemory			// save handle
	str	r0, [r1]			//
	bx	lr				//
	
.pool
