
#include <stdio.h>
#include <stdint.h>
#include "assignment.h"

extern void init_interrupts();
extern void init_keypad_test();
extern void init_square_wave();
extern void init_vector_table();
extern void start_square_wave(unsigned int period_in_us);
extern void init_gpio();
extern void systick_periodic_micro(unsigned int period_in_us);
extern void systick_stop();
extern void systick_handler(void);
extern void init_interrupts();

__attribute__((interrupt("machine"))) void exti_handler();

static uint32_t _fnv1a_32(uint32_t value) {
    uint32_t hash = 0x811c9dc5U;
    for (int i = 0; i < 4; i++) {
        hash ^= (value >> (8 * i)) & 0xFF;
        hash *= 0x01000193U;
    }
    return hash;
}

///////////////////////////////////////////////////////////////////////////////
// Check function for Assignment 1.0, there's no assignment here, don't modify!
// Tests that each register macro resolves to the correct address.
///////////////////////////////////////////////////////////////////////////////
int check_assignment_1_0() {
    int pass = 1;

    struct { const char *name; volatile uint32_t *actual; uint32_t expected_hash; } checks[] = {
        { "EXTI_INTENR",  EXTI_INTENR,  0x69F91240U },
        { "EXTI_RTENR",   EXTI_RTENR,   0x6B5A4148U },
        { "EXTI_FTENR",   EXTI_FTENR,   0x21DEDF5CU },
        { "EXTI_SWIEVR",  EXTI_SWIEVR,  0x6CBD0350U },
        { "EXTI_INTFR",   EXTI_INTFR,   0xA340D7E4U },
        { "PFIC_IENR1",   PFIC_IENR1,   0xB68D1F4AU },
        { "AFIO_EXTICR1", AFIO_EXTICR1, 0x66F50E74U },
    };

    for (int i = 0; i < 7; i++) {
        if (_fnv1a_32((uint32_t)checks[i].actual) == checks[i].expected_hash) {
            printf("    %s... pass\n", checks[i].name);
        } else {
            printf("    %s... fail (incorrect address: 0x%08lX)\n",
                   checks[i].name, (uint32_t)checks[i].actual);
            pass = 0;
        }
    }
    printf(pass ? "Assignment 1.0: PASS\n" : "Assignment 1.0: FAIL\n");
    return pass;
}

///////////////////////////////////////////////////////////////////////////////
// Check function for Assignment 1.1, there's no assignment here, don't modify!
// Calls init_gpio() and verifies the resulting register values.
///////////////////////////////////////////////////////////////////////////////
int check_assignment_1_1() {
    init_gpio();

    struct {
        const char *name;
        uint32_t    actual;
        uint32_t    expected_hash;
    } checks[] = {
        { "GPIOD_CFGLR (mode/config)",        *GPIOD_CFGLR & 0xFFFFFFFFU, 0xFBB29DD5U },
        { "GPIOD_CFGHR (mode/config)",        *GPIOD_CFGHR & 0xFFFFFFFFU, 0x80C16545U },
        { "GPIOE_CFGLR pin0 (mode/config)",   *GPIOE_CFGLR & 0x0000000FU, 0xEBEE7337U },
        { "GPIOD_OUTDR bits[3:0] (pull-ups)", *GPIOD_OUTDR & 0x0000000FU, 0xDC73306AU },
        { "GPIOD_OUTDR bits[7:4] (rows)",     *GPIOD_OUTDR & 0x000000F0U, 0x4B95F515U },
        { "GPIOD_OUTDR bits[15:8] (LEDs)",    *GPIOD_OUTDR & 0x0000FF00U, 0x4B95F515U },
    };

    int pass = 1;
    for (int i = 0; i < 6; i++) {
        if (_fnv1a_32(checks[i].actual) == checks[i].expected_hash) {
            printf("    %s... pass\n", checks[i].name);
        } else {
            printf("    %s... fail (got 0x%08lX)\n", checks[i].name, checks[i].actual);
            pass = 0;
        }
    }
    printf(pass ? "Assignment 1.1: PASS\n" : "Assignment 1.1: FAIL\n");
    return pass;
}

