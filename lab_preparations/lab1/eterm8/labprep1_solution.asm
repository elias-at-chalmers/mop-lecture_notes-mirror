###############################################################################
# Lab 1 - Preparation Task
#
#         Your task is to write an assembler function which will be tested 
#by external C code. 
#         The test harness will output results on the USART, so, if you are
#         running on Simserver, you need to attach a Console to USART1:
#         Serial communication interface 1 -> 06 Console
#
#         Start with Assignment 1 and work your way through the rest of the 
#         assignments in order.
###############################################################################

###############################################################################
# Code Section ()
###############################################################################
.text
.global _start

    j       _start                  # Jump to entry point (address 0x20000000
                                    # is always the first instruction executed)


hidden:
    ###########################################################################
    # Assignment 1: 
    # This short assignment will teach you to use the debugger and memory view.
    # The function is called from the startup code below, so place a breakpoint
    # on the `nop` instruction below.
    #
    # The only parameter to this function is the address of an array we have 
    # hidden somewhere in memory.  Before calling this function, the startup
    # code placed that address in register a0 (first parameter register).
    #
    # 1.1:  Use the register list to find out what that address is and write
    #       it down.
    #
    # 1.2:  Open the "memory" view and type in the address you just found to
    #       see what is in memory at that location.
    #
    #       The message is encoded as 10 ascii characters (10 bytes.)
    #       What is the message we have hidden?
    #
    # Once you have written down the address and the message, move on to the
    # next assignment.
    ###########################################################################
    nop
    ret


###############################################################################
# void copyvec(int src[], short dst[], int size);
# ================================================
# Copy `size` elements from array src (int) to array dst (short).
# 
# Input: 
#   a0: address of src[0]  (signed int,   4 bytes per element)
#   a1: address of dst[0]  (signed short, 2 bytes per element)
#   a2: number of elements to copy
###############################################################################

copyvec: 
    ###########################################################################
    # Assignment 2: 
    # Write a loop that copies `size` elements from src to dst
    ###########################################################################
    li t0, 0                        # t0: loop counter
    mv t1, a0                       # Address to src element
    mv t2, a1                       # Address to dst element
loop: 
    beq t0, a2, done                # If loop counter reached size, we are done
    lw t3, 0(t1)                    # Load element from src
    addi t1, t1, 4                  # Move t1 to next element
    sh t3, 0(t2)                    # Store element in dst
    addi t2, t2, 2                  # Move t2 to next element
    addi t0, t0, 1                  # Increase loop counter
    j loop
done: 
    ret                             # Return from function

###############################################################################
# int copyelements(int src[], short dst[], int start, int end);
# ===============================================================
# Copy elements src[start..end] to dst[start..end] using copyvec.
# Returns the number of elements copied.
# 
# Input: 
#   a0: address of src[0]  (signed int)
#   a1: address of dst[0]  (signed short)
#   a2: first element index to copy
#   a3: last  element index to copy
#
# Output: 
#   a0: number of elements copied
###############################################################################

copyelements: 
    
    ###########################################################################
    # Assignment 4 (do Assignment 3 first): 
    # Push callee-saved registers to the stack
    ###########################################################################
    addi sp, sp, -8
    sw s0, 4(sp)
    sw ra, 0(sp)

    ###########################################################################
    # Assignment 3: 
    # Implement this pseudo code:
    # 
    #   src_start = <address of src[start]>
    #   dst_start = <address of dst[start]>
    #   size      = end - start + 1
    #   copyvec(src_start, dst_start, size)
    ###########################################################################
    li t0, 4
    mul t0, a2, t0                  # t0 = start * 4  (byte offset in src)
    add t0, a0, t0                  # t0 = &src[start]
    li t1, 2
    mul t1, a2, t1                  # t1 = start * 2  (byte offset in dst)
    add t1, a1, t1                  # t1 = &dst[start]
    sub t2, a3, a2                  
    addi s0, t2, 1                  # s0 = end - start + 1  (number of elements)
    mv a0, t0
    mv a1, t1
    mv a2, s0
    call copyvec
    mv a0, s0                       # return value = number of elements copied

    ###########################################################################
    # Assignment 5:
    # Restore values pushed to the stack
    ###########################################################################
    lw s0, 4(sp)
    lw ra, 0(sp)
    addi sp, sp, 8

    ret


