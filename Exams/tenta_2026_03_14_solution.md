<!-- To generate html:
pandoc .\Exams\tenta_2026_03_14.md -o .\Exams\tenta.html --lua-filter=quickguide-generator/include-html.lua --standalone --css ../style.css
-->
<style>
body {
    max-width: none !important;
}
h1 {
    background-color: #2c2c2c;
    color: #ffffff;
    padding: 0.4em 0.1em;
    border-radius: 4px;
}
td pre {
    padding: 0;
    margin: 0;
}
td pre code {
    padding: 0.1em 0.1em;
    white-space: pre;
}
td:has(pre) {
    padding: 0.1rem 0.1rem;
}
</style>


# Uppgift 1 (-0.5 ... 2.5p) 

<table>
<tr>
<td>A</td><td>B</td><td>C</td><td>D</td><td>E</td><td>F</td>
</tr>
<tr>
<td>

```
.align 2
a: .space 1
.align 1
b: .space 10
c: .space 4
```

</td>
<td>

```
a: .space 1
.align 1
b: .space 10
.align 1    #WRONG: need .align2
c: .space 4
```
</td>
<td>

```
a: .space 1
.align 2
b: .space 10
.align 1 <!--  WRONG: need .align 2-->
c: .space 4
```

</td>
<td>

```
.align 2
a: .space 1
b: .space 10 <!--  WRONG: need .align 1 before-->
.align 2
c: .space 4
```

</td>
<td>

```
a: .space 1
b: .space 10 <!--  WRONG: need .align 1 before-->
.align 1 <!--  WRONG: need .align 2-->
c: .space 4
```

</td>
<td>

```
.align 1
a: .space 1
.align 2
b: .space 10
.align 1 <!--  WRONG: need .align 2-->
c: .space 4
```

</td>
</tr>
</table>


# Uppgift 2 (-0.5 ... 2.5p)
<table>
<tr>
<td>A</td><td>B</td><td>C</td><td>D</td><td>E</td><td>F</td>
</tr>
<tr>
<td>

```
lw   t0, i
la   t1, arr
slli t0, t0, 1      
addi t0, t0, 1      <!--  WRONG: adds 1 byte, not 1 element-->
add  t0, t0, t1
sh   zero, 0(t0)
```

</td>
<td>

```
lw   t0, i
la   t1, arr
addi t0, t0, 1   
slli t0, t0, 2       <!--  WRONG: multiplies by 4-->
add  t0, t0, t1     
sh   zero, 0(t0)
```

</td>
<td>

```
lw   t0, i
la   t1, arr
addi t0, t0, 2
slli t1, t1, 1       <!--  WRONG: shifts &d-->
add  t0, t0, t1      
sh   zero, 0(t0) 
```

</td>
<td>

```
lw   t0, i
la   t1, arr
addi t0, t0, 1       
slli t0, t0, 1       
add  t0, t0, t1      
sh   zero, 0(t0) 
```

</td>
<td>

```
lw   t0, i
la   t1, arr
slli t0, t0, 1      <!--  WRONG: multiplies by 4-->
addi t0, t0, 1      <!--  WRONG: adds 1 byte, not 1 element-->
add  t0, t0, t1
sh   zero, 0(t0)
```

</td>
<td>

```
lw   t0, i
la   t1, arr
addi t0, t0, 2       <!--  WRONG: increments by 2 elements-->
slli t0, t0, 1       
add  t0, t0, t1      
sh   zero, 0(t0) 
```

</td>
</tr>
</table>

# Uppgift 3 (-0.5 ... 2.5p) 
<table>
<tr>
<td>A</td><td>B</td><td>C</td><td>D</td><td>E</td><td>F</td>
</tr>
<tr>
<td>

```
lw   t0, k
lh   t1, s  <!-- WRONG: s is unsigned-->
sub  t2, t0, t1
la   t3, a
sb   t2, 0(t3)
```

</td>
<td>

```
lw   t0, k
lh   t1, s  <!-- WRONG: s is unsigned-->
sub  t2, t0, t1
la   t3, a
sh   t2, 0(t3) <!-- WRONG: a is char-->
```

</td>
<td>

```
lw   t0, k
lwu   t1, s  <!-- WRONG: s is short-->
sub  t2, t0, t1
la   t3, a
sw   t2, 0(t3)  <!-- WRONG: a is char-->
```

</td>
<td>

```
lb   t0, k  <!-- WRONG: k is word-->
lhu  t1, s
sub  t2, t0, t1
la   t3, a
sw   t2, 0(t3) <!-- WRONG: a is char-->
```

</td>
<td>

```
lw   t0, k
lhu  t1, s
sub  t2, t0, t1
la   t3, a
sb   t2, 0(t3)
```

</td>
<td>

```
lw   t0, k
lh   t1, s  <!-- WRONG: s is unsigned-->
sub  t2, t0, t1
la   t3, a
sh   t2, 0(t3) <!-- WRONG: a is char-->
```

</td>
</tr>
</table>

# Uppgift 4 (-0.5 ... 2.5p) 

<table>
<tr>
<td>A</td><td>B</td>
</tr>
<tr>
<td>

```
loop:
    blt  t1, t2, body  
    j end              
    
body:
    lh   t6, 0(t4)       # t6 = a[i]
    bge  t6, t3, skip    
    add  t0, t0, t6      # sum += a[i]
skip:
    addi t4, t4, 2       # a++
    addi t1, t1, 1       # i++
    j loop
end:
```

</td>
<td>

