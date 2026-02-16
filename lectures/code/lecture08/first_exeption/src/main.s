.section .text
.globl main


.set STEP, 3

.if STEP == 1

###############################################################################
# Skriv ett program som clearar minnet mellan 0x20001000 och 0x20001010
###############################################################################

main: 
    li t0, 0x20001000 # Startadress
    li t1, 0x20001010 # Slutadress
loop: 
    sw zero, 0(t0)      # Skriv ett ord (0) till adressen som t0 pekar på
    addi t0, t0, 1      # Gå till nästa adress
    blt t0, t1, loop    # Fortsätt loopen tills vi når slutadressen
end: j end


# Vad är felet i koden? 
# Programmet kommer crasha i andra iterationen for 0x20001001 inte är alignad

.endif

.if STEP == 2

###############################################################################
# Skriv ett program som överlever felet, och "fixar" addressen
# DON'T TRY THIS AT HOME!
###############################################################################

interrupt_handler: 
    # Antag att den dåliga addressen ligger i t0
    # och att vi behöver lägga till 3
    addi t0, t0, 3
    ret


main: 
    # Skriv addressen till interrupt_handler i mtvec
    la t0, interrupt_handler
    csrw mtvec, t0

    li t0, 0x20001000 # Startadress
    li t1, 0x20001010 # Slutadress
loop: 
    sw zero, 0(t0)      # Skriv ett ord (0) till adressen som t0 pekar på
    addi t0, t0, 1      # Gå till nästa adress
    blt t0, t1, loop    # Fortsätt loopen tills vi når slutadressen
end: j end


# Koden krashar på ret
# Vad är felet i koden?
# 1. Vad händer när vi kallar ret? 

.endif


.if STEP == 3

###############################################################################
# Skriv ett program som överlever felet, och "fixar" addressen
# DON'T TRY THIS AT HOME!
###############################################################################


interrupt_handler: 
    # Antag att den dåliga addressen ligger i t0
    # och att vi behöver lägga till 3
    addi t0, t0, 3
    mret


main: 
    # Skriv addressen till interrupt_handler i mtvec
    la t0, interrupt_handler
    csrw mtvec, t0

    li t0, 0x20001000 # Startadress
    li t1, 0x20001010 # Slutadress
loop: 
    sw zero, 0(t0)      # Skriv ett ord (0) till adressen som t0 pekar på
    addi t0, t0, 1      # Gå till nästa adress
    blt t0, t1, loop    # Fortsätt loopen tills vi når slutadressen
end:
    j end

.endif

