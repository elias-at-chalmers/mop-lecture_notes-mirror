Machine Oriented Programming
===============================================================================

Welcome to this course in *Machine Oriented Programming*!
In a previous course, you have seen how simple logic gates can be combined to create an ALU, a data path, and finally a complete working tiny processor. In this course you will build on this knowledge and learn how to program a **real**, modern, off-the-shelf *microcontroller* [^microcontroller]. You will develop programs for this device using assembly and C and you will learn how to communicate with peripheral devices (lights, buttons, displays and a tiny speaker). As illustrated in the image below, the course is also a prerequisite for several upcoming courses.

[^microcontroller]: Read: "small processor" for now.

![](../images/courses.png)

Many of you are probably accustomed to thinking of a microprocessor as one of the power-hungry beasts that control your desktop computer, your laptop, or your mobile phone. However, these account for only a *tiny fraction* of the processors you use every day. In fact, a modern mobile phone typically contains on the order of a hundred small, specialized processors and programmable controllers, responsible for tasks such as display control, radio communication, and power management.

In your immediate environment, microcontrollers are embedded in refrigerators and washing machines regulating temperature, motors, and safety interlocks; in microwave ovens and stoves controlling power delivery; in thermostats, smoke detectors, and ventilation systems maintaining indoor climate and safety; in cars and bicycles managing braking, lighting, and battery charging; and in lifts, doors, and access systems enforcing timing and safety constraints. Even seemingly inert objects such as power adapters, LED light bulbs, battery packs, and electric toothbrushes contain programmable controllers executing code continuously. In most cases these systems are designed to be invisible, reliable, and single-purpose. In short there are several *billion* microcontrollers being produced every year and all of these little processors need someone to write programs for them. 

If you do not find washing machine temperature regulation very thrilling (yet...) it is difficult not to be excited by the thousands of hobby projects that people around the world are doing using their newfound microcontroller programming skills. Have a look at [these](https://www.seeedstudio.com/blog/2021/03/26/10-raspberry-pi-pico-projects/) [sites](https://projecthub.arduino.cc/) and ask yourself if *you* wouldn't like building your own automatic plant watering station, guitar effect pedal, autonomous drone, or portable video game.

Even if you have decided, once and for all, that you only want to work with the most powerful general-purpose CPUs available, and have little interest in their smaller counterparts, don't worry. The RISC-V processor you will learn about in this course is exactly the same as the fundamental core of an multi-core 64-bit processor, and the concepts and techniques that you learn here are directly applicable (and in fact essential) to writing secure, high-performance, code for modern desktops and servers. 


What is a microcontroller?
===============================================================================




Let's say we want to build a simple thermometer that measures the temperature in the room and displays it in a readable form. We would first have to buy ourselves a [*temperature sensor*](https://se.rs-online.com/web/p/temperature-humidity-sensor-ics/5335878?gb=s) of some kind, and a [suitable display](https://se.rs-online.com/web/p/led-displays/8305139?gb=s). The temperature sensor will output the measured temperature as a _voltage_ on one pin.  The display will have one pin per LED-segment, so that the segment lights up if the corresponding pin is \ref{fig:fig1} monkey. 

To connect the two components, we would have to buy an *Analogue to Digital Converter* (ADC) that could turn the measured voltage into a binary number, and a *Decoder* chip that turns that number into a corresponding combination of 1's and 0's for the display (see Figure @fig:fig1.

![Figure 1: A 1970's style digital thermometer](images/test.png){#fig:fig1}

The Decoder chip is an example of an *Application Specific Integrated Circuit* (ASIC), and you can buy this particular chip off the shelf from any electronics retailer. But now let's imagine that we want our thermometer to have more functionality. If the temperature is more than 37 degrees, we want an alarm to go off and a red light to start blinking. 

We *could* implement this behaviour using more ICs, but adding more ICs means paying more money, and spending more time connecting cables between different components. As soon as we need anything but the most standard behavior it can be a good idea to instead invest in a *microcontroller*.

*Microcontrollers are used in everything* 

*Different kinds of microcontrollers*

*Development boards (PCB, IC)*

> Consider starting earlier, with an example of a simple sensor, connected to a simple display. 
> We can explain that these are ICs found on a PCB. 
> Now say that we want our thing to do something more complicated, enter the MCU. 

*Also clarify that a microprocessor is usually what we find in a computer, and that it usually incorporates cache hierarchies and MMUs, but expects peripherals like memory and USB controllers to be outside.*

The lab computer, MD307
===============================================================================
*MD307 is a development board. It hosts a CH32V307 microcontroller, two USB ports (one for debugging), and a number of GPIO pins.*

The microcontroller
-------------------------------------------------------------------------------
[CH32V307](https://www.wch-ic.com/products/CH32V307.html)
Developed by WCH

*The microcontroller is the chip in the image. Everything that happens on the board is initiated by an electrical signal coming into, or out of, one of the little spider legs."
*The microcontroller is not just a processor (also SRAM/EXTI/USB/USART/etc.)*

The processor: RISC-V
-------------------------------------------------------------------------------
[QuingKe V4 Processor Manual](https://www.wch-ic.com/downloads/file/367.html?time=2024-05-30%2013:33:33&code=rH4Ri4yAJQE2UN9yMb7X6nzd5fwIOYLYDEoPSX0O)
Developed by WCH

> - ISA: RV32IMAC[F]
> - Pipeline: Level 3
> - FPU: Supports single-precision floating-point
> - Branch prediction: BHT/BTB/RAS
> - Instruction cache: Support up to 64KB
> - Interrupt: Supports a total of 256 interrupts including exceptions, and supports VTF
> - HPE: Supports up to 3 levels of HPE (HPE refers to a hardware-accelerated mechanism for automatically handling the saving and restoring of register states when entering or exiting an interrupt service routine (ISR) or a function call. This is particularly important in > real-time or embedded systems where performance and low-latency interrupt handling are critical.)
> - PMP: Supports 4 memory protection zones
> - Low-power consumption mode: Supports Sleep and Deep sleep modes, and support WFI and WFE sleep methods
> - Extended instruction set: Supports half-word and byte operation compression instructions
> - Debug: 1/2-wire SDI, standard RISC-V debug 

*Talk about RISC-V in general and Quinkewhatever in particular*
*The CPU, nvic, memory bus, fpu, mpu, debugability*

The MD307 memory and address space
-------------------------------------------------------------------------------


Creating a program that runs on the MD307
-------------------------------------------------------------------------------
*Very briefly how a program ends up in memory and starts running. Use an example that flashes a LED or something*


Course organization
===============================================================================




