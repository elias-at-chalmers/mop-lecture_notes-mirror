# Serial Communication

This lecture is about transfer of data between components. Such communication occurs across a wide range of scales in machine-oriented programming. A microprocessor may transmit a byte to a digital-to-analogue converter (DAC) located on the same printed circuit board (PCB) via copper traces on the board. A byte may be conveyed through general-purpose input/output (GPIO) pins to a bargraph or hexadecimal display. A longer cable may connect a GPIO port to a terminal situated in another room. At a broader scale, the microcontroller (MCU) may be connected to the Internet.

<center>
<img src="../images/communication.png" width="100%">
</center>

The protocols and transmission media employed in these scenarios can differ substantially: ranging from a single conductor connecting a GPIO pin to a light-emitting diode (LED), to multiple parallel conductors with a simple protocol for communicating with an ASCII display, to retrieving a web page, which requires several layers of protocols and heterogeneous transmission media. 

At the lowest level of abstraction, however, the fundamental problem remains the same: a sequence of bytes must be transferred over a physical medium from one device to another.

## Parallel Communication
So far in this course, only examples of *parallel* communication have been considered. When transmitting data to the bargraph or the ASCII display, the entire byte is presented simultaneously, with one signal line per bit. This is arguably the simplest conceptual approach; however, it entails several practical limitations:

* Adjacent parallel lines may interfere with one another, producing signal degradation through *crosstalk*.
* Over longer distances, providing a large number of parallel conductors becomes costly.
* On a printed circuit board (PCB) or within a cable assembly, additional conductors require more routing space or result in thicker, bulkier cables.
* The microprocessor has a finite number of general-purpose input/output (GPIO) pins.

During the 1980s and 1990s, consumer electronics frequently employed parallel interfaces[^footnote_printer_cable]. For the reasons outlined above, however, and for additional considerations related to scalability and signal integrity, most communication at nearly all scales is now implemented using *serial* protocols. In serial communication, data are transmitted sequentially over a single channel, one bit at a time. Although this approach demands more sophisticated timing control and higher signalling rates at both ends of the link, it is generally more economical, more scalable, and more robust in practice.

Parallel interfaces remain relevant in applications requiring extremely high data throughput over very short distances, such as modern synchronous dynamic random-access memory (e.g. DDR5).

[^footnote_printer_cable]: Legacy parallel printer cables typically contained on the order of 25–36 conductors, depending on implementation. By contrast, a standard USB 2.0 cable requires only four signal conductors.


# Sending a byte of data
Let us use the gorgeous old text terminal in the image above as an example. These devices were common in earlier computing systems. Their only job was to receive bytes encoding ASCII characters for display on the screen, and to capture the keys pressed by the user, transmitting the corresponding ASCII codes back to the host computer.

Because terminals could be located in separate rooms or even in different buildings, using a parallel interface like the one we connect the ASCII display with was not a realistic option. Instead, communication was implemented using serial transmission. Data in each direction were conveyed over a single signal conductor, so the cable required two signal lines: one for transmission and one for reception[^footnote_common_ground].
This configuration is called *full duplex* communication, since both ends can transmit simultaneously on separate lines. 

If one wanted really cheap cables, one could instead use *half-duplex* communication, where the two devices take turns transmitting and share the same signal line. In that case, only one side may drive the line at any given time.

[^footnote_common_ground]: In practice, a third conductor was typically included to provide a common ground reference between the devices.

<center>
<img src="../images/shift_register.png" width="80%">
</center>

To transmit data, the transmitter places the bits on the wire sequentially at an agreed signalling rate. The receiver samples the wire at the same rate. In practice, this is often implemented using a *shift register*. On the transmitter side, the register is loaded with a byte and, at each clock pulse, the register shifts the next bit onto the line. At the receiving end, a corresponding register shifts the incoming bits into storage. If both devices share the same clock signal, this arrangement functions correctly.

Herein lies a difficulty, however. It is not realistic to assume that two independent devices will have exactly identical clock frequency and phase. Even if both are configured to operate at 1 MHz, small tolerances ensure that one clock will be slightly faster or slower than the other. Over time, this frequency mismatch accumulates as phase error, and the shift registers will gradually drift out of synchronisation.

