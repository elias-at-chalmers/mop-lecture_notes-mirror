	.file	"main.c"
	.option nopic
	.attribute arch, "rv32i2p1_m2p0_f2p2_zicsr2p0_zmmul1p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.globl	src
	.data
	.align	2
	.type	src, @object
	.size	src, 40
src:
	.word	0
	.word	1
	.word	2
	.word	3
	.word	4
	.word	5
	.word	6
	.word	7
	.word	8
	.word	9
	.globl	src_backup
	.bss
	.align	2
	.type	src_backup, @object
	.size	src_backup, 40
src_backup:
	.zero	40
	.globl	num_elements
	.section	.srodata,"a"
	.align	2
	.type	num_elements, @object
	.size	num_elements, 4
num_elements:
	.word	10
	.globl	dst
	.bss
	.align	2
	.type	dst, @object
	.size	dst, 40
dst:
	.zero	40
	.globl	message
	.data
	.align	2
	.type	message, @object
	.size	message, 17
message:
	.string	"{{{NLK{ILXPHz{{{"
	.globl	password
	.align	2
	.type	password, @object
	.size	password, 9
password:
	.string	"KZHHDLIW"
	.text
	.align	2
	.globl	init_vectors
	.type	init_vectors, @function
init_vectors:
	addi	sp,sp,-32
	sw	ra,28(sp)
	sw	s0,24(sp)
	addi	s0,sp,32
	sw	zero,-20(s0)
	j	.L2
