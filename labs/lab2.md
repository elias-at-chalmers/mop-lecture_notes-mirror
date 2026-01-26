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

The program is riddled with bugs. Given your familiarity with similar code from
the preparatory assignment, it *might* be possible to simply read the code from
beginning to end and fix all the bugs without ever running it, but don't do
that. The point is for you to learn how to debug a program in a *structured*
way. As you may have already noticed, debugging (fixing bugs) is usually a much
more time-consuming part of software development than actually writing the code.
This problem should *not* be approached by trying to learn how to write perfect
code from the outset - that is unattainable. Instead, one should learn how to
find errors efficiently - once the error is pinpointed, the solution is usually
obvious.

**Warning**: *When debugging code (especially code written by someone other than
yourself), correcting inconsequential things (like the way the code is
formatted) can be tempting, but* ***refrain*** *from doing so! When debugging,
it is useful to check* ***exactly*** *what changes have been made to the source
code (the TAs may want to do so if you run into problems), and having a bunch of
irrelevant changes makes it more difficult.*

Let's look at the program from a top-down perspective starting from `main()`,
just one level deep into the calling hierarchy:

<!-- TODO: call graph from main, one level down -->

We start off by initialising the GPIO ports. If the pins are configured
incorrectly, testing the program is going to be difficult. So let's start there.

### Task 1.1: Checking the GPIO Ports

**NB**: *You may assume the* `.h` *header files, which include the port macros,
are correct. You do not need to check those.*

Starting with the keypad (connected to *Port D 15..8*), recall that the upper
four pins (*15..12*) should be configured as *digital output (open drain, 2
MHz)*, and the lower four pins (*11..8*) as *digital input (pull up).*

Consult the QuickGuide and carefully derive what values should be written to
what registers for this configuration to work.

Look at the code in `init_keypad()` and see if you can spot the errors. If so,
modify the code and make a note of your corrections. If not, consult a TA.

After making a change, it is usually a good idea to devise a test and verify
correctness. We might (incorrectly) assume it would be a good idea to connect
the port to the *bargraph* and add some code to test the output pins, and then
connect the port to the *DIL switch* and add some code to test the input pins.
Unfortunately, that wouldn't work because the *bargraph* requires push/pull (not
open drain) and the *DIL switch* requires pull down (not pull up).

So for this task, you need to be *really* confident in your ocular inspection.
You may ask a TA to check it for you, if you wish.

To save time, you needn't bother checking `init_ascii()` - it is correctly
implemented.

*What's that? The program still doesn't work? How unfortunate!*

The delay functions are used in most parts of the program, so let's check those
next.

### Task 1.2: Checking the Delay Functions

### Task 1.3: Checking the Keypad

### Task 1.4: Checking the ASCII Display

### Task 2: Getting Approved
