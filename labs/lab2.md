# Laboration 2

In this lab assignment, you will get to run and debug a C program on real
hardware.

## Preparations

You *must* have done and submitted the preparatory assignment for this lab. If
you have not, you will probably not be able to finish this lab assignment on
time, so consult a TA and book a time slot in a later lab session.

## Lab environment

The lab environment is the same as in the previous lab (MD307 + VSCode), with
the addition of some peripheral devices:

* PTB-110, featuring a keypad, a DIL switch, a bargraph, and a 7-segment display
* PTB-111, featuring an ASCII display, and a graphical display

This lab assignment will primarily deal with the keypad and the ASCII display.
The DIL switch will be used for debugging purposes.

## Task 0: Getting Started

Create a new project folder and initialise a project with the *Lab2* template
(*Ctrl+Shift+P -> MDx07: Initialize project -> Lab Assignments -> Lab2*).

Connect the *keypad* to *Port D 15..8*. Connect the *PTB-111* to the *MD307* via
the PTB-1000 bus. Connect the *ASCII display* to *Port E* (*Data* to *15..8*,
*Control* to *7..0*).

Finally, connect the *MD307* to your computer via USB. In the end it should look
something like this:

<!-- TODO: picture of hardware setup -->

The expected behaviour of the program is for it to print characters from left to
right on the top row of the ASCII display when you press keys on the keypad.
When the end of the row is reached, it should start over from the beginning of
the row and overwrite existing characters. The grid below shows the characters
to be printed when pressing the corresponding keys on the keypad (e.g. top-left
key should print `1`, bottom-right key should print `D`).

```
|---|---|---|---|
| 1 | 2 | 3 | A |
|---|---|---|---|
| 4 | 5 | 6 | B |
|---|---|---|---|
| 7 | 8 | 9 | C |
|---|---|---|---|
| E | 0 | F | D |
|---|---|---|---|
```

Time to run the program. Go to the *Run and Debug* view (Ctrl+Shift+D), select
*Build and debug (hardware)* in the drop-down list, and press *Start Debugging*
(F5).

When the debugger has automatically stopped in the beginning of `main()`, press
*Continue* (F5) and test if the program behaves as expected.

*Rats! It doesn't work!*

Luckily, you *love* fixing broken software.

## Task 1: Fixing the Program

### Task 1.1: Checking the GPIO Ports

### Task 1.2: Checking the Delay Functions

### Task 1.3: Checking the Keypad

### Task 1.4: Checking the ASCII Display

### Task 2: Getting Approved
