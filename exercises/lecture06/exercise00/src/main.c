#include <stdio.h>
#include <stdint.h>
#include <string.h>

int port_data = 0x11223344;
int data_port_address = (int)&port_data;

void uppercase(char *a);
int comparecaseless(char *a, char *b);
char read_second_byte();


int main(void)
{
    ///////////////////////////////////////////////////////////////////////////////
    // Test assignment 1
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 1...\n");
    char a[] = "MOP makes me cry!";
    char b[] = "MOP MAKES ME CRY!";
    printf("Original: %s\n", a);
    uppercase(a);
    printf("Uppercased: %s\n", a);   
    
    if(!strcmp(a, b)) printf("PASSED!\n\n");
    else printf("FAILED!\n\n");

    ///////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////
    // Test assignment 2
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 2...\n");
    char a2[] = "MOP makes me cry!";
    char b2[] = "MOP makes me laugh!";
    char c2[] = "mop makes ME laugh!";
    printf("Comparing: '%s' and '%s'\n", a2, b2);
    if (!comparecaseless(a2, b2)) 
        printf("PASSED!\n\n");
    else
        printf("FAILED!\n\n");

    printf("Comparing: '%s' and '%s'\n", b2, c2);
    if (comparecaseless(b2, c2)) 
        printf("PASSED!\n\n");
    else 
        printf("FAILED!\n\n");

    ///////////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////
    // Test assignment 3
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 3...\n");
    char second_byte = read_second_byte();
    printf("Second byte of %x is %x\n", port_data, second_byte);
    if (second_byte == 0x33) {printf("PASSED!\n\n");}
    else {
        printf("FAILED!\n\n");
    }
    

    ///////////////////////////////////////////////////////////////////////////////
}

