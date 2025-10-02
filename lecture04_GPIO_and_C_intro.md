# Introduction to GPIO and C

**Links:**

Reference Manual: https://www.wch-ic.com/downloads/CH32FV2x_V3xRM_PDF.html
**Text and excercises in the Workbook (Arbetsboken)**

**Things that are in the Workbook that should possibly be in this lecture**

So far, we have focused on the *core* of our processor, the *Qingke V4F*, and how to move data between memory and registers. This would be quite pointless unless the processor was connected to the outside world somehow. Today we will start introducing how to communicate with off-chip devices using the GPIO ports.

We will also start looking at the programming language C, which we will start using to program our machine. You know the basics of the RISC-V assembly language now, and you have probably noticed that this quickly becomes impractical for larger programs. Throughout the rest of the course, we will switch over to the (relatively) high-level language C, but we will keep showing you how C is *compiled* into assembly language. Understanding how a high-level language gets translated to assembly, and then machine code, is very important for writing performant and secure code, on any machine.

## GPIO (General Purpose Input/Output)
Any communication between the processor chip and the outside world has to happen via one of the *pins* that connect the chip to the PCB. On your desktop or laptop computer, the majority of these pins are connected directly to the memory subsystem (allowing us to plug in DRAM modules), and to I/O interface buses (allowing us to plug in any PCI Express device, for instance). A microcontroller, on the other hand, might not even have an off-chip memory and often needs to communicate with external peripherals that do not use any standard bus protocol. Therefore, the majority of pins on a microcontroller are often connected to *General Purpose Input/Output* (GPIO) pins [^1].

[^1]: As you will see later, the pins are actually multiplexed and can also be used for standardized protocols, like USART, SPI, etc.

By allowing the processor to directly read or control the logical status of these pins, they can be connected to almost anything, as long as the software drives them with the right values at the right time. In this course, you will go from connecting a single pin to an LED to make it turn on and off, to communicating with an ASCII display via a number of pins. In theory, there is nothing stopping you from connecting some of the pins to an HDMI cable, to drive a monitor, or to a bunch of motors, to drive a radio-controlled car.

![](images/md307_ch32v307_zoom.png)

The picture above illustrates how the processor chip is connected to the GPIO pins on the MD307. If you look close enough (and turn the board over at times) you can follow a very thin wire from most of the CH32V307's tiny pins to one of the more accessible pins on the top of the board. On this MCU, the pins are divided into 16-bit *ports*, labeled A-E. On the bottom of the board, you can see that the pins that make up the ports labeled E and D are also available in a nice little connector layout, that allows us to connect peripheral devices with a standard ribbon cable.

To read or set a pin's value, we read or write to a specific memory location (the GPIO Port's *In or Out Data Register*, which we will discuss soon). When the memory subsystem sees that the address we are trying to write to from the CPU is, e.g., `0x4001400C` it knows (this is hardcoded in the chip) that that write operation should be sent on to the *GPIO Module*. The GPIO Module, in turn, knows that this address means "the Out Data Register for Port D". If the value we write is `0b00001111` it will set the will set the first four GPIOD pins (some of the little spider legs in the image of the chip, above) to 1 (3.3V) and the others to 0 (0V). These pins are connected with wires to the pins in the connector. 

## Blink
When learning programming in almost any language, the starting example is "Hello World.". Similarly, when starting MCU programming, the first thing to try is "Blink", so let's start there. We want to plug in an LED to our MCU and make it blink. This will serve as a first introduction to GPIO programming, and then we will go through the details in the next lecture.

<p align="center">
  <img src="images/IDC_layout.png" alt="My image" width="80%"/>
</p>

The image above illustrates the physical connector corresponding to the lower byte of Port D. Eight of the pins carry a voltage (0 or 3.3V) depending on whether the corresponding bit is high or low. There are two additional pins: one is always 0V (GND) and one is always 3.3V.

If we want to connect an LED we can do that as in the figure above. We connect the cathode of the LED (through a resistor) to the GND pin, and the anode to one of the data-carrying pins (the one corresponding to bit 6, in this case). The resistor is required to stay within the maximum current allowed by the LED.

Now, if we *set* bit 6 in Port D, the pin will be at 3.3V, and a current will run through the LED, making it glow. If we *clear* bit 6, the pin will be at 0V (same as the GND pin) and there will be no current, and no light.

### Configuring a pin for output
Each pin in the port can *either* be an input pin *or* an output pin, at any given time. If the pin is configured as an input pin, we can read the corresponding bit to find out if the pin is at 3.3V (bit is 1) or 0 (bit is 0). Right now, we want pin 6 to act as an output bit, so we have to configure Port D to that effect.

