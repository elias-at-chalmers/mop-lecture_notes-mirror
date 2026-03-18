# Laboration 1 (Terminal GDB)

In this lab, you will finally get to run some code on, and connect some real hardware to, the real MD307 microcontroller. By the time you take this lab you should know quite a lot about assembly programming on a RISC-V system, you should know the basics of how to put signals onto the GPIO pins, and you should be familiar with creating, compiling, and running an assembly program in the simulator environment. 

## Preparations
You MUST have done the preparation task for this lab, and have submitted your answers to Canvas. If you have not, this will take too much time so talk to a TA and see if there is room on a later Lab session. 

## Lab environment
**This whole lab should be done by connecting the MD307 to the stationary computer in the lab. You cannot use your own laptops. When connecting your MD307, make sure to connect the USB cable to the little usb hub behind the screen, *not* the screen itself.**

In all program development, but *especially* when you are programming close to the metal (machine oriented), it is essential to be able to run your program through a *debugger*. 

When you are ready to ship your program, you will compile it to an `.elf` file and use a tool to send it to your microcontrollers flash memory (so that it starts any time you turn the device on), but while developing the program, uploading, starting, and stopping is handled by the debugger. 

In this version of the lab, we do all debugging from the *terminal* using GDB (GNU DeBugger) directly — no IDE required.

### How the debug setup works

GDB is a command-line tool and a protocol. You start GDB and point it at your compiled `.elf` file:

```
$ riscv32-unknown-elf-gdb excercise02.elf
GNU gdb ('riscv32-embecosm-gcc-win64-20230813') 14.0.50.20230813-git
Copyright (C) 2023 Free Software Foundation, Inc.
For help, type "help".
Reading symbols from excercise02.elf...
(gdb)
```

The GDB client connects via TCP to a GDB *server*. When using the simulator, `SimServer` is the GDB server. When (like today) you are connecting real hardware, you start a program called `OpenOCD` instead. `OpenOCD` is a GDB server that speaks the GDB protocol on one side and forwards commands over USB to your MD307 on the other.

![](../images/debug_setup.png)

At the other end of the USB cable is your MD307. It has a little chip (the GDB module in the image) that talks directly to the USB port. When it receives a command on the USB port, it translates it into a serial protocol that talks directly with the microcontroller's debug module. The details of this protocol are *way* out of scope for this course (and, unfortunately, proprietary) but it is important to understand this basic picture of how your program gets loaded into memory, started and stopped.

### Assemble your program
1. Download this assembler program: [assignment1.asm](assignment1.asm). Assemble it using Eterm8 and make sure a `.elf` file is produced.

### Starting the debug session

1. **Start OpenOCD** using the shortcut on the desktop. You should see output ending with something like:
   ```
   Info : Listening on port 3333 for gdb connections
   ```

2. **Start GDB** using the shortcut on the desktop. A terminal window will appear. Point GDB at your `.elf` file using the `file` command:
   ```
   (gdb) file assignment1.elf
   ```

3. **Connect to OpenOCD** from within GDB:
   ```
   (gdb) target extended-remote localhost:3333
   ```
   Note: Make sure the port number matches the port number listed in step 1.

4. **Upload your program** to the MD307:
   ```
   (gdb) load
   ```

5. **Start execution**:
   ```
   (gdb) continue
   ```

### Essential GDB commands

| Command | Short | Description |
|---------|-------|-------------|
| `continue` | `c` | Resume execution until next breakpoint or crash |
| `stepi` | `si` | Execute one assembly instruction (step *into* calls) |
| `nexti` | `ni` | Execute one assembly instruction (step *over* calls) |
| `break location` | `b` | Set a breakpoint, e.g. `break assignment.s:20` |
| `info breakpoints` | `i b` | List all breakpoints |
| `delete N` | `d N` | Delete breakpoint number N |
| `info registers` | `i r` | Show all general-purpose register values |
| `print $reg` | `p` | Print a single register, e.g. `print $a0` |
| `x/Nx addr` | | Examine N words of memory at addr, e.g. `x/8x 0x20000000` |
| `list` | `l` | Show source code around current location |
| `backtrace` | `bt` | Show the call stack |
| `load` | | Re-upload the `.elf` file (after recompiling) |
| `monitor reset halt` | | Reset the microcontroller and halt at the start |
| `quit` | `q` | Exit GDB |

**Tip:** After a code change, recompile, then in GDB type `load` followed by `monitor reset halt` and `continue` — no need to restart GDB or OpenOCD.


## Lab Exercise 1 - Debug somebody else's code


> **Important:** You should spend a maximum of one hour on this exercise. If you get stuck at some point for more than 15 minutes, grab a TA --- they will guide you to the next step. *It is important that you proceed to the second exercise within an hour, whether you found all bugs or not*.

