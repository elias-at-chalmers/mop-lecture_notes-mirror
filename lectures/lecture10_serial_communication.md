# Serial Communication

From inter chip communication on PCBs, to tightly connected modules (bargraph, ascii-display), to devices in other rooms, to internet. 

although these have different protocols ranging from simple (ascii display) to extremely complicated (web page - several layers of protocols) the lowest level is always the same: we need to transfer a few bytes of data from one end of the line to the other. 

The things we have worked with so far have been parallel. Example ASCII display. This was common in consumer electronics in the eighties and nineties, but there are problems: 
* crosstalk
* cost
* space
* num pins

Therefore, most things today use serial communication. Send one bit at a time. This requires more work and faster electronics on both ends, but has now overtaken almost all. 

Parallel interfaces mainly remain where extreme speed is required (ddr5 memory) and distances are very short.

# Seding a byte of data
Example text terminal. 
Put the byte on data lines and flip en E signal to tell the reciever when the data is available. 

With serial communication, we instead send the byte bit by bit. 

Often implement with a "shift register", explain. 

Explain problem if the clocks are not exactly the same

Synchronous
How to deal with this? Separate clock signal? 

Simplest approach, very common. Will talk about some later (SPI i2c)

But the problem with interference and cost remains. 
Also, at very high speeds skew is a problem. Even if clock and data are sent simulteously from the cpu, it is hard to guarantee that they are in sync on the reciever end because of "Relative delay uncertainty between clock and data", jitter and noise. 

Asynchronous
Therefore, we often want to be able to transfer data on a single line. 

Explain the problem. Two devices try to transmit and recieve a stream of bits but fall out of sync because their clocks cannot be exactly the same. 
Depending on *how* different the clocks are, there is a given time until failure. 
If one device is exactly 1Mhz, and one is 1.001, the failure would be after 500 bits.
So if we knew that the frequencies are correct within 0.1% we know that we can send 500 bits without failure if they start at exactly the same time. 

We can make them start at exactly the same time by sending a "start bit" from the transmitter. The reciever has an edge detector that starts the clock exactly when it sees a transition from 0 to 1 on the line (you know how to implement this with interrupts!). Then it can recieve 500 bits, and then it has to wait for another startbit, and restart its clock. 

This is the basic principle behind how the UART sends and transmits data. 

Explain UART/RS232

Need to mention 
baud and bitrate
Half and full duplex


==================================================================================================
About this not being how high speed serial works
==================================================================================================
Although UART-style asynchronous serial communication is one of the earliest techniques for transmitting data without a separate clock line, it remains entirely appropriate when very high data rates are not required. Its simplicity and modest hardware demands make it well suited to embedded systems, and it is still widely used in GPS receivers, debug consoles, Bluetooth modules, and (most importantly to some of us) in the MIDI interface between keyboards and synthesisers.

Manchester coding is the entry to this. But synchronising clocks with PLLs is too complicated for this course. 

In contrast, USB (and other modern high-speed serial links) uses embedded clocking: the receiver continuously adjusts its sampling phase by observing transitions in the data stream. Instead of sampling at fixed time intervals after a single initial synchronisation event (as in UART), it continually corrects its timing so that each bit is sampled near its centre, keeping its clock aligned with the transmitter throughout the transmission.

The advantage of embedded clocking is that it allows far higher data rates because the receiver continually corrects its timing instead of relying on a fixed sampling schedule. The trade-off is increased circuit complexity, since the receiver must include a clock-recovery loop rather than a simple baud-rate timer.

(why do we teach them uart)
Although UART-style asynchronous serial communication is simple and comparatively slow, it embodies an important design principle: reliable communication with minimal hardware and modest clock accuracy. The same underlying problem — how to maintain timing between independent clocks — reappears in every modern high-speed link. USB, Ethernet, and PCIe solve it differently, using continuous clock recovery instead of per-frame synchronisation, but they are addressing the same fundamental challenge. Understanding UART therefore provides the conceptual foundation for understanding all serial communication, from simple microcontrollers to multi-gigabit systems.


# Programming the UART
Now you know about sending data asynchronously, and how frames work, you might be tempted to implement this over GPIO using interrupts. This is entirely possible with your knowledge of interrupts and SysTick timing, but you do not have to. Ever since this type of data transfer was implemented in hardware in the 1970s, microcontrollers have included dedicated UART peripherals that handle framing, timing, and buffering automatically, leaving the processor free to perform other tasks.

In fact, the ch32v307 contains 3 USART controllers each of which can be used to transmit asynchronous data in a variety of different ways. 

Register block overview

## Sending a byte of data

## Recieving a byte of data

## Using interupts to automate

# SPI and I2C

# Buses (CAN)
So far we have talked about point to point protocolls. 
Explain a situation where a bus is preferred, and mention other topologies. 
Briefly explain CAN. 
