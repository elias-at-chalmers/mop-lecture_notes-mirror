# Common RISC-V Assembler Directives (GAS)

This document summarises the most important assembler directives used in RISC-V assembly programming with GNU `as`.

---

## Data definition directives

Define raw data in memory.  
Most data directives accept one or more comma-separated values.

- .byte expr [, expr ...]  
  Emit one or more 8-bit values.

  ```s
  my_byte:     .byte 0x12
  my_bytes:    .byte 1, 2, 3, 4
  ```

- .half expr [, expr ...]  
  Emit one or more 16-bit values.

  ```s
  my_half:     .half 0x1234
  my_halves:   .half 10, 20, 30
  ```

- .word expr [, expr ...]  
  Emit one or more 32-bit values (RV32).

  ```s
  my_word:     .word 0x12345678
  my_words:    .word 100, 200, 300
```

- .dword expr [, expr ...]  
  Emit one or more 64-bit values (RV64).

  ```s
  my_dword:    .dword 0x1122334455667788
```

- .asciz "string"  
  Emit a null-terminated string.

  ```s
  message:     .asciz "Hello, world"
  ```

- .ascii "string"  
  Emit a string without a terminating null byte.

  ```s
  banner:      .ascii "RISC-V"
```

---

## Space and alignment

Reserve or align storage.

- .space n  
  Reserve n bytes (contents unspecified or zeroed by linker).

  ```s
  buffer:      .space 64
```

- .align n  
  Align the location counter to a 2ⁿ-byte boundary.

  ```s
  .align 2     # align to 4-byte boundary
```

- .balign n  
  Align to an n-byte boundary.

  ```s
  .balign 16
```

---

## Symbol visibility and linkage

Control symbol scope and linkage.

- .global symbol (or .globl)  
  Make symbol visible to the linker.

  ```s
  .global main
```

- .extern symbol  
  Declare a symbol defined in another file.

  ```s
  .extern printf
```

- .local symbol  
  Make a symbol local to this object file.

---

## Sections

Select where code and data are placed.

- .text  
  Code section.

- .data  
  Writable data section.

- .rodata  
  Read-only data.

- .bss  
  Uninitialised zero-filled data.

---

## Labels and constants

- label:  
  Define a symbol at the current address.

- .equ name, value  
  Define a symbolic constant.

  ```s
  .equ BUF_SIZE, 64
```

- .set name, value  
  Alternative to .equ.

---

## Function-related directives (conventions)

- .type symbol, @function  
  Mark a symbol as a function.

- .size symbol, .-symbol  
  Define the size of a symbol.

---

## Notes

- Directives are assembler instructions, not CPU instructions.
- They do not generate executable code directly.
- Exact behaviour can vary slightly between toolchains.
