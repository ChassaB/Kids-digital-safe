This sketch works with the typical 3.5/4" yellow TFT and ESP board. These are usually a Sunton ESP32-3248S035
The sketch uses a daisy png image in a sprite to create a transpent sprite for the "pick your icons" code to open the safe.
A bmp image is used for the opening flash screen because it is very large and this saves decoding
The images/icons used have either been created by chatGPT, me, or from free images on the internet (to the bext of my knowledge)

A pattern of x shapes in the correct order, set up in the arrays, will trigger the open_safe function.
You can use any reasonable value for x and create any pattern of touches that suits.
You can make the code into a saying for kids to remember. EG A green frog and a red cat were chased by a yellow dog
You can, of course, make your own icons instead of mine

Given the project is battery driven (cheap 5000mAh power pack (KMart) with charge level display, in safe) power is disconnected 
until a button press triggers it too start up. The ESP will then hold power on until the safe is opened
Power will stay on for 8s after the correct code is input, or if no code, or wrong code until 30s has elapsed.

Powering everything off will cause the powerpack to go to sleep thus preserving charge. Probably a year between charges.

Powering down the solenoid even if the door is still open is OK as the solenoid can't return to the locked position until the door is closed
and the knob turned to engage the bolts.

GPIO assignments are set by the board design for all touch and screen SPI. See the LGFX...h tab
On this CYD the I2C connector GPIOs is used to drive the relay signals. These are pins GPIO25 and GPIO32
The board also has a UART connecter with GND and Vcc so the 5V supply uses this connector in my design

The safe solenoid is typically 6V but works happily enough on a Vcc of 5V.
A flyback diode across the solenoid and a 47uF capacitor across the ESP supply are recommended to reduce button noise and solenoid back EMF spikes.


Hardware
A 4" CYD ESP32 display board (50mm x 86mm).
A digital safe/lock box with a touch button input with the 3 leds at the top (This type is the right size for the screen). EG Sandleford ES20
After removing the numeric touch pad and grindif a few plastic lugs the screen will fit in as a replacement with a bit of hot glue fixing
Two relays or opto-isolated highside FETs are needed to
  1. Hold the power on and
  2. Trigger the safe solenoid

