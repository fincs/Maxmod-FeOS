//
// Maxmod for FeOS
//
// A perfect excuse for writing stuff in ARM ASM!

.arch armv5te
.text

.global mmLoadARM7
.hidden mmLoadARM7
.thumb_func
.align 2

mmLoadARM7:
	push {lr}
	ldr r0, =_mmARM7name
	ldr r1, =_mmChannel
	bl FeOS_LoadARM7
	cmp r0, #0
	beq .Lret
	ldr r1, =_mmARM7handle
	str r0, [r1]
	mov r0, #1
.Lret:
	ldr r1, =_mmDidLoadARM7
	str r0, [r1]
	pop {pc}

.global mmIsARM7Loaded
.thumb_func
.align 2

mmIsARM7Loaded:
	ldr r0, =_mmDidLoadARM7
	ldr r0, [r0]
	bx lr

.align 2
_mmDidLoadARM7:
	.word 0

.global mmFreeARM7
.hidden mmFreeARM7
.thumb_func
.align 2

mmFreeARM7:
	push {lr}
	ldr r0, =_mmDidLoadARM7
	ldr r0, [r0]
	cmp r0, #0
	beq .Lret2

	ldr r0, =_mmARM7handle
	ldr r0, [r0]
	ldr r1, =_mmChannel
	ldr r1, [r1]
	push {r1}
	bl FeOS_FreeARM7
	pop {r0}
	mov r1, #0
	mov r2, #0
	bl FeOS_FifoSetValue32Handler
.Lret2:
	pop {pc}

.global _mmChannel, _mmARM7handle
.hidden _mmChannel, _mmARM7handle
.align 2
_mmChannel:
	.word 0
_mmARM7handle:
	.word 0

_mmARM7name:
	.asciz "/data/FeOS/arm7/maxmod7.fx2"

.section .init_array, "aw", %init_array
.align 2
.word mmLoadARM7(target1)

.section .fini_array, "aw", %fini_array
.align 2
.word mmFreeARM7(target1)
