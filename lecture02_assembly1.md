The RISC-V Instruction Set Architecture (ISA)
===============================================================================

Links:
[Unpriveleged ISA](https://drive.google.com/file/d/1uviu1nH-tScFfgrovvFCrj7Omv8tFtkp/view)

> There is no formal, centralized specification of RISC-V pseudo-instructions in the same way that base ISA instructions are specified by RISC-V International. Instead, pseudo-instructions are implementation-specific, typically defined directly in the source code of assemblers like GAS (GNU Assembler). The developers of GAS work from:
> -- ChatGPT

RISC-V is an *Instruction Set Architecture* (ISA). That is, it specifies a processor's behaviour in terms of the *instruction set*, the *registers*, and the *memory model*, and how all of these components interact. Anyone is allowed to use this specification to *implement* their own processor, without paying anyone. For example, in this course we will use the MD307 lab kit, which contains a CH32V307 microcontroller (a chip developed by the company WCH), which in turn contains (as part of the chip) a QingKe V4 *processor core*, which is an implementation of the RISC-V ISA.

Other companies have implemented their own processor cores, in their own microcontrollers, that are also based on the RISC-V ISA. Raspberry Pi, for instance, has implemented a RISC-V core called Hazard3 in their RP2350 microcontroller, which is the heart of the Raspberry Pico 2. Since both machines use the same ISA, they can use the same compiler toolchains to produce executable code, and can in some cases even run the *same* binary code, even though the actual hardware is completely different.

## Variants and Extensions
RISC-V is intended to be used in a huge range of devices, from small microcontrollers in your car key or fridge, to CPUs in the nodes of a machine-learning data center. Therefore, the specification is divided into a *base integer instruction set*, which only describes around 50 separate instructions, and a large number of *extensions* that describe instructions for additional functionality.

Let's consider a few examples: 

- **A TV remote control** will need a little microcontroller to recognize when you press a button and send the appropriate infrared signal to your TV. This microcontroller does not do any advanced processing of any kind, and does not have to be very fast. The basic 32-bit integer instruction set (called RV32I) will be sufficient.
- **A calculator** can be a similarly simple device, but will be quite useless if it cannot handle very large numbers. You would probably choose the 64-bit version of the basic instruction set for this (called RV64I). It would also be silly to create a calculator that can only handle integers, so you would want a microcontroller with the **F** (*single-precision floating point*) or **D** (*double-precision floating point*) extensions. If a processor implements these extensions, it means it can handle an additional set of instructions that deal with floating-point numbers.
- **A simple digital watch** needs to be extremely power efficient (so that you don't have to charge the battery several times every day). Among other things, that means reducing the amount of memory and the number of memory transactions required. Such a device might require the **C** (*compressed instructions*) extension. This extension adds a number of 16-bit wide machine instructions, and will be discussed a bit more later in the course. 
<!-- LATER: Discuss this in depth when covering how instructions are executed -->
- **A modern desktop PC**, on the other hand, will require LOTS of memory and should be able to run many processes at once, and as quickly as possible. Power efficiency is not as important. A processor for a desktop computer will need to use the (RV64I) basic instruction set (so it can access more than 4GB of memory), it will need to use several additional extensions that allow for *virtual memory*, *atomic operations*, *vector processing*, etc. These extensions are all out of scope for this course. 

The microcontroller used in this course (CH32F307) is a 32-bit processor (RV32I) with the M (integer multiplication and divide), A (atomic operations), F (floating point), C (compressed instructions), Zicsr (Control and Status Register instructions), and Zifencei (instruction-fetch fence) extensions. In the remainder of the course material, we will only focus on the base instruction set (RV32I), with the M extension. We will compile our programs in such a way that only these instructions are used (except in special cases), to keep the complexity down. You can find information about *all* the extensions and variants on the official [RISC-V homepage](https://riscv.org/specifications/ratified/).

