#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

extern uint32_t assignment1();
extern void * assignment2(int address);

struct BankAccountTest {
    int accountNumber;      
    char holderName[40];   
    float balance;         
    double interestRate;  
    char accountType; 
};
struct BankAccountTest account_test = {123456789, "Birta Eiriksdottir", 666.66, 2.56, 'd'}; 

int main(void)
{
    ///////////////////////////////////////////////////////////////////////////////
    // Test assignment 1
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 1...\n");
    uint32_t student_size = assignment1();
    if (student_size == sizeof(struct BankAccountTest)){
        printf("PASSED!\n\n");
    }
    else {
        printf("Received an unexpected structure size");
        printf("FAILED!\n\n");
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Test assignment 2
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 2...\n");
    void *student_struct = malloc(100);
    void *student_return = assignment1((int)student_struct);
    if (memcmp(student_return, &account_test, sizeof(account_test)) == 0){
        printf("PASSED!\n\n");
    }
    else if (student_return != student_struct){
        printf("Wrong address is returned...");
        printf("FAILED!\n\n");
    } 
    else{
        printf("Wrong memory content is returned...");
        printf("FAILED!\n\n");
    }
    ///////////////////////////////////////////////////////////////////////////////


}

