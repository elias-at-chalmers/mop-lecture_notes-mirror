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
Korrekt sekvens: ladda `i`, addera 1, skifta vänster 1 steg (×2), addera basadressen, spara med `STRH`.

Rätt svar: **D**

<table>
<tr>
<td>A</td><td>B</td><td>C</td><td>D ✓</td><td>E</td><td>F</td>
</tr>
<tr>
<td>

```
LDR  r0, i
LDR  r1, =arr
LSL  r0, r0, #1      @ WRONG: shifts before +1
ADD  r0, r0, #1      @ → i*2+1 (misaligned)
ADD  r0, r0, r1
MOV  r2, #0
STRH r2, [r0]
```

</td>
<td>

```
LDR  r0, i
LDR  r1, =arr
ADD  r0, r0, #1
LSL  r0, r0, #2      @ WRONG: ×4 instead of ×2
ADD  r0, r0, r1      @ treats short as int
MOV  r2, #0
STRH r2, [r0]
```

</td>
<td>

```
LDR  r0, i
LDR  r1, =arr
ADD  r0, r0, #2      @ WRONG: +2 instead of +1
LSL  r1, r1, #1      @ WRONG: shifts base address
ADD  r0, r0, r1
MOV  r2, #0
STRH r2, [r0]
```

</td>
<td>

```
LDR  r0, i
LDR  r1, =arr
ADD  r0, r0, #1      @ i+1
LSL  r0, r0, #1      @ (i+1)*2
ADD  r0, r0, r1      @ arr + (i+1)*2
MOV  r2, #0
STRH r2, [r0]        @ arr[i+1] = 0
```

</td>
<td>

```
LDR  r0, i
LDR  r1, =arr
LSL  r0, r0, #2      @ WRONG: shifts before +1
ADD  r0, r0, #1      @ → i*4+1
ADD  r0, r0, r1
MOV  r2, #0
STRH r2, [r0]
```

</td>
<td>

```
LDR  r0, i
LDR  r1, =arr
ADD  r0, r0, #2      @ WRONG: +2 instead of +1
LSL  r0, r0, #1      @ → arr[i+2], not arr[i+1]
ADD  r0, r0, r1
MOV  r2, #0
STRH r2, [r0]
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

- `k` är `unsigned int` (4 byte) → ladda med `LDR`
- `s` är `unsigned short` → ladda med `LDRH` (ej `LDRSH` — `LDRSH` sign-extendar och ger fel resultat för värden ≥ 32768)
- operation är `k - s` → `SUB r2, r0, r1` (ej omvänt)
- `a[0]` är `signed char` (1 byte) → spara med `STRB`

Rätt svar: **E**

<table>
<tr>
<td>A</td><td>B</td><td>C</td><td>D</td><td>E ✓</td><td>F</td>
</tr>
<tr style="vertical-align: top;">
<td>

```
LDR   r0, k
LDRSH r1, s      @ WRONG: LDRSH sign-extendar
SUB   r2, r0, r1
LDR   r3, =a
STRB  r2, [r3]
```

</td>
<td>

```
LDR   r0, k
LDRSH r1, s      @ WRONG: LDRSH sign-extendar
SUB   r2, r1, r0 @ WRONG: s-k, ej k-s
LDR   r3, =a
STRH  r2, [r3]   @ WRONG: STRH lagrar 2 byte
```

</td>
<td>

```
LDR   r0, k
LDR   r1, s      @ WRONG: LDR laddar 4 byte, s är 2 byte
SUB   r2, r0, r1
LDR   r3, =a
STR   r2, [r3]   @ WRONG: STR lagrar 4 byte
```

</td>
<td>

```
LDRSB r0, k      @ WRONG: k är unsigned int (4 byte)
LDRH  r1, s
SUB   r2, r0, r1
LDR   r3, =a
STR   r2, [r3]   @ WRONG: STR lagrar 4 byte
```

</td>
<td>

```
LDR   r0, k      @ unsigned int → LDR
LDRH  r1, s      @ unsigned short → LDRH
SUB   r2, r0, r1 @ k - s
LDR   r3, =a
STRB  r2, [r3]   @ signed char → STRB
```

</td>
<td>

```
LDR   r0, k
LDRSH r1, s      @ WRONG: LDRSH sign-extendar
SUB   r2, r0, r1
LDR   r3, =a
STRH  r2, [r3]   @ WRONG: STRH lagrar 2 byte
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

