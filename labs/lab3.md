# Laboration 3

In this lab assignment, you will use a library to create a synthesizer, which
includes handling internal and external interrupts!

## Preparations

You *must* have done and submitted the preparatory assignment for this lab. If
you have not, you will probably not be able to finish this lab assignment on
time, so consult a TA and book a time slot in a later lab session.

## Lab environment

The lab environment is the same as in lab 1 (MD307 + VSCode), with the addition
of a peripheral device you have used before:

* PTB-110, featuring a bargraph, a keypad, and a buzzer

You will use the keypad to make the buzzer produce different tones, and the
bargraph as an indicator of which key is being pressed.

## Task 0: Getting Started

Create a new project folder and initialize a project with the **Lab3** template
(*Ctrl+Shift+P -> MDx07: Initialize project -> Lab Assignments -> Lab3*).

Connect the keypad to **Port D [0:7]**. Connect the bargraph to **Port D
[8:15]**. Connect the buzzer to **Port E [0]**. Finally, connect the MD307 to
your computer via USB.

To clarify the buzzer connection: Use a jumper cable directly from the MD307 to
the buzzer. The ports and pin numbers are shown in the diagram below, where `+`
means **VDD** and `g` means **GND**. The pin in square brackets `[.]` is where
one end of the jumper cable should go.

```
     PE15..8          PE7..0         PD15..8          PD7..0

    0 2 4 6 +       0 2 4 6 +       0 2 4 6 +       0 2 4 6 +
  |-----------|   |-----------|   |-----------|   |-----------|
  | . . . . . |   |[.]. . . . |   | . . . . . |   | . . . . . |
  | . . . . . |   | . . . . . |   | . . . . . |   | . . . . . |
  |----   ----|   |----   ----|   |----   ----|   |----   ----|
    g 1 3 5 7       g 1 3 5 7       g 1 3 5 7       g 1 3 5 7

      UNUSED          BUZZER        BARGRAPH          KEYPAD
```

The template includes a library with various definitions and functions that you
will use to implement a synthesizer. All the initialization needed for this lab
assignment is already done (correctly this time - *no bug fixing!*).

The only thing missing is the logic inside the interrupt handlers
`systick_handler()` and `exti_handler()` in `src/main.c`.

Let's start by making some noise.

## Task 1: Sound Check

The SysTick interrupt handler `systick_handler()` in `src/main.c` is where we
need to put the logic for creating a square wave, by writing alternating
high/low (`1`/`0`) signals to the output pin connected to the buzzer (**GPIOE
[0]**).

Within `systick_handler()`, write some code that inverts the value of the buzzer
output pin. This only requires *one* line of code if you choose the right
bitwise operation! Use the GPIO interface in `inc/gpio.h` - you may use pointer
macros or struct pointers, whichever you prefer.

Also, the interrupt needs to be acknowledged to let the system know that the
handler can be called again. Add some code that acknowledges the SysTick
interrupt at the end of the handler (again, this only requires *one* line of
code).

The library includes functions for starting and stopping periodic SysTick
interrupts. Test your SysTick interrupt handler by adding a line of code in
`main()`, after the initialization calls (*not* within the infinite loop), that
starts SysTick interrupts with a period of 500 µs (see the SysTick interface in
`inc/systick.h`).

If it works, you should hear a 1 KHz tone. If not, call a TA to get some
guidance.

Before moving on, remove the 1 KHz tone from the program! Next, we will use the
keypad to control the tone generation.

## Task 2: External Control

The external interrupts have been configured such that any key on the keypad
will invoke the interrupt handler `exti_handler()` in `src/main.c`. The
interrupts are triggered on falling *and* rising edges, i.e. when a key is
pressed *and* when a key is released.

### Task 2.1: Reading the Keypad

Within `exti_handler()`, start small by adding some code that simply reads the
keypad (see the keypad interface in `inc/keypad.h`). Use the debugger to make
sure you can read all the keys properly and know what values they return. Ensure
the interrupt handler is called when a key is pressed as well as released.

Time to turn the keypad into a synthesizer!

### Task 2.2: Making Noise

Now, everything is in place. You can control the tone (`inc/systick.h`) and read
the keypad (`inc/keypad.h`). Use this to generate a tone when a key is pressed
according to the keypad diagram below, where the numbers represent SysTick
periods in microseconds.

```
|------|------|------|------|
| 1516 | 1431 | 1351 | 1275 |
|------|------|------|------|
| 1203 | 1136 | 1072 | 1012 |
|------|------|------|------|
|  955 |  901 |  851 |  803 |
|------|------|------|------|
|  758 |  715 |  675 |  637 |
|------|------|------|------|
```

A tone should only be generated while a key is pressed. Make the tone stop when
a key is released.

Now you have a working synthesizer - take the time to pat yourself on the back
and play some notes for fun before moving on!

It would be nice with some visual feedback as well though, don't you think?

## Task 3: Lights, Please!

Recall that the bargraph is connected to **GPIOD [8:15]**. Make the bargraph
indicate which key is being pressed. Each key should show a unique light
pattern. Feel free to come up with something yourself, but if you're not in a
creative mood, you could simply use the numbers 1 to 16 from top-left to
bottom-right button (interpreting the lights as a natural binary number).

Like the tone, the lights should only be on while while a key is pressed. Turn
off the lights when a key is released.

## Task 4: Melody Player

Playing a melody on the keypad is fun, but it's difficult to get the timing
right and (with the current implementation) we only have access to a very
limited set of notes.

Let's make the program play a melody automatically.

The library includes definitions for playing a particular melody. Have a look at
the interface in `inc/music.h`, particularly the `Note` struct. Add the code
below to `src/main.c` in global scope above `systick_handler()` and use it to
play the melody.

```c
Note notes[] = NOTES;
const int notes_length = sizeof(notes) / sizeof(Note);
int remaining_duration;
int current_period;
```

*Tip: You need to add code to* `systick_handler()` *and* `main()`.

Do you recognize the melody?

## Task 5: Getting Approved

Demonstrate your solution to a TA. Show them that the program works, and explain
how you implemented it. Answer any questions they might have.

***When you have been approved by a TA, it is YOUR responsibility to verify that
it has been documented in Canvas under 'Grades'. Do so before leaving!***