```
loop:
    blt  t1, t2, body  
    j end              

body:
    lh   t6, 0(t4)       # t6 = a[i]
    blt  t6, t3, skip    <!-- WRONG: if (a[i] >= b) {sum +=} -->
    add  t0, t0, t6      # sum += a[i]
skip:
    addi t4, t4, 2       # a++
    addi t1, t1, 1       # i++
    j loop
end:

```

</td>
</tr>

<tr>
<td>C</td><td>D</td>
</tr>

<tr>
<td>

```
loop:
    bge  t1, t2, end    
    lh   t6, 0(t4)       # t6 = a[i]
    bge  t6, t3, add     <!-- WRONG: if (a[i] >= b) {sum +=} -->
    j continue    
add:
    add  t0, t0, t6      # sum += a[i]
continue:
    addi t4, t4, 2       # a++
    addi t1, t1, 1       # i++
    j loop            
end:
```

</td>
<td>

```
loop:
    blt  t1, t2, end     <!-- WRONG: for (int i = 0; i >= size; i++) -->
    lh   t6, 0(t4)       # t6 = a[i]
    bge  t6, t3, skip 
    add  t0, t0, t6      # sum += a[i]
skip:
    addi t4, t4, 2       # a++
    addi t1, t1, 1       # i++
    j loop 
end:
```

</tr>
</table>

# Uppgift 5 (-0.5 ... 2.5p) 
<table>
<tr>
<td>A</td><td>B</td>
</tr>
<tr>
<td>

```
compute_power_output:
    addi sp, sp, -8
    sw   ra, 0(sp)

    lw   a0, current_mA
    call estimate_power_mW
    sw   a0, 4(sp)

    lw   a1, load_mode #WRONG: reverses the order of parameters
    lw   a0, 4(sp)
    call calculate_factor

    lw   a1, 4(sp)
    mul  a0, a0, a1

    lw   ra, 0(sp)
    addi sp, sp, 8
    ret
```

</td>
<td>

```
compute_power_output:   #WRONG: does not save ra
    lw   a0, current_mA
    call estimate_power_mW
    sw   a0, 0(sp)      #WRONG: rewrites last value in stack

    lw   a0, load_mode
    lw   a1, 0(sp)
    call calculate_factor

    lw   a1, 0(sp)
    mul  a0, a0, a1

    ret
```

</td>
</tr>

<tr>
<td>C</td><td>D</td>
</tr>

<tr>
<td>

```
compute_power_output:   #WRONG: does not save ra
    lw   a0, current_mA
    call estimate_power_mW

    mv   s0, a0         #WRONG: does not save s0 before use
    lw   a0, load_mode
    mv   a1, s0
    call calculate_factor

    mul  a0, a0, s0
    ret

```

</td>
<td>

```
compute_power_output:
    addi    sp, sp, -8
    sw      ra, 0(sp)

    lw      a0, current_mA
    call    estimate_power_mW 
    sw      a0, 4(sp) 

    lw      a0, load_mode
    lw      a1, 4(sp)
    call    calculate_factor

    lw      a1, 4(sp)
    mul     a0, a0, a1

    lw      ra, 0(sp)
    addi    sp, sp, 8
    ret
```
</td>

</tr>
</table>

**Svar: D**

- `lw a0, current_mA` — loads `current_mA` into argument register `a0` as the first parameter ✓
- `sw a0, 4(sp) ` — stores `base_power_mW` into stack to reuse ✓
- `lw a0, load_mode` — loads `current_mA` as the first parameter for `calculate_factor` ✓
- `lw a1, 4(sp)` — loads `base_power_mW` as the second parameter for `calculate_factor` ✓
- after the function call, `lw a1, 4(sp)` — reloads `base_power_mW` from stack as it may have been corrupted in the function ✓
- `mul a0, a0, a1` — the return value is computed and stored int `s0` ✓

Alternativ A reverses parameters for `calculate_factor(load_mode, base_power_mW)`: it actually calls `calculate_factor(base_power_mW, load_mode)`.
Alternativ B does not save `ra` into stack, and rewrites last value in the stack
Alternativ C 

# Uppgift 6 (-0.5 ... 2.5p) 
<table>
<tr>
<td>A</td><td>B</td>
</tr>
<tr>
<td>

```C
void apply_mask(int *array, int size, char mask) {
    for (int i = 0; i < size; i++, array++) {
        char *byte_ptr = (char *)(array + 3); //WRONG: array+3 jumps 3 elements forward
        *byte_ptr &= mask;
    }
}
```

</td>
<td>

```C
void apply_mask(int *array, int size, char mask) {
    for (int i = 0; i < size; i++) {
        char *byte_ptr = (char *)(array + i);
        byte_ptr[3] &= mask;
    }
}
```

</td>
</tr>

<tr>
<td>C</td><td>D</td>
</tr>

<tr>
<td>

```C
void apply_mask(int *array, int size, char mask) {
    for (int i = 0; i < size; i++) {
        char *byte_ptr = (char *)(array + i * sizeof(int)); //WRONG: pointer arithmetics jumps 4 bytes as it is int*
        byte_ptr[3] &= mask;
    }
}
```

</td>
<td>

```C
void apply_mask(int *array, int size, char mask) {
    char *byte_ptr = (char *)array;
    for (int i = 0; i < size; i++, byte_ptr++) { //WRONG: stepping through and modifying every byte
        byte_ptr[3] &= mask;
    }
}
```
</td>

</tr>
</table>


**Svar: B**

# Uppgift 7 (-0.5 ... 2p) (C pointers and parameters)

**Svar: E**

<!-- 
*******************************************************************
************* Open questions **************************************
*******************************************************************
-->
