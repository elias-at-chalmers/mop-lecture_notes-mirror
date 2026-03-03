.section .data
.align 2
frame: .space 6*4

# Function pointers — set by tft_init, used by application code via tft.h
.global tft_ellipse
tft_ellipse: .word 0
.global tft_rect
tft_rect:    .word 0
.global tft_line
tft_line:    .word 0
.global tft_pixel
tft_pixel:   .word 0
.global tft_sprite
tft_sprite:  .word 0

tft_init_call:
.word 0 # function = init
.word frame
tft_line_call:
.word 3 # function = line
.word frame
tft_rect_call:
.word 2 # function = rect
.word frame
tft_ellipse_call:
.word 1 # function = ellipse
.word frame


.section .text
# void tft_init(void)
#   Wires all tft_* function pointers to the simulator (ecall) implementations,
#   then tail-calls sim_tft_init(1) which installs the ecall vector and returns.
.global tft_init
tft_init:
    la t0, tft_ellipse;  la t1, sim_tft_ellipse;  sw t1, 0(t0)
    la t0, tft_rect;     la t1, sim_tft_rect;      sw t1, 0(t0)
    la t0, tft_line;     la t1, sim_tft_line;      sw t1, 0(t0)
    la t0, tft_pixel;    la t1, sim_tft_pixel;     sw t1, 0(t0)
    la t0, tft_sprite;   la t1, sim_tft_sprite;    sw t1, 0(t0)
    li a0, 1
    j  sim_tft_init         # tail-call; sim_tft_init will ret to our caller

# int sim_tft_init(int option)
#   a0 = option; returns int in a0
#   Also installs the ecall exception vector at 0x2001C014.
.global sim_tft_init
sim_tft_init:
    la  t1, tft_init_call           # address of the C ecall ISR
    li  t2, 0x2001C014      # exception vector slot
    sw  t1, 0(t2)           # install handler
    la  a7, tft_init_call
    la  t0, frame
    sw  a0, 0(t0)           # option
    ecall
    lw  a0, 0(t0)           # return value from _tft_init (t0 survives ecall)
    ret

.global sim_tft_pixel
sim_tft_pixel:
    la a7, tft_line_call
    la t0, frame
    sw a0, 0(t0) # x
    sw a1, 4(t0) # y
    sw a0, 8(t0) # x2
    addi a1, a1, 1
    sw a1, 12(t0) # y2
    sw a2, 16(t0) # colour
    ecall
    ret


# void tft_line(int x1, int y1, int x2, int y2, int colour)
#   a0=x1, a1=y1, a2=x2, a3=y2, a4=colour
.global sim_tft_line
sim_tft_line:
    la  a7, tft_line_call
    la  t0, frame
    sw  a0,  0(t0)  # x1
    sw  a1,  4(t0)  # y1
    sw  a2,  8(t0)  # x2
    sw  a3, 12(t0)  # y2
    sw  a4, 16(t0)  # colour
    ecall
    ret


# void tft_rect(int x1, int y1, int x2, int y2, int colour, int fill)
#   a0=x1, a1=y1, a2=x2, a3=y2, a4=colour, a5=fill
.global sim_tft_rect
sim_tft_rect:
    la  a7, tft_rect_call
    la  t0, frame
    sw  a0,  0(t0)  # x1
    sw  a1,  4(t0)  # y1
    sw  a2,  8(t0)  # x2
    sw  a3, 12(t0)  # y2
    sw  a4, 16(t0)  # colour
    sw  a5, 20(t0)  # fill
    ecall
    ret


# void tft_ellipse(int xc, int yc, int xw, int yw, int colour, int fill)
#   a0=xc, a1=yc, a2=xw, a3=yw, a4=colour, a5=fill
.global sim_tft_ellipse
sim_tft_ellipse:
    la  a7, tft_ellipse_call
    la  t0, frame
    sw  a0,  0(t0)  # xc
    sw  a1,  4(t0)  # yc
    sw  a2,  8(t0)  # xw
    sw  a3, 12(t0)  # yw
    sw  a4, 16(t0)  # colour
    sw  a5, 20(t0)  # fill
    ecall
    ret


# void tft_sprite(int x, int y, const uint16_t *data, int w, int h)
#   a0 = x (top-left)
#   a1 = y (top-left)
#   a2 = data pointer (row-major uint16_t pixels)
#   a3 = w
#   a4 = h
.global sim_tft_sprite
sim_tft_sprite:
    la  a7, tft_line_call
    la  t3, frame
    li  t0, 0               # row = 0
.Lrow:
    bge t0, a4, .Ldone
    li  t1, 0               # col = 0
.Lcol:
    bge t1, a3, .Lnext_row
    add t4, a0, t1          # screen_x = x + col
    add t5, a1, t0          # screen_y = y + row
    lhu t6, 0(a2)           # colour = *data
    sw  t4,  0(t3)          # x1
    sw  t5,  4(t3)          # y1
    sw  t4,  8(t3)          # x2 (same column)
    addi t2, t5, 1
    sw  t2, 12(t3)          # y2 = screen_y + 1
    sw  t6, 16(t3)          # colour
    ecall
    addi a2, a2, 2          # data++
    addi t1, t1, 1          # col++
    j   .Lcol
.Lnext_row:
    addi t0, t0, 1          # row++
    j   .Lrow
.Ldone:
    ret
