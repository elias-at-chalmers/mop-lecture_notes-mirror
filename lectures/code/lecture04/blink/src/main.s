.section .text
.globl main

###############################################################################
# 1: Gå igenom dessa addressuträkningar
###############################################################################
.equ GPIO_D_CFGLR, 0x40011400
.equ GPIO_D_CFGHR, 0x40011404
.equ GPIO_D_INDR,  0x40011408
.equ GPIO_D_OUTDR, 0x4001140C

main:
    ###############################################################################
    # 2: Gå långsamt igenom hur vi konfigurerar registret
    ###############################################################################

    # Configure GPIO D, pin 2 as output, 2Mhz, push-pull 
    la t0, GPIO_D_CFGLR
    lw t1, 0(t0)
    li t2, 0xFFFFF0FF
    and t1, t1, t2
    li t2, 0x00000200  # Clear bits for pin 2
    or t1, t1, t2   # Set pin 2 
    sw t1, 0(t0)

loop:
    ###############################################################################
    # 3: Blinka. 
    #    1. Först utan s0, och utan delay
    #    2. Visa att vi kan stega igenom men om vi kör lyser den svagt
    #    3. Lägg till delay funktionen
    #       Om tid finnes: Resonera kring hur lång delay vi behöver
    #    4. Fråga vad som gick fel
    #    5. Använd s0
    ###############################################################################

    la s0, GPIO_D_OUTDR
    li t1, 0b100
    sw t1, 0(s0)          

    call delay

    li t1, 0b000
    sw t1, 0(s0)          

    call delay

    j loop                # Repeat indefinitely
    ret


delay: 
    li t0, 0xFFFFFF
delay_loop: 
    addi t0, t0, -1
    bnez t0, delay_loop
    ret



.equ GPIO_D_BSHR, 0x40011410
.equ GPIO_D_BCR,  0x40011414

main: 
    # Konfigurera pinnen
    ...
    # Skriv 1 till pin 2
    la t0, GPIO_D_BSHR
    li t1, 0b100
    sh t1, 0(t0)
    
    # Eller skriv 0    
    la t0, GPIO_D_BCR
    li t1, 0b100
    sh t1, 0(t0)


