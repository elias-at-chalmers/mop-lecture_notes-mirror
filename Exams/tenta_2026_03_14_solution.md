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

# Uppgift 1a (-0.5 ... 3p)
Rätt deklarationer är:
> ```
> char a;
> short b[5];
> int c;
> ```

Rätt svar: **A**

<table>
<tr>
<td>A ✓</td><td>B</td><td>C</td><td>D</td><td>E</td><td>F</td>
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
.align 1   @ WRONG: need .align 2
c: .space 4
```

</td>
<td>

```
a: .space 1
.align 2
b: .space 10
.align 1   @ WRONG: need .align 2
c: .space 4
```

</td>
<td>

```
.align 2
a: .space 1
b: .space 10   @ WRONG: need .align 1 before
.align 2
c: .space 4
```

</td>
<td>

```
a: .space 1
b: .space 10   @ WRONG: need .align 1 before
.align 1       @ WRONG: need .align 2
c: .space 4
```

</td>
<td>

```
.align 1
a: .space 1
.align 2
b: .space 10
.align 1   @ WRONG: need .align 2
c: .space 4
```

</td>
</tr>
</table>

# Uppgift 1b (-0.5 ... 3p)
Rätt deklarationer:
> ```c
> int i;
> short arr[255];
> ```

Rätt tilldelning: `arr[i+1] = 0;`

`short` är 2 byte stort, så byte-offseten för `arr[i+1]` är `(i+1) * 2`.
Korrekt sekvens: ladda `i`, addera 1, skifta vänster 1 steg (×2), addera basadressen, spara med `sh`.

Rätt svar: **D**

<table>
<tr>
<td>A</td><td>B</td><td>C</td><td>D ✓</td><td>E</td><td>F</td>
</tr>
<tr>
<td>

```
lw   t0, i
la   t1, arr
slli t0, t0, 1      @ WRONG: shifts before +1
addi t0, t0, 1      @ → offset = i*2+1 (misaligned)
add  t0, t0, t1
sh   zero, 0(t0)
```

</td>
<td>

```
lw   t0, i
la   t1, arr
addi t0, t0, 1
slli t0, t0, 2      @ WRONG: ×4 instead of ×2
add  t0, t0, t1     @ treats short as int
sh   zero, 0(t0)
```

</td>
<td>

```
lw   t0, i
la   t1, arr
addi t0, t0, 2      @ WRONG: +2 instead of +1
slli t1, t1, 1      @ WRONG: shifts base address
add  t0, t0, t1
sh   zero, 0(t0)
```

</td>
<td>

```
lw   t0, i
la   t1, arr
addi t0, t0, 1      @ i+1
slli t0, t0, 1      @ (i+1)*2
add  t0, t0, t1     @ arr + (i+1)*2
sh   zero, 0(t0)    @ arr[i+1] = 0
```

</td>
<td>

```
lw   t0, i
la   t1, arr
slli t0, t0, 2      @ WRONG: shifts before +1
addi t0, t0, 1      @ → offset = i*4+1 (misaligned)
add  t0, t0, t1
sh   zero, 0(t0)
```

</td>
<td>

```
lw   t0, i
la   t1, arr
addi t0, t0, 2      @ WRONG: +2 instead of +1
slli t0, t0, 1      @ → arr[i+2], not arr[i+1]
add  t0, t0, t1
sh   zero, 0(t0)
```

</td>
</tr>
</table>


# Uppgift 1c (-0.5 ... 3p)
Rätt deklarationer:
> ```c
> unsigned int k;
> signed char a[5];
> unsigned short s;
> ```

Rätt tilldelning: `a[0] = k - s;`

- `k` är `unsigned int` (4 byte) → ladda med `lw`
- `s` är `unsigned short` → ladda med `lhu` (ej `lh` — `lh` sign-extendar och ger fel resultat för värden ≥ 32768)
- operation är `k - s` → `sub t2, t0, t1` (ej omvänt)
- `a[0]` är `signed char` (1 byte) → spara med `sb`

Rätt svar: **E**

<table>
<tr>
<td>A</td><td>B</td><td>C</td><td>D</td><td>E ✓</td><td>F</td>
</tr>
<tr style="vertical-align: top;">
<td>

```
lw   t0, k
lh   t1, s      @ WRONG: lh sign-extendar
sub  t2, t0, t1
la   t3, a
sb   t2, 0(t3)
```

</td>
<td>

```
lw   t0, k
lh   t1, s      @ WRONG: lh sign-extendar
sub  t2, t1, t0 @ WRONG: s-k, ej k-s
la   t3, a
sh   t2, 0(t3)  @ WRONG: sh lagrar 2 byte
```

</td>
<td>

```
lw   t0, k
lw   t1, s      @ WRONG: lw laddar 4 byte, s är 2 byte
sub  t2, t0, t1
la   t3, a
sw   t2, 0(t3)  @ WRONG: sw lagrar 4 byte
```

</td>
<td>

```
lb   t0, k      @ WRONG: k är unsigned int (4 byte)
lhu  t1, s
sub  t2, t0, t1
la   t3, a
sw   t2, 0(t3)  @ WRONG: sw lagrar 4 byte
```

</td>
<td>

```
lw   t0, k      @ unsigned int → lw
lhu  t1, s      @ unsigned short → lhu
sub  t2, t0, t1 @ k - s
la   t3, a
sb   t2, 0(t3)  @ signed char → sb
```

</td>
<td>

```
lw   t0, k
lh   t1, s      @ WRONG: lh sign-extendar
sub  t2, t0, t1
la   t3, a
sh   t2, 0(t3)  @ WRONG: sh lagrar 2 byte
```

</td>
</tr>
</table>

# Uppgift 2a (-1 ... 3p)
C-kod:
> ```c
> int compute_power_output(void) {
>     int base_power_mW = estimate_power_mW(current_mA);
>     int adjusted_power_mW = calculate_factor(load_mode, base_power_mW);
>     return adjusted_power_mW * base_power_mW;
> }
> ```

Krav: `ra` måste sparas (funktionen gör call-instruktioner). `base_power_mW` måste sparas över det andra anropet.  
`calculate_factor(load_mode, base_power)` → `a0 = load_mode`, `a1 = base_power_mW`.


Rätt svar: **D**

<table>
<tr>
<td>A</td><td>B</td><td>C</td><td>D ✓</td>
</tr>
<tr style="vertical-align: top;">
<td>

```
compute_power_output:
addi sp, sp, -8
sw   ra, 0(sp)

