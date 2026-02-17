///////////////////////////////////////////////////////////////////////////////
//
// Lecture 07 - Exercise 02
// In this exercise you will train to work with Structs, Unions and Ports in C
//
//  Once again, connect Console to Serial Interface 1, do the assignments
//  and verify that all the tests pass.
//
///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <string.h>

#define SOLUTION 1

///////////////////////////////////////////////////////////////////////////////
// Assignment 1 - define a structure `BankAccount` holding (in this specific
// order): 4 bytes for `account_number`
//         40 bytes for `holder_name`      (char holder_name[40])
//         4 bytes for `balance`           (floating point)
//         8 bytes for `interest_rate`     (double precision floating)
//         1 byte for `account_type`       (a character)
//
// After you do that, return a size of the structure in function assignment1.
// Use `sizeof()` operator!
//
///////////////////////////////////////////////////////////////////////////////

#ifdef SOLUTION
struct BankAccount {
    int account_number;      
    char holder_name[40];   
    float balance;         
    double interest_rate;  
    char account_type; 
};
#else
    // TODO: implement this structure
    struct BankAccount;
#endif


uint32_t assignment1(){
#ifdef SOLUTION
    return sizeof(struct BankAccount);
#else
    return 0;
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 2 - function assignment2 receives an address in memory.
// Convert this address into a pointer to `BankAccount` structure
// and fill account with this data:
//           account_number = 123456789
//           holder_name = Birta Eiriksdottir  (use strcpy() )
//           balance = 666.66
//           interest_rate = 2.56
//           account_type = 'd'
//
// Return the pointer to your structure.
//
///////////////////////////////////////////////////////////////////////////////

struct BankAccount * assignment2(int address){
#ifdef SOLUTION
    struct BankAccount *account = (struct BankAccount *)address;
    account->account_number = 123456789;
    strcpy(account->holder_name, "Birta Eiriksdottir");
    account->balance = 666.66;
    account->interest_rate = 2.56;
    account->account_type = 'd';

    return account;
#else
    return NULL;
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 3 - at address 0x20005000 a port with two 16-bit registers
// is located. Register CREG at offset 0x0 has two fields f0 (4 bits starting 
// at bit 0) and f1 (3 bits starting at bit 6). The rest of the bits are reserved
// and should never be modified. Register XREG has offset of 0x4 and it has
// two fields XLOW and XHIGH of 8 bits each. See register map:
// _______________________________________________________________Register | Offset
// | 15 |14 |13 |12 |11 |10 |09 |08 |07 |06 |05 |04 |03 |02 |01 |00 | 
// |----------------------------------------------------------------|
// |  x | x | x | x | x | x | x |    f1     | x | x |      f0       | CREG | 0x0
// |----------------------------------------------------------------|
// |              XHIGH              |              XLOW            | XREG | 0x4
// ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
// Note: bits 16-31 are not shown on the map. They are reserved and you should
// add padding bits to your structure.
//
// Your task is to write a struct definition named "Port" for this port, using bitfields.
// Then, in function assignment3, create a pointer to the port and 
// write 0b101 into f1, and 0x55 into XHIGH. Other fields, or reserved bytes 
// should not be modified.
//
///////////////////////////////////////////////////////////////////////////////

#ifdef SOLUTION
struct Port{
    struct {
        unsigned int f0: 4;
        unsigned int _res1: 2;
        unsigned int f1: 3;
        unsigned int _res2: 7;
        unsigned int _res3: 16;
    };

    union{
        uint16_t XREG;
        struct{
            uint8_t XLOW;
            uint8_t XHIGH;
        };
    };
    
    unsigned short _res4;
};
#endif

void assignment3(){
#ifdef SOLUTION
    struct Port *a = (struct Port *) 0x20005000;
    a->f1 = 0b101;
    a->XHIGH = 0x55;
#else
    // Your code here
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 4 - update the definition of Port in Assignment 3 ...so that
// the XREG register can be accessed both as a a complete 16-bit register,
// and as two separate 8-bit fields named XLOW and XHIGH.
// You need to use a Union for this!
//
// In function `assignment4` use the same port at address 0x20005000 and
// return the value of the whole XREG as uint16_t (unsigned short).  
//
///////////////////////////////////////////////////////////////////////////////

uint16_t assignment4(){
    struct Port *a = (struct Port *) 0x20005000;
    // Uncomment the next line:
    return a->XREG;
}