###############################################################################
# Assignment 6:
# If all tests passed, the secret password will be printed in the console.
# Submit that along with the hidden message and address in Canvas.
###############################################################################








###############################################################################
# Constants (No more assignments below!)
###############################################################################
.equ NUM_ELEMENTS,      10
.equ NUM_DST_ELEMENTS,  20          # NUM_ELEMENTS * 2

# --- RCC (Reset and Clock Control) ---
.equ RCC_APB2PCENR,     0x40021018  # APB2 peripheral clock enable register

# --- GPIOA ---
.equ GPIOA_CRH,         0x40010804  # Port config register high (PA8-PA15)

# --- USART1 ---
.equ USART1_STATR,      0x40013800  # Status register
.equ USART1_DATAR,      0x40013804  # Data register
.equ USART1_BRR,        0x40013808  # Baud rate register
.equ USART1_CTLR1,      0x4001380C  # Control register 1

# Baud rate divisor: PCLK2 / desired_baud
# For 8 MHz HSI (default, no PLL) and 115200 baud: 8000000/115200 ≈ 69 = 0x45
# For 144 MHz (PLL configured) and 115200 baud: 144000000/115200 = 1250 = 0x4E2
# Adjust this value to match your system clock configuration.
.equ USART1_BRR_VAL,    0x45


#==============================================================================
# _start  -  Entry point (replaces main() from the C test harness)
#
# Register usage across the test harness:
#   s0 : number of passed tests (callee-saved, safe across calls)
#   s1 : "passed" flag for the current test
#==============================================================================
_start:
    # ---- Stack at top of 64 KB SRAM (0x20000000 .. 0x2000FFFF) ----
    li      sp, 0x20010000

    # ---- Initialize USART1 so we can print results ----
    call    usart1_init

    # ---- Print a few blank lines (like the C code does) ----
    la      a0, str_newlines
    call    uart_puts

    # ---- Decrypt hidden message and password ----
    call    decrypt_message

    # ---- Assignment 1: call hidden(message) ----
    # Place a breakpoint inside hidden() and examine a0 in the debugger.
    la      a0, message
    call    hidden

    # ---- Back up the source array before running any tests ----
    call    backup_src

    # ---- Test pass counter ----
    li      s0, 0

    ########################################################################
    # Testing copyvec()
    ########################################################################
    la      a0, str_test_copyvec
    call    uart_puts

    #===================================================================
    # Test 1 – Copy ALL elements
    #===================================================================
    call    init_vectors
    la      a0, str_copy_all
    call    uart_puts

    la      a0, src
    la      a1, dst
    li      a2, NUM_ELEMENTS
    call    copyvec

    li      s1, 1                   # assume passed

    # dst[0 .. 9] must equal src[0 .. 9]
    li      a0, 0
    li      a1, NUM_ELEMENTS
    call    verify_dst_eq_src
    and     s1, s1, a0

    # dst[10 .. 19] must be zero
    li      a0, NUM_ELEMENTS
    li      a1, NUM_DST_ELEMENTS
    call    verify_dst_zero
    and     s1, s1, a0

    # src must be unchanged
    call    verify_src_unchanged
    and     s1, s1, a0

    beqz    s1, test1_fail
    la      a0, str_passed
    call    uart_puts
    addi    s0, s0, 1
    j       test1_done
test1_fail:
    la      a0, str_failed
    call    uart_puts
test1_done:

    #===================================================================
    # Test 2 – Copy SOME (half) elements
    #===================================================================
    call    init_vectors
    la      a0, str_copy_some
    call    uart_puts

    la      a0, src
    la      a1, dst
    li      a2, NUM_ELEMENTS / 2    # 5
    call    copyvec

    li      s1, 1

    # dst[0 .. 4] must equal src[0 .. 4]
    li      a0, 0
    li      a1, NUM_ELEMENTS / 2
    call    verify_dst_eq_src
    and     s1, s1, a0

    # dst[5 .. 19] must be zero
    li      a0, NUM_ELEMENTS / 2
    li      a1, NUM_DST_ELEMENTS
    call    verify_dst_zero
    and     s1, s1, a0

    # src must be unchanged
    call    verify_src_unchanged
    and     s1, s1, a0

    beqz    s1, test2_fail
    la      a0, str_passed
    call    uart_puts
    addi    s0, s0, 1
    j       test2_done
test2_fail:
    la      a0, str_failed
    call    uart_puts