lw   a0, current_mA
call estimate_power_mW
sw   a0, 4(sp)

lw   a1, load_mode   @ WRONG: a1=load_mode, a0=base_power
lw   a0, 4(sp)       @ calculate_factor arg order swapped
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
compute_power_output:
lw   a0, current_mA
call estimate_power_mW  @ WRONG: ra not saved,
sw   a0, 0(sp)          @ clobbered by call;
                        @ no stack frame allocated
lw   a0, load_mode
lw   a1, 0(sp)
call calculate_factor

lw   a1, 0(sp)
mul  a0, a0, a1

ret  @ returns to wrong place
```

</td>
<td>

```
compute_power_output:
lw   a0, current_mA
call estimate_power_mW  @ WRONG: ra not saved

mv   s0, a0             @ WRONG: s0 is callee-saved,
lw   a0, load_mode      @ must save/restore s0
mv   a1, s0             @ corrupts caller's s0
call calculate_factor

mul  a0, a0, s0

ret  @ returns to wrong place (ra clobbered)
```

</td>
<td>

```
compute_power_output:
addi sp, sp, -8
sw   ra, 0(sp)      @ save return address

lw   a0, current_mA
call estimate_power_mW
sw   a0, 4(sp)      @ save base_power_mW

lw   a0, load_mode  @ a0 = load_mode (arg 1)
lw   a1, 4(sp)      @ a1 = base_power_mW (arg 2)
call calculate_factor

lw   a1, 4(sp)      @ reload base_power_mW
mul  a0, a0, a1     @ adjusted * base