///////////////////////////////////////////////////////////////////////////////
// Check function for Assignment 1.2, there's no assignment here, don't modify!
// Calls systick_periodic_micro(1000) and verifies the resulting register values.
///////////////////////////////////////////////////////////////////////////////
int check_assignment_1_2() {
    systick_periodic_micro(1000);

    int pass = 1;

    uint32_t stk_cmp_val = (uint32_t)*STK_CMP & 0xFFFFFFFFU;
    if (_fnv1a_32(stk_cmp_val) == 0x73286BA6U) {
        printf("    STK_CMP (compare value)... pass\n");
    } else if (_fnv1a_32(stk_cmp_val) == 0x5A0F45DDU) {
        printf("    STK_CMP (compare value)... pass (warning: off by one - did you forget to subtract 1?)\n");
    } else {
        printf("    STK_CMP (compare value)... fail (got 0x%08lX)\n", stk_cmp_val);
        pass = 0;
    }

    uint32_t stk_ctlr_val = *STK_CTLR & 0xFU;
    if (_fnv1a_32(stk_ctlr_val) == 0xDC73306AU) {
        printf("    STK_CTLR bits[3:0] ... pass\n");
    } else {
        printf("    STK_CTLR bits[3:0] ... fail (got 0x%08lX)\n", stk_ctlr_val);
        pass = 0;
    }

    systick_stop();
    printf(pass ? "Assignment 1.2: PASS\n" : "Assignment 1.2: FAIL\n");
    return pass;
}

///////////////////////////////////////////////////////////////////////////////
// Check function for Assignment 1.3, there's no assignment here, don't modify!
// Calls systick_handler() directly to verify it toggles GPIOE pin 0 and
// clears STK_SR.
///////////////////////////////////////////////////////////////////////////////
int check_assignment_1_3() {
    int pass = 1;
    uint32_t saved_mepc;

    *GPIOE_OUTDR &= ~0x1U;
    *STK_SR = 1;
    __asm__ volatile ("csrr %0, mepc" : "=r"(saved_mepc));
    __asm__ volatile ("csrw mepc, %0" : : "r"(&&after_call_1));
    systick_handler();
    after_call_1:
    __asm__ volatile ("csrw mepc, %0" : : "r"(saved_mepc));

    uint32_t outdr_after_1 = *GPIOE_OUTDR & 0x1U;
    if (_fnv1a_32(outdr_after_1) == 0xFB69B604U) {
        printf("    GPIOE_OUTDR bit0 toggle (0->1)... pass\n");
    } else {
        printf("    GPIOE_OUTDR bit0 toggle (0->1)... fail (got 0x%08lX)\n", outdr_after_1);
        pass = 0;
    }

    uint32_t stk_sr_after = *STK_SR & 0x1U;
    if (_fnv1a_32(stk_sr_after) == 0x4B95F515U) {
        printf("    STK_SR cleared after handler... pass\n");
    } else {
        printf("    STK_SR cleared after handler... fail (got 0x%08lX)\n", stk_sr_after);
        pass = 0;
    }

    *STK_SR = 1;
    __asm__ volatile ("csrr %0, mepc" : "=r"(saved_mepc));
    __asm__ volatile ("csrw mepc, %0" : : "r"(&&after_call_2));
    systick_handler();
    after_call_2:
    __asm__ volatile ("csrw mepc, %0" : : "r"(saved_mepc));

    uint32_t outdr_after_2 = *GPIOE_OUTDR & 0x1U;
    if (_fnv1a_32(outdr_after_2) == 0x4B95F515U) {
        printf("    GPIOE_OUTDR bit0 toggle (1->0)... pass\n");
    } else {
        printf("    GPIOE_OUTDR bit0 toggle (1->0)... fail (got 0x%08lX)\n", outdr_after_2);
        pass = 0;
    }

    printf(pass ? "Assignment 1.3: PASS\n" : "Assignment 1.3: FAIL\n");
    return pass;
}