test2_done:

    #===================================================================
    # Test 3 – Copy NO elements
    #===================================================================
    call    init_vectors
    la      a0, str_copy_none
    call    uart_puts

    la      a0, src
    la      a1, dst
    li      a2, 0
    call    copyvec

    li      s1, 1

    # dst[0 .. 19] must all be zero
    li      a0, 0
    li      a1, NUM_DST_ELEMENTS
    call    verify_dst_zero
    and     s1, s1, a0

    # src must be unchanged
    call    verify_src_unchanged
    and     s1, s1, a0

    beqz    s1, test3_fail
    la      a0, str_passed
    call    uart_puts
    addi    s0, s0, 1
    j       test3_done
test3_fail:
    la      a0, str_failed
    call    uart_puts
test3_done:

    ########################################################################
    # Testing copyelements()
    ########################################################################
    la      a0, str_test_copyelems
    call    uart_puts

    #===================================================================
    # Test 4 – copyelements: slice [3, 7]
    #===================================================================
    call    init_vectors
    la      a0, str_copy_slice
    call    uart_puts

    la      a0, src
    la      a1, dst
    li      a2, 3                   # start
    li      a3, 7                   # end  (NUM_ELEMENTS - 3)
    call    copyelements

    li      s1, 1

    # Return value must be end - start + 1 = 5
    li      t0, 5
    beq     a0, t0, test4_ret_ok
    li      s1, 0
    la      a0, str_wrong_ret
    call    uart_puts
test4_ret_ok:

    # dst[0 .. 2] must be zero
    li      a0, 0
    li      a1, 3
    call    verify_dst_zero
    and     s1, s1, a0

    # dst[3 .. 7] must equal src[3 .. 7]
    li      a0, 3
    li      a1, 8                   # exclusive upper bound
    call    verify_dst_eq_src
    and     s1, s1, a0

    # dst[8 .. 9] must be zero
    li      a0, 8
    li      a1, NUM_ELEMENTS
    call    verify_dst_zero
    and     s1, s1, a0

    # src must be unchanged
    call    verify_src_unchanged
    and     s1, s1, a0

    beqz    s1, test4_fail
    la      a0, str_passed
    call    uart_puts
    addi    s0, s0, 1
    j       test4_done
test4_fail:
    la      a0, str_failed
    call    uart_puts
test4_done:

    #===================================================================
    # Test 5 – copyelements: slice [4, 8]
    #===================================================================
    call    init_vectors
    la      a0, str_copy_slice
    call    uart_puts

    la      a0, src
    la      a1, dst
    li      a2, 4                   # start
    li      a3, 8                   # end  (NUM_ELEMENTS - 2)
    call    copyelements

    li      s1, 1

    # (C test 5 does not check the return value)

    # dst[0 .. 3] must be zero
    li      a0, 0
    li      a1, 4
    call    verify_dst_zero
    and     s1, s1, a0

    # dst[4 .. 8] must equal src[4 .. 8]
    li      a0, 4
    li      a1, 9                   # exclusive upper bound
    call    verify_dst_eq_src
    and     s1, s1, a0

    # dst[9] must be zero
    li      a0, 9
    li      a1, NUM_ELEMENTS
    call    verify_dst_zero
    and     s1, s1, a0

    # src must be unchanged
    call    verify_src_unchanged
    and     s1, s1, a0

    beqz    s1, test5_fail
    la      a0, str_passed
    call    uart_puts
    addi    s0, s0, 1
    j       test5_done
test5_fail:
    la      a0, str_failed
    call    uart_puts
test5_done:

    ########################################################################
    # Final verdict
    ########################################################################
    li      t0, 5
    bne     s0, t0, not_all_passed

    la      a0, str_all_passed
    call    uart_puts
    la      a0, password
    call    uart_puts
    la      a0, str_newline
    call    uart_puts
    j       halt

not_all_passed:
    la      a0, str_some_failed
    call    uart_puts

halt:
    j       halt                    # spin forever


