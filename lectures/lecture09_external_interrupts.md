# External Interrupts
So far we have seen how to interrupt the program when a single, *internal*, interrupt occurs (SysTick counted to zero). In this lecture we will dive deeper into interrupt handling, and show how we can handle several simultaneous interrupts from modules outside the Qingke processor and finally from interrupts originating from the GPIO pins. 

## Configuring multiple interrupts
Recall from the previous lecture that every module that can cause an interrupt on the chip will have a hard-coded connection to the *Programmable Fast Interrupt Controller* (PFIC) module. When, for instance, the "Ethernet Wakeup" signal comes in from the ethernet cable, the ethernet module will signal the PFIC, and the PFIC will (if it has been configured to do so) interrupt the running code and make the processor run an interrupt handler. 

Every possible interrupt that can occur has an assigned *interrupt vector number* which you can find in the [QuickGuide](../quickguide/interrupt_vector_table.html).

<center>
<img src= "../images/interrupts.png" width=100%>
</center>
> <b>TODO</b> Update quickguide snippet with corret numbers...

## Basic Timers
> TODO: ** These will be covered, along with a struct interface, in lecture 07 instead **

<div class='boxed'>
<details open>
<summary>Base addresses</summary>

{{python quickguide-generator/main.py baseaddress ^TIM(6|7)}}

</details>

<details open>
<summary>
Register Block Overview
</summary>

{{python quickguide-generator/main.py overview-table -no-grouping TIM6}}

</details>

{{python quickguide-generator/main.py register-details -no-grouping TIM6 CTLR1}}

<!--
{{python quickguide-generator/main.py register-details -no-grouping TIM6 CTLR2}}

{{python quickguide-generator/main.py register-details -no-grouping TIM6 DMAINTENR}}

{{python quickguide-generator/main.py register-details -no-grouping TIM6 INTFR}}

{{python quickguide-generator/main.py register-details -no-grouping TIM6 SWEVGR}}

{{python quickguide-generator/main.py register-details -no-grouping TIM6 CNT}}

{{python quickguide-generator/main.py register-details -no-grouping TIM6 PSC}}

{{python quickguide-generator/main.py register-details -no-grouping TIM6 ATRLR}}

{{python quickguide-generator/main.py register-details -no-grouping TIM6 .*}} -->
</div>


Show an example where we start systick, timer6 and timer7 to blink three lights at different frequencies. 

```
// Set up timers at three frequencies
main() {
    SYSTICK * systick = 0x....; 
    BASIC_TIMER * timer6 = 0x40001000; 
    BASIC_TIMER * timer7 = 0x40001400; 
}
```

Set up PFIC for TIMER6 and 7 as well. 

Set up a single interrupt handler as in previous lecture
Explain that we now have to look at `mcause` to find out which timer started the interrupt

```
void SysTick_Handler() { flip_one_light }
void Timer6() { flip_another_light }
void Timer7() { flip_yet_another_light }

void Interrupt_Handler()
{
    check mcause; 
    call one of the handlers
}
```

Point out that this is potentially quite a lot of code to run through to flip a single bit, and that interrupt handlers have to be fast. 
Consider the case where one of the interrupts happen every few nanoseconds. 

## Vectored Interrupt Handling
Explain mode 2, and how we encode the mode in the last two bits of `mtvec`.

> image that shows the difference

Show how we can create the vector table in an assembly file

### The WCH Mode
Just briefly, with an image. Possibly "advanced". 

## Nested Interupts

Talk, quite briefly, about what happens when an interrupt with a higher priority interrupts. 
Talk about pending and active interrupts and how we can read this in registers. 


## Interupts from GPIO pins

We can use as an example that we want to be able to flip a fourth LED with a dipswitch. 
