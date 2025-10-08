# Basics of GPIO and C

**Links:**
**Text and excercises in the Workbook (Arbetsboken)**
**Things that are in the Workbook that should possibly be in this lecture**

In the previous lecture, we learned how to control the voltage on a GPIO pin to switch an LED on or off. We also saw how to configure a pin as either an output or an input, but we glossed over a few important details.

When we write software for regular computers, we can usually live in blissful ignorance of the fact that all those ones and zeros flying around are really electrical signals. But when we start connecting real devices to microcontrollers, that abstraction breaks down. To make our circuits work properly (and preferably not burn) we need to understand a few fundamental electrical concepts, which require us to think a bit when configuring our input and output ports.

In the second half of the lecture, we will continue with some basic C programming concepts. 


## Digital Input

We saw in the previous lecture that the physical pins on the connector are connected to a register `OUTDR` and that, if properly configured, each pin will get a voltage of 0 or 3.3V depending on whether the corresponding bit in the `OUTDR` register is 0 or 1. By setting the bits differently in the `CFGLR/HR` register, we can instead connect each pin with the `INDR` register [^1]. In this mode, the function is reversed. The bits in the `INDR` register will be loaded with `1` if the physical pin has a voltage of 3.3V connected, and `0` if the pin is at 0V. 

[^1]: Of course, in reality, both registers are physically connected to the pin all the time, but the connection can be switched by a transistor. 

<p align="center">
  <img src="images/in_out_switch.png" alt="My image" width="75%" />
</p>




### Pull Up and Pull Down

The simplest input device we could connect is arguably a single button, so let's start there. How does a button work? How can we connect it to our computer? This image suggests a pretty good start: 

<p align="center">
  <img src="images/simple_button.png" alt="My image" width="75%" />
</p>

On the right side, we illustrate a simple push button, of the kind you can buy from any [electronics outlet](https://www.electrokit.com/tryckknapp-12.2mm-1-pol-off-onrod). The button has two cables connected to it and when you push the button down, these are connected and a current can run through the circuit. 

We have connected one of these cables to the V<sub>dd</sub> (3.3V) pin, and the other to pin 5 (which is configured as an input pin). Thus, when the button is pushed down, we have 3.3V at the pin, and we can read `1` in the corresponding bit in the `INDR` register. But what happens when the button is released (and we are back to the situation in the image)? 

We would like be certain that if we read the bit when the button is released, the answer should be `0`. But actually, the pin is not connected to *anything* now, so the actual potential at the pin is unknown. The pin is said to be *floating*. 

The standard way to solve this problem is to add a small "Pull-Down Resistor": 
<p align="center">
  <img src="images/simple_pull_down.png" alt="My image" width="75%" />
</p>

Now, if the button is released (as in the image), pin 5 is connected to ground, so the potential is 0V. If we read the bit it will be `0`. If the button is pushed down, pin 5 is directly connected to 3.3V, so if we read the bit it will be `1`. 

Because the resistor connects 3.3V to ground when the button is pressed, a small *leakage current* will run through this connection, and some energy will be lost.If we use a resistor with a high resistance, the current will be very small, and the energy loss will be negligible. 

## Digital Output

Push-Pull: 
Open Drain: Simple example is connect