Krav: `lr` måste sparas (funktionen gör `BL`-anrop, vilket skriver över `lr`). `base_power_mW` måste sparas över det andra anropet.  
`calculate_factor(load_mode, base_power)` → `r0 = load_mode`, `r1 = base_power_mW`.

Rätt svar: **D**

<table>
<tr>
<td>A</td><td>B</td><td>C</td><td>D ✓</td>
</tr>
<tr style="vertical-align: top;">
<td>

```
compute_power_output:
    PUSH    {r4, lr}

    LDR     r0, current_mA
    BL      estimate_power_mW
    MOV     r4, r0

    LDR     r1, load_mode    @ WRONG: r1=load_mode, r0=base_power
    MOV     r0, r4           @ calculate_factor arg order swapped
    BL      calculate_factor

    MUL     r0, r0, r4

    POP     {r4, pc}
```

</td>
<td>

```
compute_power_output:
    LDR     r0, current_mA
    BL      estimate_power_mW  @ WRONG: lr not saved,
    STR     r0, [sp]           @ clobbered by BL;
                               @ no stack frame allocated
    LDR     r0, load_mode
    LDR     r1, [sp]
    BL      calculate_factor

    LDR     r1, [sp]
    MUL     r0, r0, r1

    BX      lr                 @ returns to wrong place
```

</td>
<td>

```
compute_power_output:
    LDR     r0, current_mA
    BL      estimate_power_mW  @ WRONG: lr not saved

    MOV     r4, r0             @ WRONG: r4 is callee-saved,
    LDR     r0, load_mode      @ must save/restore r4
    MOV     r1, r4             @ corrupts caller's r4
    BL      calculate_factor

    MUL     r0, r0, r4

    BX      lr                 @ returns to wrong place (lr clobbered)
```

</td>
<td>

```
compute_power_output:
    PUSH    {r4, lr}           @ save lr and r4

    LDR     r0, current_mA
    BL      estimate_power_mW
    MOV     r4, r0             @ save base_power_mW in r4

    LDR     r0, load_mode      @ r0 = load_mode (arg 1)
    MOV     r1, r4             @ r1 = base_power_mW (arg 2)
    BL      calculate_factor

    MUL     r0, r0, r4         @ adjusted * base

    POP     {r4, pc}           @ restore r4, return via pc
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

Knappen är kopplad mellan 3.3V och pin 5 i port D (PD5).  
STM32F407: GPIOD basadress = `0x40020C00`, registeroffset: MODER=`+0x00`, PUPDR=`+0x0C`, IDR=`+0x10`, ODR=`+0x14`.

## 3a) Makrodefinitioner (2p)

```c
#define GPIOD_MODER    ((volatile uint32_t *)0x40020C00)
#define GPIOD_PUPDR    ((volatile uint32_t *)0x40020C0C)
#define GPIOD_IDR      ((volatile uint32_t *)0x40020C10)
#define GPIOD_ODR      ((volatile uint32_t *)0x40020C14)
#define GPIOD_ODR_LOW  ((volatile uint8_t  *)0x40020C14)
```

## 3b) GPIO-läge (2p)

Knappen kopplar pin 5 till 3.3V när den trycks in; när den är öppen är pinnen svävande.  
För ett tillförlitligt avläsning krävs **input med pull-down** (MODER=`00`, PUPDR=`10`).  
Pull-down håller pinnen på ett definitivt lågt värde när knappen är öppen, och 3.3V driver den högt när knappen trycks in.

## 3c) Konfiguration av pin 5 (2p)

Pin 5 i MODER: bits [11:10] = `00` (input). Pin 5 i PUPDR: bits [11:10] = `10` (pull-down) = `0x2`.

```c
// Konfigurera MODER: sätt bits [11:10] = 0b00 (input)
*GPIOD_MODER &= ~(0x3 << 10);

