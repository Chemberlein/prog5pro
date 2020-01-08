	.cpu arm7tdmi
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 18, 4
	.file	"simple2.c"
	.section	.rodata
	.align	2
.LC0:
	.ascii	"hello world\000"
	.text
	.align	2
	.global	func
	.type	func, %function
func:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	ldr	r0, .L2
	bl	puts
	ldmfd	sp!, {fp, pc}
.L3:
	.align	2
.L2:
	.word	.LC0
	.size	func, .-func
	.align	2
	.global	func2
	.type	func2, %function
func2:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	bl	func
	ldmfd	sp!, {fp, pc}
	.size	func2, .-func2
	.ident	"GCC: (GNU) 4.5.3"
