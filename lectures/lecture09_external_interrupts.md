# TBD



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
