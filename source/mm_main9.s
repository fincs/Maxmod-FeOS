/****************************************************************************
 *                                                          __              *
 *                ____ ___  ____ __  ______ ___  ____  ____/ /              *
 *               / __ `__ \/ __ `/ |/ / __ `__ \/ __ \/ __  /               *
 *              / / / / / / /_/ />  </ / / / / / /_/ / /_/ /                *
 *             /_/ /_/ /_/\__,_/_/|_/_/ /_/ /_/\____/\__,_/                 *
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
 
#include "mp_defs.inc"
#include "mp_macros.inc"

.equ	FIFO_MAXMOD,	3

.struct 0		// mm_ds9_system
MMDS9S_MOD_COUNT:	.space 4
MMDS9S_SAMP_COUNT:	.space 4
MMDS9S_MEM_BANK:	.space 4
MMDS9S_FIFO_CHANNEL:	.space 4

//-----------------------------------------------------------------------------
	.BSS
	.ALIGN
//-----------------------------------------------------------------------------

/******************************************************************************
 * mmcbMemory : Word
 *
 * Function pointer to soundbank operation callback
 ******************************************************************************/
							.global mmcbMemory
							.hidden mmcbMemory
mmcbMemory:	.space 4

/******************************************************************************
 * mmModuleCount : Word
 *
 * Number of modules in sound bank
 ******************************************************************************/
							.global mmModuleCount
							.hidden mmModuleCount
mmModuleCount:	.space 4

/******************************************************************************
 * mmSampleCount : Word
 *
 * Number of samples in sound bank
 ******************************************************************************/
							.global mmSampleCount
							.hidden mmSampleCount
mmSampleCount:	.space 4

/******************************************************************************
 * mmModuleBank : Word
 *
 * Address of module bank
 ******************************************************************************/
							.global mmMemoryBank
							.hidden mmMemoryBank
							.global mmModuleBank
							.hidden mmModuleBank
mmMemoryBank:
mmModuleBank:	.space 4

/******************************************************************************
 * mmSampleBank : Word
 *
 * Address of sample bank
 ******************************************************************************/
							.global mmSampleBank
							.hidden mmSampleBank
mmSampleBank:	.space 4

/******************************************************************************
 * mmCallback : Word
 *
 * Pointer to event handler
 ******************************************************************************/
							.global mmCallback
							.hidden mmCallback
mmCallback: 	.space 4

/******************************************************************************
 * mmActiveStatus : Byte
 *
 * Record of playing status.
 ******************************************************************************/
							.global mmActiveStatus
							.hidden mmActiveStatus
mmActiveStatus:	.space 1

//-----------------------------------------------------------------------------
	.TEXT
	.THUMB
	.ALIGN 2
//-----------------------------------------------------------------------------

/******************************************************************************
 * mmActive
 *
 * Returns nonzero if module is playing
 ******************************************************************************/
						.global mmActive
						.thumb_func
mmActive:
	
	ldr	r0,=mmActiveStatus
	ldrb	r0, [r0]
	bx	lr

/******************************************************************************
 * mmSetEventHandler
 *
 * Set function for handling playback events
 ******************************************************************************/
						.global mmSetEventHandler
						.thumb_func
mmSetEventHandler:

	ldr	r1,=mmCallback
	str	r0, [r1]
	bx	lr

/******************************************************************************
 * mmInit( system )
 *
 * Initialize Maxmod (manual settings)
 ******************************************************************************/
						.global mmInit
						.thumb_func
mmInit:
	
	push	{r4-r7, lr}			// preserve registers
	mov	r7, r0				// r7 = system

	ldmia	r0!, {r1-r3}			// r1,r2,r3,r4 = mod_count, samp_count, mod_bank, samp_bank
	lsl	r4, r1, #2			//
	add	r4, r3				//
	ldr	r5,=mmModuleCount		// write to local memory
	stmia	r5!, {r1-r4}			//
	
	add	r1, r2				// clear the memory bank to zero
	beq	2f
	mov	r0, #0				//
1:	stmia	r3!, {r0}			//
	sub	r1, #1				//
	bne	1b				//
2:
	
	ldr	r0, [r7, #MMDS9S_FIFO_CHANNEL]	// setup communications
	bl	mmSetupComms			//

	ldr	r0, [r7, #MMDS9S_MOD_COUNT]	// send soundbank info to ARM7
	ldr	r1, [r7, #MMDS9S_MEM_BANK]
	bl	mmSendBank

	pop	{r4, r5,r6,r7, pc}		// pop regs and return

// THIS PART WAS EXTRAORDINARILY EDITED BY fincs
// The mmInitDefault* functions have been rewritten in C, because the official
// implementations horribly sucked (look at mm_stuff.c for more details).

#ifdef FEOS_MAXMOD_ENABLESTREAMING

/******************************************************************************
 * mmSuspendIRQ_t
 *
 * Function to disable interrupts via the status register
 ******************************************************************************/
						.global mmSuspendIRQ_t
						.hidden mmSuspendIRQ_t
						.thumb_func
mmSuspendIRQ_t:
	push {lr}
	bl FeOS_SuspendIRQ_t
	adr r1, previous_irq_state
	str r0, [r1]
	pop {pc}

/******************************************************************************
 * mmRestoreIRQ_t
 *
 * Function to enable interrupts via the status register
 ******************************************************************************/	
						.global	mmRestoreIRQ_t
						.hidden mmRestoreIRQ_t
						.thumb_func
mmRestoreIRQ_t:
	push {lr}
	adr r0, previous_irq_state
	ldr r0, [r0]
	bl FeOS_RestoreIRQ_t
	pop {pc}

	.align 2
previous_irq_state:
	.space	4

#endif

.pool
