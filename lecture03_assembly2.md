# Jumping around

**Links:**
https://projectf.io/posts/riscv-cheat-sheet/
https://d3s.mff.cuni.cz/files/teaching/nswi200/202324/doc/riscv-abi.pdf

**Text and excercises in the Workbook (Arbetsboken)**
Chapter 1, Pages 34-

**Things that are in the Workbook that should possibly be in this lecture**
Sign Extension

**Notes:** Try to make it the goal of this lecture to call a C function.

This lecture will focus on *Program Control Flow*, i.e., how to make jumps in our code, call subroutines, and make function calls. This will include using the ABI conventions for function parameters and return values, understanding how the *stack* works, and how to deal with *register spilling* (i.e., what to do when we do not have enough registers).


## Jump and Link
There are only two instructions in RISC-V that perform unconditional jumps (`jal` and `jalr`). These are used for a number of pseudoinstructions that make the code easier to read. The table below describes these two instructions.

| Instruction | Mnemonic            | Meaning                                      |
|------------------|----------------|----------------------------------------------|
| `jal ra, offset` | Jump And Link  | Store return address in `ra`.                |
|                  |                | Jump to `pc + offset`.                       |
|                  |                | `offset` must fit in 20 bits                 |
| `jalr ra, offset(rs)` | Jump And Link Register | Store return address in `ra`    |
|                       |                        | Jump to `rs + offset`           |

The word "Link" in these mnemonics means that we store the return address, so that we can return from the jump later.

There are two versions of the jump and link instruction because the simpler one, `jal`, only allows us to jump a certain offset (which is limited to +/- 1MB) [^1] from the current value of `pc`. Since our SRAM (where the code normally resides) is only 64KB in size, that is not normally a problem for us. However, if we need to jump to code in FLASH memory, the distance would be too far for `jal` so we must put our address into a separate register, and then use `jalr`, instead. There are other reasons for using `jalr` which we will encounter shortly.

[^1]: If you are very awake, you might have noticed that a 20-bit offset should only allow us to jump +/- 0.5MB. However, since all instructions are guaranteed to be 2-byte aligned, the offset in the machine instruction is multiplied by 2 before being added to `pc`.

### Jump
Sometimes, all we want to do is jump to some other part of the code. Let's say you want to make an LED lamp blink continuously as soon as you start your machine.  Consider the example below:

```
  <initialization code>
blink:
  <code that turns on an LED lamp, waits 1 second, turns it off, waits 1 sec>
  j blink    // Jump to the "blink" label
```

Here, when making our jump, we have no interest in what the return address is (so we do not need "Link") but the assembler will still translate this pseudo instruction (`j`) into the real instruction (`jal`):

| Instruction      | Mnemonic | Implementation     |
|------------------|----------|--------------------|
| `j offset`       | Jump     | `jal zero, offset` |

Since we do not need the return address for this pseudo instruction, `jal` simply stores it into the `zero` register (writes to `zero`, or `x0`, are always ignored).

The `offset` can be either a label (like in the example), or an absolute address:
`j 0x20000000    // Jump to the start of our program`

In either case, the compiler (or linker) will complain if the address we want to jump to is too far away from the current address (i.e., if the offset from `pc` would require > 20 bits). In such cases, we have to use the `jr` (Jump Register) instruction instead.

| Instruction      | Mnemonic | Implementation     |
|------------------|----------|--------------------|
| `jr rs`       | Jump     | `jalr zero, 0(rs)`    |

If we, for example, knew that we had some useful code at address 0x00000100 (in FLASH memory), we could write:

```riscv
li t0, 0x00000100       // Load the address into t0
jr t0                   // Jump to that address
```

## Branching
Conditionally jumping based on some condition is called "branching" and we wouldn't be able to do much with our computers without it. The real instruction that we use for branching are listed in the table below:

