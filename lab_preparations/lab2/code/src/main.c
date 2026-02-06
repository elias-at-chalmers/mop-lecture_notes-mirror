
#include <stdio.h>
#include <stdint.h>


#define TIMER6_CTLR1  ((volatile uint32_t *)0x40001000)
#define TIMER6_CTLR2  ((volatile uint32_t *)0x40001004)
#define TIMER6_DMAAINTENR ((volatile uint32_t *)0x4000100C)
#define TIMER6_CNT  ((volatile uint16_t *)0x40001024)
#define TIMER6_PSC  ((volatile uint16_t *)0x40001028)
#define TIMER6_ATRLR  ((volatile uint32_t *)0x4000102C)

#define GPIOE_CFGLR ((volatile uint32_t *)0x40011800)
#define GPIOE_CFGHR ((volatile uint32_t *)0x40011804)
#define GPIOE_INDR ((volatile uint32_t *)0x40011808)
#define GPIOE_OUTDR ((volatile uint32_t *)0x4001180C)

#define EN 4 // Bit 2
#define RW 2 // Bit 1
#define RS 1 // Bit 0

extern void init_gpio_port_e();
extern void ascii_write_controller(uint8_t command_or_data);
extern void ascii_write_command(uint8_t command);
extern void ascii_initialize_display();
extern uint8_t ascii_read_status();
void ascii_write_data(uint8_t data);
void delay_us(uint32_t us);
void delay_ms(uint32_t us);

int assignment_1_checked = 0; 
void check_assignment_1() {
    if(assignment_1_checked) return; // Only check once
    assignment_1_checked = 1;
    printf("Checking assignment 1...");
    if (*GPIOE_CFGLR != 0x22222222 || *GPIOE_CFGHR != 0x22222222) {
        printf("\nAssignment 1 is not correct CFGLR=0x%08X, CFGHR=0x%08X.\n", *GPIOE_CFGLR, *GPIOE_CFGHR);
    } else {
        printf("PASSED!\n");
    }
}

int assignment_2_1_checked = 0;
void check_assignment_2_1(uint8_t command_or_data) {
    if(assignment_2_1_checked) return; // Only check once
    assignment_2_1_checked = 1;
    printf("Checking assignment 2.1...");
    if((*GPIOE_OUTDR & EN) == 0) {
        printf("\nAssignment 2.1 is not correct, Enable not set.\n", *GPIOE_OUTDR);
    } 
    else if((*GPIOE_OUTDR >> 8) != command_or_data) {
        printf("\nAssignment 2.1 is not correct, Output data is 0x%02X, expected 0x%02X.\n", *GPIOE_OUTDR, command_or_data);
    }
    else {
        printf("PASSED!\n");
    }
}

int assignment_2_2_checked = 0;
void check_assignment_2_2(uint8_t command_or_data) {
    if(assignment_2_2_checked) return; // Only check once
    assignment_2_2_checked = 1; 
    printf("Checking assignment 2.2...");
    if((*GPIOE_OUTDR & EN) != 0) {
        printf("\nAssignment 2.2 is not correct, Enable is still set.\n");
    } 
    else {
        printf("PASSED!\n");
    }
}

int assignment_3_checked = 0;
void check_assignment_3() {
    if(assignment_3_checked) return; // Only check once
    assignment_3_checked = 1;
    printf("Checking assignment 3...");
    if((*GPIOE_OUTDR & (RS | RW | EN)) != 0) {
        printf("\nAssignment 3 is not correct, RS, RW, or E is set.\n");
    } 
    else {
        printf("PASSED!\n");
    }
}

int assignment_5_1_checked = 0;
void check_assignment_5_1() {
    if(assignment_5_1_checked) return; // Only check once
    assignment_5_1_checked = 1;
    printf("Checking assignment 5.1...");
    if (*GPIOE_CFGHR != 0x44444444) {
        printf("\nAssignment 5.1 is not correct CFGHR=0x%08X.\n", *GPIOE_CFGHR);
    } else {
        printf("PASSED!\n");
    }
}