///////////////////////////////////////////////////////////////////////////////
// Check function for Assignment 1.4, there's no assignment here, don't modify!
// Reads the mtvec CSR to find the vector table, then decodes the instruction
// at the SysTick slot (IRQ 12) to verify it jumps to systick_handler.
///////////////////////////////////////////////////////////////////////////////
int check_assignment_1_4() {
    int pass = 1;

    uint32_t mtvec;
    __asm__("csrr %0, mtvec" : "=r"(mtvec));
    uint32_t *stk_entry = (uint32_t *)((mtvec & ~0x3U) + 12 * 4);
    uint32_t instr = *stk_entry;

    uint8_t opcode = instr & 0x7FU;
    uint8_t rd     = (instr >> 7) & 0x1FU;

    if (opcode != 0x6FU || rd != 0) {
        printf("    SysTick vector entry (IRQ 12)... fail (not a J instruction)\n");
        pass = 0;
    } else {
        int32_t imm = 0;
        imm |= ((int32_t)(instr >> 31) & 0x1)  << 20;
        imm |= ((instr >> 12) & 0xFFU)          << 12;
        imm |= ((instr >> 20) & 0x1U)           << 11;
        imm |= ((instr >> 21) & 0x3FFU)         <<  1;
        if (imm & (1 << 20)) imm |= ~((1 << 21) - 1);

        uint32_t target = (uint32_t)stk_entry + imm;
        if (target == (uint32_t)systick_handler) {
            printf("    SysTick vector entry (IRQ 12) -> systick_handler... pass\n");
        } else {
            printf("    SysTick vector entry (IRQ 12)... fail (jumps to wrong address)\n");
            pass = 0;
        }
    }

    printf(pass ? "Assignment 1.4: PASS\n" : "Assignment 1.4: FAIL\n");
    return pass;
}

///////////////////////////////////////////////////////////////////////////////
// Check function for Assignment 1.5, there's no assignment here, don't modify!
// Starts the systick timer, then calls systick_stop() and verifies that
// STK_CTLR is fully cleared.
///////////////////////////////////////////////////////////////////////////////
int check_assignment_1_5() {
    int pass = 1;

    systick_periodic_micro(1000);
    systick_stop();

    uint32_t ctlr = *STK_CTLR;
    if (_fnv1a_32(ctlr) == 0x4B95F515U) {
        printf("    STK_CTLR (all disabled)... pass\n");
    } else {
        printf("    STK_CTLR (all disabled)... fail (got 0x%08lX)\n", ctlr);
        pass = 0;
    }
    printf(pass ? "Assignment 1.5: PASS\n" : "Assignment 1.5: FAIL\n");
    return pass;
}

///////////////////////////////////////////////////////////////////////////////
// Check function for Assignment 2.0, there's no assignment here, don't modify!
// Calls init_keypad() and verifies the resulting register values.
///////////////////////////////////////////////////////////////////////////////
int check_assignment_2_0() {
    init_interrupts();
    int pass = 1;

    struct {
        const char *name;
        uint32_t    actual;
        uint32_t    expected_hash;
    } checks[] = {
        { "PFIC_ISR1 bits[25:22] (EXTI0-3 enabled)",  *PFIC_ISR1   & 0x03C00000U, 0xC6BA219CU },
        { "AFIO_EXTICR1 bits[15:0] (Port D mapping)", *AFIO_EXTICR1 & 0x0000FFFFU, 0x3068F21BU },
        { "EXTI_INTENR bits[3:0]",                    *EXTI_INTENR  & 0x0000000FU, 0xDC73306AU },
        { "EXTI_RTENR bits[3:0]",                     *EXTI_RTENR   & 0x0000000FU, 0xDC73306AU },
        { "EXTI_FTENR bits[3:0]",                     *EXTI_FTENR   & 0x0000000FU, 0xDC73306AU },
    };

    for (int i = 0; i < 5; i++) {
        if (_fnv1a_32(checks[i].actual) == checks[i].expected_hash) {
            printf("    %s... pass\n", checks[i].name);
        } else {
            printf("    %s... fail (got 0x%08lX)\n", checks[i].name, checks[i].actual);
            pass = 0;
        }
    }
    printf(pass ? "Assignment 2.0: PASS\n" : "Assignment 2.0: FAIL\n");
    return pass;
}