| Instr   | Description                | Use                  | Result                        |
|---------|----------------------------|----------------------|-------------------------------|
| beq     | Branch Equal               | beq rs1, rs2, imm    | if(rs1 == rs2) pc += imm      |
| bne     | Branch Not Equal           | bne rs1, rs2, imm    | if(rs1 ≠ rs2) pc += imm       |
| blt     | Branch Less Than           | blt rs1, rs2, imm    | if(rs1 < rs2) pc += imm       |
| bge     | Branch Greater or Equal    | bge rs1, rs2, imm    | if(rs1 ≥ rs2) pc += imm       |
| bltu    | Branch Less Than Unsigned  | bltu rs1, rs2, imm   | if(rs1 < rs2) pc += imm       |
| bgeu    | Branch Greater or Equal Unsigned | bgeu rs1, rs2, imm | if(rs1 ≥ rs2) pc += imm   |

You might think that this looks limited. Why is there no "Branch Greater Than" or "Branch Less or Equal"? The answer, as usual, is that this functionality can be implemented with existing instructions. Since a < b => b > a, `bgt rs1, rs2, label` (Branch Greater Than) can be written as `blt rs2, rs1, label`. There are pseudoinstructions that cover all of the missing operations:

| Instr   | Description                | Use                  | Result                        |
|---------|----------------------------|----------------------|-------------------------------|
| bgt     | *Branch Greater Than (p)*  | bgt rs1, rs2, imm    | if(rs1 > rs2) pc += imm
| ble     | *Branch Less or Equal (p)* | ble rs1, rs2, imm    | if(rs1 ≤ rs2) pc += imm
| bgtu    | *Branch Greater Than Unsigned (p)* | bgtu rs1, rs2, imm | if(rs1 > rs2) pc += imm
| bleu    | *Branch Less or Equal Unsigned (p)* | bleu rs1, rs2, imm | if(rs1 ≤ rs2) pc += imm

Many of these instructions exist in a signed and an unsigned version (e.g., `blt` and `bltu`). This is necessary since the processor does not know if you consider the value in a register to be a signed or an unsigned number. Consider the instruction `blt x1, zero`. If `x1` contains `0xFFFFFFFB`, it is less than zero if we consider it a signed integer (-5), but *much* more than zero if we consider it unsigned (4294967291).

In addition, there are a number of pseudoinstructions that compare a register's value to zero. These are just for convenience and are easily implemented using the correspoding instructions and using `zero` as one of the operands:

| Instr   | Description                | Use                  | Result                        |
|---------|----------------------------|----------------------|-------------------------------|
| beqz    | *Branch Equal Zero (p)*    | beqz rs1, imm        | if(rs1 == 0) pc += imm        |
| bnez    | *Branch Not Equal Zero (p)*| bnez rs1, rs2, imm   | if(rs1 ≠ 0) pc += imm         |
| bltz    | *Branch Less Than Zero (p)*| bltz rs1, imm        | if(rs1 < 0) pc += imm         |
| bgtz    | *Branch Greater Than Zero (p)* | bgtz rs1, imm       | if(rs1 > 0) pc += imm         |
| blez    | *Branch Less or Equal Zero (p)* | blez rs1, imm       | if(rs1 ≤ 0) pc += imm         |
| bgez    | *Branch Greater or Equal Zero (p)* | bgez rs1, imm      | if(rs1 ≥ 0) pc += imm         |


Let's put this to use. We want to calculate the factorial of 5 (5! = 5 * 4 * 3 * 2 * 1). In C, or any C-like high-level language, this could look like:
```c
int y = 1;
for(int i=1; i<=5; i++) {
  y = y * i;
}
```

We can write this same program in RISC-V assembly language as (read and make sure you follow):

```riscv
li t0, 1              # Use register t0 for `y`, and set it to 1
li t1, 1              # Use register t1 for `i`, and set it to 1
forloop:
  li t2, 5               # Put the value 5 in a temporary register
  bgt t1, t2, end        # Check if `i` is more than 5
                         # and jump to `end` if it is
  mul t0, t0, t1         # y * i -> y
  addi t1, t1, 1         # i++
  j forloop             # Repeat
end:
  # t0 now contains the answer.
```

