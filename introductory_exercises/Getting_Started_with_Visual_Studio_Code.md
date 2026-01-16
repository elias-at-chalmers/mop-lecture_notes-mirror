# Getting Started with Visual Studio Code
These are the instructions for setting up the programming environment you will use throughout the course.

* Install Visual Studio Code from [https://code.visualstudio.com/](https://code.visualstudio.com/).
* Launch Visual Studio Code.
* Bring up the command palette by pressing CTRL+SHIFT+P (CMD+SHIFT+P on Mac). Type and select "Extensions: Install Extensions"

<p align="center">
  <img src="../images/getting_started_with_vs_code/image1.png" alt="My image" width="80%"/>
</p>
* Search for "MDx07 Templates" and install it.
<p align="center">
  <img src="../images/getting_started_with_vs_code/image2.png" alt="My image" width="80%"/>
</p>

* Create a folder where you want your project code to reside. 
  * **Note**: It is easiest to avoid using a path that contains non ascii characters. If you really want to use such a path, see notes below. 
* Go the top menu and select "File->Open Folder..." and open this (empty) folder.
* **Install development tools**: Bring up the command palette again (CTRL+SHIFT+P). Select "MDx07: Install MD307 Development Tools".
  * This will take a little while, and you should have at least 2GB of free space on your system harddisk.
* **Create your project**: Once again bring up the command palette and select "MDx07: Initialize project". Then select "Basic Templates" and then "MD307 Assembly Project".
* **Start the MD307 simulator**: In the command palette, select "MDx07: Launch Simserver". The Simserver window will now open. 
  * The first time you start simserver, choose "Server->Set Target..." from the menu, and select MD307
  * In the Simserver window, select "Server->IO Setup". 
    * Under "IO-Ports", select "Serial Communication Interface 1" and then select "06 Console" under "IO-Devices".
<p align="center">
  <img src="../images/getting_started_with_vs_code/image3.png" alt="My image" width="80%"/>
</p>
    * Press the "Connect" button and then the "OK" button.
    * You should now see a little console window, like this: 
    
<p align="center">
  <img src="../images/getting_started_with_vs_code/image4.png" alt="My image" width="70%"/>
</p>

* **Run a little program to make sure everything works**: 

   * Go back to Back to Visual Studio Code and press F5 ("Start Debugging") and wait until the little yellow arrow appears at the first instruction.
   * Then, step through the instructions with F11 ("Step Into") until you get a greeting on the console window.

If you have any problems, go to one of the exercise sessions and ask a TA to help you. Otherwise, you are done and can start the course!

## Turn off your AI "friend"

*If* you have GitHub copilot installed, it will immediately start "helping" you by suggesting the next instruction for you. This can be extremely useful for you in the future, when you know assembly programming well, but is disastrous for learning purposes. So (for your own sake) turn it off by unchecking the boxes in the image below.

<p align="center">
  <img src="../images/turn_off_ai.png" alt="My image" width="50%"/>
</p>

## If you have 'åäö' in your project path on Windows:

If you have any non-ascii-character like 'åäö' in your username, your "Documents" or "Desktop" paths will have that as well. This may cause problems in VSCode and GDB. You can circumvent the problem by activating support for UTF-8 in your paths (Or just put your code in another directory, like "C:\MOP\").

  1. Rightclick in your start-menu → Run
  2. Run intl.cpl
  3. Choose the tab Administrative.
  4. Choose Change system locale...
  5. Mark Beta: Use Unicode UTF-8 for worldwide language support.
  6. Press Ok and restart the computer.
