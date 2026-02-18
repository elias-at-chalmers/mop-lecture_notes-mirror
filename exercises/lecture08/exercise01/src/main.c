#include <stdio.h>
#include <stdint.h>

void assignment1(); 
void whackamole(); 

#define FAILED(n)                       \
    do {                                \
        printf("\nFAILED: %s", n);      \
        assignment_passed = 0;          \
    } while(0);                

void check_assignment1()
{
    printf("Checking assignment 1.1...");
    int assignment_passed = 1; 
    if(*((volatile uint32_t *)0xE000F000) != 0b10100) FAILED("STK_CTLR is not properly written.");
    if(*((volatile uint32_t *)0xE000F004) != 0) FAILED("STK_SR is not properly written.");
    if(*((volatile uint32_t *)0xE000F008) != 1) FAILED("STK_CNTL is not properly written.");
    if(*((volatile uint32_t *)0xE000F00C) != 2) FAILED("STK_CNTH is not properly written.");
    if(*((volatile uint32_t *)0xE000F010) != 3) FAILED("STK_CMPL is not properly written.");
    if(*((volatile uint32_t *)0xE000F014) != 4) FAILED("STK_CMPH is not properly written.");
    if(assignment_passed) printf("PASSED!\n");
}

int main(void)
{
    assignment1(); 
    check_assignment1(); 
    whackamole(); 
}