Note that there are no *immediate* versions of the branch instructions. We cannot write `bgt t1, 5, end`, for example, but have to put the value you want to compare against into a register first.

💡 **Note:** *If you’ve worked with other CPU architectures (perhaps in a previous course), you may notice that branch handling is different on RISC-V. In many older architectures, a branch instruction is preceded by a compare instruction, which compares two values using the ALU and sets condition flags (e.g., Negative, Zero, etc.) in a flag register. In contrast, RISC-V performs these comparisons directly as part of the branch instruction. This design avoids the need for a global condition-code register, simplifying the hardware and reducing potential pipeline dependencies.*

### Branchless (Advanced)
Explain why branchless can be very good. (branch prediction...)
Show a = min(b, c) branchless
Connect to a = (b<c)?b:c
(there should also be an example where the comparison is reused several times... perhaps not important)

## The Stack
Before we move on to discuss how function calls are implemented, let's quickly recap the *Stack* and learn how it is used on a RISC-V architecture. The stack is a memory area where computer programs can store temporary data. The number of registers on any CPU are limited, so sometimes we need to *push* data to memory, temporarily, and then *pop* it back into registers when we need it.

On many architectures, the process might look like this:

```
  <code that does stuff>
  <we need to perform a calculation, but we have no free registers>
  PUSH {t0, t1, t2}       # Copy the contents or registers t0, t1, and t2 to the stack
  <do calculations that might overwrite t0, t1 and t2>
  POP {t0, t1, t2}        # Copy the values from the stack, back into t0, t1, and t2
```

