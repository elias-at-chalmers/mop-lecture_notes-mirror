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

Since it is so common to connect passive components of various kinds to the GPIO pins, most MCUs have pull-up and -down resistors built into the chip. On the CH32V307, this is implemented as in the image below. When the pin is configured as an input pin, we can activate a pull-up/down resistor. This resistor is connected to the corresponding *output* bit, of the `OUTDR` register.

This might seem confusing at first. We have seen that the `OUTDR` register is used when the pin is configured as an *output* pin, and that the value of the bit sets the voltage of the pin. This is cleverly reused here. When the pin is configured as an input pin, the `OUTDR` register is instead used as a selector, that decides whether the resistor should pull up or down. This is equivalent to physically connecting a resistor to either GND or V<sub>dd</sub>, as we did before.

So, if we need a pull-down resistor for the button connected to pin2 (as in the image) we activate pull up/down, and we set bit 2 of the `OUTDR` register to 0 (GND). This will, exactly as before, ensure that a floating input is "pulled down" to 0V.

<p align="center">
  <img src="images/built-in-pull.png" alt="My image" width="75%" />
</p>

Now, let's revisit relevant parts from the [Quickguide](LINK) to see how we configure our pin:

<div class="boxed">

{{include quickguide/gpio-cfg.html}}

{{include quickguide/gpio-cfg-regs.html}}

</div>

We want to plug in a button as in the image above, so we want to configure pin2 as an input pin and activate a *pull-down* resistor on this pin. To configure the pin as input, we only need to set `MODE` to `00`.

To activate the pull up/down resistor, we set `CNF` to `10`. The alternative configurations for an input pin are: *Analog* (00) -- Used when the input voltage varies anywhere between 0 and 3.3V and we want to read its value. You might want to try this out towards the end of the course, but we leave it for now, *Floating* (10) -- this is when we don't want any pull up/down resistor connected, *Pull-up/Pull-down* (10) -- Activates the resistor, and *Reserved* (11) -- Not used.


To choose pull *down*, rather than pull up, resistor, we set bit 2 in `OUTDR` to 0.
Once the GPIO port is configured, we can read the current status of the input pin by reading the corresponding bit (bit 2 in this case) of the `INDR` register.

In assembly, a program that loops until the button has been pushed could look like:

```
TODO
```

## Digital Output

In the last lecture, we turned on an LED light by writing to the `OUTDR` register, and to configure the output pin, we set `CFG` to 00, without going into why. We can see in the table above that the possible configurations an output pin can have are: *Push-Pull* (00), *Open Drain* (01), *Alternate Function, Push Pull* (10), or *Alternate Function, Open Drain*. The *alternate function* setting is used when we want the pin to work with something *other* than the GPIO module (for example, the USB module) and we will leave that for much later. But we still have to clarify what *Push-Pull* and *Open-Drain* means.

The name “push-pull” originally comes from amplifier design, where two active devices alternately “push” and “pull” current. In digital electronics, a push-pull output stage means the pin is actively driven both high (e.g. 3.3 V when outputting 1) and low (0 V when outputting 0). This is the standard behavior, and probably what you would have expected it to do. With this configuration we can for example make the LED glow, as we have seen.

The interesting question is why we would ever do something else? You will see later that there are many cases where this configuration could cause problems, but let us start with a simple example that illustrates the main problem:

**Example:** *We have a system where pins 3 and 5 will be set to zero if some error has occurred. We want to connect a red LED that warns us if either of these error-pins are zero:*
<p align="center">
  <img src="images/two_pins_one_led.PNG" alt="My image" width="75%" />
</p>

Now, consider what happens if we connect the LED as in the image above and configure pins 3 and 5 as push-pull outputs:

* If both bits are set to 1, no current flows through the LED (the anode and cathode are both at 3.3V), and it stays off.
* If both bits are set to 0, current flows from V<sub>DD</sub> through the LED to the pins, and the LED lights up.

*But if one pin outputs 1 and the other outputs 0*, we create a direct short: one pin is actively driving 3.3 V while the other is actively pulling to ground. This causes a large current to flow directly between the two pins instead of through the LED. This is very bad and might damage the transistors inside the microcontroller.

The solution to this problem is to configure the pins as "Open Drain" instead of "Push Pull". In an Open Drain configuration, writing `0` to the corresponding bit drives the pin to 0V, but writing `1` sets it in *floating* mode (it is as if it wasn't connected at all). Now: 

* If both bits are set to 1, it is as if we had removed the two green lines in the image. No current can flow, and the LED is off. 
* If both bits are set to 0, the pins are both set to 0V, and current flows from V<sub>DD</sub> through the LED to the pins, and the LED lights up.
* If one bit is set to 0 (pin is 0V) and the other is set to 1 (pin is floating) current still flows from V<sub>DD</sub> to the pin at 0V, and the LED lights up. 





Push-Pull:
Open Drain: Simple example is connecting several pins to one led.
BSHR
BCR

Keyboard!