.L3:
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-20(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	sh	zero,0(a5)
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L2:
	li	a5,10
	slli	a5,a5,1
	lw	a4,-20(s0)
	blt	a4,a5,.L3
	sw	zero,-24(s0)
	j	.L4
.L5:
	lui	a5,%hi(src_backup)
	addi	a4,a5,%lo(src_backup)
	lw	a5,-24(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a4,0(a5)
	lui	a5,%hi(src)
	addi	a3,a5,%lo(src)
	lw	a5,-24(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	sw	a4,0(a5)
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
.L4:
	li	a5,10
	lw	a4,-24(s0)
	blt	a4,a5,.L5
	nop
	nop
	lw	ra,28(sp)
	lw	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	init_vectors, .-init_vectors
	.section	.rodata
	.align	2
.LC0:
	.string	"\n\n\n"
	.align	2
.LC1:
	.string	"Testing copyvec()"
	.align	2
.LC2:
	.string	"  Copy all elements: ... "
	.align	2
.LC3:
	.string	"passed."
	.align	2
.LC4:
	.string	"failed."
	.align	2
.LC5:
	.string	"  Copy some elements: ... "
	.align	2
.LC6:
	.string	"  Copy no elements: ... "
	.align	2
.LC7:
	.string	"Testing copyelements()"
	.align	2
.LC8:
	.string	"  Copy random slice: ..."
	.align	2
.LC9:
	.string	"(wrong return value)"
	.align	2
.LC10:
	.string	"\nAll tests passed! The Secret Password is:"
	.align	2
.LC11:
	.string	"\nSome tests failed. Keep trying!"
	.text
	.align	2
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-128
	sw	ra,124(sp)
	sw	s0,120(sp)
	addi	s0,sp,128
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	puts
	call	decrypt_message
	lui	a5,%hi(message)
	addi	a0,a5,%lo(message)
	call	hidden
	sw	zero,-20(s0)
	j	.L7
.L8:
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-20(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a4,0(a5)
	lui	a5,%hi(src_backup)
	addi	a3,a5,%lo(src_backup)
	lw	a5,-20(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	sw	a4,0(a5)
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L7:
	li	a5,10
	lw	a4,-20(s0)
	blt	a4,a5,.L8
	sw	zero,-24(s0)
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	call	init_vectors
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	printf
	lui	a5,%hi(_impure_ptr)
	lw	a5,%lo(_impure_ptr)(a5)
	lw	a5,8(a5)
	mv	a0,a5
	call	fflush
	li	a5,10
	mv	a2,a5
	lui	a5,%hi(dst)
	addi	a1,a5,%lo(dst)
	lui	a5,%hi(src)
	addi	a0,a5,%lo(src)
	call	copyvec
	li	a5,1
	sw	a5,-28(s0)
	sw	zero,-32(s0)
	j	.L9
.L11:
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-32(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a5,0(a5)
	lui	a4,%hi(dst)
	addi	a3,a4,%lo(dst)
	lw	a4,-32(s0)
	slli	a4,a4,1
	add	a4,a3,a4
	lh	a4,0(a4)
	beq	a5,a4,.L10
	sw	zero,-28(s0)
.L10:
	lw	a5,-32(s0)
	addi	a5,a5,1
	sw	a5,-32(s0)
.L9:
	li	a5,10
	lw	a4,-32(s0)
	blt	a4,a5,.L11
	li	a5,10
	sw	a5,-36(s0)
	j	.L12
.L14:
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-36(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	beq	a5,zero,.L13
	sw	zero,-28(s0)
.L13:
	lw	a5,-36(s0)
	addi	a5,a5,1
	sw	a5,-36(s0)
.L12:
	li	a5,10
	slli	a5,a5,1
	lw	a4,-36(s0)
	blt	a4,a5,.L14
	sw	zero,-40(s0)
	j	.L15
.L17:
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-40(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a4,0(a5)
	lui	a5,%hi(src_backup)
	addi	a3,a5,%lo(src_backup)
	lw	a5,-40(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	lw	a5,0(a5)
	beq	a4,a5,.L16
	sw	zero,-28(s0)
.L16:
	lw	a5,-40(s0)
	addi	a5,a5,1
	sw	a5,-40(s0)
.L15:
	li	a5,10
	lw	a4,-40(s0)
	blt	a4,a5,.L17
	lw	a5,-28(s0)
	beq	a5,zero,.L18
	lui	a5,%hi(.LC3)
	addi	a0,a5,%lo(.LC3)
	call	puts
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
	j	.L19
.L18:
	lui	a5,%hi(.LC4)
	addi	a0,a5,%lo(.LC4)
	call	puts
.L19:
	call	init_vectors
	lui	a5,%hi(.LC5)
	addi	a0,a5,%lo(.LC5)
	call	printf
	lui	a5,%hi(_impure_ptr)
	lw	a5,%lo(_impure_ptr)(a5)
	lw	a5,8(a5)
	mv	a0,a5
	call	fflush
	li	a5,10
	srai	a5,a5,1
	mv	a2,a5
	lui	a5,%hi(dst)
	addi	a1,a5,%lo(dst)
	lui	a5,%hi(src)
	addi	a0,a5,%lo(src)
	call	copyvec
	li	a5,1
	sw	a5,-44(s0)
	sw	zero,-48(s0)
	j	.L20
.L22:
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-48(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a5,0(a5)
	lui	a4,%hi(dst)
	addi	a3,a4,%lo(dst)
	lw	a4,-48(s0)
	slli	a4,a4,1
	add	a4,a3,a4
	lh	a4,0(a4)
	beq	a5,a4,.L21
	sw	zero,-44(s0)
.L21:
	lw	a5,-48(s0)
	addi	a5,a5,1
	sw	a5,-48(s0)
.L20:
	li	a5,10
	srai	a5,a5,1
	lw	a4,-48(s0)
	blt	a4,a5,.L22
	li	a5,10
	srai	a5,a5,1
	sw	a5,-52(s0)
	j	.L23
.L25:
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-52(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	beq	a5,zero,.L24
	sw	zero,-44(s0)
.L24:
	lw	a5,-52(s0)
	addi	a5,a5,1
	sw	a5,-52(s0)
.L23:
	li	a5,10
	slli	a5,a5,1
	lw	a4,-52(s0)
	blt	a4,a5,.L25
	sw	zero,-56(s0)
	j	.L26
.L28:
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-56(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a4,0(a5)
	lui	a5,%hi(src_backup)
	addi	a3,a5,%lo(src_backup)
	lw	a5,-56(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	lw	a5,0(a5)
	beq	a4,a5,.L27
	sw	zero,-44(s0)
.L27:
	lw	a5,-56(s0)
	addi	a5,a5,1
	sw	a5,-56(s0)
.L26:
	li	a5,10
	lw	a4,-56(s0)
	blt	a4,a5,.L28
	lw	a5,-44(s0)
	beq	a5,zero,.L29
	lui	a5,%hi(.LC3)
	addi	a0,a5,%lo(.LC3)
	call	puts
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
	j	.L30
.L29:
	lui	a5,%hi(.LC4)
	addi	a0,a5,%lo(.LC4)
	call	puts
.L30:
	call	init_vectors
	lui	a5,%hi(.LC6)
	addi	a0,a5,%lo(.LC6)
	call	printf
	lui	a5,%hi(_impure_ptr)
	lw	a5,%lo(_impure_ptr)(a5)
	lw	a5,8(a5)
	mv	a0,a5
	call	fflush
	li	a2,0
	lui	a5,%hi(dst)
	addi	a1,a5,%lo(dst)
	lui	a5,%hi(src)
	addi	a0,a5,%lo(src)
	call	copyvec
	li	a5,1
	sw	a5,-60(s0)
	sw	zero,-64(s0)
	j	.L31
.L33:
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-64(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	beq	a5,zero,.L32
	sw	zero,-60(s0)
.L32:
	lw	a5,-64(s0)
	addi	a5,a5,1
	sw	a5,-64(s0)
.L31:
	li	a5,10
	slli	a5,a5,1
	lw	a4,-64(s0)
	blt	a4,a5,.L33
	sw	zero,-68(s0)
	j	.L34
.L36:
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-68(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a4,0(a5)
	lui	a5,%hi(src_backup)
	addi	a3,a5,%lo(src_backup)
	lw	a5,-68(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	lw	a5,0(a5)
	beq	a4,a5,.L35
	sw	zero,-60(s0)
.L35:
	lw	a5,-68(s0)
	addi	a5,a5,1
	sw	a5,-68(s0)
.L34:
	li	a5,10
	lw	a4,-68(s0)
	blt	a4,a5,.L36
	lw	a5,-60(s0)
	beq	a5,zero,.L37
	lui	a5,%hi(.LC3)
	addi	a0,a5,%lo(.LC3)
	call	puts
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
	j	.L38
.L37:
	lui	a5,%hi(.LC4)
	addi	a0,a5,%lo(.LC4)
	call	puts
.L38:
	lui	a5,%hi(.LC7)
	addi	a0,a5,%lo(.LC7)
	call	puts
	call	init_vectors
	lui	a5,%hi(.LC8)
	addi	a0,a5,%lo(.LC8)
	call	printf
	lui	a5,%hi(_impure_ptr)
	lw	a5,%lo(_impure_ptr)(a5)
	lw	a5,8(a5)
	mv	a0,a5
	call	fflush
	li	a5,3
	sw	a5,-112(s0)
	li	a5,10
	addi	a5,a5,-3
	sw	a5,-116(s0)
	li	a5,1
	sw	a5,-72(s0)
	lw	a3,-116(s0)
	lw	a2,-112(s0)
	lui	a5,%hi(dst)
	addi	a1,a5,%lo(dst)
	lui	a5,%hi(src)
	addi	a0,a5,%lo(src)
	call	copyelements
	sw	a0,-120(s0)
	lw	a4,-116(s0)
	lw	a5,-112(s0)
	sub	a5,a4,a5
	addi	a5,a5,1
	lw	a4,-120(s0)
	beq	a4,a5,.L39
	lui	a5,%hi(.LC9)
	addi	a0,a5,%lo(.LC9)
	call	printf
	sw	zero,-72(s0)
.L39:
	sw	zero,-76(s0)
	j	.L40
.L42:
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-76(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	beq	a5,zero,.L41
	sw	zero,-72(s0)
.L41:
	lw	a5,-76(s0)
	addi	a5,a5,1
	sw	a5,-76(s0)
.L40:
	lw	a4,-76(s0)
	lw	a5,-112(s0)
	blt	a4,a5,.L42
	lw	a5,-112(s0)
	sw	a5,-80(s0)
	j	.L43
.L45:
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-80(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	mv	a3,a5
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-80(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a5,0(a5)
	beq	a3,a5,.L44
	sw	zero,-72(s0)
.L44:
	lw	a5,-80(s0)
	addi	a5,a5,1
	sw	a5,-80(s0)
.L43:
	lw	a4,-80(s0)
	lw	a5,-116(s0)
	ble	a4,a5,.L45
	lw	a5,-116(s0)
	addi	a5,a5,1
	sw	a5,-84(s0)
	j	.L46
.L48:
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-84(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	beq	a5,zero,.L47
	sw	zero,-72(s0)
.L47:
	lw	a5,-84(s0)
	addi	a5,a5,1
	sw	a5,-84(s0)
.L46:
	li	a5,10
	lw	a4,-84(s0)
	blt	a4,a5,.L48
	sw	zero,-88(s0)
	j	.L49
.L51:
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-88(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a4,0(a5)
	lui	a5,%hi(src_backup)
	addi	a3,a5,%lo(src_backup)
	lw	a5,-88(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	lw	a5,0(a5)
	beq	a4,a5,.L50
	sw	zero,-72(s0)
.L50:
	lw	a5,-88(s0)
	addi	a5,a5,1
	sw	a5,-88(s0)
.L49:
	li	a5,10
	lw	a4,-88(s0)
	blt	a4,a5,.L51
	lw	a5,-72(s0)
	beq	a5,zero,.L52
	lui	a5,%hi(.LC3)
	addi	a0,a5,%lo(.LC3)
	call	puts
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
	j	.L53
.L52:
	lui	a5,%hi(.LC4)
	addi	a0,a5,%lo(.LC4)
	call	puts
.L53:
	call	init_vectors
	lui	a5,%hi(.LC8)
	addi	a0,a5,%lo(.LC8)
	call	printf
	lui	a5,%hi(_impure_ptr)
	lw	a5,%lo(_impure_ptr)(a5)
	lw	a5,8(a5)
	mv	a0,a5
	call	fflush
	li	a5,4
	sw	a5,-124(s0)
	li	a5,10
	addi	a5,a5,-2
	sw	a5,-128(s0)
	li	a5,1
	sw	a5,-92(s0)
	lw	a3,-128(s0)
	lw	a2,-124(s0)
	lui	a5,%hi(dst)
	addi	a1,a5,%lo(dst)
	lui	a5,%hi(src)
	addi	a0,a5,%lo(src)
	call	copyelements
	sw	zero,-96(s0)
	j	.L54
.L56:
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-96(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	beq	a5,zero,.L55
	sw	zero,-92(s0)
.L55:
	lw	a5,-96(s0)
	addi	a5,a5,1
	sw	a5,-96(s0)
.L54:
	lw	a4,-96(s0)
	lw	a5,-124(s0)
	blt	a4,a5,.L56
	lw	a5,-124(s0)
	sw	a5,-100(s0)
	j	.L57
.L59:
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-100(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	mv	a3,a5
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-100(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a5,0(a5)
	beq	a3,a5,.L58
	sw	zero,-92(s0)
.L58:
	lw	a5,-100(s0)
	addi	a5,a5,1
	sw	a5,-100(s0)
.L57:
	lw	a4,-100(s0)
	lw	a5,-128(s0)
	ble	a4,a5,.L59
	lw	a5,-128(s0)
	addi	a5,a5,1
	sw	a5,-104(s0)
	j	.L60
.L62:
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-104(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	beq	a5,zero,.L61
	sw	zero,-92(s0)
.L61:
	lw	a5,-104(s0)
	addi	a5,a5,1
	sw	a5,-104(s0)
.L60:
	li	a5,10
	lw	a4,-104(s0)
	blt	a4,a5,.L62
	sw	zero,-108(s0)
	j	.L63
.L65:
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-108(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a4,0(a5)
	lui	a5,%hi(src_backup)
	addi	a3,a5,%lo(src_backup)
	lw	a5,-108(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	lw	a5,0(a5)
	beq	a4,a5,.L64
	sw	zero,-92(s0)
.L64:
	lw	a5,-108(s0)
	addi	a5,a5,1
	sw	a5,-108(s0)
.L63:
	li	a5,10
	lw	a4,-108(s0)
	blt	a4,a5,.L65
	lw	a5,-92(s0)
	beq	a5,zero,.L66
	lui	a5,%hi(.LC3)
	addi	a0,a5,%lo(.LC3)
	call	puts
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
	j	.L67
.L66:
	lui	a5,%hi(.LC4)
	addi	a0,a5,%lo(.LC4)
	call	puts
.L67:
	lw	a4,-24(s0)
	li	a5,5
	bne	a4,a5,.L68
	lui	a5,%hi(.LC10)
	addi	a0,a5,%lo(.LC10)
	call	puts
	lui	a5,%hi(password)
	addi	a0,a5,%lo(password)
	call	puts
	j	.L69
.L68:
	lui	a5,%hi(.LC11)
	addi	a0,a5,%lo(.LC11)
	call	puts
.L69:
	li	a5,0
	mv	a0,a5
	lw	ra,124(sp)
	lw	s0,120(sp)
	addi	sp,sp,128
	jr	ra
	.size	main, .-main
	.align	2
	.globl	decrypt_message
	.type	decrypt_message, @function
decrypt_message:
	addi	sp,sp,-32
	sw	ra,28(sp)
	sw	s0,24(sp)
	addi	s0,sp,32
	sw	zero,-20(s0)
	j	.L72
.L73:
	lui	a5,%hi(message)
	addi	a4,a5,%lo(message)
	lw	a5,-20(s0)
	add	a5,a4,a5
	lbu	a5,0(a5)
	li	a4,-101
	sub	a5,a4,a5
	andi	a4,a5,0xff
	lui	a5,%hi(message)
	addi	a3,a5,%lo(message)
	lw	a5,-20(s0)
	add	a5,a3,a5
	sb	a4,0(a5)
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L72:
	lw	a4,-20(s0)
	li	a5,15
	bleu	a4,a5,.L73
	sw	zero,-24(s0)
	j	.L74
.L75:
	lui	a5,%hi(password)
	addi	a4,a5,%lo(password)
	lw	a5,-24(s0)
	add	a5,a4,a5
	lbu	a5,0(a5)
	li	a4,-101
	sub	a5,a4,a5
	andi	a4,a5,0xff
	lui	a5,%hi(password)
	addi	a3,a5,%lo(password)
	lw	a5,-24(s0)
	add	a5,a3,a5
	sb	a4,0(a5)
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
.L74:
	lw	a4,-24(s0)
	li	a5,7
	bleu	a4,a5,.L75
	nop
	nop
	lw	ra,28(sp)
	lw	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	decrypt_message, .-decrypt_message
	.ident	"GCC: ('riscv-embecosm-embedded-macos-20250309') 15.0.1 20250308 (experimental)"
	.section	.note.GNU-stack,"",@progbits