int assignment_5_2_checked = 0;
void check_assignment_5_2() {
    if(assignment_5_2_checked) return; // Only check once
    assignment_5_2_checked = 1;
    printf("Checking assignment 5.2...");
    if((*GPIOE_OUTDR & EN) != 0 || (*GPIOE_OUTDR & RW) == 0) {
        printf("\nAssignment 5.2 is not correct, RS or RW not set correctly.\n");
    } 
    else {
        printf("PASSED!\n");
    }
}

int assignment_5_3_checked = 0;
void check_assignment_5_3() {
    if(assignment_5_3_checked) return; // Only check once
    assignment_5_3_checked = 1;
    printf("Checking assignment 5.3...");
    if((*GPIOE_OUTDR & EN) == 0) {
        printf("\nAssignment 5.3 is not correct, Enable not set.\n");
    } 
    else {
        printf("PASSED!\n");
    }
}

int assignment_5_4_checked = 0;
void check_assignment_5_4() {
    if(assignment_5_4_checked) return; // Only check once
    assignment_5_4_checked = 1;
    printf("Checking assignment 5.4...");
    if((*GPIOE_OUTDR & EN) != 0) {
        printf("\nAssignment 5.4 is not correct, Enable is still set.\n");
    } 
    else {
        printf("PASSED!\n");
    }
}

int assignment_5_5_checked = 0;
void check_assignment_5_5() {
    if(assignment_5_5_checked) return; // Only check once
    assignment_5_5_checked = 1;
    printf("Checking assignment 5.5...");
    if (*GPIOE_CFGHR != 0x22222222) {
        printf("\nAssignment 5.5 is not correct CFGHR=0x%08X.\n", *GPIOE_CFGHR);
    } else {
        printf("PASSED!\n");
    }
}

void await_status()
{
    while(ascii_read_status() != 0) {
        int a = 0; 
    }
    //delay_ms(8);
}

int main(void)
{

    *GPIOE_CFGLR = 0x22222222; // Push Pull Output
    *GPIOE_CFGHR = 0x22222222; // Push Pull Output

    ascii_write_command(0x1); // Clear display
    await_status();
    ascii_write_command(0b1111); // Display on
    await_status();
    ascii_write_command(0b1000); // Display off
    await_status();
    ascii_write_command(0b1111); // Display on
    await_status();

    const char * msg = "Hello           "; 
    int ctr = 0; 
    while(1) {
        ascii_write_command(0x1); // Clear display
        await_status();
        ascii_write_command(0x2); // Return home
        await_status();
        for(int i=0; i<16; i++) {
            ascii_write_data(msg[(ctr + i) % 16]);
            await_status();

        }
        ctr += 1; 

        delay_ms(500);
    }
    printf("Hello");



    init_gpio_port_e(); 
    check_assignment_1();
    ascii_write_controller(0xAB);
    ascii_write_command(0x38);
    check_assignment_3(); 
    ascii_initialize_display();
    ascii_read_status(); 

    printf("Polling status until display is ready...");
    while(ascii_read_status() != 0) {
        // Wait until display is ready
        printf(".");
    }
    printf("\nDisplay is ready!\n");

    ascii_write_command(0b1000); // Display off
    while(ascii_read_status() != 0) {}
    ascii_write_command(0b1111); // Display on
    while(ascii_read_status() != 0) {}
    ascii_write_command(0b1111); // Display on
    while(ascii_read_status() != 0) {}


    ascii_write_command(0b00111000); // Function set: 8-bit, 2 lines, 5x8 dots
    while(ascii_read_status() != 0) {}
    ascii_write_command(0b00001110); // Display on, cursor off, blink
    while(ascii_read_status() != 0) {}
    ascii_write_command(0b00000110); // Cursor right, no display shift
    while(ascii_read_status() != 0) {}
    ascii_write_command(0b00000001); // Clear display
    while(ascii_read_status() != 0) {}


    while(1) {
        ascii_write_data('A'); // Write character 'A' to display
        while(ascii_read_status() != 0) {}
    }
    return 0;
}

