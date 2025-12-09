# Exceptions and Internal Interrupts 
In this lecture, we will start looking into *exceptions*, *faults*, *traps*, and *interrupts*. These are all names for situations where the currently running program is *interrupted* by some event, either from outside or inside the processor. 

A simple real-world analogy would be the ring signal on your phone. You probably do not look at your phone every few seconds to see if someone is calling you. Instead, you go about your life, and sometimes the phone rings and you drop everything for a short while to handle that phone call, and then go on with your life as if nothing had happened.

In the same way, a microcontroller in your fridge might run along happily regulating the temperature. When the fridge door opens, it has to turn on the lights. If the door stays open for too long it has to start an annoying beep that reminds you to close the door. 

We *could* try to check if the door is open, every few instructions, in our code, but that would be very difficult and lead to horribly complicated code. Instead, all modern microprocessors have an exception mechanism that allows the processor to: 

1. Store the current state of the machine (the register values) to memory.
2. Jump to an *exception handler* - another piece of code that handles the event that just occurred. 
3. Restore the machine state and resume your program as if nothing had happened. 

## Different types of exceptions
Exceptions are categorized in a number of different ways, and you will see that essentially the same functionality has different names depending on where and how it happens: 

* **RESET** - A very special type of exception occurs when someone presses the *reset* button (or the processor was told to reset by the debug module). The running program is then interrupted, and the processor starts a RESET sequence (the program is never resumed).
* **FAULT** - The running code might do something that is not allowed. For example, it might do an unaligned memory access (as we have discussed before), it might try to write to a protected part of the memory area, or it might simply try to divide a value by zero. All of these errors are likely to cause the machine to crash if not handled appropriately. Instead, the program can be immediately interrupted and some system code can run that handles the error in an appropriate way and then might allow the program to resume. 
* **INTERRUPT** - "The developer of the program might *request* that the program be interrupted "if a specific thing happens. In this course we will divide interrupts into two different types: 
  - **External Interrupts** - This is when an interrupt originates from *outside* the microcontroller. The fridge door, for example, might be connected to one of the GPIO pins, and when the door opens the programmer wants the change in the GPIO pin's value to immediately interrupt the running program. 
  - **Internal Interrupts** - This is when the signal to interrupt originates from within the microcontroller. When the fridge door opens, the programmer might start a SysTick timer, and configure it to interrupt the running code after 30 seconds. 
* **TRAP** - Sometimes, the code itself might ask to be interrupted. This might sound strange, but in reality there are many cases where this is useful - for example when invoking system services or entering a debugger. We will return to this later.

<div class="boxed">
<small>
**Enrichment:** The following is interesting and will help your understanding, but is not essential for the course. 
</small>

# Machine Privilege modes
A RISC-V processor can run in different *privilege modes*. At lower privilege levels, some instructions are not allowed. It is easy to see why this is necessary on a desktop computer: if any program could execute any instruction and freely access the memory of other programs, it could cause serious **security problems** (reading or modifying private data) or **stability problems** (crashing the entire system). For this reason, the operating system runs with full system access, while user applications run with very limited privileges.

We will not explore this topic in depth in this course, but even on the small microcontrollers we use, privilege modes can be very useful. Consider a smart fridge as an example. Some parts of the system are **safety-critical**, such as temperature regulation, and are developed very carefully to run reliably for many years without change. Other parts, such as the touchscreen user interface, are updated frequently as features and visual designs evolve. If a bug is introduced while adding a new font to the clock app, it is crucial that this mistake cannot affect the safety-critical code and spoil the food.

For this reason, the less critical code can run in a lower privilege mode, where it does not have the ability to interfere with or crash the core control software that keeps the fridge operating safely.

Even code that runs in a lower privilege mode (such as the user-interface tasks) will still need to interact with hardware. For example, the UI must update the display, read button presses, or query the current temperature. However, direct access to most peripherals and critical registers is restricted to privileged code.  

To handle this safely, user-mode programs do not access hardware directly. Instead, they **request services from privileged code**, which performs the sensitive operations on their behalf. This is done by temporarily transitioning from user mode to machine mode using a *trap* (often called a *system call*).

When the UI wants to update the screen or read a sensor, it triggers a special instruction that raises a trap. The processor immediately switches into machine mode and begins executing a trusted service routine. This routine checks the request, performs the necessary hardware access in a controlled way, and then returns the result to the user-mode program before switching back to user mode.

> Quiz: If all interrupt handlers run in privileged mode, why couldn't a malicious programmer simply write an interrupt handler that does the malicious stuff?
</div>

# How to write a program with interrupts
We will explore why and how we program with interrupts using a simple example. Let's say we are employed by a Fridge company, and they want a prototype program from us. When the fridge door has been left open for too long they want a microcontroller to play a loud and annoying note, while at the same time displaying the current temperature on a 7-segment display. 

Luckily, our PTB-110 IO board has both a little speaker (called "buzzer" in the image below), and a 7 segment display, so all we have to do is plug in our MD307 and write the code. 

![](../images/sound_and_graphics.png)

Let's start with playing a note. A sound signal sent to a speaker is usually an analog signal with varying voltage, and all we know (yet) is how to output 0 or 3.3V from our GPIO ports. For us to perceive a signal as a note, however, all we need is a signal with a frequency in the 20Hz-20kHz range. So in fact, just flipping a GPIO pin between 0 and 1 (0 and 3.3V) every ~2 ms and connecting that signal to a speaker will produce a 261Hz signal that sounds like a middle C. 

We have already practiced sending signals to GPIO pins *and* precise timing (with SysTick), so we can immediately write the code for this. 
```C
int main(void)
{
    *GPIOD_CFGLR = 0x00000002;  // Configure pin 0 as Output, Push-Pull, 2MHz
    *SYSTICK_CMPH = 0x0;        // Set systick compare value to 2ms 
    *SYSTICK_CMPL = 147000 * 2; //
    *SYSTICK_CTLR = 0b101101;   // Start systick timer, count down, enable restart
    while(1) 
    {
        while((*SYSTICK_SR & 0x1) == 0) {}  // Wait until systick has counted to zero (2ms)
        *SYSTICK_SR = 0;                    // Reset status flag
        *GPIOD_ODATA ^= 0x1;                // Flip pin 0
    }
}
```
If we connect GPIO pin `D0` to the buzzer, we will hear a tone playing. 

For the second task -- showing the temperature on the 7-segment display -- we will pretend that we are given two helper functions in a library: 

* `int GetTemperature()` - Blocks until a new temperature value is available (max 100 ms), then returns the temperature (always between 0 and 9 degrees)
* `uint8_t Get7SegCode(int x)` - Returns the bitmask (the lit segments) corresponding to the number `x`.

If we connect GPIO port E(7:0) to our 7-segment display, we could now run the following program: 

```C
int main(void)
{
    *GPIOE_CFGLR = 0x22222222;  // Configure all pins as Output, Push-Pull, 20MHz
    while(1) 
    {
        int t = GetTemperature(); 
        *GPIOE_ODATA = Get7SegCode(t);
    }
}
```

But how can we combine these two programs? Since the `GetTemperature` function can take anywhere between 0 and 100ms it seems impossible to keep flipping pin `D0` every 2ms. The answer (as you have probably guessed) is to let SysTick interrupt our code every time it has counted down to zero, flip `D0` in the interrupt handler, and return to the the main program. But before we can write that code, we need to know a bit more about interrupt handlers. 

## What happens when an interrupt occurs? 

New model of the processor state machine

Explain that it jumps to the entry in the vector table

## How to write an interrupt handler for systick

# If this is too short, take some pointer stuff from lecture 06