///////////////////////////////////////////////////////////////////////////////
// Check function for Assignment 2.1, there's no assignment here, don't modify!
// Calls exti_handler() directly and verifies observable side-effects:
//   - EXTI_INTENR bits[3:0] are re-enabled after the handler returns
//   - GPIOD_OUTDR bits[7:4] (rows) are driven LOW
///////////////////////////////////////////////////////////////////////////////
int check_assignment_2_1() {
    int pass = 1;

    *GPIOD_OUTDR |= 0x00F0U;
    *EXTI_INTENR |= 0xFU;

    uint32_t saved_mepc;
    __asm__ volatile ("csrr %0, mepc" : "=r"(saved_mepc));

    void *after_exti = &&after_exti_call;
    __asm__ volatile ("csrw mepc, %0" : : "r"(after_exti));
    exti_handler();
    after_exti_call:
    __asm__ volatile ("csrw mepc, %0" : : "r"(saved_mepc));

    uint32_t intenr = *EXTI_INTENR & 0xFU;
    if (_fnv1a_32(intenr) == 0xDC73306AU) {
        printf("    EXTI_INTENR bits[3:0] re-enabled... pass\n");
    } else {
        printf("    EXTI_INTENR bits[3:0] re-enabled... fail (got 0x%08lX)\n", intenr);
        pass = 0;
    }

    uint32_t rows = (*GPIOD_OUTDR >> 4) & 0xFU;
    if (_fnv1a_32(rows) == 0x4B95F515U) {
        printf("    GPIOD_OUTDR bits[7:4] (rows) driven LOW... pass\n");
    } else {
        printf("    GPIOD_OUTDR bits[7:4] (rows) driven LOW... fail (got 0x%08lX)\n", rows);
        pass = 0;
    }
    printf(pass ? "Assignment 2.1: PASS\n" : "Assignment 2.1: FAIL\n");
    return pass;
}

///////////////////////////////////////////////////////////////////////////////
// Check function for Assignment 2.2, there's no assignment here, don't modify!
// Reads mtvec to locate the vector table, then decodes the JAL instruction at
// each of the four EXTI slots (IRQ 22-25) and verifies they all jump to
// exti_handler.
///////////////////////////////////////////////////////////////////////////////
int check_assignment_2_2() {
    int pass = 1;

    uint32_t mtvec;
    __asm__("csrr %0, mtvec" : "=r"(mtvec));

    static const char * const names[] = {
        "EXTI0 (IRQ 22)", "EXTI1 (IRQ 23)",
        "EXTI2 (IRQ 24)", "EXTI3 (IRQ 25)"
    };

    for (int i = 0; i < 4; i++) {
        int irq = 22 + i;
        uint32_t *entry = (uint32_t *)((mtvec & ~0x3U) + irq * 4);
        uint32_t instr  = *entry;

        uint8_t opcode = instr & 0x7FU;
        uint8_t rd     = (instr >> 7) & 0x1FU;

        if (opcode != 0x6FU || rd != 0) {
            printf("    %s vector entry... fail (not a J instruction)\n", names[i]);
            pass = 0;
            continue;
        }

        int32_t imm = 0;
        imm |= ((int32_t)(instr >> 31) & 0x1) << 20;
        imm |= ((instr >> 12) & 0xFFU)         << 12;
        imm |= ((instr >> 20) & 0x1U)          << 11;
        imm |= ((instr >> 21) & 0x3FFU)        <<  1;
        if (imm & (1 << 20)) imm |= ~((1 << 21) - 1);

        uint32_t target = (uint32_t)entry + imm;
        if (target == (uint32_t)exti_handler) {
            printf("    %s -> exti_handler... pass\n", names[i]);
        } else {
            printf("    %s... fail (jumps to wrong address)\n", names[i]);
            pass = 0;
        }
    }
    printf(pass ? "Assignment 2.2: PASS\n" : "Assignment 2.2: FAIL\n");
    return pass;
}


__attribute__((interrupt("machine")))
void Mode0_Handler(void)
{
    a: goto a;
}


__attribute__((interrupt("machine")))
void HardFault_Handler(void)
{
    a: goto a;
}



int main(void)
{
    check_assignment_1_0();

    init_vector_table();
    init_gpio();
    init_interrupts();

    check_assignment_1_1();
    check_assignment_1_2();
    check_assignment_1_3();
    check_assignment_1_4();
    check_assignment_1_5();
    check_assignment_2_0();
    check_assignment_2_1();
    check_assignment_2_2();

    systick_periodic_micro(500); 
    while(1);
}

