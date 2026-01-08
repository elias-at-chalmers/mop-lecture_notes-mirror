|Register     |Usage
|---------    |---------- 
|`mstatus`    |Machine status (global interrupt enables, privilege bits)
|`mcause`     |Cause of last exception (MSB is `1` if interrupt, `0` otherwise)
|`mepc`       |Exception program counter (return address)
|`mtval`      |Trap value (faulting address or instruction)
|`mtvec`      |Machine exception vector base address
|`mie`        |Machine interrupt enable