// Konfigurera PUPDR: sätt bits [11:10] = 0b10 (pull-down)
*GPIOD_PUPDR = (*GPIOD_PUPDR & ~(0x3 << 10)) | (0x2 << 10);
```

# Uppgift 4 (7p)

Systemklocka: 144 MHz. CLKSOURCE=1 → AHB = 144 MHz → 144 klockcykler per µs.

## 4a) `config_delay` (3p)

```c
void config_delay(int us) {
    *STK_LOAD = (uint32_t)us * 144 - 1;  // reload value (24-bit)
    *STK_VAL  = 0;                        // clear counter och COUNTFLAG
    *STK_CTRL = (1 << 2);                 // CLKSOURCE=AHB, TICKINT=0, ENABLE=0
}
```

## 4b) `delay` (3p)

```c
void delay() {
    *STK_VAL   = 0;                       // clear counter och COUNTFLAG
    *STK_CTRL |= 1;                       // ENABLE=1: starta

    while (!(*STK_CTRL & (1 << 16)));     // vänta på COUNTFLAG (bit 16)

    *STK_CTRL &= ~1;                      // ENABLE=0: stoppa
}
```

## 4c) Maximal tid med CLKSOURCE=0 och 16-bitars STK_VAL (1p)

CLKSOURCE=0 → klockfrekvens = AHB/8 = 144 MHz / 8 = **18 MHz** → 1 tick = 1/18 000 000 s.

16-bitars STK_VAL: maxvärde = 2¹⁶ − 1 = 65 535 ticks.

$$t_{max} = \frac{65535}{18\,000\,000} \approx 3.64 \text{ ms}$$

# Uppgift 6 (7p)

USART1 är IRQ 37 på STM32F407. ARM Cortex-M har 16 systemundantag före de peripherala avbrotten i vektortabellen, så USART1 ligger på index 37 + 16 = **53** i tabellen.

## 6a) NVIC-konfiguration (2p)

NVIC_ISER är uppdelat i 32-bit register där varje bit aktiverar ett IRQ. IRQ 37 ligger i `NVIC_ISER[1]` (register index 37/32 = 1), bit 37%32 = 5.

```c
*NVIC_ISER1 = (1 << 5);   // aktivera IRQ 37 (USART1)
```

## 6b) Vektortabell-relokeringen (2p)

```c
*SCB_VTOR = 0x2001C000;
```

## 6c) Sätt in funktionspekaren (3p)

Vektortabellen är en array av `uint32_t`-funktionspekare. USART1 ligger på index 53.

```c
void (*usart_isr)(void);   // deklaration av den givna hanteraren

uint32_t *vtor = (uint32_t *)0x2001C000;
vtor[53] = (uint32_t)usart_isr;
```

# Uppgift 7 (7p)

Pin 14 och 15 ligger i SYSCFG_EXTICR4 (täcker pins 12–15).  
Port E-värdet i SYSCFG_EXTICR är `0x4`.  
På STM32F407 delar EXTI14 och EXTI15 en gemensam vector (EXTI15_10).

## 7a) EXTI och SYSCFG_EXTICR (3p)

Från kretsbilden: magnetisk sensor (PE14) ger fallande flank vid intrång (dörr öppnas, strömbrytare öppnas); vibrationssensor (PE15) ger stigande flank (rörelse stänger strömbrytaren).

```c
// Koppla EXTI14 och EXTI15 till port E
*SYSCFG_EXTICR4 = (*SYSCFG_EXTICR4 & ~((0xF << 4) | (0xF << 8)))
                | (0x4 << 4)   // EXTI14 = port E
                | (0x4 << 8);  // EXTI15 = port E

// Aktivera interrupt mask för linje 14 och 15
*EXTI_IMR |= (1 << 14) | (1 << 15);

// Magnetisk sensor: fallande flank (dörr öppnas)
*EXTI_FTSR |= (1 << 14);

// Vibrationssensor: stigande flank (rörelse detekteras)
*EXTI_RTSR |= (1 << 15);
```

## 7b) Interrupt handler (4p)

På ARM Cortex-M är en interrupt handler en vanlig C-funktion — ingen special-attribut behövs. Pending-biten i `EXTI_PR` måste rensas manuellt (skriv 1 för att rensa).

```c
void interrupt_handler(void) {
    if (*EXTI_PR & (1 << 14)) {
        *EXTI_PR = (1 << 14);  // rensa pending
        sound_alarm(1);         // magnetisk sensor
    }
    if (*EXTI_PR & (1 << 15)) {
        *EXTI_PR = (1 << 15);  // rensa pending
        sound_alarm(2);         // vibrationssensor
    }
}
```

