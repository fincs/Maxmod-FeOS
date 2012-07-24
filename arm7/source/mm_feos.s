//
// Maxmod for FeOS
//
// A perfect excuse for writing stuff in ARM ASM!

.arch armv4t
.text

.global arm7_main
.hidden arm7_main
.thumb_func
.align 2

arm7_main: @ int arm7_main(int fifoCh)
	push {lr}

	push {r0}
	bl _soundOn
	pop {r0}

	adr r1, .Ltemp
	str r0, [r1]
	bl mmInstall

	pop {r3}
	mov r0, #0
	bx r3

.global arm7_fini
.hidden arm7_fini
.thumb_func
.align 2

arm7_fini:
	push {lr}

	mov r0, #0x08
	mov r1, #0
	bl irqSet

	adr r0, .Ltemp
	ldr r0, [r0]
	mov r1, #0
	mov r2, #0
	bl fifoSetDatamsgHandler

	adr r0, .Ltemp
	ldr r0, [r0]
	mov r1, #0
	mov r2, #0
	bl fifoSetValue32Handler

	bl _soundOff

	pop {r3}
	bx r3

.align 2
.Ltemp:
	.word 0

.global _call_via_r1
.hidden _call_via_r1
.align 2
.thumb_func
_call_via_r1:
	bx r1

.global _call_via_r3
.hidden _call_via_r3
.align 2
.thumb_func
_call_via_r3:
	bx r3
