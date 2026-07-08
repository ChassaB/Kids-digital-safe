
<img width="4032" height="3024" alt="IMG_0906" src="https://github.com/user-attachments/assets/1ecc7dd0-73dc-4a0e-9d66-ec04058627bd" />


**Description**
This project converts a small electronic digital safe into a touchscreen safe for children using a 3.5”/4” Cheap Yellow Display (CYD) and an ESP32. The CYD is usually a Sunton ESP32-3248S035.

The standard numeric keypad is replaced by a picture-based code, allowing young children to unlock the safe by touching a sequence of icons rather than entering numbers.

A bmp image is used for the opening flash screen because it is very large and this saves decoding
The images/icons used have either been created by chatGPT, me, or from free images on the internet (to the bext of my knowledge)

THe unlock code is a pattern of x shapes in the correct order, set up in the arrays that trigger the open_safe function.

You can use any reasonable value for x and create any pattern of touches that suits.
You can make the code into a saying for kids to remember. EG "A green frog and a red cat were chased by a yellow dog"
You can, of course, make your own icons instead of mine

**Power management**
Given the project is battery driven (cheap 5000mAh power pack (KMart) with charge level display, in safe) power is disconnected until a button press triggers it too start up. The ESP will then hold power on until the safe is opened
Power will stay on for 8s after the correct code is input, or if no code, or wrong code until 30s has elapsed.

Powering everything off will cause the powerpack to go to sleep thus preserving charge. Probably a year between charges.

Powering down the solenoid, even if the door is still open, is OK as the solenoid can't return to the locked position until the door is closed and the knob turned to engage the bolts.

The safe solenoid is typically 6V but works happily enough on a Vcc of 5V.
A flyback diode across the solenoid and a 47uF capacitor across the ESP supply are recommended to reduce button noise and solenoid back EMF spikes.

**GPIO connections**
GPIO assignments are set by the board design for all touch and screen SPI. See the LGFX...h tab
On this CYD the I2C connector GPIOs is used to drive the relay signals. These are pins GPIO25 and GPIO32
The board also has a UART connecter with GND and Vcc so the 5V supply uses this connector, in my design.

**Install**
To fit the screen in the safe open the safe, do what needs to be done to get access to the back of the numeric keypad. Pop it out of its clips and grind away any plastic lugs that stop the screen from sitting flush. Then the screen will fit in as a replacement with a bit of hot glue fixing.

Note: The USB C port may end up blocked after install so make sure you have your code all working or cut away whatever is blocking it.

**Hardware**
A 4" CYD ESP32 display board (Typically Sunton ESP32-3248S035)
A digital safe/lock box with a touch button input with the 3 leds at the top (This type is the right size for the screen). EG Sandleford ES20

Two relays or opto-isolated highside FETs are needed to
  1. Hold the power on and
  2. Trigger the safe solenoid