A straightforward solution is to transmit the clock signal over an additional wire. This is known as *synchronous* transmission and is widely used. SPI and I<sup>2</sup>C are examples of synchronous protocols that will be discussed later in the lecture. However, adding a clock line does not eliminate the earlier concerns regarding cost and interference. Furthermore, at very high signalling rates, *skew* becomes significant. At GHz frequencies, even if clock and data are launched simultaneously from the CPU, differences in propagation delay can prevent them from arriving in perfect alignment at the receiver.

## Asynchronous transmission

We would like to transmit data over a single wire without a shared clock signal. However, if two devices run on independent clocks, they will inevitably drift out of synchronisation.

Consider the diagram below. The blue and green traces represent two clocks that are *almost* the same frequency, but the green clock runs slightly faster. The transmitting device places a new bit on the wire at each clock pulse. Initially, the receiving device samples the line just before the next bit transition, and everything works correctly. After some time (see the arrows), the accumulated drift causes the sampling instant to move into the neighbouring bit period — and the receiver misinterprets a bit.

<center>
<img src="../images/clocks.png" width="80%">
</center>

If we knew the *exact* clock frequencies of the two devices (we never do, but let us pretend), we could calculate how many bits can be transmitted before an error occurs. For example, if one device runs at exactly 1 MHz and the other at exactly 1.001 MHz (a 0.1% mismatch), sampling will drift by half a bit period after approximately 500 bits. Thus, with a 0.1% frequency difference and perfect initial alignment, roughly 500 bits can be transmitted before failure.

To ensure correct initial alignment, the transmitter sends a *start bit*. The receiver detects the high-to-low transition (idle is logic 1) and uses that edge to align its internal timing, for example by resetting a counter or starting a timer interrupt (which you already know how to implement). From that point, it samples subsequent bits at regular intervals. After a fixed number of bits, it waits for the next start bit and re-synchronises.

This is the fundamental principle behind the *Universal Asynchronous Receiver/Transmitter* (UART). A UART is a standard hardware block found in most microcontrollers that implements this start–stop asynchronous protocol.

<center>
<img src="../images/frame.png" width="80%">
</center>

The UART transmits data bytes encapsulated in a *frame*, as shown above. A frame begins with a start bit (logic 0). The receiver detects this transition and begins sampling the incoming bits. It then receives eight data bits — the byte to be transmitted.

Optionally, a *parity bit* may follow. For even parity, the transmitter computes the XOR of all data bits (`bit0 XOR bit1 XOR ... XOR bit7`) and transmits the result. The receiver performs the same calculation and compares it with the received parity bit. If the values differ, a transmission error is detected. Parity cannot guarantee correctness, but it reliably detects any single-bit error.

Finally, the transmitter sends one stop bit (logic 1), returning the line to its idle state. The receiver then waits for the next start bit. (Some configurations use two stop bits, but 8N1 uses one.)

Thus, to transmit 8 data bits with parity and one stop bit, the UART places 11 symbols on the line. The number of symbols per second is the *baud rate*. In a UART, one symbol corresponds to one bit on the wire, so the baud rate equals the raw line bit rate. The useful payload rate is lower because of start, parity, and stop bits.

A common UART setting is 115200 baud. With 8 data bits, even parity, and one stop bit (8E1), the effective data rate is:

115200 × 8 / 11 ≈ 83,782 bit/s.

You might wonder why frames are typically only 8 bits long. Longer frames would reduce overhead from start, parity, and stop bits. However, the longer the frame, the more clock drift accumulates before re-synchronisation. An 8-bit frame gives a combined clock mismatch tolerance of roughly 5–6% (about ±2–3% per device), which aligns well with what inexpensive oscillators in typical UART systems can achieve.

Although UART-style asynchronous serial communication is one of the earliest techniques for transmitting data without a separate clock line, it remains entirely appropriate when very high data rates are not required. Its simplicity and modest hardware demands make it well suited to embedded systems, and it is still widely used in GPS receivers, debug consoles, Bluetooth modules, and (most importantly to some of us) in the MIDI interface between keyboards and synthesisers.

