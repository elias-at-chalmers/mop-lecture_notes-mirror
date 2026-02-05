# ASCII Display
The PTB-111 development board is equipped with a 2-row LCD display. This page is a summary of how to communicate with it. 
The LCD display has three control signals, and 8 data signals: 

| Signal | Description | Pin |
|--------|-------------|-----|
|**Register Select (RS)** | This signal is used to tell the display whether it’s a command or data that we are sending/receiving. | 0 |
|**Read/Write (R/W)** | This signal is used to tell the display whether we want to send data to it, or receive data from it.| 1 |
|**Enable (E)**| This signal is used to synchronize communication with the display| 2 |
|**Data Lines (DB0-DB7)**| These 8 lines contain the command or data that we want to send (one byte).| 8-15 |

On the PTB-111, the control signals are connected to the first three pins of an 8-bit port that is in turn connected to the upper or lower byte of one of the GPIO ports on the MD307 with a gray flat cable. The data lines are connected to another port. For instance, control signals might be connected to `PE[0..2]` and data lines to `PE[8..15]`.

## Writing a command or data to the display
Before any command or data is written to the display, you must first ensure that the display is not busy with a previous command. This is done by reading the status bit, as explained in the next section. 

Whether you are writing a command or a character to the display, you must follow the procedure and timings specified in the diagram for a write cycle, below: 

![](../images/ascii_display_write_cycle.png)

The possible commands (RS = 0, R/W = 0) are given in this table: 

<table style="font-size: 0.85em;">
  <thead>
    <tr>
      <th>Command</th>
      <th>D7</th>
      <th>D6</th>
      <th>D5</th>
      <th>D4</th>
      <th>D3</th>
      <th>D2</th>
      <th>D1</th>
      <th>D0</th>
      <th>Description</th>
      <th>Execution time</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Clear display</td>
      <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>1</td>
      <td>Clears the display</td>
      <td>1.53 ms</td>
    </tr>
    <tr>
      <td>Return Home</td>
      <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>1</td><td>-</td>
      <td>Reset cursor</td>
      <td>1.53 ms</td>
    </tr>
    <tr>
      <td>Entry mode set</td>
      <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>1</td><td>ID</td><td>SH</td>
      <td>ID: Cursor direction<br>(0 left, 1 right)</td>
      <td>39 µs</td>
    </tr>
    <tr>
      <td></td>
      <td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td>
      <td>SH: Shift (0 off, 1 on)</td>
      <td></td>
    </tr>
    <tr>
      <td>Display control</td>
      <td>0</td><td>0</td><td>0</td><td>0</td><td>1</td><td>D</td><td>C</td><td>B</td>
      <td>
        Display (D), Cursor (C), Blink (B)<br>
        (0 off, 1 on)
      </td>
      <td>39 µs</td>
    </tr>
    <tr>
      <td>Function set</td>
      <td>0</td><td>0</td><td>1</td><td>1</td><td>N</td><td>F</td><td>-</td><td>-</td>
      <td>N = number of rows − 1</td>
      <td>39 µs</td>
    </tr>
    <tr>
      <td></td>
      <td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td>
      <td>F: Character size<br>(0: 5×8, 1: 5×11)</td>
      <td></td>
    </tr>
    <tr>
      <td>Set address</td>
      <td>1</td><td>A6</td><td>A5</td><td>A4</td><td>A3</td><td>A2</td><td>A1</td><td>A0</td>
      <td>Set address for next character</td>
      <td>39 µs</td>
    </tr>
  </tbody>
</table>

When writing data (a character) to the device (RS = 1, R/W = 0), the data bits contain the character to write in ASCII code. Writing data has an execution time of 43 µs.

*Execution time* represents the time it takes after issuing the command until
the action is guaranteed to be finished.

## Reading status or data from the display
When reading the display status (busy flag and current address), or data in display memory, you must follow the procedure and timings specified in the diagram for a read cycle, below: 

![](../images/ascii_display_read_cycle.png)

When reading status (RS = 0, R/W = 1), `D7` is the Busy Flag. Remaining data bits are the current address. Reading status has an execution time of 0 µs.

When reading data (RS = 1, R/W = 1), the data bits are the character read at the current address. Reading data has an execution time of 43 µs.


