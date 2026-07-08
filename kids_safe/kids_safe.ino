// Daisie's safe. Pre-release 16/06/26
#include "LGFX_ESP32_3248S035.h"

LGFX tft; // Initialize the customized display instance
LGFX_Sprite imgSprite(&tft); // for transprent daisy icon
 //Other local includes after tft setup
#include "daisy_png.h"
#include "flash2_bmp.h"
#include "safe_bmp.h"

struct TouchState {
  bool touching = false;
  bool pressValid = false;
  uint32_t touchStartTime = 0;
  int16_t startX = 0;
  int16_t startY = 0;
  int currentBox = -1;
};

TouchState touchState;

const uint16_t HOLD_TIME_MS = 250; // Need to touch the icon for a reasonable amout of time. 1/4s seems about right
const uint8_t TOUCH_PADDING = 10; //Variable to adjust area of touched box if needed. Set to asmall value so boxes don't accidentally over lap

struct Boxes {
  int16_t x;    // Top-left X coordinate of your icon
  int16_t y;    // Top-left Y coordinate of your icon
  int16_t size; // Width and Height of your square icon
};

// Grid of nine touch boxes using structure above. Icons located in same position
Boxes boxes[9] = {
  {50, 50, 70},   {157, 50, 70},   {260, 50, 70},
  {50, 210, 70},  {157, 210, 70},  {260, 210, 70},
  {50, 385, 70},  {157, 385, 70},  {260, 385, 70}
};

// global variables
unsigned long milliCount = 0; //to count timing for countdown timer
unsigned long milliTimer = 1000; //To give a per second countdown
unsigned long safeOpenAt = 0; // To hold millis value at opening of solenoid
unsigned long solenoidTime = 8000; //keep solenoid open for xms. This also drives shut off sequence
int timeForCode = 30; //allowed time to enter correct code
bool correct = false; //state variable triggered by taking too long to enter code or getting code wrong. If triggered to true safe stays locked and ESP shuts down
bool tooLong = false; // state variable for not entering a code within timeForCode
const int CODE_LENGTH = 7; //Set the number of icons in the code
int correctCode[CODE_LENGTH] = {3,8,0,5,2,1,7}; //Set your code for the order of touching icons
int enteredCode[CODE_LENGTH]; //To store the touches when entering the code on screen
int enteredCount = 0;
int unLockPin = 25; // To drive the solenoid relay/FET
int powerPin = 32; // To drive the power on relay/FET in parallel with the push button


void setup() {
  pinMode(unLockPin,OUTPUT);
  pinMode(powerPin,OUTPUT);
  digitalWrite(unLockPin,LOW); //Set to off(locked) to be sure safe ESP pin isn't accidentaly high on start up.
  digitalWrite(powerPin,HIGH);//High to turn on to take over from activate buttonon the FET module powering the ESP/screen.
  //Serial.begin(115200); // uncomment all serial for debugging
  tft.init();
  tft.setRotation(0); // 0 = portrait. Top left at ESP32 end of board.
  uint16_t calData[8] = {3797, 289, 3800, 3920, 210, 270, 258, 3918};
  tft.setTouchCalibrate(calData);
  tft.setBrightness(128); // Set backlight dimming (0 to 255). Half bright is plenty
  tft.setSwapBytes(true); //Ensure correct colours from bit maps
  tft.pushImage(0, 0, 320, 480, epd_bitmap_daisie_flash_screen2); //Throw up the flash/welcome screen
  tft.setTextColor(TFT_WHITE, 0x0005); //same background as screen colour
  tft.setTextSize(3);// For countdown timer
  delay(3000);
  tft.fillScreen(0x0005);//good background colour for the touch icons

  drawIcons();
}

void loop() {
  handleTouchInput();
	if(solenoidTime < millis() - safeOpenAt && correct)
	 {
		correct = false;
	  digitalWrite(unLockPin,HIGH);
		//Serial.println("Time is up to open safe. Solenoid resetting to locked");
    tft.fillScreen(0x0005);
    tft.setTextColor(TFT_WHITE, 0x0005); //same background as screen colour
		tft.setCursor(40, 200);
    tft.print("Shut Safe now");
    tft.setCursor(100, 230);
    tft.print("to lock");
    delay(3000);// leave message on screen briefly
    digitalWrite(powerPin,LOW); //Turn off power.
    //Serial.println("Powering down after locking safe");
	 }
  // On screen coutdown timer for time to enter correct code sequence. If not entered within time then shut down. Stops battery drain from being left on with incomplete code entry
	static int i = timeForCode;
  if(milliTimer < millis() - milliCount && i != 0 && !correct){
   milliCount = millis();
   tft.setCursor(143, 300);
   if(i == 9){tft.print("  ");tft.setCursor(145, 300);}
   tft.print(i);
   i--;
	 if (i == 0) {
    tft.setTextColor(TFT_YELLOW, 0xF800); //same background as screen
    tft.fillScreen(0xF800);
    tft.setCursor(138, 300);
    tft.print("   ");// erase countdown
    tft.setCursor(115,230);
    tft.print("Daisie");
    tft.setCursor(75,260);
    tft.print("Time's up!");
    tft.setCursor(30,290);
    tft.print("Try again in a");
    tft.setCursor(55,320);
    tft.print("few seconds.");
	  //Serial.println("Didn't enter code soon enough. Power off. Press button to retry");
    delay(2500);// Time for message on screen before shut down. Could add an on screen button to touch for a retry
	  digitalWrite(powerPin,LOW); //Turn off power. Kill ESP etc
	 }
  }
}

