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
//         40 bytes for `holder_name` (string)
//         4 bytes for `balance` (floating point)
//         8 bytes for `interest_rate` (double precision floating)
//         1 byte for `account_type` (a character)
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
// Use this addres as a base address for the `BankAccount` and fill account with
// this data:
//           account_number = 123456789
//           holder_name = Birta Eiriksdottir  (use strcpy() )
//           balance = 666.66
//           interest_rate = 2.56
//           account_type = 'd'
//
///////////////////////////////////////////////////////////////////////////////

void * assignment2(int address){
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