###############################################################################
#                       USART1 INITIALIZATION
#
# Enables clocks for GPIOA and USART1, configures PA9 as AF push-pull TX
# and PA10 as floating-input RX, sets the baud rate, and turns on the USART.
###############################################################################
usart1_init:
    # 1) Enable peripheral clocks: IOPAEN (bit 2) and USART1EN (bit 14)
    li      t0, RCC_APB2PCENR
    lw      t1, 0(t0)
    li      t2, (1 << 14) | (1 << 2)
    or      t1, t1, t2
    sw      t1, 0(t0)

    # 2) Configure GPIOA pin modes in CRH (controls PA8-PA15)
    #    PA9  (TX):  CNF=10 (AF push-pull), MODE=11 (50 MHz output) → 0xB
    #    PA10 (RX):  CNF=01 (floating input), MODE=00 (input)       → 0x4
    li      t0, GPIOA_CRH
    lw      t1, 0(t0)
    li      t2, 0xFFFFF00F          # clear bits [11:4] (PA9 + PA10 fields)
    and     t1, t1, t2
    li      t2, 0x000004B0          # PA10=0x4, PA9=0xB
    or      t1, t1, t2
    sw      t1, 0(t0)

    # 3) Baud-rate register
    li      t0, USART1_BRR
    li      t1, USART1_BRR_VAL
    sw      t1, 0(t0)

    # 4) Enable USART: UE (bit 13), TE (bit 3), RE (bit 2)
    li      t0, USART1_CTLR1
    li      t1, (1 << 13) | (1 << 3) | (1 << 2)
    sw      t1, 0(t0)

    ret


###############################################################################
#                          UART PRINT FUNCTIONS
###############################################################################

# --------------------------------------------------------------------------
# uart_putc – transmit one character
# Input:  a0 = character
# --------------------------------------------------------------------------
uart_putc:
    li      t0, USART1_STATR
1:  lw      t1, 0(t0)
    andi    t1, t1, (1 << 7)        # TXE – Transmit Data Register Empty
    beqz    t1, 1b
    li      t0, USART1_DATAR
    sw      a0, 0(t0)
    ret

# --------------------------------------------------------------------------
# uart_puts – transmit a null-terminated string
# Input:  a0 = address of string
# --------------------------------------------------------------------------
uart_puts:
    addi    sp, sp, -8
    sw      ra, 4(sp)
    sw      s3, 0(sp)
    mv      s3, a0
1:  lbu     a0, 0(s3)
    beqz    a0, 2f
    call    uart_putc
    addi    s3, s3, 1
    j       1b
2:  lw      s3, 0(sp)
    lw      ra, 4(sp)
    addi    sp, sp, 8
    ret


###############################################################################
#                          HELPER FUNCTIONS
###############################################################################

# --------------------------------------------------------------------------
# decrypt_message – decrypt message[] and password[] in-place
# Cipher: new_byte = 0x9B - old_byte   (i.e. 'A' + 'Z' - old)
# --------------------------------------------------------------------------
decrypt_message:
    li      t2, 0x9B                # constant:  'A' + 'Z'

    la      t0, message
1:  lbu     t1, 0(t0)
    beqz    t1, 2f
    sub     t1, t2, t1
    sb      t1, 0(t0)
    addi    t0, t0, 1
    j       1b

2:  la      t0, password
3:  lbu     t1, 0(t0)
    beqz    t1, 4f
    sub     t1, t2, t1
    sb      t1, 0(t0)
    addi    t0, t0, 1
    j       3b
4:  ret

# --------------------------------------------------------------------------
# init_vectors – zero the entire dst array and restore src from backup
# --------------------------------------------------------------------------
init_vectors:
    # Zero dst (NUM_DST_ELEMENTS shorts = 40 bytes)
    la      t0, dst
    li      t1, NUM_DST_ELEMENTS
1:  beqz    t1, 2f
    sh      zero, 0(t0)
    addi    t0, t0, 2
    addi    t1, t1, -1
    j       1b
2:
    # Restore src from src_backup (NUM_ELEMENTS ints = 40 bytes)
    la      t0, src
    la      t1, src_backup
    li      t2, NUM_ELEMENTS
3:  beqz    t2, 4f
    lw      t3, 0(t1)
    sw      t3, 0(t0)
    addi    t0, t0, 4
    addi    t1, t1, 4
    addi    t2, t2, -1
    j       3b
4:  ret

# --------------------------------------------------------------------------
# backup_src – one-time copy of src[] → src_backup[] at startup
# --------------------------------------------------------------------------
backup_src:
    la      t0, src
    la      t1, src_backup
    li      t2, NUM_ELEMENTS
1:  beqz    t2, 2f
    lw      t3, 0(t0)
    sw      t3, 0(t1)
    addi    t0, t0, 4
    addi    t1, t1, 4
    addi    t2, t2, -1
    j       1b
