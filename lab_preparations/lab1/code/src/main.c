///////////////////////////////////////////////////////////////////////////////
// This is the test harnes file
// DO NOT MODIFY. You don't even have to read or understand this stuff (yet)
// All instructions are in the assignments.s file
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

extern void copyvec(int src[], short dst[], int size);
int copyelements(int src[], short dst[], int start, int end);
void decrypt_message(); 
extern void hidden(char msg[]);

int src[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int src_backup[sizeof(src)/sizeof(src[0])];
const int num_elements = sizeof(src)/sizeof(src[0]); 
short dst[sizeof(src)/sizeof(src[0]) * 2];
char message[] = "{{{NLK{ILXPHz{{{";
char password[] = "KZHHDLIW";

void init_vectors()
{
    for(int i=0; i<num_elements * 2; i++) dst[i] = 0;
    for(int i=0; i<num_elements; i++) src[i] = src_backup[i];
}

int main(void)
{
    printf("\n\n\n\n");
    // Decrypt message
    decrypt_message();
    hidden(message); 

    // Backup source
    for(int i=0; i<num_elements; i++) src_backup[i] = src[i]; 

    int num_passed_tests = 0; 
    printf("Testing copyvec()\n");
    {
        init_vectors(); 
        printf("  Copy all elements: ... ");
        fflush(stdout); 
        copyvec(src, dst, num_elements);
        int passed = 1;
        for(int i=0; i<num_elements; i++) if(src[i] != dst[i]) passed = 0; 
        for(int i=num_elements; i<num_elements * 2; i++) if(dst[i] != 0) passed = 0; 
        for(int i=0; i<num_elements; i++) if(src[i] != src_backup[i]) passed = 0;  
        if(passed) { printf("passed.\n"); num_passed_tests++; }
        else printf("failed.\n");
    }

    {
        init_vectors(); 
        printf("  Copy some elements: ... ");
        fflush(stdout); 
        copyvec(src, dst, num_elements/2);
        int passed = 1;
        for(int i=0; i<num_elements/2; i++) if(src[i] != dst[i]) passed = 0; 
        for(int i=num_elements/2; i<num_elements * 2; i++) if(dst[i] != 0) passed = 0; 
        for(int i=0; i<num_elements; i++) if(src[i] != src_backup[i]) passed = 0;  
        if(passed) { printf("passed.\n"); num_passed_tests++; }
        else printf("failed.\n");
    }
    
    {
        init_vectors(); 
        printf("  Copy no elements: ... ");
        fflush(stdout); 
        copyvec(src, dst, 0);
        int passed = 1;
        for(int i=0; i<num_elements * 2; i++) if(dst[i] != 0) passed = 0; 
        for(int i=0; i<num_elements; i++) if(src[i] != src_backup[i]) passed = 0;  
        if(passed) { printf("passed.\n"); num_passed_tests++; }
        else printf("failed.\n");
    }

    printf("Testing copyelements()\n");
    {
        init_vectors();
        printf("  Copy random slice: ...");
        fflush(stdout); 
        int start = 3;
        int end = num_elements - 3;
        int passed = 1;
        int result = copyelements(src, dst, start, end);
        if(result != end - start + 1) { printf("(wrong return value)"); passed = 0; } 
        for(int i=0; i<start; i++) if(dst[i] != 0) passed = 0; 
        for(int i=start; i<=end; i++) if(dst[i] != src[i]) passed = 0; 
        for(int i=end+1; i<num_elements; i++) if(dst[i] != 0) passed = 0; 
        for(int i=0; i<num_elements; i++) if(src[i] != src_backup[i]) passed = 0;  
        if(passed) { printf("passed.\n"); num_passed_tests++; }
        else printf("failed.\n");
    } 
    {
        init_vectors();
        printf("  Copy random slice: ...");
        fflush(stdout); 
        int start = 4;
        int end = num_elements - 2;
        int passed = 1;
        copyelements(src, dst, start, end);
        for(int i=0; i<start; i++) if(dst[i] != 0) passed = 0; 
        for(int i=start; i<=end; i++) if(dst[i] != src[i]) passed = 0; 
        for(int i=end+1; i<num_elements; i++) if(dst[i] != 0) passed = 0; 
        for(int i=0; i<num_elements; i++) if(src[i] != src_backup[i]) passed = 0;  
        if(passed) { printf("passed.\n"); num_passed_tests++; }
        else printf("failed.\n");
    }     

    if(num_passed_tests == 5) {
        printf("\nAll tests passed! The Secret Password is:\n");
        printf("%s\n", password);
    } else {
        printf("\nSome tests failed. Keep trying!\n");
    }

}

// We could hide these in an .o file to stop students from wasting their
// time in trying to crack my encryption instead of learning about assembly...

void decrypt_message()
{
    for(unsigned int i=0; i < sizeof(message) - 1; i++) {
        message[i] = 'A' - (message[i] - 'Z');
    }     
    for(unsigned int i=0; i < sizeof(password) - 1; i++) {
        password[i] = 'A' - (password[i] - 'Z');
    }     
}