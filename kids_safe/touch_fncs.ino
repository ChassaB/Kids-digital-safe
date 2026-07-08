void handleTouchInput() {
  int32_t x, y; // int32_t to match LovyanGFX's native return type
  bool touched = tft.getTouch(&x, &y);

  // NEW TOUCH EVENT
  if (touched && !touchState.touching) {
    touchState.touching = true;
    touchState.startX = x;
    touchState.startY = y;
    touchState.touchStartTime = millis();
    touchState.currentBox = getTouchedBox(x, y);
    touchState.pressValid = (touchState.currentBox != -1); // Only valid if they actually hit a box
  }
  
  // CONTINUOUS HOLD EVENT
  else if (touched && touchState.touching) {
    int newBox = getTouchedBox(x, y);
    
    // User slid their finger completely off the current icon box
    if (newBox != touchState.currentBox) {
      touchState.currentBox = newBox;
      touchState.touchStartTime = millis();
      touchState.pressValid = (newBox != -1);
    }
    
    // If holding down a valid target, process the hold threshold
    if (touchState.pressValid && touchState.currentBox != -1) {
      if (millis() - touchState.touchStartTime > HOLD_TIME_MS) {
        processBoxPress(touchState.currentBox);
        touchState.pressValid = false; // Prevents continuous double-triggering while holding
      }
    }
  }
  
  // FINGER RELEASE EVENT
  else if (!touched && touchState.touching) {
    touchState.touching = false;
    touchState.currentBox = -1;
    touchState.pressValid = false;
  }
}

// 2. HIGHLY OPTIMIZED BOUNDARY CHECKING
int getTouchedBox(int16_t tx, int16_t ty) {
  for (int i = 0; i < 9; i++) {
    // Expand the bounding box boundaries using your padding variable
    int16_t xMin = boxes[i].x - TOUCH_PADDING;
    int16_t xMax = boxes[i].x + boxes[i].size + TOUCH_PADDING;
    int16_t yMin = boxes[i].y - TOUCH_PADDING;
    int16_t yMax = boxes[i].y + boxes[i].size + TOUCH_PADDING;

    // Check if touch coordinates fall perfectly inside the padded square zone
    if (tx >= xMin && tx <= xMax && ty >= yMin && ty <= yMax) {
      return i; // Returns the index matching the exact box touched
    }
  }
  return -1; // No icon box was hit
}

void processBoxPress(int boxNumber)
{
  //Serial.print("Box pressed: ");
  //Serial.println(boxNumber);
	//Change the selected icon to show successful press
  tft.fillCircle(boxes[boxNumber].x,boxes[boxNumber].y,3+boxes[boxNumber].size/2,TFT_WHITE);
  enteredCode[enteredCount] = boxNumber; // Store pressed icon for safe code sequence

  enteredCount++;
  // Enough digits entered?
  if (enteredCount >= CODE_LENGTH)
  {
    correct = true; //default is true then check for mismatch for false with the "if" below

    // Compare entered code to correct code
    for (int i = 0; i < CODE_LENGTH; i++)
    {
      if (enteredCode[i] != correctCode[i])
      {
        correct = false;
        break;
      }
    }
    // Result
    if (correct)
    {
      //Serial.println("SAFE UNLOCKED! Solenoid released");
			safeOpenAt = millis(); //This will start the timing for the solenoid to be open for SolenoidTime
			digitalWrite(unLockPin,HIGH); //Power up solenoid to cause it to open. Check pin voltages for on and off. LOW/HIGH or HIGH/LOW
      //Serial.println("Correct code opening safe now.");
			tft.fillScreen(0x0005);
      tft.pushImage(0, 0, 320, 480, epd_bitmap_open_safe);
			//Maybe animate the safe opening at some point
    }
    else
    {
      //Serial.println("WRONG CODE");
      tft.setCursor(30, 200);
      tft.setTextSize(4);
      tft.print("Wrong code!");
      tft.setTextSize(3);// reset to default
    }
    // Reset for next attempt
    enteredCount = 0;
  }
}