2:  ret


###############################################################################
#                      VERIFICATION HELPERS
# These are leaf functions (no further calls), so they only clobber
# t-registers and return the result in a0.
###############################################################################

# --------------------------------------------------------------------------
# verify_dst_eq_src – check dst[start .. end-1] == src[start .. end-1]
# Input:  a0 = start index (inclusive)
#         a1 = end   index (exclusive)
# Output: a0 = 1 if all match, 0 otherwise
# Note:   src[i] is a 32-bit int (lw), dst[i] is a 16-bit short (lh).
# --------------------------------------------------------------------------
verify_dst_eq_src:
    la      t0, src
    la      t1, dst
    mv      t2, a0                  # i = start
1:  bge     t2, a1, 3f              # i >= end → all matched
    slli    t3, t2, 2               # src byte offset  (int:   4 bytes)
    add     t3, t0, t3
    lw      t3, 0(t3)               # src[i]
    slli    t4, t2, 1               # dst byte offset  (short: 2 bytes)
    add     t4, t1, t4
    lh      t4, 0(t4)               # dst[i]  (sign-extended to 32 bits)
    bne     t3, t4, 2f              # mismatch → fail
    addi    t2, t2, 1
    j       1b
2:  li      a0, 0
    ret
3:  li      a0, 1
    ret

# --------------------------------------------------------------------------
# verify_dst_zero – check dst[start .. end-1] are all zero
# Input:  a0 = start index (inclusive)
#         a1 = end   index (exclusive)
# Output: a0 = 1 if all zero, 0 otherwise
# --------------------------------------------------------------------------
verify_dst_zero:
    la      t0, dst
    mv      t1, a0                  # i = start
1:  bge     t1, a1, 3f
    slli    t2, t1, 1               # byte offset (short: 2 bytes)
    add     t2, t0, t2
    lh      t2, 0(t2)
    bnez    t2, 2f
    addi    t1, t1, 1
    j       1b
2:  li      a0, 0
    ret
3:  li      a0, 1
    ret

# --------------------------------------------------------------------------
# verify_src_unchanged – check src[0 .. NUM_ELEMENTS-1] == src_backup[]
# Output: a0 = 1 if all match, 0 otherwise
# --------------------------------------------------------------------------
verify_src_unchanged:
    la      t0, src
    la      t1, src_backup
    li      t2, 0                   # i = 0
1:  li      t3, NUM_ELEMENTS
    bge     t2, t3, 3f
    slli    t3, t2, 2
    add     t4, t0, t3
    lw      t4, 0(t4)              # src[i]
    add     t5, t1, t3
    lw      t5, 0(t5)              # src_backup[i]
    bne     t4, t5, 2f
    addi    t2, t2, 1
    j       1b
2:  li      a0, 0
    ret
3:  li      a0, 1
    ret


###############################################################################
#                           DATA SECTION
###############################################################################
.data

# Source array:  10 signed ints
src:
    .word   0, 1, 2, 3, 4, 5, 6, 7, 8, 9

# Backup copy of src (filled once at startup)
src_backup:
    .space  40                      # 10 ints × 4 bytes

# Destination array:  20 signed shorts (double the element count for
# overflow checking, matching the C declaration)
dst:
    .space  40                      # 20 shorts × 2 bytes

# Encrypted strings (decrypted at runtime by decrypt_message)
message:
    .asciz  "NLK{ILXPHz"
password:
    .asciz  "KZHHDLIW"

# ---- Constant strings used by the test harness ----
str_newlines:
    .asciz  "\n\n\n\n"
str_test_copyvec:
    .asciz  "Testing copyvec()\n"
str_copy_all:
    .asciz  "  Copy all elements: ... "
str_copy_some:
    .asciz  "  Copy some elements: ... "
str_copy_none:
    .asciz  "  Copy no elements: ... "
str_test_copyelems:
    .asciz  "Testing copyelements()\n"
str_copy_slice:
    .asciz  "  Copy random slice: ..."
str_passed:
    .asciz  "passed.\n"
str_failed:
    .asciz  "failed.\n"
str_wrong_ret:
    .asciz  "(wrong return value)"
str_all_passed:
    .asciz  "\nAll tests passed! The Secret Password is:\n"
str_some_failed:
    .asciz  "\nSome tests failed. Keep trying!\n"
str_newline:
    .asciz  "\n"
