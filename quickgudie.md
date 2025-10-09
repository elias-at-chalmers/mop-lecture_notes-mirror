https://projectf.io/posts/riscv-cheat-sheet/

## Testing generator




## RV32I Instruction Tables

### Arithmetic

| Instr   | Description                | Use                   | Result                        | Guide      |
|---------|----------------------------|-----------------------|-------------------------------|------------|
| add     | Add                        | add rd, rs1, rs2      | rd = rs1 + rs2                | arithmetic |
| addi    | Add Immediate              | addi rd, rs1, imm     | rd = rs1 + imm                | arithmetic |
| neg     | *Negate (p)*               | neg rd, rs2           | rd = -rs2                     | arithmetic |
| sub     | Subtract                   | sub rd, rs1, rs2      | rd = rs1 - rs2                | arithmetic |
| mul     | Multiply                   | mul rd, rs1, rs2      | rd = (rs1 * rs2)[31:0]        | multiply   |
| mulh    | Multiply High              | mulh rd, rs1, rs2     | rd = (rs1 * rs2)[63:32]       | multiply   |
| mulhu   | Multiply High Unsigned     | mulhu rd, rs1, rs2    | rd = (rs1 * rs2)[63:32]       | multiply   |
| mulhsu  | Multiply High Signed Unsigned | mulhsu rd, rs1, rs2 | rd = (rs1 * rs2)[63:32]       | multiply   |
| div     | Divide                     | div rd, rs1, rs2      | rd = rs1 / rs2                | divide     |
| rem     | Remainder                  | rem rd, rs1, rs2      | rd = rs1 % rs2                | divide     |

### Bitwise Logic

| Instr   | Description        | Use                | Result           | Guide    |
|---------|--------------------|--------------------|------------------|----------|
| and     | AND                | and rd, rs1, rs2   | rd = rs1 & rs2   | logical  |
| andi    | AND Immediate      | andi rd, rs1, imm  | rd = rs1 & imm   | logical  |
| not     | *NOT (p)*          | not rd, rs1        | rd = ~rs1        | logical  |
| or      | OR                 | or rd, rs1, rs2    | rd = rs1 | rs2   | logical  |
| ori     | OR Immediate       | ori rd, rs1, imm   | rd = rs1 | imm   | logical  |
| xor     | XOR                | xor rd, rs1, rs2   | rd = rs1 ^ rs2   | logical  |
| xori    | XOR Immediate      | xori rd, rs1, imm  | rd = rs1 ^ imm   | logical  |

### Shift

| Instr   | Description                    | Use                  | Result           | Guide  |
|---------|-------------------------------|----------------------|------------------|--------|
| sll     | Shift Left Logical            | sll rd, rs1, rs2     | rd = rs1 << rs2  | shift  |
| slli    | Shift Left Logical Immediate  | slli rd, rs1, imm    | rd = rs1 << imm  | shift  |
| srl     | Shift Right Logical           | srl rd, rs1, rs2     | rd = rs1 >> rs2  | shift  |
| srli    | Shift Right Logical Immediate | srli rd, rs1, imm    | rd = rs1 >> imm  | shift  |
| sra     | Shift Right Arithmetic        | sra rd, rs1, rs2     | rd = rs1 >>> rs2 | shift  |
| srai    | Shift Right Arithmetic Immediate | srai rd, rs1, imm | rd = rs1 >>> imm | shift  |

### Load Immediate

| Instr   | Description                | Use              | Result             | Guide      |
|---------|----------------------------|------------------|--------------------|------------|
| li      | *Load Immediate (p)*       | li rd, imm       | rd = imm           | arithmetic |
| lui     | Load Upper Immediate       | lui rd, imm      | rd = imm << 12     | arithmetic |
| auipc   | Add Upper Immediate to PC  | auipc rd, imm    | rd = pc + (imm<<12)| branch     |

### Load and Store

| Instr   | Description                | Use                  | Result                    | Guide  |
|---------|----------------------------|----------------------|---------------------------|--------|
| lw      | Load Word                  | lw rd, imm(rs1)      | rd = mem[rs1+imm]         | load   |
| lh      | Load Half                  | lh rd, imm(rs1)      | rd = mem[rs1+imm][0:15]   | load   |
| lhu     | Load Half Unsigned         | lhu rd, imm(rs1)     | rd = mem[rs1+imm][0:15]   | load   |
| lb      | Load Byte                  | lb rd, imm(rs1)      | rd = mem[rs1+imm][0:7]    | load   |
| lbu     | Load Byte Unsigned         | lbu rd, imm(rs1)     | rd = mem[rs1+imm][0:7]    | load   |
| la      | *Load Symbol Address (p)*  | la rd, symbol        | rd = &symbol              | load   |
| sw      | Store Word                 | sw rs2, imm(rs1)     | mem[rs1+imm] = rs2        | store  |
| sh      | Store Half                 | sh rs2, imm(rs1)     | mem[rs1+imm][0:15] = rs2  | store  |
| sb      | Store Byte                 | sb rs2, imm(rs1)     | mem[rs1+imm][0:7] = rs2   | store  |

### Jump and Function

| Instr   | Description                | Use                  | Result                        | Guide    |
|---------|----------------------------|----------------------|-------------------------------|----------|
| j       | *Jump (p)*                 | j imm                | pc += imm                     | jump     |
| jal     | Jump and Link              | jal rd, imm          | rd = pc+4; pc += imm          | jump     |
| jalr    | Jump and Link Register     | jalr rd, rs1, imm    | rd = pc+4; pc = rs1+imm       | jump     |
| call    | *Call Function (p)*        | call symbol          | ra = pc+4; pc = &symbol       | function |
| ret     | *Return from Function (p)* | ret                  | pc = ra                       | function |