A stack has one associated register called the *stack pointer* (the ABI convention is to use register `x2`/`sp`). This register always holds the address to the *top of the stack*, i.e., where the last value was pushed. The stack pointer needs to be initialized to point at *the end of* some memory area that is known to be free (called the *bottom of the stack*). By convention, the stack grows downward, from higher to lower memory addresses. Then, when we need to store a value (let's say a 4 byte integer) on the stack, we can store it in memory, at the address held by `sp`, and then reduce the address stored in `sp` by 4 bytes.

On many architectures, *push* and *pop* are actual instructions, implemented by the hardware, but in RISC-V it is done explicitly with existing instructions. To push register `t0` to the stack, you would write:

```
  addi sp, sp, -4       # Reduce the stack pointer so it points to where we will store t0
  sw  t0, 0(sp)         # Store t0 at the address held by sp
```

As you will see soon, we often want to push several registers to the stack at the same time. If we needed to push `t0`, `t1`, and `t2` to the stack, we could write:

```
0:  addi sp, sp, -12      # Reduce the stack pointer so it points to where we will store
1:                        # the LAST value we push (t2)
2:  sw t0, 8(sp)          # Store the three registers
3:  sw t1, 4(sp)
4:  sw t2, 0(sp)
5:
```


<figure>
  <img src="images/pushing_on_stack.png">
  <figcaption style="text-align: center;"><em> Left: Initial state, stack pointer is at end-of-stack. Middle: At Line 2, stack pointer moved. Right: Line 5: All registers copied to stack.</em></figcaption>
</figure>

The process is also illustrated in the Figure above.

To pop the values, we just do the same thing in reverse. We first use the current address in the stack-pointer to read out the last three values that were pushed, and then we increase the stack pointer. We do not *remove* the values from the stack, but any subsequent push operation will overwrite that memory:

```
  lw t2, 0(sp)    # Read the values back into registers
  lw t1, 4(sp)
  lw t0, 8(sp)
  addi sp, sp, 12  # Restore the stack pointer to where it was before we pushed
```

## Function Calls
Most high-level languages have the concept of *function calls*. A function takes a number of parameters and returns a value. The assembly language does *not* have function calls built into the language, but even when writing pure assembly, we still want to divide code into functions. Additionally, we want to be able to mix languages. Sometimes we want to write a function in assembly, and call it from our C code, or vice versa. For that to work, it is important that we, as assembly programmers, follow exactly the same rules as the C compiler does.

Therefore, there are several *conventions* (described by the ABI) that define how function calls should be done. Let us consider a simple function that takes two parameters and returns the smallest:

```C
int min(int a, int b) {
  if(a < b) return a;
  else return b;
}
```

How can we write this function in RISC-V assembly? How are the arguments `a` and `b` passed to the function? How do we return a value? All of this is described in detail in the ABI specification, and we will go through the basics here.

### Calling and returning from a function
When calling a function we need to be able to return from that function, and the convention is that the caller saves the return address in register `ra` (The ABI name for register `x1`). Assume that there is a label `min` (somewhere in the code) that corresponds to the address where the function code begins. If the distance from the calling instruction to the function fits in the 20-bit offset of the `jal ra, min` instruction, we could use that directly. Otherwise, we have to first calculate the full target address into a register, and then use the `jalr` instruction.

Whether or not the function is close enough could be hard to know in some cases. Fortunately, the assembler will accept a simple pseudoinstruction, `call`, which puts the return address into `ra` and generates the proper instructions (`jal`, or `auipc`+`jalr`) as needed.

Since the return address is available in `ra`, returning from a function can always be achieved with: `jalr zero, 0(ra)`. There is also a pseudoinstruction `ret` that translates into the same thing.


So to call and return from a function we can write:

```
call min      # Stores the return address in ra and jumps to min
<min will return to here>
...
min:
  <definition of min goes here>
  ret         # Return from function
```

### Function parameters and return values
In simpler cases, when there are at most eight parameters and each parameter fits into 4 bytes (one 32-bit register), the convention for passing arguments to a function is simple: The parameters are stored, in order, in the `a0`-`a7` registers (as many of them as needed) before calling the function. As long as the return value fits in a single 32-bit register, it is stored in `a0` before returning from the function.

There are a few things to note about this:
* In C, the arguments to a function are often of datatypes that are smaller than 4 bytes (`char` or `short`). In assembly, such variables are, by convention, kept in one 32-bit register each even if several of them *could* be packed into a single register.
* When the parameters, or return value, are more advanced datatypes (structs in C, or classes in C++) the rules are more complex and they are sometimes sent in the `a` registers and sometimes pushed onto the stack instead. We will cover some of these cases later in this course.

We are now ready to implement our `min` function and call it:

```
li a0, 20     # Load the value 20 into register a0
li a1, 10     # Load the value 10 into register a1
call min      # Stores the return address in ra and jumps to min
# a0 should now contain the smaller value of 10 and 20
...
min:
  blt a0, a1, min_end     # If a0 is less than a1, we can return because the smallest value is already in a0
  mv  a0, a1              # Otherwise, copy the value of a1 into a0 (the return value)
min_end:
  ret         # Return from function (the return value should now be in a0)
```

### Register Saving
There is one more important thing to consider when writing, or calling, functions in assembly. We will illustrate this by an example.

Let's say you have written the function `int max(int a, int b)` in C and that you are linking the compiled code with your assembly program [^2]. You are now given the task to write an assembly program that finds the smallest of the values stored in registers `t0`, `t1`, and `t2`. You might write the following code:
[^2]: You don't know how to do that just yet, but you will in a few lessons time.

```
mv a0, t0        # Use t0 as the first parameter
mv a1, t1        # and t1 as the second parameter
call max         # After this line, the maximum of t0 and t1 is in a0
                 # Since a0 is already max(t0, t1) we do not have to do anything for the first parameter
mv a1, t2        # Use t2 as the second parameter
call max         # After this line a0 should contain max(max(t0, t1), t2)...
                 # Or should it...
```
 There is a potential bug in this program. Can you spot it? That's right:
 *We do not know what happens in `max`. It might have overwritten our `t2` register!*
 This is a general problem. When writing code, we can not feasibly read through every function we call, just to find out if it might overwrite any register we want to preserve.

We *could* save *all* registers, before calling any function, but that would almost always be a waste. In the `min` function that we wrote earlier, we only overwrite `a0`, so saving every single register everytime we need to call `min` would be very silly.

An alternative would be to let the *callee* (the function being called) save the registers. When *writing* the `min` function, we know that we only need to save the `a0` register, so we would only save that. But then, another function that uses many registers would have to save all of them, regardless of whether they are important to the calling function or not.

The solution to this problem is a compromise. The ABI conventions say that the *calling* function is responsible for saving `t0-t6` and `a0-a7`, if it needs them to be preserved. The *callee* is responsible for saving any other register, if it might modify them.

Put differently, whenever calling a function, you have to think about which of `t0-t6` and `a0-a7` you might need later, and save them. Whenever you are writing a function, and use any of the *other* registers, you have to make sure that you save them first, and restore them before returning.

So for our example to be guaranteed to work, we need to save `t2` to the stack before calling `max` the first time. Note that `max` might also overwrite `a0`, `a1`, `a2`, and `t1` but since we do not need those values any more we do not need to save them:

```
mv a0, t0        # Use t0 as the first parameter
mv a1, t1        # and t1 as the second parameter

addi sp, -8      # Decrease the stack pointer
sw   t2, 4(sp)   # Save t2 for later

call max         # After this line, the maximum of t0 and t1 is in a0
                 # Since a0 is already max(t0, t1) we do not have to do anything for the first parameter

lw   t2, 4(sp)    # Restore t2 from the stack
addi sp, 8        # And increase the stack pointer

mv a1, t2        # Use t2 as the second parameter
call max         # After this line a0 contains max(max(t0, t1), t2)...
```


💡 **Note:** *These rules might seem arbitrary, but have proven to work well for minimizing redundant register saving in most cases. When writing performance-critical code, an optimizer can often find remaining redundant register saving and remove it.*






## Arrays
(slightly out of place in this lecture, but probably won't fit in the previous)


### RISC-V Jumping and Branching Instructions

| Pseudoinstruction | Real Instruction(s) | Description |
|------------------|------------------|-------------|
| `j label`        | `jal x0, label`  | Unconditional jump to `label`. Does not save return address. |
| `jalr rd, rs1, imm` | `jalr rd, imm(rs1)` | Jump to `rs1 + imm`, storing return address in `rd`. |
| `jr rs1`         | `jalr x0, 0(rs1)` | Jump to address in `rs1`, do not save link. |
| `ret`            | `jalr x0, 0(ra)` | Return from subroutine. Jump to address in `ra`. |
| `call label`     | `jal ra, label`  | Call subroutine at `label`; save return address in `ra`. |
| `beq rs1, rs2, label` | `beq rs1, rs2, label` | Branch if `rs1 == rs2` to `label`. |
| `bne rs1, rs2, label` | `bne rs1, rs2, label` | Branch if `rs1 != rs2` to `label`. |
| `blt rs1, rs2, label` | `blt rs1, rs2, label` | Branch if `rs1 < rs2` (signed) to `label`. |
| `bge rs1, rs2, label` | `bge rs1, rs2, label` | Branch if `rs1 >= rs2` (signed) to `label`. |
| `bltu rs1, rs2, label` | `bltu rs1, rs2, label` | Branch if `rs1 < rs2` (unsigned) to `label`. |
| `bgeu rs1, rs2, label` | `bgeu rs1, rs2, label` | Branch if `rs1 >= rs2` (unsigned) to `label`. |
| `j label`        | `jal x0, label`  | Pseudoinstruction alias for unconditional jump. |
| `jal label`      | `jal ra, label`  | Jump and link; used for function calls. |
| `nop`            | `addi x0, x0, 0` | No operation (used in delay slots or padding, sometimes with branches). |
| `li rd, imm`     | `addi rd, x0, imm` or `lui+addi` | Load immediate (can be used to set up jump addresses in some pseudo branches). |

