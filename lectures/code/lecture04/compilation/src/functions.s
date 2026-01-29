	.file	"functions.i"
	.option nopic
	.attribute arch, "rv32i2p1_m2p0_a2p1_c2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.globl	__mulsf3
	.align	1
	.globl	function
	.type	function, @function
function:
	addi	sp,sp,-32
	sw	ra,28(sp)
	sw	s0,24(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	call	pi_squared
	mv	a5,a0
	lw	a1,-20(s0)
	mv	a0,a5
	call	__mulsf3
	mv	a5,a0
	mv	a0,a5
	lw	ra,28(sp)
	lw	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	function, .-function
	.ident	"GCC: ('riscv32-embecosm-gcc-win64-20230813') 14.0.0 20230812 (experimental)"
