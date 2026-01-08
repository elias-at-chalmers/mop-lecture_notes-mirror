# General Purpose Registers

{{include quickguide/registers_gp.md}}

Registers `ra`, `t0`-`t7`, and `a0`- `a7` may be freely overwritten by any function. Hence, they are caller saved. 
All other registers must be saved by the callee, if overwritten. 

# Control and Status Registers

(only a small subset are listed here)

{{include quickguide/registers_csr.md}}
