# The RISC-V Instruction Set Architecture (ISA)

<!-- markdownlint-disable MD012 -->
<!-- markdownlint-disable MD022 -->


Links:
[Unpriveleged ISA](https://drive.google.com/file/d/1uviu1nH-tScFfgrovvFCrj7Omv8tFtkp/view)

In this course, you will learn to program a microcontroller based on a RISC-V processor. Today, almost every phone, computer, or smart device runs on a processor from one of a handful of companies (e.g. ARM, Intel). RISC-V changes that: it’s an open alternative that anyone can use, extend, and build on, and it is becoming increasingly important in both industry and research. In this course, you’ll learn to program real hardware built on RISC-V. So what does it mean to be a "RISC-V" processor?

RISC-V is an *Instruction Set Architecture* (ISA). That is, it specifies a processor's behaviour in terms of the *instruction set*, the *registers*, and the *memory model*, and how all of these components interact. Anyone is allowed to use this specification to *implement* their own processor, without paying anyone. For example, in this course we will use the MD307 lab kit, which contains a CH32V307 microcontroller (a chip developed by the company WCH), which in turn contains (as part of the chip) a QingKe V4 *processor core*, which is an implementation of the RISC-V ISA.

Other companies have implemented their own processor cores, in their own microcontrollers, that are also based on the RISC-V ISA. Raspberry Pi, for instance, has implemented a RISC-V core called Hazard3 in their RP2350 microcontroller, which is the heart of the Raspberry Pico 2. Since both machines use the same ISA, they can use the same compiler toolchains to produce executable code, and can in some cases even run the *same* binary code, even though the actual hardware is completely different.

## Variants and Extensions
RISC-V is intended to be used in a huge range of devices, from small microcontrollers in your car key or fridge, to CPUs in the nodes of a machine-learning data center. Therefore, the specification is divided into what is called the *base integer instruction set*, which only describes 47 separate instructions, and a large number of *extensions* that describe instructions for additional functionality. Anyone that decides to build a RISC-V processor will have to decide which of these extensions they want to support. Implementing more extensions naturally means more work, and probably means that the chip will be more expensive and draw more power. Similarly, anyone who *buys* a processor for use in their product will have to choose which extensions it should support.

Let's consider a few examples:

- **A TV remote control** will need a little microcontroller to recognize when you press a button and send the appropriate infrared signal to your TV. This microcontroller does not do any advanced processing of any kind, and does not have to be very fast. The basic 32-bit integer instruction set (called RV32I) will be sufficient.
- **A calculator** can be a similarly simple device, but will be quite useless if it cannot handle very large numbers. You would probably choose the 64-bit version of the basic instruction set for this (called RV64I). It would also be silly to create a calculator that can only handle integers, so you would want a microcontroller with the **F** (*single-precision floating point*) or **D** (*double-precision floating point*) extensions. If a processor implements these extensions, it means it can handle an additional set of instructions that deal with floating-point numbers.
- **A simple digital watch** needs to be extremely power efficient (so that you don't have to charge the battery several times every day). Among other things, that means reducing the amount of memory and the number of memory transactions required. Such a device might require the **C** (*compressed instructions*) extension. This extension adds a number of 16-bit wide machine instructions, and will be discussed a bit more later in the course.
<!-- LATER: Discuss this in depth when covering how instructions are executed -->
- **A modern desktop PC**, on the other hand, will require LOTS of memory and should be able to run many processes at once, and as quickly as possible. Power efficiency is not as important. A processor for a desktop computer will need to use the (RV64I) basic instruction set (so it can access more than 4GB of memory), it will need to use several additional extensions that allow for *virtual memory*, *atomic operations*, *vector processing*, etc. These extensions are all out of scope for this course.

The microcontroller used in this course (CH32F307) is a 32-bit processor (RV32I) with the M (integer multiplication and divide), A (atomic operations), F (floating point), C (compressed instructions), Zicsr (Control and Status Register instructions), and Zifencei (instruction-fetch fence) extensions. In the remainder of the course material, we will only focus on the base instruction set (RV32I), with the M and Zicsr extensions. We will compile our programs in such a way that only these instructions are used (except in special cases), to keep the complexity down. You can find information about *all* the extensions and variants on the official [RISC-V homepage](https://riscv.org/specifications/ratified/).

## The Application Binary Interface (ABI)
Before we go any further in describing how a RISC-V processor works, it's important to understand the **ABI**. While the ISA defines the rules that a hardware design must follow to qualify as a RISC-V processor, the ABI (Application Binary Interface) defines how compiled programs interact at the binary level. In other words, it provides rules for how machine code should be written so that independently compiled programs and libraries can work together.

For example, the ISA specifies that the processor must have 32 general-purpose 32-bit registers, but it doesn’t define what each register is used for. The ABI, on the other hand, specifies that when a function returns an integer, it should place the result in register x10 (also known as a0). If your assembly program - or your C compiler - follows that rule, it can seamlessly interoperate with other code that follows the same ABI.

In this course, all compilers and assembly code will adhere to the ABI. This not only ensures compatibility between different pieces of code, but also makes assembly programs much easier to understand and maintain.

## The Basic 32-bit Integer Instruction Set Architecture (RV32I)

Initially, we will only talk about the very basics of the RISC-V specification, RV32I. This part of the specification must be implemented by *any* 32-bit RISC-V processor. The specification tells us what *registers* shall be available, what the basic set of *machine instructions* are, and how they are encoded.

### Registers
A processor that implements RV32I must implement 33 registers. There are 32 *general purpose* registers, ``x0-x31``. These registers can contain any 32 bit word and any instruction can use either of these registers interchangeably. The first register, ``x0``, is special however; it is hardwired to always be zero, and writes to it are ignored. We will soon see why this is a good idea.


While the **ISA** puts no restrictions on how these registers are used, the **ABI** suggests which registers are to be used for what, and gives them special names.

|Register     |ABI Name   |Usage
|---------    |---------- |----------
|x0           |zero       |Hard-wired to zero
|x1           |ra         |Return Address
|x2	          |sp	      |Stack pointer
|x3	          |gp	      |Global pointer
|x4	          |tp	      |Thread pointer
|x5-x7	      |t0-t2	  |Temporary
|x8	          |s0 / fp	  |Frame pointer
|x9	          |s1	      |Saved register
|x10-x17	  |a0-a7	  |Function argument / Return value
|x18-x27      |s2-s11     |Saved Register
|x28-x31  	  |t3-t6	  |Temporary

We will return to all of these different uses later on, and for now it is sufficient that you are aware that, in assembly code, you can refer to either the register name or the ABI name, and it really makes no difference other than for compatibility and readability.

Apart from the general purpose registers, the only other register is the program counter, ``pc``. This register always contains the address to the next instruction to be executed, and exists in almost all processor designs.

💡 **Note:** *If you have previous experience with assembly programming on other processors you might be surprised that all registers are general-purpose and that the is no specific stack pointer, link register, or even a flag register described in the ISA. There are good reasons for this, and we will address why RISC-V does not need them in upcoming chapters.*


### Instruction Set and Assembly Code
The RV32I instruction set only contains 47 distinct instructions. RISC (Reduced Instruction Set Computer) architectures aim to keep instructions simple and limited in number, emphasizing a small set of frequently used operations. The idea is to keep the hardware as simple as possible, and leave it to the compiler (or programmer) to create more complex behavior by combining simple instructions. Using simple instructions often allows each one to execute in a single clock cycle, simplifies pipelining [^2], and improves processor scalability.

However, this also means that a program written directly in machine instructions is not always easy for a programmer to understand. Therefore, we usually use *assembly language* as the lowest-level language in which we write programs. The assembly language includes all the instructions that the machine understands, but also a number of *pseudo instructions* that the "assembler" translates into one or more machine instructions.

As a simple example, you will have seen in previous courses that one thing we often want to do is to copy a value from one register to another. In assembly language, this looks like:

```
mv x1, x2              // Move (actually copy) the contents of x2 to x1
```
However, the RV32I instruction set does not include a specific instruction for copying values between registers. Instead, the `mv` pseudo-instruction will be translated by the assembler into:

```
add x1, x2, x0        // Add 0 to x2 and store the result in x1
```

This might look strange (to someone reading the machine code), but has exactly the same effect: the contents of x2 are copied to x1. Since we can implement `mv` with `add`, the processor simply reuses the `add` instruction. We will see many more examples of how pseudo-instructions are compiled into machine code later.


### Load/Store architecture.

Another important principle behind the development of the RISC-V architecture is that it is a "Load/Store" architecture. This means that the "load" and "store" instructions are the *only* instructions that communicate with the memory bus. All other instructions (e.g., arithmetic instructions, shifts, or branches) operate only on registers. As an example, the Intel x86 architecture has instructions like [^1] :

[^1]: This is not actual x86 assembly code.

```
add r0, [r1]           // Take the value in memory at the address pointed to by r1, and add it to r0
```

In RISC-V, this is expressed in two instructions:

``` asm
lw x3, 0(x2)         // Load the value in memory at the address pointed to by x2
add x1, x3           // Add that value to x1
```

This might seem unnecessary, but there are several reasons behind this choice. Firstly, it greatly simplifies the hardware design (simpler hardware usually means faster and less error-prone hardware). Secondly, it makes pipelining simpler [^2] and allows the compiler to make optimizations that make the code run faster.

[^2]: Pipelining is when the processor executes several instructions at once, and will not be covered in this course. It is explained in detail in the Computer Architecture course.


### Instruction Cycle
Before we go on to write a first little program, let's recap how a processor executes code. We will introduce a simple model that will do for now, and then extend it a little when we get to interrupts and exceptions, later on in the course.

When the machine turns on, it will enter a `RESET` phase, which initializes registers and puts the processor into a known state. After that, the following "state machine" starts running (one step every clock cycle):

1. **Fetch Instruction -** In this state, whatever address is in the `PC` register will be used to fetch the next instruction. In a basic RV32I architecture *every* instruction (including all arguments) is exactly 32 bits[^3]. The 32 bits are used to pack the "op-code" (which instruction this is, e.g., `add`) and the arguments (e.g., source register x1 and destination register x2). When the instruction has been fetched from memory, the address in PC is increased by 4 bytes (32 bits) so that it points to the next instruction.
2. **Decode Instruction -** The processor will now look at all the bits in the instruction and decide on the operation to execute. For example, if the instruction is `add x1, x2, x0`, it will make registers `x2` and `x0` available as the inputs to the ALU and will configure the ALU to do an addition and store it in `x1`.
3. **Execute Instruction -** In this state, the operation, whatever it was, is executed. This might mean performing the addition for an `add` instruction, or calculating the address of a `load` instruction.
4. **Store -** For instructions that do memory operations, this final phase stores a register's value into memory (for a `store` instruction), or reads the value from memory and stores it in a register (for a `load` instruction).

[^3]: On other architectures, instructions may be followed by arguments, and a RISC-V with the `C` extension allows for "compressed" instructions that are 16 bit wide, but in this course we will only use purely 32-bit instructions.

When the "store" phase is complete, the cycle starts again from the beginning.

💡 **Note:** *If all of this were strictly true, the processor would only execute an instruction once every four clock-cycles when, in fact, it will execute approximately one instruction every clock-cycle. This is due to pipelining and instruction pre-fetching, which is out of scope for this course.*


# Introduction to RISC-V Assembly Programming
We will now look at how to write assembly code for a very simple program, and examine what machine code the assembler produces. Let's say we want our program to do the following:

```
1. Put the value 10 in t0
2. Decrease the value in t0 by 2
3. If t0 > 0, go back to line 2
```

In RISC-V assembly code, we could write this as:
```
    li t0, 10         # 1. Load 10 into t0
loop:
    addi t0, t0, -2   # 2. Subtract 2 from t0
    bgtz t0, loop     # 3. If t0 > 0, go back to 'loop'
```

* In the first line: `li t0, 10`, `li` stands for "Load Immediate", that is, we load a register with a value given immediately in the code (not a value from memory). The first argument, `t0`, is the *destination register*, and the second value is the value we want to put there.

* The second line is just a "label". When compiled, every instruction will be at a specific address in memory and the label, "loop" in this case, will be translated to that address.

* On the third line, `addi t0, t0, -2`, `addi` stands for "Add Immediate". Again, "immediate" means that the value that we want to add to the source register is given directly in the instruction. The first argument is, again, the *destination register*. The second argument is the *source register*, and the third argument is the value we want to add to the source register and put in the destination register. Note that there is no "Subtract Immediate" instruction, because adding `-x` to a register is the same as subtracting `x`.

* Finally, on the fourth line: `bgtz t0, loop`, `bgtz` stands for "Branch if Greater Than Zero". The first argument is the register we want to check, and the second argument is the label (address) we want to jump to if the value of that register is greater than zero.

Now let's see what happens if we compile this assembly program to machine code [^4]. If we take the machine code created by the assembler and ask the `objdump` tool to tell us what instructions it represents, the answer is:

```
   addi    t0,zero,10
   loop:
   addi    t0,t0,-2
   blt     zero,t0,4 loop
```

This is slightly different from the code we wrote! Let's see what changed and why:
* `li t0, 10` ➔ `addi t0, zero, 10`: The "Load Immediate" instruction that we used is a pseudo instruction. The processor does not have to implement this instruction, because it already has the "Add Immediate" instruction. By adding the constant 10 to the "zero" register (which is always 0), and store the result in `t0`, we achieve the same thing.
* `bgtz t0, loop` ➔ `blt zero,t0,4`: In our assembly code, we used the "Branch if Greater Than Zero" instruction, but the assembler has translated this into the "Branch if Less Than" instruction. It compares if 0 is less than `t0`, and jumps if that is true. This is equivalent because (x > y) ➔ (y < x).

In this course, you will learn how to write *assembly code*, and we will not worry too much about what *machine code* it turns into, most of the time. It is important to understand, however, that even when writing assembly, the code you write is not always exactly the code that is executed. So far we have seen that the assembler will sometimes replace your pseudo instruction with an equivalent instruction and next we will see that some pseudo instructions will turn into *several* machine code instructions.

### Loading a large constant

We will now change the very first line of our program to:
```
   li t0, 1000000
```
The only difference is that we load the register with one million, rather than 10. If we examine the machine code now, we will see that this turns into:
```
   lui     t0,0xf4
   add     t0,t0,0x240
```
Feel free to raise an eybrow and shake your head a little at this point. When you are done, let's see what happened.
When we asked the assembler to put the value 10 into `t0`, this could be achieved with a single "addi" instruction. In machine code, any instruction is coded into a 32-bit value, and the processor knows how to decode this value. The value will consist of an "opcode" that identifies the type of instruction, and some arguments to the instruction. In the case of the `addi` instruction, this looks like[^5]
:
```
[Opcode(7 bits) | Function(3 bits) | Source Register(5 bits) | Destination Register(5 bits) | Value (12 bits)]
```
So there are 7 bits for the opcode (which allows for 128 different opcodes), 3 bits for the "function" (different variants of the same instruction), 5 bits for the registers (which let's us point out any of the 32 general-purpose registers) and 12 bits for the value that we want to add. When the assembler is asked to load a value that does *not* fit into 12 bits, it simply cannot express that in a single, 32-bit, instruction.

Instead, it will translate your assembly instruction into *two* machine instructions. The first instruction, `lui`, stands for "Load Upper Immediate". It takes a destination register and a 20-bit value as arguments, and it loads the 20-bit value into the 20 upper bits of the destination register.

Since 1000000, in decimal, is `0xf4240` in hexadecimal, `t0` will be loaded with the value `0x000F4000` after the `lui` instruction. The next instruction has to fill in the lower 12 bits, which can be achieved with an `add` instruction (where we have 12 bits for the value).

### Arithmetic and Logical instructions

The table below lists all the ALU instructions in RV32I (the instructions that perform some operation on the input and stores the result in a register). These can be divided into "register-register" instructions where the input consists only of registers, and "register-immediate" instructions, where part of the input is a (small) constant that is embedded in the instruction's machine code. All immediate instructions end with i (for immediate), except for a few that use u to indicate unsigned interpretation of the immediate value.

| Instruction | Explanation |
|-------------|-------------|
| **Arithmetic Instructions** ||
| `add rd, rs1, rs2` | rd = rs1 + rs2 |
| `sub rd, rs1, rs2` | rd = rs1 - rs2 |
| `addi rd, rs1, imm` | rd = rs1 + imm (sign-extended 12-bit immediate) |
| **Shift Instructions** ||
| `sll rd, rs1, rs2` | rd = rs1 << rs2 (logical shift left) |
| `srl rd, rs1, rs2` | rd = rs1 >> rs2 (logical shift right, fill with 0) |
| `sra rd, rs1, rs2` | rd = rs1 >> rs2 (arithmetic shift right, sign-extended) |
| `slli rd, rs1, imm` | rd = rs1 << imm (logical shift left by immediate) |
| `srli rd, rs1, imm` | rd = rs1 >> imm (logical shift right by immediate, fill 0) |
| `srai rd, rs1, imm` | rd = rs1 >> imm (arithmetic shift right by immediate, sign-extended) |
| **Bitwise Logical Instructions** ||
| `xor rd, rs1, rs2` | rd = rs1 ⊕ rs2 (bitwise XOR) |
| `or rd, rs1, rs2` | rd = rs1 ∨ rs2 (bitwise OR) |
| `and rd, rs1, rs2` | rd = rs1 ∧ rs2 (bitwise AND) |
| `xori rd, rs1, imm` | rd = rs1 ⊕ imm |
| `ori rd, rs1, imm` | rd = rs1 ∨ imm |
| `andi rd, rs1, imm` | rd = rs1 ∧ imm |
| **Compare Instructions** ||
| `slt rd, rs1, rs2` | rd = (rs1 < rs2) ? 1 : 0 (signed compare) |
| `sltu rd, rs1, rs2` | rd = (rs1 < rs2) ? 1 : 0 (unsigned compare) |
| `slti rd, rs1, imm` | rd = (rs1 < imm) ? 1 : 0 (signed compare with imm) |
| `sltiu rd, rs1, imm` | rd = (rs1 < imm) ? 1 : 0 (unsigned compare with imm) |

The last category of instructions (the "compare" instructions) will be discussed further in a later lesson when we talk about branching.

In addition to these instructions, there are a number of pseudo instructions that are convenient to use in your assembly code, but will be compiled into one or two "real" instructions by the assembler:

| Pseudo Instruction | Expands to | Meaning |
|--------|-----------|---------|
| `nop` | `addi x0, x0, 0` | No operation |
| `li rd, imm` | `addi` / `lui` + `ori` (depending on imm size) | Load immediate |
| `mv rd, rs` | `addi rd, rs, 0` | Copy register |
| `not rd, rs` | `xori rd, rs, -1` | Bitwise NOT |
| `neg rd, rs` | `sub rd, x0, rs` | Two’s complement negate |
| `seqz rd, rs` | `sltiu rd, rs, 1` | Set if equal to zero |
| `snez rd, rs` | `sltu rd, x0, rs` | Set if not equal to zero |
| `sltz rd, rs` | `slt rd, rs, x0` | Set if less than zero |
| `sgtz rd, rs` | `slt rd, x0, rs` | Set if greater than zero |


[^4]: For information on how to compile, run, and disassemble programs, please see LINK.
[^5]: This is not the exact ordering of the bits used in reality.

# Load and Store Operations

So far, we have seen the basic instructions that let us perform calculations on constants and values in registers. But there is very little point in doing that if we cannot somehow communicate the results to a user, or store them in memory. This is all done by the Load and Store instructions, which we will discuss next.



* Briefly explain the memory bus and introduce them to the idea that all of the address space is not memory.
* Explain alignment
* Show some simple assembly that reads/writes directly from absolute adresses.
* Show how variables are used in assembly.



## Stuff they should know after this lecture

* Difference between assembly and machine code, and why it needs to be different
* ... TBD

## Assignments we can give them to test their knowledge of this lecture
* Ask them to check how different assembly instructions turn into which machine instructions
* Some sort of concept-map where we ask them to connect the different concepts introduced (ISA/ABI/Processor/Processor Core, etc...).