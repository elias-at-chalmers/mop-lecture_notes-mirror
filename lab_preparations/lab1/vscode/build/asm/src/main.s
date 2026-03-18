	.file	"main.c"
	.option nopic
	.attribute arch, "rv32i2p1_m2p0_f2p2_zicsr2p0_zmmul1p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.file 0 "/Users/elias-work/Desktop/mop-lecture_notes/lab_preparations-LP4/lab1/code" "src/main.c"
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
.LFB3:
	.file 1 "src/main.c"
	.loc 1 23 1
	.cfi_startproc
	addi	sp,sp,-32
	.cfi_def_cfa_offset 32
	sw	ra,28(sp)
	sw	s0,24(sp)
	.cfi_offset 1, -4
	.cfi_offset 8, -8
	addi	s0,sp,32
	.cfi_def_cfa 8, 0
.LBB2:
	.loc 1 24 13
	sw	zero,-20(s0)
	.loc 1 24 5
	j	.L2
.L3:
	.loc 1 24 50 discriminator 3
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-20(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	sh	zero,0(a5)
	.loc 1 24 39 discriminator 3
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L2:
	.loc 1 24 33 discriminator 1
	li	a5,10
	slli	a5,a5,1
	.loc 1 24 19 discriminator 1
	lw	a4,-20(s0)
	blt	a4,a5,.L3
.LBE2:
.LBB3:
	.loc 1 25 13
	sw	zero,-24(s0)
	.loc 1 25 5
	j	.L4
.L5:
	.loc 1 25 58 discriminator 3
	lui	a5,%hi(src_backup)
	addi	a4,a5,%lo(src_backup)
	lw	a5,-24(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a4,0(a5)
	.loc 1 25 46 discriminator 3
	lui	a5,%hi(src)
	addi	a3,a5,%lo(src)
	lw	a5,-24(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	sw	a4,0(a5)
	.loc 1 25 35 discriminator 3
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
.L4:
	.loc 1 25 19 discriminator 1
	li	a5,10
	lw	a4,-24(s0)
	blt	a4,a5,.L5
.LBE3:
	.loc 1 26 1
	nop
	nop
	lw	ra,28(sp)
	.cfi_restore 1
	lw	s0,24(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 32
	addi	sp,sp,32
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE3:
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
.LFB4:
	.loc 1 29 1
	.cfi_startproc
	addi	sp,sp,-128
	.cfi_def_cfa_offset 128
	sw	ra,124(sp)
	sw	s0,120(sp)
	.cfi_offset 1, -4
	.cfi_offset 8, -8
	addi	s0,sp,128
	.cfi_def_cfa 8, 0
	.loc 1 30 5
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	puts
	.loc 1 32 5
	call	decrypt_message
	.loc 1 33 5
	lui	a5,%hi(message)
	addi	a0,a5,%lo(message)
	call	hidden
.LBB4:
	.loc 1 36 13
	sw	zero,-20(s0)
	.loc 1 36 5
	j	.L7
.L8:
	.loc 1 36 58 discriminator 3
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-20(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a4,0(a5)
	.loc 1 36 53 discriminator 3
	lui	a5,%hi(src_backup)
	addi	a3,a5,%lo(src_backup)
	lw	a5,-20(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	sw	a4,0(a5)
	.loc 1 36 35 discriminator 3
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L7:
	.loc 1 36 19 discriminator 1
	li	a5,10
	lw	a4,-20(s0)
	blt	a4,a5,.L8
.LBE4:
	.loc 1 38 9
	sw	zero,-24(s0)
	.loc 1 39 5
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
.LBB5:
	.loc 1 41 9
	call	init_vectors
	.loc 1 42 9
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	printf
	.loc 1 43 16
	lui	a5,%hi(_impure_ptr)
	lw	a5,%lo(_impure_ptr)(a5)
	lw	a5,8(a5)
	.loc 1 43 9
	mv	a0,a5
	call	fflush
	.loc 1 44 9
	li	a5,10
	mv	a2,a5
	lui	a5,%hi(dst)
	addi	a1,a5,%lo(dst)
	lui	a5,%hi(src)
	addi	a0,a5,%lo(src)
	call	copyvec
	.loc 1 45 13
	li	a5,1
	sw	a5,-28(s0)
.LBB6:
	.loc 1 46 17
	sw	zero,-32(s0)
	.loc 1 46 9
	j	.L9
.L11:
	.loc 1 46 49 discriminator 6
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-32(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a5,0(a5)
	.loc 1 46 59 discriminator 6
	lui	a4,%hi(dst)
	addi	a3,a4,%lo(dst)
	lw	a4,-32(s0)
	slli	a4,a4,1
	add	a4,a3,a4
	lh	a4,0(a4)
	.loc 1 46 45 discriminator 6
	beq	a5,a4,.L10
	.loc 1 46 71 discriminator 2
	sw	zero,-28(s0)
.L10:
	.loc 1 46 39 discriminator 4
	lw	a5,-32(s0)
	addi	a5,a5,1
	sw	a5,-32(s0)
.L9:
	.loc 1 46 23 discriminator 5
	li	a5,10
	lw	a4,-32(s0)
	blt	a4,a5,.L11
.LBE6:
.LBB7:
	.loc 1 47 17
	li	a5,10
	sw	a5,-36(s0)
	.loc 1 47 9
	j	.L12
.L14:
	.loc 1 47 64 discriminator 6
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-36(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	.loc 1 47 60 discriminator 6
	beq	a5,zero,.L13
	.loc 1 47 81 discriminator 2
	sw	zero,-28(s0)
.L13:
	.loc 1 47 54 discriminator 4
	lw	a5,-36(s0)
	addi	a5,a5,1
	sw	a5,-36(s0)
.L12:
	.loc 1 47 48 discriminator 5
	li	a5,10
	slli	a5,a5,1
	.loc 1 47 34 discriminator 5
	lw	a4,-36(s0)
	blt	a4,a5,.L14
.LBE7:
.LBB8:
	.loc 1 48 17
	sw	zero,-40(s0)
	.loc 1 48 9
	j	.L15
.L17:
	.loc 1 48 49 discriminator 6
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-40(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a4,0(a5)
	.loc 1 48 66 discriminator 6
	lui	a5,%hi(src_backup)
	addi	a3,a5,%lo(src_backup)
	lw	a5,-40(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	lw	a5,0(a5)
	.loc 1 48 45 discriminator 6
	beq	a4,a5,.L16
	.loc 1 48 78 discriminator 2
	sw	zero,-28(s0)
.L16:
	.loc 1 48 39 discriminator 4
	lw	a5,-40(s0)
	addi	a5,a5,1
	sw	a5,-40(s0)
.L15:
	.loc 1 48 23 discriminator 5
	li	a5,10
	lw	a4,-40(s0)
	blt	a4,a5,.L17
.LBE8:
	.loc 1 49 11
	lw	a5,-28(s0)
	beq	a5,zero,.L18
	.loc 1 49 22 discriminator 1
	lui	a5,%hi(.LC3)
	addi	a0,a5,%lo(.LC3)
	call	puts
	.loc 1 49 59 discriminator 2
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
	j	.L19
.L18:
	.loc 1 50 14
	lui	a5,%hi(.LC4)
	addi	a0,a5,%lo(.LC4)
	call	puts
.L19:
.LBE5:
.LBB9:
	.loc 1 54 9
	call	init_vectors
	.loc 1 55 9
	lui	a5,%hi(.LC5)
	addi	a0,a5,%lo(.LC5)
	call	printf
	.loc 1 56 16
	lui	a5,%hi(_impure_ptr)
	lw	a5,%lo(_impure_ptr)(a5)
	lw	a5,8(a5)
	.loc 1 56 9
	mv	a0,a5
	call	fflush
	.loc 1 57 9
	li	a5,10
	srai	a5,a5,1
	mv	a2,a5
	lui	a5,%hi(dst)
	addi	a1,a5,%lo(dst)
	lui	a5,%hi(src)
	addi	a0,a5,%lo(src)
	call	copyvec
	.loc 1 58 13
	li	a5,1
	sw	a5,-44(s0)
.LBB10:
	.loc 1 59 17
	sw	zero,-48(s0)
	.loc 1 59 9
	j	.L20
.L22:
	.loc 1 59 51 discriminator 6
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-48(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a5,0(a5)
	.loc 1 59 61 discriminator 6
	lui	a4,%hi(dst)
	addi	a3,a4,%lo(dst)
	lw	a4,-48(s0)
	slli	a4,a4,1
	add	a4,a3,a4
	lh	a4,0(a4)
	.loc 1 59 47 discriminator 6
	beq	a5,a4,.L21
	.loc 1 59 73 discriminator 2
	sw	zero,-44(s0)
.L21:
	.loc 1 59 41 discriminator 4
	lw	a5,-48(s0)
	addi	a5,a5,1
	sw	a5,-48(s0)
.L20:
	.loc 1 59 36 discriminator 5
	li	a5,10
	srai	a5,a5,1
	.loc 1 59 23 discriminator 5
	lw	a4,-48(s0)
	blt	a4,a5,.L22
.LBE10:
.LBB11:
	.loc 1 60 31
	li	a5,10
	.loc 1 60 17
	srai	a5,a5,1
	sw	a5,-52(s0)
	.loc 1 60 9
	j	.L23
.L25:
	.loc 1 60 66 discriminator 6
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-52(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	.loc 1 60 62 discriminator 6
	beq	a5,zero,.L24
	.loc 1 60 83 discriminator 2
	sw	zero,-44(s0)
.L24:
	.loc 1 60 56 discriminator 4
	lw	a5,-52(s0)
	addi	a5,a5,1
	sw	a5,-52(s0)
.L23:
	.loc 1 60 50 discriminator 5
	li	a5,10
	slli	a5,a5,1
	.loc 1 60 36 discriminator 5
	lw	a4,-52(s0)
	blt	a4,a5,.L25
.LBE11:
.LBB12:
	.loc 1 61 17
	sw	zero,-56(s0)
	.loc 1 61 9
	j	.L26
.L28:
	.loc 1 61 49 discriminator 6
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-56(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a4,0(a5)
	.loc 1 61 66 discriminator 6
	lui	a5,%hi(src_backup)
	addi	a3,a5,%lo(src_backup)
	lw	a5,-56(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	lw	a5,0(a5)
	.loc 1 61 45 discriminator 6
	beq	a4,a5,.L27
	.loc 1 61 78 discriminator 2
	sw	zero,-44(s0)
.L27:
	.loc 1 61 39 discriminator 4
	lw	a5,-56(s0)
	addi	a5,a5,1
	sw	a5,-56(s0)
.L26:
	.loc 1 61 23 discriminator 5
	li	a5,10
	lw	a4,-56(s0)
	blt	a4,a5,.L28
.LBE12:
	.loc 1 62 11
	lw	a5,-44(s0)
	beq	a5,zero,.L29
	.loc 1 62 22 discriminator 1
	lui	a5,%hi(.LC3)
	addi	a0,a5,%lo(.LC3)
	call	puts
	.loc 1 62 59 discriminator 2
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
	j	.L30
.L29:
	.loc 1 63 14
	lui	a5,%hi(.LC4)
	addi	a0,a5,%lo(.LC4)
	call	puts
.L30:
.LBE9:
.LBB13:
	.loc 1 67 9
	call	init_vectors
	.loc 1 68 9
	lui	a5,%hi(.LC6)
	addi	a0,a5,%lo(.LC6)
	call	printf
	.loc 1 69 16
	lui	a5,%hi(_impure_ptr)
	lw	a5,%lo(_impure_ptr)(a5)
	lw	a5,8(a5)
	.loc 1 69 9
	mv	a0,a5
	call	fflush
	.loc 1 70 9
	li	a2,0
	lui	a5,%hi(dst)
	addi	a1,a5,%lo(dst)
	lui	a5,%hi(src)
	addi	a0,a5,%lo(src)
	call	copyvec
	.loc 1 71 13
	li	a5,1
	sw	a5,-60(s0)
.LBB14:
	.loc 1 72 17
	sw	zero,-64(s0)
	.loc 1 72 9
	j	.L31
.L33:
	.loc 1 72 53 discriminator 6
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-64(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	.loc 1 72 49 discriminator 6
	beq	a5,zero,.L32
	.loc 1 72 70 discriminator 2
	sw	zero,-60(s0)
.L32:
	.loc 1 72 43 discriminator 4
	lw	a5,-64(s0)
	addi	a5,a5,1
	sw	a5,-64(s0)
.L31:
	.loc 1 72 37 discriminator 5
	li	a5,10
	slli	a5,a5,1
	.loc 1 72 23 discriminator 5
	lw	a4,-64(s0)
	blt	a4,a5,.L33
.LBE14:
.LBB15:
	.loc 1 73 17
	sw	zero,-68(s0)
	.loc 1 73 9
	j	.L34
.L36:
	.loc 1 73 49 discriminator 6
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-68(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a4,0(a5)
	.loc 1 73 66 discriminator 6
	lui	a5,%hi(src_backup)
	addi	a3,a5,%lo(src_backup)
	lw	a5,-68(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	lw	a5,0(a5)
	.loc 1 73 45 discriminator 6
	beq	a4,a5,.L35
	.loc 1 73 78 discriminator 2
	sw	zero,-60(s0)
.L35:
	.loc 1 73 39 discriminator 4
	lw	a5,-68(s0)
	addi	a5,a5,1
	sw	a5,-68(s0)
.L34:
	.loc 1 73 23 discriminator 5
	li	a5,10
	lw	a4,-68(s0)
	blt	a4,a5,.L36
.LBE15:
	.loc 1 74 11
	lw	a5,-60(s0)
	beq	a5,zero,.L37
	.loc 1 74 22 discriminator 1
	lui	a5,%hi(.LC3)
	addi	a0,a5,%lo(.LC3)
	call	puts
	.loc 1 74 59 discriminator 2
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
	j	.L38
.L37:
	.loc 1 75 14
	lui	a5,%hi(.LC4)
	addi	a0,a5,%lo(.LC4)
	call	puts
.L38:
.LBE13:
	.loc 1 78 5
	lui	a5,%hi(.LC7)
	addi	a0,a5,%lo(.LC7)
	call	puts
.LBB16:
	.loc 1 80 9
	call	init_vectors
	.loc 1 81 9
	lui	a5,%hi(.LC8)
	addi	a0,a5,%lo(.LC8)
	call	printf
	.loc 1 82 16
	lui	a5,%hi(_impure_ptr)
	lw	a5,%lo(_impure_ptr)(a5)
	lw	a5,8(a5)
	.loc 1 82 9
	mv	a0,a5
	call	fflush
	.loc 1 83 13
	li	a5,3
	sw	a5,-112(s0)
	.loc 1 84 32
	li	a5,10
	.loc 1 84 13
	addi	a5,a5,-3
	sw	a5,-116(s0)
	.loc 1 85 13
	li	a5,1
	sw	a5,-72(s0)
	.loc 1 86 22
	lw	a3,-116(s0)
	lw	a2,-112(s0)
	lui	a5,%hi(dst)
	addi	a1,a5,%lo(dst)
	lui	a5,%hi(src)
	addi	a0,a5,%lo(src)
	call	copyelements
	sw	a0,-120(s0)
	.loc 1 87 26
	lw	a4,-116(s0)
	lw	a5,-112(s0)
	sub	a5,a4,a5
	.loc 1 87 34
	addi	a5,a5,1
	.loc 1 87 11
	lw	a4,-120(s0)
	beq	a4,a5,.L39
	.loc 1 87 41 discriminator 1
	lui	a5,%hi(.LC9)
	addi	a0,a5,%lo(.LC9)
	call	printf
	.loc 1 87 80 discriminator 2
	sw	zero,-72(s0)
.L39:
.LBB17:
	.loc 1 88 17
	sw	zero,-76(s0)
	.loc 1 88 9
	j	.L40
.L42:
	.loc 1 88 42 discriminator 6
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-76(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	.loc 1 88 38 discriminator 6
	beq	a5,zero,.L41
	.loc 1 88 59 discriminator 2
	sw	zero,-72(s0)
.L41:
	.loc 1 88 32 discriminator 4
	lw	a5,-76(s0)
	addi	a5,a5,1
	sw	a5,-76(s0)
.L40:
	.loc 1 88 23 discriminator 5
	lw	a4,-76(s0)
	lw	a5,-112(s0)
	blt	a4,a5,.L42
.LBE17:
.LBB18:
	.loc 1 89 17
	lw	a5,-112(s0)
	sw	a5,-80(s0)
	.loc 1 89 9
	j	.L43
.L45:
	.loc 1 89 45 discriminator 6
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-80(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	mv	a3,a5
	.loc 1 89 55 discriminator 6
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-80(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a5,0(a5)
	.loc 1 89 41 discriminator 6
	beq	a3,a5,.L44
	.loc 1 89 67 discriminator 2
	sw	zero,-72(s0)
.L44:
	.loc 1 89 35 discriminator 4
	lw	a5,-80(s0)
	addi	a5,a5,1
	sw	a5,-80(s0)
.L43:
	.loc 1 89 27 discriminator 5
	lw	a4,-80(s0)
	lw	a5,-116(s0)
	ble	a4,a5,.L45
.LBE18:
.LBB19:
	.loc 1 90 17
	lw	a5,-116(s0)
	addi	a5,a5,1
	sw	a5,-84(s0)
	.loc 1 90 9
	j	.L46
.L48:
	.loc 1 90 53 discriminator 6
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-84(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	.loc 1 90 49 discriminator 6
	beq	a5,zero,.L47
	.loc 1 90 70 discriminator 2
	sw	zero,-72(s0)
.L47:
	.loc 1 90 43 discriminator 4
	lw	a5,-84(s0)
	addi	a5,a5,1
	sw	a5,-84(s0)
.L46:
	.loc 1 90 27 discriminator 5
	li	a5,10
	lw	a4,-84(s0)
	blt	a4,a5,.L48
.LBE19:
.LBB20:
	.loc 1 91 17
	sw	zero,-88(s0)
	.loc 1 91 9
	j	.L49
.L51:
	.loc 1 91 49 discriminator 6
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-88(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a4,0(a5)
	.loc 1 91 66 discriminator 6
	lui	a5,%hi(src_backup)
	addi	a3,a5,%lo(src_backup)
	lw	a5,-88(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	lw	a5,0(a5)
	.loc 1 91 45 discriminator 6
	beq	a4,a5,.L50
	.loc 1 91 78 discriminator 2
	sw	zero,-72(s0)
.L50:
	.loc 1 91 39 discriminator 4
	lw	a5,-88(s0)
	addi	a5,a5,1
	sw	a5,-88(s0)
.L49:
	.loc 1 91 23 discriminator 5
	li	a5,10
	lw	a4,-88(s0)
	blt	a4,a5,.L51
.LBE20:
	.loc 1 92 11
	lw	a5,-72(s0)
	beq	a5,zero,.L52
	.loc 1 92 22 discriminator 1
	lui	a5,%hi(.LC3)
	addi	a0,a5,%lo(.LC3)
	call	puts
	.loc 1 92 59 discriminator 2
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
	j	.L53
.L52:
	.loc 1 93 14
	lui	a5,%hi(.LC4)
	addi	a0,a5,%lo(.LC4)
	call	puts
.L53:
.LBE16:
.LBB21:
	.loc 1 96 9
	call	init_vectors
	.loc 1 97 9
	lui	a5,%hi(.LC8)
	addi	a0,a5,%lo(.LC8)
	call	printf
	.loc 1 98 16
	lui	a5,%hi(_impure_ptr)
	lw	a5,%lo(_impure_ptr)(a5)
	lw	a5,8(a5)
	.loc 1 98 9
	mv	a0,a5
	call	fflush
	.loc 1 99 13
	li	a5,4
	sw	a5,-124(s0)
	.loc 1 100 32
	li	a5,10
	.loc 1 100 13
	addi	a5,a5,-2
	sw	a5,-128(s0)
	.loc 1 101 13
	li	a5,1
	sw	a5,-92(s0)
	.loc 1 102 9
	lw	a3,-128(s0)
	lw	a2,-124(s0)
	lui	a5,%hi(dst)
	addi	a1,a5,%lo(dst)
	lui	a5,%hi(src)
	addi	a0,a5,%lo(src)
	call	copyelements
.LBB22:
	.loc 1 103 17
	sw	zero,-96(s0)
	.loc 1 103 9
	j	.L54
.L56:
	.loc 1 103 42 discriminator 6
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-96(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	.loc 1 103 38 discriminator 6
	beq	a5,zero,.L55
	.loc 1 103 59 discriminator 2
	sw	zero,-92(s0)
.L55:
	.loc 1 103 32 discriminator 4
	lw	a5,-96(s0)
	addi	a5,a5,1
	sw	a5,-96(s0)
.L54:
	.loc 1 103 23 discriminator 5
	lw	a4,-96(s0)
	lw	a5,-124(s0)
	blt	a4,a5,.L56
.LBE22:
.LBB23:
	.loc 1 104 17
	lw	a5,-124(s0)
	sw	a5,-100(s0)
	.loc 1 104 9
	j	.L57
.L59:
	.loc 1 104 45 discriminator 6
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-100(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	mv	a3,a5
	.loc 1 104 55 discriminator 6
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-100(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a5,0(a5)
	.loc 1 104 41 discriminator 6
	beq	a3,a5,.L58
	.loc 1 104 67 discriminator 2
	sw	zero,-92(s0)
.L58:
	.loc 1 104 35 discriminator 4
	lw	a5,-100(s0)
	addi	a5,a5,1
	sw	a5,-100(s0)
.L57:
	.loc 1 104 27 discriminator 5
	lw	a4,-100(s0)
	lw	a5,-128(s0)
	ble	a4,a5,.L59
.LBE23:
.LBB24:
	.loc 1 105 17
	lw	a5,-128(s0)
	addi	a5,a5,1
	sw	a5,-104(s0)
	.loc 1 105 9
	j	.L60
.L62:
	.loc 1 105 53 discriminator 6
	lui	a5,%hi(dst)
	addi	a4,a5,%lo(dst)
	lw	a5,-104(s0)
	slli	a5,a5,1
	add	a5,a4,a5
	lh	a5,0(a5)
	.loc 1 105 49 discriminator 6
	beq	a5,zero,.L61
	.loc 1 105 70 discriminator 2
	sw	zero,-92(s0)
.L61:
	.loc 1 105 43 discriminator 4
	lw	a5,-104(s0)
	addi	a5,a5,1
	sw	a5,-104(s0)
.L60:
	.loc 1 105 27 discriminator 5
	li	a5,10
	lw	a4,-104(s0)
	blt	a4,a5,.L62
.LBE24:
.LBB25:
	.loc 1 106 17
	sw	zero,-108(s0)
	.loc 1 106 9
	j	.L63
.L65:
	.loc 1 106 49 discriminator 6
	lui	a5,%hi(src)
	addi	a4,a5,%lo(src)
	lw	a5,-108(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a4,0(a5)
	.loc 1 106 66 discriminator 6
	lui	a5,%hi(src_backup)
	addi	a3,a5,%lo(src_backup)
	lw	a5,-108(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	lw	a5,0(a5)
	.loc 1 106 45 discriminator 6
	beq	a4,a5,.L64
	.loc 1 106 78 discriminator 2
	sw	zero,-92(s0)
.L64:
	.loc 1 106 39 discriminator 4
	lw	a5,-108(s0)
	addi	a5,a5,1
	sw	a5,-108(s0)
.L63:
	.loc 1 106 23 discriminator 5
	li	a5,10
	lw	a4,-108(s0)
	blt	a4,a5,.L65
.LBE25:
	.loc 1 107 11
	lw	a5,-92(s0)
	beq	a5,zero,.L66
	.loc 1 107 22 discriminator 1
	lui	a5,%hi(.LC3)
	addi	a0,a5,%lo(.LC3)
	call	puts
	.loc 1 107 59 discriminator 2
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
	j	.L67
.L66:
	.loc 1 108 14
	lui	a5,%hi(.LC4)
	addi	a0,a5,%lo(.LC4)
	call	puts
.L67:
.LBE21:
	.loc 1 111 7
	lw	a4,-24(s0)
	li	a5,5
	bne	a4,a5,.L68
	.loc 1 112 9
	lui	a5,%hi(.LC10)
	addi	a0,a5,%lo(.LC10)
	call	puts
	.loc 1 113 9
	lui	a5,%hi(password)
	addi	a0,a5,%lo(password)
	call	puts
	j	.L69
.L68:
	.loc 1 115 9
	lui	a5,%hi(.LC11)
	addi	a0,a5,%lo(.LC11)
	call	puts
.L69:
	li	a5,0
	.loc 1 118 1
	mv	a0,a5
	lw	ra,124(sp)
	.cfi_restore 1
	lw	s0,120(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 128
	addi	sp,sp,128
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE4:
	.size	main, .-main
	.align	2
	.globl	decrypt_message
	.type	decrypt_message, @function
decrypt_message:
.LFB5:
	.loc 1 124 1
	.cfi_startproc
	addi	sp,sp,-32
	.cfi_def_cfa_offset 32
	sw	ra,28(sp)
	sw	s0,24(sp)
	.cfi_offset 1, -4
	.cfi_offset 8, -8
	addi	s0,sp,32
	.cfi_def_cfa 8, 0
.LBB26:
	.loc 1 125 22
	sw	zero,-20(s0)
	.loc 1 125 5
	j	.L72
.L73:
	.loc 1 126 36
	lui	a5,%hi(message)
	addi	a4,a5,%lo(message)
	lw	a5,-20(s0)
	add	a5,a4,a5
	lbu	a5,0(a5)
	.loc 1 126 26
	li	a4,-101
	sub	a5,a4,a5
	andi	a4,a5,0xff
	.loc 1 126 20
	lui	a5,%hi(message)
	addi	a3,a5,%lo(message)
	lw	a5,-20(s0)
	add	a5,a3,a5
	sb	a4,0(a5)
	.loc 1 125 53 discriminator 3
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L72:
	.loc 1 125 29 discriminator 1
	lw	a4,-20(s0)
	li	a5,15
	bleu	a4,a5,.L73
.LBE26:
.LBB27:
	.loc 1 131 22
	sw	zero,-24(s0)
	.loc 1 131 5
	j	.L74
.L75:
	.loc 1 132 38
	lui	a5,%hi(password)
	addi	a4,a5,%lo(password)
	lw	a5,-24(s0)
	add	a5,a4,a5
	lbu	a5,0(a5)
	.loc 1 132 27
	li	a4,-101
	sub	a5,a4,a5
	andi	a4,a5,0xff
	.loc 1 132 21
	lui	a5,%hi(password)
	addi	a3,a5,%lo(password)
	lw	a5,-24(s0)
	add	a5,a3,a5
	sb	a4,0(a5)
	.loc 1 131 54 discriminator 3
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
.L74:
	.loc 1 131 29 discriminator 1
	lw	a4,-24(s0)
	li	a5,7
	bleu	a4,a5,.L75
.LBE27:
	.loc 1 134 1
	nop
	nop
	lw	ra,28(sp)
	.cfi_restore 1
	lw	s0,24(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 32
	addi	sp,sp,32
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE5:
	.size	decrypt_message, .-decrypt_message
.Letext0:
	.file 2 "/Users/elias-work/Library/Application Support/Code/User/globalStorage/beanarch.mdx07-templates/riscv-gcc/lib/gcc/riscv32-unknown-elf/15.0.1/include/stddef.h"
	.file 3 "/Users/elias-work/Library/Application Support/Code/User/globalStorage/beanarch.mdx07-templates/riscv-gcc/riscv32-unknown-elf/include/sys/_types.h"
	.file 4 "/Users/elias-work/Library/Application Support/Code/User/globalStorage/beanarch.mdx07-templates/riscv-gcc/riscv32-unknown-elf/include/sys/reent.h"
	.file 5 "/Users/elias-work/Library/Application Support/Code/User/globalStorage/beanarch.mdx07-templates/riscv-gcc/riscv32-unknown-elf/include/sys/lock.h"
	.file 6 "/Users/elias-work/Library/Application Support/Code/User/globalStorage/beanarch.mdx07-templates/riscv-gcc/riscv32-unknown-elf/include/stdio.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.4byte	0xb88
	.2byte	0x5
	.byte	0x1
	.byte	0x4
	.4byte	.Ldebug_abbrev0
	.uleb128 0x18
	.4byte	.LASF116
	.byte	0xc
	.4byte	.LASF0
	.4byte	.LASF1
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	.Ldebug_line0
	.uleb128 0x9
	.byte	0x1
	.byte	0x6
	.4byte	.LASF2
	.uleb128 0x9
	.byte	0x1
	.byte	0x8
	.4byte	.LASF3
	.uleb128 0x9
	.byte	0x2
	.byte	0x5
	.4byte	.LASF4
	.uleb128 0x9
	.byte	0x2
	.byte	0x7
	.4byte	.LASF5
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.4byte	.LASF6
	.uleb128 0x9
	.byte	0x4
	.byte	0x7
	.4byte	.LASF7
	.uleb128 0x9
	.byte	0x8
	.byte	0x5
	.4byte	.LASF8
	.uleb128 0x9
	.byte	0x8
	.byte	0x7
	.4byte	.LASF9
	.uleb128 0x19
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x12
	.4byte	0x5e
	.uleb128 0x9
	.byte	0x4
	.byte	0x7
	.4byte	.LASF10
	.uleb128 0x13
	.4byte	.LASF11
	.byte	0x2
	.2byte	0x167
	.byte	0x17
	.4byte	0x6a
	.uleb128 0xc
	.4byte	.LASF12
	.byte	0x3
	.byte	0x2e
	.byte	0xe
	.4byte	0x42
	.uleb128 0xc
	.4byte	.LASF13
	.byte	0x3
	.byte	0x74
	.byte	0xe
	.4byte	0x42
	.uleb128 0xc
	.4byte	.LASF14
	.byte	0x3
	.byte	0x93
	.byte	0x17
	.4byte	0x5e
	.uleb128 0x1a
	.byte	0x4
	.byte	0x3
	.byte	0xa5
	.byte	0x3
	.4byte	0xc2
	.uleb128 0x14
	.4byte	.LASF15
	.byte	0xa7
	.byte	0xc
	.4byte	0x71
	.uleb128 0x14
	.4byte	.LASF16
	.byte	0xa8
	.byte	0x13
	.4byte	0xc2
	.byte	0
	.uleb128 0x7
	.4byte	0x2d
	.4byte	0xd2
	.uleb128 0x8
	.4byte	0x6a
	.byte	0x3
	.byte	0
	.uleb128 0x1b
	.byte	0x8
	.byte	0x3
	.byte	0xa2
	.byte	0x9
	.4byte	0xf6
	.uleb128 0x2
	.4byte	.LASF17
	.byte	0x3
	.byte	0xa4
	.byte	0x7
	.4byte	0x5e
	.byte	0
	.uleb128 0x2
	.4byte	.LASF18
	.byte	0x3
	.byte	0xa9
	.byte	0x5
	.4byte	0xa2
	.byte	0x4
	.byte	0
	.uleb128 0xc
	.4byte	.LASF19
	.byte	0x3
	.byte	0xaa
	.byte	0x3
	.4byte	0xd2
	.uleb128 0x1c
	.byte	0x4
	.uleb128 0xc
	.4byte	.LASF20
	.byte	0x4
	.byte	0x17
	.byte	0x19
	.4byte	0x49
	.uleb128 0xc
	.4byte	.LASF21
	.byte	0x5
	.byte	0xc
	.byte	0xd
	.4byte	0x5e
	.uleb128 0xc
	.4byte	.LASF22
	.byte	0x4
	.byte	0x24
	.byte	0x1b
	.4byte	0x110
	.uleb128 0xe
	.4byte	.LASF27
	.byte	0x18
	.byte	0x35
	.4byte	0x17e
	.uleb128 0x2
	.4byte	.LASF23
	.byte	0x4
	.byte	0x37
	.byte	0x13
	.4byte	0x17e
	.byte	0
	.uleb128 0xa
	.string	"_k"
	.byte	0x38
	.byte	0x7
	.4byte	0x5e
	.byte	0x4
	.uleb128 0x2
	.4byte	.LASF24
	.byte	0x4
	.byte	0x38
	.byte	0xb
	.4byte	0x5e
	.byte	0x8
	.uleb128 0x2
	.4byte	.LASF25
	.byte	0x4
	.byte	0x38
	.byte	0x14
	.4byte	0x5e
	.byte	0xc
	.uleb128 0x2
	.4byte	.LASF26
	.byte	0x4
	.byte	0x38
	.byte	0x1b
	.4byte	0x5e
	.byte	0x10
	.uleb128 0xa
	.string	"_x"
	.byte	0x39
	.byte	0xb
	.4byte	0x183
	.byte	0x14
	.byte	0
	.uleb128 0x5
	.4byte	0x128
	.uleb128 0x7
	.4byte	0x104
	.4byte	0x193
	.uleb128 0x8
	.4byte	0x6a
	.byte	0
	.byte	0
	.uleb128 0xe
	.4byte	.LASF28
	.byte	0x24
	.byte	0x3d
	.4byte	0x214
	.uleb128 0x2
	.4byte	.LASF29
	.byte	0x4
	.byte	0x3f
	.byte	0x9
	.4byte	0x5e
	.byte	0
	.uleb128 0x2
	.4byte	.LASF30
	.byte	0x4
	.byte	0x40
	.byte	0x9
	.4byte	0x5e
	.byte	0x4
	.uleb128 0x2
	.4byte	.LASF31
	.byte	0x4
	.byte	0x41
	.byte	0x9
	.4byte	0x5e
	.byte	0x8
	.uleb128 0x2
	.4byte	.LASF32
	.byte	0x4
	.byte	0x42
	.byte	0x9
	.4byte	0x5e
	.byte	0xc
	.uleb128 0x2
	.4byte	.LASF33
	.byte	0x4
	.byte	0x43
	.byte	0x9
	.4byte	0x5e
	.byte	0x10
	.uleb128 0x2
	.4byte	.LASF34
	.byte	0x4
	.byte	0x44
	.byte	0x9
	.4byte	0x5e
	.byte	0x14
	.uleb128 0x2
	.4byte	.LASF35
	.byte	0x4
	.byte	0x45
	.byte	0x9
	.4byte	0x5e
	.byte	0x18
	.uleb128 0x2
	.4byte	.LASF36
	.byte	0x4
	.byte	0x46
	.byte	0x9
	.4byte	0x5e
	.byte	0x1c
	.uleb128 0x2
	.4byte	.LASF37
	.byte	0x4
	.byte	0x47
	.byte	0x9
	.4byte	0x5e
	.byte	0x20
	.byte	0
	.uleb128 0xe
	.4byte	.LASF38
	.byte	0x8
	.byte	0x74
	.4byte	0x23a
	.uleb128 0x2
	.4byte	.LASF39
	.byte	0x4
	.byte	0x75
	.byte	0x11
	.4byte	0x23a
	.byte	0
	.uleb128 0x2
	.4byte	.LASF40
	.byte	0x4
	.byte	0x76
	.byte	0x6
	.4byte	0x5e
	.byte	0x4
	.byte	0
	.uleb128 0x5
	.4byte	0x2d
	.uleb128 0xe
	.4byte	.LASF41
	.byte	0x68
	.byte	0x99
	.4byte	0x378
	.uleb128 0xa
	.string	"_p"
	.byte	0x9a
	.byte	0x12
	.4byte	0x23a
	.byte	0
	.uleb128 0xa
	.string	"_r"
	.byte	0x9b
	.byte	0x7
	.4byte	0x5e
	.byte	0x4
	.uleb128 0xa
	.string	"_w"
	.byte	0x9c
	.byte	0x7
	.4byte	0x5e
	.byte	0x8
	.uleb128 0x2
	.4byte	.LASF42
	.byte	0x4
	.byte	0x9d
	.byte	0x9
	.4byte	0x34
	.byte	0xc
	.uleb128 0x2
	.4byte	.LASF43
	.byte	0x4
	.byte	0x9e
	.byte	0x9
	.4byte	0x34
	.byte	0xe
	.uleb128 0xa
	.string	"_bf"
	.byte	0x9f
	.byte	0x11
	.4byte	0x214
	.byte	0x10
	.uleb128 0x2
	.4byte	.LASF44
	.byte	0x4
	.byte	0xa0
	.byte	0x7
	.4byte	0x5e
	.byte	0x18
	.uleb128 0x2
	.4byte	.LASF45
	.byte	0x4
	.byte	0xa7
	.byte	0xa
	.4byte	0x102
	.byte	0x1c
	.uleb128 0x2
	.4byte	.LASF46
	.byte	0x4
	.byte	0xa9
	.byte	0x1d
	.4byte	0x48e
	.byte	0x20
	.uleb128 0x2
	.4byte	.LASF47
	.byte	0x4
	.byte	0xab
	.byte	0x1d
	.4byte	0x4b6
	.byte	0x24
	.uleb128 0x2
	.4byte	.LASF48
	.byte	0x4
	.byte	0xae
	.byte	0xd
	.4byte	0x4d9
	.byte	0x28
	.uleb128 0x2
	.4byte	.LASF49
	.byte	0x4
	.byte	0xaf
	.byte	0x9
	.4byte	0x4f2
	.byte	0x2c
	.uleb128 0xa
	.string	"_ub"
	.byte	0xb2
	.byte	0x11
	.4byte	0x214
	.byte	0x30
	.uleb128 0xa
	.string	"_up"
	.byte	0xb3
	.byte	0x12
	.4byte	0x23a
	.byte	0x38
	.uleb128 0xa
	.string	"_ur"
	.byte	0xb4
	.byte	0x7
	.4byte	0x5e
	.byte	0x3c
	.uleb128 0x2
	.4byte	.LASF50
	.byte	0x4
	.byte	0xb7
	.byte	0x11
	.4byte	0x4f7
	.byte	0x40
	.uleb128 0x2
	.4byte	.LASF51
	.byte	0x4
	.byte	0xb8
	.byte	0x11
	.4byte	0x507
	.byte	0x43
	.uleb128 0xa
	.string	"_lb"
	.byte	0xbb
	.byte	0x11
	.4byte	0x214
	.byte	0x44
	.uleb128 0x2
	.4byte	.LASF52
	.byte	0x4
	.byte	0xbe
	.byte	0x7
	.4byte	0x5e
	.byte	0x4c
	.uleb128 0x2
	.4byte	.LASF53
	.byte	0x4
	.byte	0xbf
	.byte	0xa
	.4byte	0x7e
	.byte	0x50
	.uleb128 0x2
	.4byte	.LASF54
	.byte	0x4
	.byte	0xc2
	.byte	0x12
	.4byte	0x396
	.byte	0x54
	.uleb128 0x2
	.4byte	.LASF55
	.byte	0x4
	.byte	0xc6
	.byte	0xc
	.4byte	0x11c
	.byte	0x58
	.uleb128 0x2
	.4byte	.LASF56
	.byte	0x4
	.byte	0xc8
	.byte	0xe
	.4byte	0xf6
	.byte	0x5c
	.uleb128 0x2
	.4byte	.LASF57
	.byte	0x4
	.byte	0xc9
	.byte	0x9
	.4byte	0x5e
	.byte	0x64
	.byte	0
	.uleb128 0xf
	.4byte	0x96
	.4byte	0x396
	.uleb128 0x3
	.4byte	0x396
	.uleb128 0x3
	.4byte	0x102
	.uleb128 0x3
	.4byte	0x47d
	.uleb128 0x3
	.4byte	0x5e
	.byte	0
	.uleb128 0x5
	.4byte	0x39b
	.uleb128 0x1d
	.4byte	.LASF58
	.2byte	0x140
	.byte	0x4
	.2byte	0x242
	.byte	0x8
	.4byte	0x47d
	.uleb128 0x1
	.4byte	.LASF59
	.2byte	0x244
	.byte	0x7
	.4byte	0x5e
	.byte	0
	.uleb128 0x1
	.4byte	.LASF60
	.2byte	0x249
	.byte	0xb
	.4byte	0x524
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF61
	.2byte	0x249
	.byte	0x14
	.4byte	0x524
	.byte	0x8
	.uleb128 0x1
	.4byte	.LASF62
	.2byte	0x249
	.byte	0x1e
	.4byte	0x524
	.byte	0xc
	.uleb128 0x1
	.4byte	.LASF63
	.2byte	0x24b
	.byte	0x8
	.4byte	0x5e
	.byte	0x10
	.uleb128 0x1
	.4byte	.LASF64
	.2byte	0x24c
	.byte	0x8
	.4byte	0x6c9
	.byte	0x14
	.uleb128 0x1
	.4byte	.LASF65
	.2byte	0x251
	.byte	0x16
	.4byte	0x6de
	.byte	0x30
	.uleb128 0x1
	.4byte	.LASF66
	.2byte	0x257
	.byte	0xa
	.4byte	0x6ee
	.byte	0x34
	.uleb128 0x1
	.4byte	.LASF67
	.2byte	0x25a
	.byte	0x13
	.4byte	0x17e
	.byte	0x38
	.uleb128 0x1
	.4byte	.LASF68
	.2byte	0x25b
	.byte	0x7
	.4byte	0x5e
	.byte	0x3c
	.uleb128 0x1
	.4byte	.LASF69
	.2byte	0x25c
	.byte	0x13
	.4byte	0x17e
	.byte	0x40
	.uleb128 0x1
	.4byte	.LASF70
	.2byte	0x25d
	.byte	0x14
	.4byte	0x6f3
	.byte	0x44
	.uleb128 0x1
	.4byte	.LASF71
	.2byte	0x260
	.byte	0x7
	.4byte	0x5e
	.byte	0x48
	.uleb128 0x1
	.4byte	.LASF72
	.2byte	0x261
	.byte	0x9
	.4byte	0x47d
	.byte	0x4c
	.uleb128 0x1
	.4byte	.LASF73
	.2byte	0x290
	.byte	0x7
	.4byte	0x6b1
	.byte	0x50
	.uleb128 0x1e
	.4byte	.LASF74
	.byte	0x4
	.2byte	0x298
	.byte	0xb
	.4byte	0x703
	.2byte	0x138
	.byte	0
	.uleb128 0x5
	.4byte	0x482
	.uleb128 0x9
	.byte	0x1
	.byte	0x8
	.4byte	.LASF75
	.uleb128 0x12
	.4byte	0x482
	.uleb128 0x5
	.4byte	0x378
	.uleb128 0xf
	.4byte	0x96
	.4byte	0x4b1
	.uleb128 0x3
	.4byte	0x396
	.uleb128 0x3
	.4byte	0x102
	.uleb128 0x3
	.4byte	0x4b1
	.uleb128 0x3
	.4byte	0x5e
	.byte	0
	.uleb128 0x5
	.4byte	0x489
	.uleb128 0x5
	.4byte	0x493
	.uleb128 0xf
	.4byte	0x8a
	.4byte	0x4d9
	.uleb128 0x3
	.4byte	0x396
	.uleb128 0x3
	.4byte	0x102
	.uleb128 0x3
	.4byte	0x8a
	.uleb128 0x3
	.4byte	0x5e
	.byte	0
	.uleb128 0x5
	.4byte	0x4bb
	.uleb128 0xf
	.4byte	0x5e
	.4byte	0x4f2
	.uleb128 0x3
	.4byte	0x396
	.uleb128 0x3
	.4byte	0x102
	.byte	0
	.uleb128 0x5
	.4byte	0x4de
	.uleb128 0x7
	.4byte	0x2d
	.4byte	0x507
	.uleb128 0x8
	.4byte	0x6a
	.byte	0x2
	.byte	0
	.uleb128 0x7
	.4byte	0x2d
	.4byte	0x517
	.uleb128 0x8
	.4byte	0x6a
	.byte	0
	.byte	0
	.uleb128 0x13
	.4byte	.LASF76
	.byte	0x4
	.2byte	0x10e
	.byte	0x1a
	.4byte	0x23f
	.uleb128 0x5
	.4byte	0x517
	.uleb128 0x1f
	.4byte	.LASF77
	.byte	0xe
	.byte	0x4
	.2byte	0x132
	.byte	0x8
	.4byte	0x55f
	.uleb128 0x1
	.4byte	.LASF78
	.2byte	0x133
	.byte	0x12
	.4byte	0x55f
	.byte	0
	.uleb128 0x1
	.4byte	.LASF79
	.2byte	0x134
	.byte	0x12
	.4byte	0x55f
	.byte	0x6
	.uleb128 0x1
	.4byte	.LASF80
	.2byte	0x135
	.byte	0x12
	.4byte	0x3b
	.byte	0xc
	.byte	0
	.uleb128 0x7
	.4byte	0x3b
	.4byte	0x56f
	.uleb128 0x8
	.4byte	0x6a
	.byte	0x2
	.byte	0
	.uleb128 0x20
	.byte	0xe8
	.byte	0x4
	.2byte	0x265
	.byte	0x7
	.4byte	0x671
	.uleb128 0x1
	.4byte	.LASF81
	.2byte	0x26a
	.byte	0x12
	.4byte	0x47d
	.byte	0
	.uleb128 0x1
	.4byte	.LASF82
	.2byte	0x26b
	.byte	0x10
	.4byte	0x671
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF83
	.2byte	0x26c
	.byte	0x17
	.4byte	0x193
	.byte	0x20
	.uleb128 0x1
	.4byte	.LASF84
	.2byte	0x26d
	.byte	0xf
	.4byte	0x5e
	.byte	0x44
	.uleb128 0x1
	.4byte	.LASF85
	.2byte	0x26e
	.byte	0x2c
	.4byte	0x57
	.byte	0x48
	.uleb128 0x1
	.4byte	.LASF86
	.2byte	0x26f
	.byte	0x1a
	.4byte	0x529
	.byte	0x50
	.uleb128 0x1
	.4byte	.LASF87
	.2byte	0x270
	.byte	0x16
	.4byte	0xf6
	.byte	0x60
	.uleb128 0x1
	.4byte	.LASF88
	.2byte	0x271
	.byte	0x16
	.4byte	0xf6
	.byte	0x68
	.uleb128 0x1
	.4byte	.LASF89
	.2byte	0x272
	.byte	0x16
	.4byte	0xf6
	.byte	0x70
	.uleb128 0x1
	.4byte	.LASF90
	.2byte	0x273
	.byte	0x10
	.4byte	0x681
	.byte	0x78
	.uleb128 0x1
	.4byte	.LASF91
	.2byte	0x274
	.byte	0x10
	.4byte	0x691
	.byte	0x80
	.uleb128 0x1
	.4byte	.LASF92
	.2byte	0x275
	.byte	0xf
	.4byte	0x5e
	.byte	0x98
	.uleb128 0x1
	.4byte	.LASF93
	.2byte	0x276
	.byte	0x16
	.4byte	0xf6
	.byte	0x9c
	.uleb128 0x1
	.4byte	.LASF94
	.2byte	0x277
	.byte	0x16
	.4byte	0xf6
	.byte	0xa4
	.uleb128 0x1
	.4byte	.LASF95
	.2byte	0x278
	.byte	0x16
	.4byte	0xf6
	.byte	0xac
	.uleb128 0x1
	.4byte	.LASF96
	.2byte	0x279
	.byte	0x16
	.4byte	0xf6
	.byte	0xb4
	.uleb128 0x1
	.4byte	.LASF97
	.2byte	0x27a
	.byte	0x16
	.4byte	0xf6
	.byte	0xbc
	.uleb128 0x1
	.4byte	.LASF98
	.2byte	0x27b
	.byte	0x8
	.4byte	0x5e
	.byte	0xc4
	.uleb128 0x1
	.4byte	.LASF99
	.2byte	0x287
	.byte	0x9
	.4byte	0x6a1
	.byte	0xc8
	.byte	0
	.uleb128 0x7
	.4byte	0x482
	.4byte	0x681
	.uleb128 0x8
	.4byte	0x6a
	.byte	0x19
	.byte	0
	.uleb128 0x7
	.4byte	0x482
	.4byte	0x691
	.uleb128 0x8
	.4byte	0x6a
	.byte	0x7
	.byte	0
	.uleb128 0x7
	.4byte	0x482
	.4byte	0x6a1
	.uleb128 0x8
	.4byte	0x6a
	.byte	0x17
	.byte	0
	.uleb128 0x7
	.4byte	0x482
	.4byte	0x6b1
	.uleb128 0x8
	.4byte	0x6a
	.byte	0x1f
	.byte	0
	.uleb128 0x21
	.byte	0xe8
	.byte	0x4
	.2byte	0x263
	.byte	0x3
	.4byte	0x6c9
	.uleb128 0x22
	.4byte	.LASF58
	.byte	0x4
	.2byte	0x288
	.byte	0xb
	.4byte	0x56f
	.byte	0
	.uleb128 0x7
	.4byte	0x482
	.4byte	0x6d9
	.uleb128 0x8
	.4byte	0x6a
	.byte	0x18
	.byte	0
	.uleb128 0x23
	.4byte	.LASF117
	.uleb128 0x5
	.4byte	0x6d9
	.uleb128 0x15
	.4byte	0x6ee
	.uleb128 0x3
	.4byte	0x396
	.byte	0
	.uleb128 0x5
	.4byte	0x6e3
	.uleb128 0x5
	.4byte	0x17e
	.uleb128 0x15
	.4byte	0x703
	.uleb128 0x3
	.4byte	0x5e
	.byte	0
	.uleb128 0x5
	.4byte	0x708
	.uleb128 0x5
	.4byte	0x6f8
	.uleb128 0x24
	.4byte	.LASF101
	.byte	0x4
	.2byte	0x31d
	.byte	0x17
	.4byte	0x396
	.uleb128 0xc
	.4byte	.LASF100
	.byte	0x6
	.byte	0x42
	.byte	0x10
	.4byte	0x517
	.uleb128 0x7
	.4byte	0x5e
	.4byte	0x736
	.uleb128 0x8
	.4byte	0x6a
	.byte	0x9
	.byte	0
	.uleb128 0x16
	.string	"src"
	.byte	0xf
	.byte	0x5
	.4byte	0x726
	.uleb128 0x5
	.byte	0x3
	.4byte	src
	.uleb128 0x10
	.4byte	.LASF102
	.byte	0x10
	.byte	0x5
	.4byte	0x726
	.uleb128 0x5
	.byte	0x3
	.4byte	src_backup
	.uleb128 0x10
	.4byte	.LASF103
	.byte	0x11
	.byte	0xb
	.4byte	0x65
	.uleb128 0x5
	.byte	0x3
	.4byte	num_elements
	.uleb128 0x7
	.4byte	0x34
	.4byte	0x779
	.uleb128 0x8
	.4byte	0x6a
	.byte	0x13
	.byte	0
	.uleb128 0x16
	.string	"dst"
	.byte	0x12
	.byte	0x7
	.4byte	0x769
	.uleb128 0x5
	.byte	0x3
	.4byte	dst
	.uleb128 0x7
	.4byte	0x482
	.4byte	0x79a
	.uleb128 0x8
	.4byte	0x6a
	.byte	0x10
	.byte	0
	.uleb128 0x10
	.4byte	.LASF104
	.byte	0x13
	.byte	0x6
	.4byte	0x78a
	.uleb128 0x5
	.byte	0x3
	.4byte	message
	.uleb128 0x7
	.4byte	0x482
	.4byte	0x7bb
	.uleb128 0x8
	.4byte	0x6a
	.byte	0x8
	.byte	0
	.uleb128 0x10
	.4byte	.LASF105
	.byte	0x14
	.byte	0x6
	.4byte	0x7ab
	.uleb128 0x5
	.byte	0x3
	.4byte	password
	.uleb128 0x11
	.4byte	.LASF106
	.byte	0x1
	.byte	0xb
	.4byte	0x5e
	.4byte	0x7f0
	.uleb128 0x3
	.4byte	0x7f0
	.uleb128 0x3
	.4byte	0x7f5
	.uleb128 0x3
	.4byte	0x5e
	.uleb128 0x3
	.4byte	0x5e
	.byte	0
	.uleb128 0x5
	.4byte	0x5e
	.uleb128 0x5
	.4byte	0x34
	.uleb128 0x17
	.4byte	.LASF109
	.byte	0xa
	.4byte	0x814
	.uleb128 0x3
	.4byte	0x7f0
	.uleb128 0x3
	.4byte	0x7f5
	.uleb128 0x3
	.4byte	0x5e
	.byte	0
	.uleb128 0x11
	.4byte	.LASF107
	.byte	0x6
	.byte	0xc6
	.4byte	0x5e
	.4byte	0x829
	.uleb128 0x3
	.4byte	0x829
	.byte	0
	.uleb128 0x5
	.4byte	0x71a
	.uleb128 0x11
	.4byte	.LASF108
	.byte	0x6
	.byte	0xce
	.4byte	0x5e
	.4byte	0x844
	.uleb128 0x3
	.4byte	0x4b1
	.uleb128 0x25
	.byte	0
	.uleb128 0x17
	.4byte	.LASF110
	.byte	0xd
	.4byte	0x854
	.uleb128 0x3
	.4byte	0x47d
	.byte	0
	.uleb128 0x26
	.4byte	.LASF118
	.byte	0x1
	.byte	0x7b
	.byte	0x6
	.4byte	.LFB5
	.4byte	.LFE5-.LFB5
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x89b
	.uleb128 0x6
	.4byte	.LBB26
	.4byte	.LBE26-.LBB26
	.4byte	0x884
	.uleb128 0x4
	.string	"i"
	.byte	0x7d
	.byte	0x16
	.4byte	0x6a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0xd
	.4byte	.LBB27
	.4byte	.LBE27-.LBB27
	.uleb128 0x4
	.string	"i"
	.byte	0x83
	.byte	0x16
	.4byte	0x6a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.byte	0
	.uleb128 0x27
	.4byte	.LASF119
	.byte	0x1
	.byte	0x1c
	.byte	0x5
	.4byte	0x5e
	.4byte	.LFB4
	.4byte	.LFE4-.LFB4
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xb48
	.uleb128 0xb
	.4byte	.LASF111
	.byte	0x26
	.byte	0x9
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x6
	.4byte	.LBB4
	.4byte	.LBE4-.LBB4
	.4byte	0x8dd
	.uleb128 0x4
	.string	"i"
	.byte	0x24
	.byte	0xd
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x6
	.4byte	.LBB5
	.4byte	.LBE5-.LBB5
	.4byte	0x943
	.uleb128 0xb
	.4byte	.LASF112
	.byte	0x2d
	.byte	0xd
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x6
	.4byte	.LBB6
	.4byte	.LBE6-.LBB6
	.4byte	0x912
	.uleb128 0x4
	.string	"i"
	.byte	0x2e
	.byte	0x11
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x6
	.4byte	.LBB7
	.4byte	.LBE7-.LBB7
	.4byte	0x92c
	.uleb128 0x4
	.string	"i"
	.byte	0x2f
	.byte	0x11
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.byte	0
	.uleb128 0xd
	.4byte	.LBB8
	.4byte	.LBE8-.LBB8
	.uleb128 0x4
	.string	"i"
	.byte	0x30
	.byte	0x11
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.byte	0
	.byte	0
	.uleb128 0x6
	.4byte	.LBB9
	.4byte	.LBE9-.LBB9
	.4byte	0x9a9
	.uleb128 0xb
	.4byte	.LASF112
	.byte	0x3a
	.byte	0xd
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x6
	.4byte	.LBB10
	.4byte	.LBE10-.LBB10
	.4byte	0x978
	.uleb128 0x4
	.string	"i"
	.byte	0x3b
	.byte	0x11
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.byte	0
	.uleb128 0x6
	.4byte	.LBB11
	.4byte	.LBE11-.LBB11
	.4byte	0x992
	.uleb128 0x4
	.string	"i"
	.byte	0x3c
	.byte	0x11
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -52
	.byte	0
	.uleb128 0xd
	.4byte	.LBB12
	.4byte	.LBE12-.LBB12
	.uleb128 0x4
	.string	"i"
	.byte	0x3d
	.byte	0x11
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.byte	0
	.byte	0
	.uleb128 0x6
	.4byte	.LBB13
	.4byte	.LBE13-.LBB13
	.4byte	0x9f6
	.uleb128 0xb
	.4byte	.LASF112
	.byte	0x47
	.byte	0xd
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -60
	.uleb128 0x6
	.4byte	.LBB14
	.4byte	.LBE14-.LBB14
	.4byte	0x9de
	.uleb128 0x4
	.string	"i"
	.byte	0x48
	.byte	0x11
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.byte	0
	.uleb128 0xd
	.4byte	.LBB15
	.4byte	.LBE15-.LBB15
	.uleb128 0x4
	.string	"i"
	.byte	0x49
	.byte	0x11
	.4byte	0x5e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -68
	.byte	0
	.byte	0
	.uleb128 0x6
	.4byte	.LBB16
	.4byte	.LBE16-.LBB16
	.4byte	0xaa8
	.uleb128 0xb
	.4byte	.LASF113
	.byte	0x53
	.byte	0xd
	.4byte	0x5e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -112
	.uleb128 0x4
	.string	"end"
	.byte	0x54
	.byte	0xd
	.4byte	0x5e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -116
	.uleb128 0xb
	.4byte	.LASF112
	.byte	0x55
	.byte	0xd
	.4byte	0x5e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -72
	.uleb128 0xb
	.4byte	.LASF114
	.byte	0x56
	.byte	0xd
	.4byte	0x5e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -120
	.uleb128 0x6
	.4byte	.LBB17
	.4byte	.LBE17-.LBB17
	.4byte	0xa5a
	.uleb128 0x4
	.string	"i"
	.byte	0x58
	.byte	0x11
	.4byte	0x5e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -76
	.byte	0
	.uleb128 0x6
	.4byte	.LBB18
	.4byte	.LBE18-.LBB18
	.4byte	0xa75
	.uleb128 0x4
	.string	"i"
	.byte	0x59
	.byte	0x11
	.4byte	0x5e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -80
	.byte	0
	.uleb128 0x6
	.4byte	.LBB19
	.4byte	.LBE19-.LBB19
	.4byte	0xa90
	.uleb128 0x4
	.string	"i"
	.byte	0x5a
	.byte	0x11
	.4byte	0x5e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -84
	.byte	0
	.uleb128 0xd
	.4byte	.LBB20
	.4byte	.LBE20-.LBB20
	.uleb128 0x4
	.string	"i"
	.byte	0x5b
	.byte	0x11
	.4byte	0x5e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -88
	.byte	0
	.byte	0
	.uleb128 0xd
	.4byte	.LBB21
	.4byte	.LBE21-.LBB21
	.uleb128 0xb
	.4byte	.LASF113
	.byte	0x63
	.byte	0xd
	.4byte	0x5e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -124
	.uleb128 0x4
	.string	"end"
	.byte	0x64
	.byte	0xd
	.4byte	0x5e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -128
	.uleb128 0xb
	.4byte	.LASF112
	.byte	0x65
	.byte	0xd
	.4byte	0x5e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -92
	.uleb128 0x6
	.4byte	.LBB22
	.4byte	.LBE22-.LBB22
	.4byte	0xaf9
	.uleb128 0x4
	.string	"i"
	.byte	0x67
	.byte	0x11
	.4byte	0x5e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -96
	.byte	0
	.uleb128 0x6
	.4byte	.LBB23
	.4byte	.LBE23-.LBB23
	.4byte	0xb14
	.uleb128 0x4
	.string	"i"
	.byte	0x68
	.byte	0x11
	.4byte	0x5e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -100
	.byte	0
	.uleb128 0x6
	.4byte	.LBB24
	.4byte	.LBE24-.LBB24
	.4byte	0xb2f
	.uleb128 0x4
	.string	"i"
	.byte	0x69
	.byte	0x11
	.4byte	0x5e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -104
	.byte	0
	.uleb128 0xd
	.4byte	.LBB25
	.4byte	.LBE25-.LBB25
	.uleb128 0x4
	.string	"i"
	.byte	0x6a
	.byte	0x11
	.4byte	0x5e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -108
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x28
	.4byte	.LASF115
	.byte	0x1
	.byte	0x16
	.byte	0x6
	.4byte	.LFB3
	.4byte	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x6
	.4byte	.LBB2
	.4byte	.LBE2-.LBB2
	.4byte	0xb74
	.uleb128 0x4
	.string	"i"
	.byte	0x18
	.byte	0xd
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0xd
	.4byte	.LBB3
	.4byte	.LBE3-.LBB3
	.uleb128 0x4
	.string	"i"
	.byte	0x19
	.byte	0xd
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 8
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 5
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 3
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 13
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x1f
	.uleb128 0x1b
	.uleb128 0x1f
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x17
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x20
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x21
	.uleb128 0x17
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x22
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x23
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x24
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x25
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x26
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x27
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x28
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.4byte	0x1c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	0
	.4byte	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF108:
	.string	"printf"
.LASF40:
	.string	"_size"
.LASF77:
	.string	"_rand48"
.LASF64:
	.string	"_emergency"
.LASF54:
	.string	"_data"
.LASF105:
	.string	"password"
.LASF96:
	.string	"_wcrtomb_state"
.LASF97:
	.string	"_wcsrtombs_state"
.LASF9:
	.string	"long long unsigned int"
.LASF44:
	.string	"_lbfsize"
.LASF117:
	.string	"__locale_t"
.LASF94:
	.string	"_mbrtowc_state"
.LASF103:
	.string	"num_elements"
.LASF89:
	.string	"_wctomb_state"
.LASF29:
	.string	"__tm_sec"
.LASF8:
	.string	"long long int"
.LASF2:
	.string	"signed char"
.LASF50:
	.string	"_ubuf"
.LASF110:
	.string	"hidden"
.LASF39:
	.string	"_base"
.LASF31:
	.string	"__tm_hour"
.LASF45:
	.string	"_cookie"
.LASF118:
	.string	"decrypt_message"
.LASF6:
	.string	"long int"
.LASF42:
	.string	"_flags"
.LASF100:
	.string	"FILE"
.LASF60:
	.string	"_stdin"
.LASF52:
	.string	"_blksize"
.LASF72:
	.string	"_cvtbuf"
.LASF53:
	.string	"_offset"
.LASF119:
	.string	"main"
.LASF95:
	.string	"_mbsrtowcs_state"
.LASF93:
	.string	"_mbrlen_state"
.LASF25:
	.string	"_sign"
.LASF112:
	.string	"passed"
.LASF22:
	.string	"_flock_t"
.LASF62:
	.string	"_stderr"
.LASF27:
	.string	"_Bigint"
.LASF84:
	.string	"_gamma_signgam"
.LASF46:
	.string	"_read"
.LASF68:
	.string	"_result_k"
.LASF28:
	.string	"__tm"
.LASF10:
	.string	"unsigned int"
.LASF16:
	.string	"__wchb"
.LASF61:
	.string	"_stdout"
.LASF71:
	.string	"_cvtlen"
.LASF7:
	.string	"long unsigned int"
.LASF43:
	.string	"_file"
.LASF5:
	.string	"short unsigned int"
.LASF91:
	.string	"_signal_buf"
.LASF82:
	.string	"_asctime_buf"
.LASF67:
	.string	"_result"
.LASF15:
	.string	"__wch"
.LASF11:
	.string	"wint_t"
.LASF55:
	.string	"_lock"
.LASF57:
	.string	"_flags2"
.LASF47:
	.string	"_write"
.LASF34:
	.string	"__tm_year"
.LASF104:
	.string	"message"
.LASF33:
	.string	"__tm_mon"
.LASF115:
	.string	"init_vectors"
.LASF12:
	.string	"_off_t"
.LASF70:
	.string	"_freelist"
.LASF99:
	.string	"_getlocalename_l_buf"
.LASF107:
	.string	"fflush"
.LASF21:
	.string	"_LOCK_RECURSIVE_T"
.LASF3:
	.string	"unsigned char"
.LASF73:
	.string	"_new"
.LASF98:
	.string	"_h_errno"
.LASF4:
	.string	"short int"
.LASF36:
	.string	"__tm_yday"
.LASF38:
	.string	"__sbuf"
.LASF76:
	.string	"__FILE"
.LASF19:
	.string	"_mbstate_t"
.LASF41:
	.string	"__sFILE"
.LASF56:
	.string	"_mbstate"
.LASF85:
	.string	"_rand_next"
.LASF87:
	.string	"_mblen_state"
.LASF63:
	.string	"_inc"
.LASF65:
	.string	"_locale"
.LASF66:
	.string	"__cleanup"
.LASF24:
	.string	"_maxwds"
.LASF58:
	.string	"_reent"
.LASF78:
	.string	"_seed"
.LASF17:
	.string	"__count"
.LASF18:
	.string	"__value"
.LASF48:
	.string	"_seek"
.LASF101:
	.string	"_impure_ptr"
.LASF111:
	.string	"num_passed_tests"
.LASF13:
	.string	"_fpos_t"
.LASF59:
	.string	"_errno"
.LASF75:
	.string	"char"
.LASF30:
	.string	"__tm_min"
.LASF116:
	.string	"GNU C99 15.0.1 20250308 (experimental) -mabi=ilp32f -misa-spec=20191213 -march=rv32imf_zicsr_zmmul -g -std=c99"
.LASF79:
	.string	"_mult"
.LASF23:
	.string	"_next"
.LASF81:
	.string	"_strtok_last"
.LASF80:
	.string	"_add"
.LASF20:
	.string	"__ULong"
.LASF92:
	.string	"_getdate_err"
.LASF106:
	.string	"copyelements"
.LASF26:
	.string	"_wds"
.LASF35:
	.string	"__tm_wday"
.LASF14:
	.string	"_ssize_t"
.LASF90:
	.string	"_l64a_buf"
.LASF74:
	.string	"_sig_func"
.LASF109:
	.string	"copyvec"
.LASF51:
	.string	"_nbuf"
.LASF37:
	.string	"__tm_isdst"
.LASF83:
	.string	"_localtime_buf"
.LASF49:
	.string	"_close"
.LASF86:
	.string	"_r48"
.LASF113:
	.string	"start"
.LASF88:
	.string	"_mbtowc_state"
.LASF69:
	.string	"_p5s"
.LASF102:
	.string	"src_backup"
.LASF114:
	.string	"result"
.LASF32:
	.string	"__tm_mday"
	.section	.debug_line_str,"MS",@progbits,1
.LASF1:
	.string	"/Users/elias-work/Desktop/mop-lecture_notes/lab_preparations-LP4/lab1/code"
.LASF0:
	.string	"src/main.c"
	.ident	"GCC: ('riscv-embecosm-embedded-macos-20250309') 15.0.1 20250308 (experimental)"
	.section	.note.GNU-stack,"",@progbits
