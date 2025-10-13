<p>The General Purpose Input Output module consists of five identical register blocks, one for each GPIO port</p>

<p>
<b> Base addresses: </b>

{{python quickguide-generator/main.py baseaddress GPIO*}}

</p>

<p>
<b> Register Block Overview </b>

{{python quickguide-generator/main.py overview-table GPIOA}}

</p>

<p>
Configuration of the port is done using the <code>CFGLR</code> and <code>CFGHR</code> for the lower eight and upper eight pins, respectively. 
For each pin, there are two bits to set the MODE (Input or Output, and speed of output pins), and two bits to set the CoNFiguration (the meaning of these pins depend on whether the pin is an input pin or an output pin):
<table style="width:100%; table-layout:auto">
  <tr>
    <th colspan="2">MODE</th>
    <th colspan="2">CNF</th>
  </tr>
  <tr>
    <td rowspan="4">00</td>
    <td rowspan="4">Input Mode</td>
    <td>00</td>
    <td>Analog</td>
  </tr>
  <tr>
    <td>01</td>
    <td>Floating</td>
  </tr>
  <tr>
    <td>10</td>
    <td>Pull-up/Pull-down</td>
  </tr>
  <tr>
    <td>11</td>
    <td>Reserved</td>
  </tr>
  <tr>
    <td rowspan="4", colspan="2">
      01 - Output Mode 10Mhz <br>
      10 - Output Mode 2Mhz <br>
      11 - Output Mode 50Mhz
    </td>
    <td>00</td>
    <td>Push Pull</td>
  <tr>
    <td> 01 </td>
    <td>Open Drain</td>
  </tr>
  <tr>
    <td> 10 </td>
    <td>Alternate Function, Push Pull</td>
  </tr>
  <tr>
    <td>11</td>
    <td>Alternate Function, Open Drain</td>
  </tr>

</table>
</p>


<p>
<hr>

{{python quickguide-generator/main.py register-details GPIOA .*}}

</p>