Time to try it. The program you have just uploaded to the MD307 is an attempt at solving the same task as you solved for the preparation, but unfortunately whoever wrote this was not as bright as you and it is riddled with bugs!

In fact, if you try to run the code, it will just crash. So, start the program and then slowly step through it using `stepi` (or `si`). **Where did it crash?** (GDB will print something like `Program received signal SIGTRAP` or simply stop responding to `stepi`.)

Now restart the program (`monitor reset halt`) and step until you are at the instruction where it crashed last time (but don't execute it, or it crashes again). Inspect the register values with `info registers`. **Why does this instruction crash the machine?** Look at the surrounding code (`list`) and you should be able to fix the first bug. Write down what it was, so you can show the TA.

If you run the program again it will still crash, but this time you should make it through the `copyelements` function. Set a breakpoint right after the call to that function:
```
(gdb) break assignment.s:LINE_NUMBER
```
(replace `LINE_NUMBER` with the line right after the function call) and run the program until you hit the breakpoint (`continue`). 

Now keep stepping with `si` until you see where it crashes. **Where does it crash?** Restart the program and run to this line again. Look at the registers with `info registers`. Do the contents of the registers make sense? Where did they stop making sense (step through the program)? **Fix the bug.**

Now set breakpoints at the two test-related lines that contains the instruction `j worked` and `j error`
```
(gdb) break assignment.s:FIRST_LINE
(gdb) break assignment.s:SECOND_LINE
```

Run the program again (`continue`) and when it breaks on the first instruction, continue (`c`) — the program will stop at the second breakpoint, so there are still problems!

Figure out why the program does not pass the test, then find a TA and show them your findings.

## Lab Exercise 2 - Let there be light!
Finally time to play with LED lights. In the box next to your MD307 you should have found a spider-like contraption looking something like this (or maybe just a single colored LED light with two legs): 
![](../images/rgb_led_light.png)

Your next task is to make this light turn on. You can test it immediately by connecting the ground leg (the one with the resistor) to GND, and (one of) the other legs to 3.3V: 

![](../images/LED.png)

But obviously, the goal is to control it from your program, so connect the anode to one of the GPIO pins instead (PD6 could be a good one). Download template code: [assignment2.asm](assignment2.asm). There is not much code there, so it's up to you now. 

> ⚠️ **Important:** You will probably not have time to complete all of these assignments. It is much more important that you understand what you are doing than that you complete all the tasks.

### Task 1: Turn it on. 
You have seen this in the lectures and you might have done something very similar in exercises already. Configure your chosen pin as a Push-Pull, Output, pin and write a `1` to the corresponding bit in the `ODATA` register. 
Step through your code with `si` and make sure the light goes on when you expect it to. 

### Task 2: Make it blink
Write a little `delay` function that is just a for loop that keeps the machine busy for a while. Then write a loop where you: 
```
    Turn the light on
    delay
    Turn the light off
    delay
    repeat
```

### Task 3: Making it pulsate...
Now we want the light to go from off, to stronger and stronger. But how can we do that when all we can do is turn it on or off? We will use a trick called *Pulse Width Modulation* (PWM). The idea is that we divide time into a very short interval, just a few thousand clock-cycles (4096 will work pretty well). We will call this time interval a *period* (P). Then we say that the light will be *on* for a part of this interval (0-100%, we call this the *duty cycle*) and off for the remainder of the interval. 

In other words, we make the lamp blink very very quickly. If our period is short enough, we will not perceive this as blinking; we will just see the light glowing with different strength. With a low duty cycle (say, 10% or 400 cycles), it will be very dim, and with a high duty cycle it will shine brightly.

![](../images/duty_cycle.png)

Start by making the light glow dimly, with a low duty cycle, and then try to write a loop where you increase the duty cycle gradually until you hit max. In pseudo code: 

```
duty cycle = 0
loop: 
    increase duty_cycle
    if duty_cycle > 4096, set to 0
    for i = 0 to 4096: 
        if i < duty_cycle: 
           light should be on
        else 
           light should be off
    jump to loop
```

### Task 4: RGB Lights!
If you have made it this far, great work! If you have time to complete this task as well, **wow!** Go find the TA and ask for an RGB LED, if you don't have one already. Then connect it and make it pulsate with all the colors of the rainbow. Then record it and show your lecturer for a gold star sticker[^1].

[^1]: The gold star has no monetary value and does not affect your grade. It might also be a virtual gold star sticker if I cannot find a real one. 

### Task 5: Getting Approved

After thoroughly testing and verifying that your program works as expected,
demonstrate your solution to a TA. Show them that the program works. Explain the
bugs you found and how you fixed them. Answer any questions they may have.

***If you are approved by the TA, it is YOUR responsibility to verify that it
has been documented in Canvas under 'Grades'. Do so before leaving!***
