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

On the right side, we illustrate a simple push button, of the kind you can buy from any [electronics outlet](https://www.electrokit.com/tryckknapp-12.2mm-1-pol-off-onrod). The button has cables connected to two terminals and when you push the button down, these come into contact and a current can run through the circuit. 

We have connected one of these cables to the V<sub>dd</sub> (3.3V) pin, and the other to pin 5 (which is configured as an input pin). Thus, when the button is pushed down, we have 3.3V at the pin, and we can read `1` in the corresponding bit in the `INDR` register. But what happens when the button is released (and we are back to the situation in the image)? 

We would like be certain that if we read the bit when the button is released, the answer should be `0`. But actually, the pin is not connected to *anything* now, so the actual potential at the pin is unknown. The pin is said to be *floating*, and if we read it we might get a 0, or a 1. 

The standard way to solve this problem is to add a small "Pull-Down Resistor", as in the image below: 
<p align="center">
  <img src="images/simple_pull_down.png" alt="My image" width="75%" />
</p>

Now, if the button is released (as in the image), pin 5 is still connected to ground (through the resistor), so the potential is 0V. If we read the bit it will be `0`. We say that this new connection "*pulls* the floating signal *down* to ground". If the button is pushed down, pin 5 is directly connected to 3.3V, as before, so if we read the bit it will be `1`. 

Because the resistor connects 3.3V to ground when the button is pressed, a small *leakage current* will run through this connection, and some energy will be lost. If we use a resistor with a high resistance, the current will be very small, and the energy loss will be negligible. 

Whether we need this pull down resistor or not depends entirely on what we have connected. We could, for instance, connect the same button but connect one of the terminals (the blue cable) to `GND` instead of V<sub>dd</sub>. In that case, we know that we will get a 0 when the button is pressed, but to avoid a floating value when the button is released, we need to connect a "Pull-Up Resistor", to 3.3V. This is illustrated in the left image below. We say that "the floating signal is *pulled* *up* to 3.3V".

<p align="center">
  <img src="images/pull-up.png" alt="My image" width="45%" />
  <img src="images/no-pull.png" alt="My image" width="45%" />
</p>

A push button is a *Passive Component*. In the right image above, we have connected an *Active Component* (Perhaps and OR gate, or a DAC). This chip will put *either* 0 or 3.3V on its output pin (it is never floating), and then we don't need any pull-up or pull-down resistors. 

Since it is so common to connect passive components of various kinds to the GPIO pins, most MCUs have pull-up and -down resistors built into the chip. There is one of each, for each pin on the GPIO ports. These can be activated or de-activated by writing to the configuration registers (as we will see soon). By activating only the internal pull-down resistor, we can connect a push button with only two cables and still be certain to read either `0` or `1`, depending on the buttons state. 

<p align="center">
  <img src="images/built-in-pull.png" alt="My image" width="75%" />
</p>



## Digital Output

Push-Pull: 
Open Drain: Simple example is connect