lw   ra, 0(sp)
addi sp, sp, 8
ret
```

</td>
</tr>
</table>

# Uppgift 2b (-1 ... 3p)
Funktionen `apply_mask(int *array, int size, char mask)` ska applicera `mask` (bitvis AND) på den **fjärde byten (byte index 3, MSB)** av varje element. ch32v307/RISC-V är little-endian → MSB ligger på offset +3 från elementets startadress.

Rätt svar: **B**

<table>
<tr>
<td>A</td><td>B ✓</td><td>C</td><td>D</td>
</tr>
<tr style="vertical-align: top;">
<td>

```c
for (int i = 0; i < size; i++, array++) {
    char *byte_ptr = (char *)(array + 3);
    *byte_ptr &= mask;
}
// WRONG: array är int*, så array+3
// avancerar 3*4=12 byte, inte 3 byte.
// Pekar på array[3], inte byte 3 av array[0].
```

</td>
<td>

```c
for (int i = 0; i < size; i++) {
    char *byte_ptr = (char *)(array + i);
    byte_ptr[3] &= mask;
}
// array+i → pekar på array[i] (int*)
// cast till char* → byte_ptr[3] är
// den 4:e byten av array[i]. Korrekt.
```

</td>
<td>

```c
for (int i = 0; i < size; i++) {
    char *byte_ptr = (char *)(array + i * sizeof(int));
    byte_ptr[3] &= mask;
}
// WRONG: array är int*, dubbel skalning:
// array + i*4 → i*4*4 = 16 byte per steg
// istället för 4. Hoppar över element.
```

</td>
<td>

```c
char *byte_ptr = (char *)array;
for (int i = 0; i < size; i++, byte_ptr++) {
    byte_ptr[3] &= mask;
}
// WRONG: byte_ptr++ avancerar 1 byte/iteration.
// i=1: byte_ptr pekar på byte 1 av array[0],
// inte på array[1].
```

</td>
</tr>
</table>

# Uppgift 2c (-0.5 ... 2p)
Spårning av `foo(a, &b)` där `a = 5`, `b = 5`:

```
x = x + 2;   → lokal kopia x = 7  (a i main påverkas ej, pass by value)
*y = *y + 3; → b = 5 + 3 = 8
y = &x;      → y pekar nu på lokal x (7), ej på b
*y = 100;    → lokal x = 100 (ingen effekt utanför foo)
```

Efter anropet: `a = 5`, `b = 8`

Rätt svar: **E**

| A | B | C | D | E ✓ | F |
|---|---|---|---|-----|---|
| `5 5` | `7 8` | `100 8` | `5 100` | `5 8` | `100 5` |

# Uppgift 3 (6p)

> **OBS:** Bilden (knapp.png) visar en kontakt märkt "Pb7.0" (Port B) men frågetexten anger Port D — bilden är missvisande.

Knappen är kopplad mellan 3.3V och pin 5 i port D (PD5).
CH32V307: GPIOD basadress = `0x40011400`, registeroffset: CFGLR=`+0x00`, INDR=`+0x08`, OUTDR=`+0x0C`.

## 3a) Makrodefinitioner (2p)

```c
#define GPIOD_CFGLR     ((volatile uint32_t *)0x40011400)
#define GPIOD_INDR      ((volatile uint32_t *)0x40011408)
#define GPIOD_OUTDR     ((volatile uint32_t *)0x4001140C)
#define GPIOD_OUTDR_LOW ((volatile uint8_t  *)0x4001140C)
```

## 3b) GPIO-läge (2p)

Knappen kopplar pin 5 till 3.3V när den trycks in; när den är öppen är pinnen svävande.
För ett tillförlitligt avläsning krävs **input med pull-down** (CNF=`10`, MODE=`00`, OUTDR bit 5 = 0).
Pull-down håller pinnen på ett definitivt lågt värde när knappen är öppen, och 3.3V driver den högt när knappen trycks in.

## 3c) Konfiguration av pin 5 (2p)

Pin 5 ligger i CFGLR, bits [23:20]. Värde för input med pull-down: CNF=`10`, MODE=`00` → `0b1000` = `0x8`.
OUTDR bit 5 = 0 väljer pull-down (istället för pull-up).

```c
// Konfigurera CFGLR: sätt bits [23:20] = 0b1000
*GPIOD_CFGLR = (*GPIOD_CFGLR & ~(0xF << 20)) | (0x8 << 20);