## Embedded clock signal
In contrast, USB (and other modern high-speed serial links) uses *embedded* clocking: the receiver continuously adjusts its sampling phase by observing transitions in the data stream. Instead of sampling at fixed time intervals after a single initial synchronisation event (as in UART), it continually corrects its timing so that each bit is sampled near its centre, keeping its clock aligned with the transmitter throughout the transmission.

The advantage of embedded clocking is that it allows far higher data rates because the receiver continually corrects its timing instead of relying on a fixed sampling schedule. The trade-off is increased circuit complexity, since the receiver must include a clock-recovery loop rather than a simple baud-rate timer.

We will not dive deeper into embedded clock signals in this course. 

# Programming the UART
Now you know about sending data asynchronously, and how frames work, you might be tempted to implement this over GPIO using interrupts. This is entirely possible with your knowledge of interrupts and SysTick timing, but you do not have to. Ever since this type of data transfer was implemented in hardware in the 1970s, microcontrollers have included dedicated UART peripherals that handle framing, timing, and buffering automatically, leaving the processor free to perform other tasks.

In fact, the ch32v307 contains 3 USART controllers each of which can be used to transmit asynchronous data in a variety of different ways. The register block for each USART looks like this in the quickguide: 

{{python quickguide-generator/main.py overview-table USART1 -no-grouping}}

The USART can communicate using a lot of different protocols, and all of these registers will not be relevant to us. 

## Sending a byte of data
Before we start any transmission, we need to configure the USART so that it knows what protocol to use and at what speed to transmit. We start by setting the UE, M, PCE, and TE bits in the `CTLR1` register. This turns the USART on (UE), sets up one parity bit (M, PCE), and enables the transmitter part of the USART (TE).

<div class = "boxed">
{{python quickguide-generator/main.py register-details USART1 CTLR1 -no-folding}}
</div>

Then we need to set the speed of the transmission. Since the USART block itself does not know what the clock frequency of our microcontroller is, we have to do this by telling it what to divide the clock frequency with, to get the speed we want. We need to give it the value `USARTDIV` which is given by: 

USARTDIV = f<sub>PCLK</sub> / (16 × baudrate)

If we want a baudrate of 115200 bits/sec, and out clock frequency is 144MHz, USARTDIV = 78.125.
We write the mantissa and fraction into two fields in the  Buad Rate Register (`BRR`): 

<div class = "boxed">
{{python quickguide-generator/main.py register-details USART1 BRR -no-folding}}
</div>

Now, when we want to send a byte of data, we simply write that byte into the Data Register (`DATAR`). The USART will immediately start shifting the bits out, one-by-one, on the GPIOA[9] pin (which pin to use is also configurable, but we will not go into that here).

Our code should then wait until the `TXE` bit in the Status Register (`STATR`) is set. This means that the USART har shifted out all of the bits, and can accept a new byte of data. 

{{python quickguide-generator/main.py register-details USART1 STATR -no-folding}}


## Recieving a byte of data
On the reciever end, our program will look similar. We need to configure the USAR in the same way, but enabling the reciever part of the circuit intead, in the `CTLR1` register. When the byte arrives (on the GPIOA[10] pin), the USART will automatically shift the bits into the Data Register (`DATAR`). We can read the `RXNE` bit to see when the whole byte has arrived, and can then read the data register. 

To send several bytes from one MD307 to another, the transmitting MD307 could lie in a loop and feed a new byte into the data register every time the `TXE` byte is set. The recieving MD307 would also lie in a loop and read the Data register every time the `RXNE` bit was set. 

Since this would occupy both machines completely, this is usually implemented using interrupts instead. The transmitting MD307 would have a buffer of data that it wanted to send, and (by setting the `TXIE` bit in the `CTLR1` register) would recieve an interrupt every-time the USART was ready to transmit another byte. Similarly, the recieving MD307 program would do whatever it was doing and when a new byte was ready to be recieved, an interupt handler would put that into a buffer. 

# Other transmission protocols on the MD307
Write a paragraph each about SPI, I2C and CAN.

