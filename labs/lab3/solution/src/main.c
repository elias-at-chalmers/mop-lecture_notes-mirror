#include "afio.h"
#include "exti.h"
#include "gpio.h"
#include "interrupts.h"
#include "keypad.h"
#include "music.h"
#include "pfic.h"
#include "systick.h"
#include "vector_table.h"
#include <stdint.h>
#include <stdio.h>

/* SUMMARY OF TASKS
 *
 * Task 1: Sound Check
 * - in systick_handler(), invert the buzzer pin and acknowledge interrupt
 * - in main(), add systick_periodic_micro(500) to test the buzzer
 * - remove test code before moving on
 *
 * Task 2: External Control
 * - Task 2.1: Reading the Keypad
 * --- in exti_handler(), read keypad and acknowledge interrupt
 * --- use debugger to verify being able to read key press AND release
 * - Task 2.2: Making Noise
 * --- add array with period values, in scope from exti_handler()
 * --- in exti_handler(), start/stop sound on key press/release
 *
 * Task 3: Lights, Please!
 * - in exti_handler() where sound is started, also turn on lights
 * - in exti_handler() where sound is stopped, also turn off lights
 *
 * Task 4: Melody Player
 * - add given global variables to main.c, above systick_handler()
 * - in main(), iterate through list of notes, starting each note and
 *   wait while remaining_duration > 0
 *   (WARNING: do NOT use while(remaining_duration != 0) to wait!)
 * - in systick_handler(), reduce remaining_duration by current_period
 */

// Globals for Task 4
Note notes[] = NOTES;
const int notes_length = sizeof(notes) / sizeof(Note);
int remaining_duration;
int current_period;

// Handle internal SysTick interrupt
__attribute__((interrupt("machine")))
void systick_handler() {
  // Downcount remaining note duration
  remaining_duration -= current_period;
  // Invert buzzer pin
  GPIOE->ODR ^= 1;
  // Acknowledge interrupt
  STK->SR = 0;
}

// Handle external keypad interrupt (on press *and* on release)
__attribute__((interrupt("machine")))
void exti_handler() {
  const int periods[16] = {1516, 1431, 1351, 1275,
                           1203, 1136, 1072, 1012,
                            955,  901,  851,  803,
                            758,  715,  675,  637};
  // Read keypad
  uint8_t key = keypad();
  // On key press:
  if (key != 0xFF) {
    // Start sound
    systick_periodic_micro(periods[key]);
    // Turn on bargraph
    GPIOD->BSR = (key + 1) << 8;
  // On key release:
  } else {
    // Stop sound
    systick_stop();
    // Turn off bargraph
    GPIOD->BCR = 0xFF << 8;
  }
  // Acknowledge interrupt on lines {0,1,2,3}
  EXTI->INTFR |= 0xF << 0;
}

// Entry point
int main(void) {
  // Initialize
  init_gpio();
  init_vector_table();
  init_interrupts();
  #ifdef TESTS
  // Test buzzer with 1 KHz tone (remove after testing)
  systick_periodic_micro(500);
  #endif
  /*
  // Play melody once
  for(int i = 0; i < notes_length; i++) {
    remaining_duration = notes[i].duration_micro;
    current_period = notes[i].period_micro;
    systick_periodic_micro(current_period);
    while(remaining_duration > 0) {
    }
  }*/
  systick_stop();
  // Wait for interrupts
  while (1) {
    // Could put concurrently running code here!
  }
}
