# Jumping around


**Text and excercises in the Workbook (Arbetsboken)**
Chapter 1, Pages 34-

**Things that are in the Workbook that should possibly be in this lecture**
Sign Extension

**Notes:** Try to make it the goal of this lecture to call a C function.

> *"I came to get down, I came to get down
> So get out your seat and jump around"*
> 
>  -- House of Pain, 1992

This lecture will focus on *Program Control Flow*, i.e., how to make jumps in our code, call subroutines, and how to make function calls. This will include using the ABI conventions for function parameters and return parameters, understanding how the *stack* works, and how to deal with *register spilling* (i.e., what to do when we do not have enough registers).


## Jump and Link
There are only two instructions in RISC-V that perform unconditional jumps (`jal` and `jalr`). These are used for a number of pseudoinstructions that make the code much easier to read. The table below describes these two instructions. 

| Instruction | Mnemonic            | Meaning                                      |
|------------------|----------------|----------------------------------------------|
| `jal ra, offset` | Jump And Link  | Store return address in `ra`.                | 
|                  |                | Jump to `pc + offset`.                       |
|                  |                | `offset` must fit in 20 bits                 |
| `jalr ra, offset(rs)` | Jump And Link Register | Store return address in `ra`    | 
|                       |                        | Jump to `rs + offset`           | 

There are two versions of the jump and link instruction because the simpler one, `jal`, only allows us to jump a certain offset (which is limited to +/- 500KB) from the current value of `pc`. Since our SRAM (where the code normally resides) is only 64KB large, that is not normally a problem, but if we needed to jump to code in FLASH memory, the distance would be too far for `jal` and we would have to put our address into a separate register and then use `jalr`, instead. There are other reasons for using `jalr` which we will encounter shortly. 

### Jump
Sometimes, all we want to do is jump to some other part of the code. Let's say you want to make an LED lamp blink continously as soon as you started your machine.  Consider the example below: 

```
  <initialization code>
blink: 
  <code that turns on a led lamp, waits 1 second, turns it off, waits 1 sec>
  j blink    // Jump to the "blink" label
```

Here, when making our jump, we have no interest in what the return address but the assembler will still turn this pseudo instruction (`j`) into a real instruction (`jal`): 

| Instruction      | Mnemonic | Implementation     | 
|------------------|----------|--------------------|
| `j offset`       | Jump     | `jal zero, offset` | 

Since we do not need the return address for this pseudo instruction, we simply tell `jal` to store it into the `zero` register (`x0`, which is always 0).

The `offset` can be either a label (like in the example), or an absolute address: 
`j 0x20000000    // Jump to to the start of our program`

In either case, the compiler (or linker) will complain if the address we want to jump to is too far away from the current address (the offset from `pc` would require > 20 bits). In such cases, we have to use the `jr` (Jump Register) instruction instead.

| Instruction      | Mnemonic | Implementation     | 
|------------------|----------|--------------------|
| `jr rs`       | Jump     | `jalr zero, 0(rs)`    | 

If we, for example, knew that we had some useful code att address 0x00000100 (in FLASH memory), we could write: 

```
li t0, 0x00000100       // Load the address into t0
jr t0                   // Jump to that address
```






## Branching
Show an example of a simple for-loop, for example, BEQ, BGE, etc.
Show the pseudo instructions.
BLT vs BLTU, etc

### Branchless
Explain why branchless can be very good. (branch prediction...)
Show a = min(b, c) branchless
Connect to a = (b<c)?b:c
(there should also be an example where the comparison is reused several times... perhaps not important)

## The Stack
Alignment?
No push pop (important first year, at least)
Show what push/pop really does. Why not do that with existing instructions? RISC.
Why not push pull as pseudo instructions?
More efficient to think of a stack frame.
Frame pointer.
Return address.

## Function Calls
## Register Spilling
##I Arrays
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