// Sätt OUTDR bit 5 = 0 → pull-down
*GPIOD_OUTDR &= ~(1 << 5);
```

# Uppgift 4 (7p)

Systemklocka: 144 MHz. STCLK=1 → HCLK = 144 MHz → 144 klockcykler per µs.

## 4a) `config_delay` (3p)

```c
void config_delay(int us) {
    uint64_t cycles = (uint64_t)us * 144;
    *SYSTICK_CMPLR = (uint32_t)(cycles & 0xFFFFFFFF);
    *SYSTICK_CMPHR = (uint32_t)(cycles >> 32);

    // Rensa flagga, välj HCLK (STCLK=1), starta EJ (STE=0)
    *SYSTICK_SR = 0;
    *SYSTICK_CTLR = (1 << 2);    // STCLK=1, STE=0
}
```

## 4b) `delay` (3p)

```c
void delay() {
    // Nollställ räknaren och flaggan inför varje anrop
    *SYSTICK_CTLR |= (1 << 5);   // INIT: nollställ CNT
    *SYSTICK_CTLR &= ~(1 << 5);
    *SYSTICK_SR = 0;

    *SYSTICK_CTLR |= 1;           // STE=1: starta

    while (!(*SYSTICK_SR & 1));   // vänta på CNTIF

    *SYSTICK_CTLR &= ~1;          // STE=0: stoppa
}
```

## 4c) Maximal tid med STCLK=0 och 16-bitars CNT (1p)

STCLK=0 → klockfrekvens = HCLK/8 = 144 MHz / 8 = **18 MHz** → 1 tick = 1/18 000 000 s.

16-bitars CNT: maxvärde = 2¹⁶ − 1 = 65 535 ticks.

$$t_{max} = \frac{65535}{18\,000\,000} \approx 3.64 \text{ ms}$$

# Uppgift 5 (5p)

Peripheral basadress: `0xE0008000`. Registerlayout:

| Offset | Register | Storlek |
|--------|----------|---------|
| 0x00 | CTRL | 8 bitar (resten grå) |
| 0x04 | STATUS | 16 bitar (resten grå) |
| 0x08 | *(reserverat)* | helt grå |
| 0x0C | DATA | 32 bitar (LOW=bits[15:0], HIGH=bits[31:16]) |

## 5a) Struct-definition och användning (3p)

Det reserverade ordet vid offset `0x08` måste paddas i structen, annars hamnar DATA på fel offset.

```c
typedef struct {
    volatile uint8_t  CTRL;        // offset 0x00
    uint8_t           _pad0[3];    // offset 0x01-0x03 (grå, ej volatile)
    volatile uint16_t STATUS;      // offset 0x04
    uint16_t          _pad1;       // offset 0x06-0x07 (grå)
    uint32_t          _reserved;   // offset 0x08-0x0B (helt grå)
    volatile uint32_t DATA;        // offset 0x0C
} MyPeriph_t;

#define MY_PERIPH ((MyPeriph_t *)0xE0008000)

// Skriv till DATA-registret:
MY_PERIPH->DATA = 0x12345678;
```

## 5b) Bitfält för CTRL (1p)

```c
typedef struct {
    uint8_t EN    : 1;  // bit 0
    uint8_t MODE  : 4;  // bits 1-4
    uint8_t SPEED : 3;  // bits 5-7
} CTRL_t;
```

## 5c) Union för DATA (1p)

```c
typedef union {
    uint32_t DATA;
    struct {
        uint16_t DATA_LOW;   // bits [15:0]
        uint16_t DATA_HIGH;  // bits [31:16]
    };
} DATA_t;
```
