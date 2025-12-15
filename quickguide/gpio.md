<p>The General Purpose Input Output module consists of five identical register blocks, one for each GPIO port</p>

<p>
<details>
<summary>Base addresses</summary>

{{python quickguide-generator/main.py baseaddress GPIO*}}

</details>

<details open>
<summary>
Register Block Overview
</summary>

{{python quickguide-generator/main.py overview-table GPIOA}}

<p>
Configuration of the port is done using the <code>CFGLR</code> and <code>CFGHR</code> for the lower eight and upper eight pins, respectively.
For each pin, there are two bits to set the MODE (Input or Output, and speed of output pins), and two bits to set the CoNFiguration (the meaning of these pins depend on whether the pin is an input pin or an output pin):

{{include quickguide/gpio-cfg.html}}

</p>
</details>


<p>
<hr>

{{python quickguide-generator/main.py register-details GPIOA .*}}

</p>


