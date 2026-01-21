#include <stdio.h>
#include <stdint.h>

int get_keyboard_button(void);

int main(void)
{
    printf("Hello\n");

    while(1) {
        int button = get_keyboard_button();
        if(button >= 0) {
            printf("Button pressed: %d\n", button);
        }
    }
    return 0;
}