### Branch

| Instr   | Description                | Use                  | Result                        | Guide    |
|---------|----------------------------|----------------------|-------------------------------|----------|
| beq     | Branch Equal               | beq rs1, rs2, imm    | if(rs1 == rs2) pc += imm      | branch   |
| beqz    | *Branch Equal Zero (p)*    | beqz rs1, imm        | if(rs1 == 0) pc += imm        | branch   |
| bne     | Branch Not Equal           | bne rs1, rs2, imm    | if(rs1 ≠ rs2) pc += imm       | branch   |
| bnez    | *Branch Not Equal Zero (p)*| bnez rs1, rs2, imm   | if(rs1 ≠ 0) pc += imm         | branch   |
| blt     | Branch Less Than           | blt rs1, rs2, imm    | if(rs1 < rs2) pc += imm       | branch   |
| bltu    | Branch Less Than Unsigned  | bltu rs1, rs2, imm   | if(rs1 < rs2) pc += imm       | branch   |
| bltz    | *Branch Less Than Zero (p)*| bltz rs1, imm        | if(rs1 < 0) pc += imm         | branch   |
| bgt     | *Branch Greater Than (p)*  | bgt rs1, rs2, imm    | if(rs1 > rs2) pc += imm       | branch   |
| bgtu    | *Branch Greater Than Unsigned (p)* | bgtu rs1, rs2, imm | if(rs1 > rs2) pc += imm      | branch   |
| bgtz    | *Branch Greater Than Zero (p)* | bgtz rs1, imm       | if(rs1 > 0) pc += imm         | branch   |
| ble     | *Branch Less or Equal (p)* | ble rs1, rs2, imm    | if(rs1 ≤ rs2) pc += imm       | branch   |
| bleu    | *Branch Less or Equal Unsigned (p)* | bleu rs1, rs2, imm | if(rs1 ≤ rs2) pc += imm      | branch   |
| blez    | *Branch Less or Equal Zero (p)* | blez rs1, imm       | if(rs1 ≤ 0) pc += imm         | branch   |
| bge     | Branch Greater or Equal    | bge rs1, rs2, imm    | if(rs1 ≥ rs2) pc += imm       | branch   |
| bgeu    | Branch Greater or Equal Unsigned | bgeu rs1, rs2, imm | if(rs1 ≥ rs2) pc += imm      | branch   |
| bgez    | *Branch Greater or Equal Zero (p)* | bgez rs1, imm      | if(rs1 ≥ 0) pc += imm         | branch   |

### Set

| Instr   | Description                | Use                  | Result                        | Guide    |
|---------|----------------------------|----------------------|-------------------------------|----------|
| slt     | Set Less Than              | slt rd, rs1, rs2     | rd = (rs1 < rs2)              | set      |
| slti    | Set Less Than Immediate    | slti rd, rs1, imm    | rd = (rs1 < imm)              | set      |
| sltu    | Set Less Than Unsigned     | sltu rd, rs1, rs2    | rd = (rs1 < rs2)              | set      |
| sltiu   | Set Less Than Immediate Unsigned | sltui rd, rs1, imm | rd = (rs1 < imm)              | set      |
| seqz    | *Set Equal Zero (p)*       | seqz rd, rs1         | rd = (rs1 == 0)               | set      |
| snez    | *Set Not Equal Zero (p)*   | snez rd, rs1         | rd = (rs1 ≠ 0)                | set      |
| sltz    | *Set Less Than Zero (p)*   | sltz rd, rs1         | rd = (rs < 0)                 | set      |
| sgtz    | *Set Greater Than Zero (p)*| sgtz rd, rs1         | rd = (rs1 > 0)                | set      |

### Counters

| Instr      | Description                      | Use            | Result                    | Guide         |
|------------|----------------------------------|----------------|---------------------------|---------------|
| rdcycle    | *CPU Cycle Count (p)*            | rdcycle rd     | rd = csr_cycle[31:0]      | not yet avail |
| rdcycleh   | *CPU Cycle Count High (p)*       | rdcycleh rd    | rd = csr_cycle[63:32]     | not yet avail |
| rdtime     | *Current Time (p)*               | rdtime rd      | rd = csr_time[31:0]       | not yet avail |
| rdtimeh    | *Current Time High (p)*          | rdtimeh rd     | rd = csr_time[63:32]      | not yet avail |
| rdinstret  | *CPU Instructions Retired (p)*   | rdinstret rd   | rd = csr_instret[31:0]    | not yet avail |
| rdinstreth | *CPU Instructions Retired High (p)*| rdinstreth rd| rd = csr_instret[63:32]   | not yet avail |

### Misc Instructions

| Instr   | Description                        | Use           | Result         | Guide         |
|---------|------------------------------------|---------------|----------------|---------------|
| ebreak  | Environment Break (Debugger Call)  | ebreak        | -              | not yet avail |
| ecall   | Environment Call (OS Function)     | ecall         | -              | not yet avail |
| fence   | I/O Ordering                       | fence         | -              | not yet avail |
| mv      | *Copy Register (p)*                | mv rd, rs1    | rd = rs1       | arithmetic    |
| nop     | *No Operation (p)*                 | nop           | -              | arithmetic    |