As previously mentioned, any communication between the processor core and the outside is achieved by reading from or writing to the memory subsystem. We have, for instance, seen that we can access the SRAM module by writing to the `0x20000000` - `0x2000FFFF` region. In the same way, to communicate with the GPIO module, we read and write to the `0x4001800`-`0x40011BFF` region. In that region, there are a number of registers for each GPIO Port. To find out which registers there are, and how to configure our GPIO Module, we consult the [QuickGuide](TODO_nolinkyet). The section about the GPIO Module looks like: 

<p align="center">
  <img src="images/Quickguide_GPIO.png" alt="My image" width="95%" style="border: 2px solid black; border-radius: 8px;"/>
</p>

This text is pretty dense, but in a few weeks' time you will find it an invaluable resource when programming.

> 💡 **Tip:** *The QuickGuide is the only help you are allowed to bring to the exam, so learn to find your way around it as soon as possible!*

Let's parse this text to find out what we need to do to get pin number 6, on Port D, to 3.3V so that our LED lights up: 
* There are two configuration registers: `CFGLR` and `CFGHR` for each port. 
  - We can see that the base address for Port D is `0x40011400`
  - The offset for `CFGLR` is `0`, so the address for `GPIOD_CFGLR` is `0x40011400`
  - The offset for `CFGHR` is `4`, so the address for `GPIOD_CFGHR` is `0x40011404`
* The registers are divided into 4 bits per pin, with the first 8 pins in `CFGLR` and the next 8 pins in `CFGHR`
* For each pin, two of the bits set the `MODE` and two of the bits set the `CNF`

We want to set pin 6 as an *output* pin. From the table we can see that we then want to set `MODE` to be `01`, `10`, or `11` depending on what *maximum frequency* we need. If the frequency is set to 10Mhz, that means that we can flip the value of the pin 10 million times per second, and get a reliable output. If we were going to use the pin to send out some digital signal that changed quickly, we might need to worry about that, but since we are just going to turn a light on and off, 2Mhz is more than enough (and this consumes the least energy). So we want to set `MODE` to `10`.

Since `MODE` is an *output* mode, the `CNF` value lets us choose between `Push-Pull`, `Open Drain`, `Alternative Function Push-Pull`, and `Alternative Function Open Drain`. We will go through the meaning of this in the next lecture. For now, we set it to `Push-Pull` (`00`), which means that it will output 0V if the corresponding bit in `OUTDR` is set to 0 and 3.3V if set to 1. 

So, we want to set `MODE` for pin 6 (bits 25:24 in `CFGLR`) to 10, and `CNF` for pin 6 (bits 23:22 in `CFGLR`) to 00. We do not care about the other pins, and will just set them to 0, so we should write the binary value 0000 **1000** 0000 0000 0000 0000 0000 0000, or, in hexadecimal `0x08000000` to `CFGLR` to configure our pin. 

In assembly, that looks like: 
```
la t0, 0x40011400    # Address of CFGLR to t0
li t1, 0x08000000    # Configuration value to t1
sw t1, 0(t0)         # Write the configuration to CFGLR                    
```

### Turning on the LED
Now that the configuration is done, all we have to do is set bit 6 in the *out data* register for GPIO Port D. We look at the [QuickGuide](TODO_nolinkyet) again (or the snippet above) to see that the *base address* for GPIO Port D is still `0x40011400`, and that the *offset* for the out data register, `OUTDR`, is `0xC`. So the address to `GPIOD_OUTDR` is `0x40011400 + 0xC` = `0x4001140C`. 

We want to set bit 6 (the seventh bit) to make pin 6 go to 3.3V and turn on the LED: 

```
la t0, 0x4001140C  # Address to GPIOD_OUTDR to t0
li t1, 0b1000000   # Set bit number 6 in t1
sh t1, 0(t0)       # Set pin 6 to 3.3V (and all others to 0V)
```

Nothing new here, the only thing you should note is that when we write to `OUTDR` we only write a halfword, because we can see in the quickguide that the register is only 16 bits. 

Let's put all of this together into a little Blink program: 

```
la t0, 0x40011400    # Address of CFGLR to t0
li t1, 0x08000000    # Configuration value to t1
sw t1, 0(t0)         # Write the configuration to CFGLR                    

loop: 
la t0, 0x4001140C  # Address to GPIOD_OUTDR to t0
li t1, 0b1000000   # Set bit number 6 in t1
sh t1, 0(t0)       # Set pin 6 to 3.3V (and all others to 0V)

la t0, 0x4001140C  # Address to GPIOD_OUTDR to t0
li t1, 0b0000000   # Clear all bits in t1
sh t1, 0(t0)       # Set all pins to 0V

j loop             # Loop forever
```

If we connect an LED as described above and run this program... nothing seems to happen. 

But that is not very strange, since we will be turning the light on and off every 20 nanoseconds, or so. If we step through the program instead, we can see that the light turns on and off every time we write to `OUTDR`. Hurrah!
