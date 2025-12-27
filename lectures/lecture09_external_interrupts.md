# External Interrupts
So far we have seen how to interrupt the program when a single, *internal*, interrupt occurs (SysTick counted to zero). In this lecture we will dive deeper into interrupt handling, and show how we can handle interrupts from modules outside the Qingke processor and finally from interrupts originating from the GPIO pins. 

## Configuring multiple interrupts
Recall from the previous lecture that every module that can cause an interrupt on the chip will have a hard-coded connection to the *Programmable Fast Interrupt Controller* (PFIC) module. When, for instance, the "Ethernet Wakeup" signal comes in from the ethernet cable, the ethernet module will signal the PFIC, and the PFIC will (if it has been configured to do so) interrupt the running code and make the processor run an interrupt handler. 

<center>
<img src= "../images/interrupts.png" width=100%>
</center>

In the previous lecture, we simply put the address of the interrupt handler into the `mtvec` CSR register, and whenever any interrupt occured, the code at that address would run. When we have many possible interrupts (the machine might be simultaneously listening to ethernet, USB, two timers and some GPIO pins) it becomes inefficient and quite cumbersome to have a single interrupt handler. 

To play a tone on a little buzzer, we used SysTick to interrupt the processor at a frequency that corresponded to the tone we wanted to play. Imagine a situation where we wanted to play *two* different notes on two speakers. We can set up SysTick to interrupt the processor at a specific frequency, but how do we handle the second tone? Luckily, most microcontrollers come with a number of extra timers, so we could set up one of those 



It would be possible to handle this with a single interrupt handler, but:
* The interrupt handler would have to go through some code to find out which 
* It is much easier to have a separate handler for the second tone. 


### Generating a table of interrupt vectors in memory
The easiest way to set up a vector table for our program is to add a new assembly file to our project (we will call it `vector_table.s`): 

```
.section .text
.global vector_table        # Make this variable available to the C code
.extern SysTick_Handler     # Make the interrupt handler available to our code

.align 2                    # The vector table must begin on an address divisible by 4
vector_table: 
.zero 12 * 4                # Reserve space for 12 interrupt vectors before systick
j SysTick_Handler           # At `vector_table + 0x30` we place an instruction that 
                            # jumps to our interrupt handler
```

When we compile this code and load it into memory on the machine, we don't know exactly where `vector_table` will begin (depends on the compiler), but we know that it will contain a table with 12 empty entries, and then a single jump instruction at offset `0x30`. So the next step is to tell the processor that `vector_table` is the base address it wants to use when looking for the systick interrupt handler. So, we have to put the address to `vector_table` into our `mtvec` CSR. This is easily done by adding a little function to our assembly file: 

```
.global init_interrupts     # Make this function available to our C code

init_interrupts: 
    la t0, vector_table     # Put the address of `vector_table` in t0
    csrw mtvec, t0          # Move that value into the `mtvec` CSR
    ret
```
