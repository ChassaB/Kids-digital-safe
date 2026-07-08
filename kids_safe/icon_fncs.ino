//draw screen isons at the touch locations
 void drawIcons(){
  //row1
  drawHeart(tft,60,60,80,TFT_RED);//x must be greater than size/4
  drawCatFace(tft,157,60,30,TFT_LIGHTGREY);
  drawHeart(tft,260,60,80,TFT_BLUE);//x must be greater than size/4
  //row 2
  drawPanda(tft,60,225, 30);
  drawDaisy(); //position set in the drawDaisy() function
  drawFrog(tft,260,225,30,TFT_GREEN);
  //row 3
  drawHeart(tft,60,400,80,TFT_PURPLE);//x must be greater than size/4
  drawDog(tft, 157, 400,30,TFT_ORANGE);
  drawHeart(tft,260,400,80,TFT_YELLOW);//x must be greater than size/4
 }


void drawHeart(LGFX &tft, int16_t x, int16_t y, int16_t size, uint16_t color)
{
  // size controls overall heart scale
  int16_t r = size / 4;

  // Top circles. Radius is r+2 to match with triangles better
  tft.fillCircle(x - r, y - r, r+2, color);
  tft.fillCircle(x + r, y - r, r+2, color);

  // Bottom triangle point
  tft.fillTriangle(
    x - size / 2, y - r / 2,
    x + size / 2, y - r / 2,
    x, y + size / 2,
    color
  );
}
// functions to draw things using graphics primitives
void drawDaisy() {
  imgSprite.setColorDepth(16); 
  imgSprite.createSprite(120, 130);

  // 1. Clear the canvas to solid black 
  imgSprite.fillScreen(TFT_BLACK); 

  // 2. Decode the PNG array onto the black canvas. Used Tomeko file to hex
  imgSprite.drawPng(daisy_png_data, daisy_png_size, 0, 0);

  // 3. Push to screen and pass TFT_BLACK as the final argument.
  // This tells LovyanGFX to skip over all black pixels during the push operation!
  // Arguments: (&target_screen, screen_x, screen_y, transparent_color_key)
  imgSprite.pushSprite(&tft, 100, 163, TFT_BLACK);

  // 4. Clean up the memory
  imgSprite.deleteSprite();
}

//-----Cat face with wiskers------
void drawCatFace(LGFX &tft, int x, int y, int size, uint16_t color) {
    int r = size;  // base radius
    // Face
    tft.fillCircle(x, y, r, color);
    // Ears (triangles)
    tft.fillTriangle(x - r*.75, y - r/2,
                     x - r,   y - r - r/2,
                     x,       y - r/2,
                     color);

    tft.fillTriangle(x + r*.75, y - r/2,
                     x + r,   y - r - r/2,
                     x,       y - r/2,
                     color);

    // Eyes
    int eyeOffsetX = r / 2;
    int eyeOffsetY = r / 4;
    int eyeSize = r / 6;
    tft.fillCircle(x - eyeOffsetX, y - eyeOffsetY, eyeSize, TFT_BLACK);
    tft.fillCircle(x + eyeOffsetX, y - eyeOffsetY, eyeSize, TFT_BLACK);
    // Nose (small triangle)
    tft.fillTriangle(x, y,
                     x - r/8, y + r/6,
                     x + r/8, y + r/6,
                     TFT_PINK);

    // Whiskers
    for (int i = -1; i <= 1; i++) {
        tft.drawLine(x - r/8, y + r/6 + i*5, x - r, y + i*5, TFT_WHITE);
        tft.drawLine(x + r/8, y + r/6 + i*5, x + r, y + i*5, TFT_WHITE);
    }
}

//------Panda face------
void drawPanda(LGFX &tft, int x, int y, int size) {
    int r = size;

    // Face (white)
    tft.fillCircle(x, y, r, TFT_WHITE);
    tft.drawCircle(x, y, r, TFT_BLACK);  // outline

    // Ears
    int earR = r / 3;
    tft.fillCircle(x - r/2, y - r, earR, TFT_BLACK);
    tft.fillCircle(x + r/2, y - r, earR, TFT_BLACK);

    // Eye patches
    int eyeOffsetX = r / 2;
    int eyeOffsetY = r / 5;
    int patchR = r / 3;

    tft.fillCircle(x - eyeOffsetX, y - eyeOffsetY, patchR, TFT_BLACK);
    tft.fillCircle(x + eyeOffsetX, y - eyeOffsetY, patchR, TFT_BLACK);

    // Eyes (small white dots inside patches)
    int eyeR = r / 8;
    tft.fillCircle(x - eyeOffsetX, y - eyeOffsetY, eyeR, TFT_WHITE);
    tft.fillCircle(x + eyeOffsetX, y - eyeOffsetY, eyeR, TFT_WHITE);

    // Nose
    int noseY = y + r / 6;
    tft.fillTriangle(
        x, noseY,
        x - r/8, noseY + r/6,
        x + r/8, noseY + r/6,
        TFT_BLACK
    );

    // Mouth (simple lines)
    tft.drawLine(x, noseY + r/6, x - r/6, y + r/2, TFT_BLACK);
    tft.drawLine(x, noseY + r/6, x + r/6, y + r/2, TFT_BLACK);
}

//------Frog face------
void drawFrog(LGFX &tft, int x, int y, int size, uint16_t color) {
    int r = size;

    // Head
    tft.fillCircle(x, y, r, color);
    tft.drawCircle(x, y, r, TFT_BLACK);

    // Eyes (sit above head)
    int eyeR = r / 3;
    int eyeOffsetX = r / 2;

    tft.fillCircle(x - eyeOffsetX, y - r, eyeR, color);
    tft.fillCircle(x + eyeOffsetX, y - r, eyeR, color);

    // Eye whites
    tft.fillCircle(x - eyeOffsetX, y - r, eyeR / 2, TFT_WHITE);
    tft.fillCircle(x + eyeOffsetX, y - r, eyeR / 2, TFT_WHITE);

    // Pupils
    tft.fillCircle(x - eyeOffsetX, y - r, eyeR / 4, TFT_BLACK);
    tft.fillCircle(x + eyeOffsetX, y - r, eyeR / 4, TFT_BLACK);

    // Mouth (wide smile)
    //tft.drawArc(x, y + r/4, r * 0.7, r * 0.5, 200, 340, TFT_BLACK);
    //tft.drawArc(x, y + r/3, r, r/2, 210, 330, TFT_BLACK);
    //tft.drawArc(x, y + r/4, r * 0.7, r * 0.5, 20, 160, TFT_BLACK);
    tft.drawArc(x, y + r/3 - 10, r * 0.8, r * 0.4, 20, 160, TFT_BLACK);

    // Optional nostrils (tiny dots)
    tft.fillCircle(x - r/6, y, 2, TFT_BLACK);
    tft.fillCircle(x + r/6, y, 2, TFT_BLACK);
}

//------Bunny face------
void drawBunny(LGFX &tft, int x, int y, int size, uint16_t color) {
    int r = size;

    // Head
    tft.fillCircle(x, y, r, color);
    tft.drawCircle(x, y, r, TFT_BLACK);

    // Ears (tall rounded look using circles + fill)
    int earW = r / 3;
    int earH = r;

    // Left ear outer
    tft.fillRoundRect(x - r/2 - earW/2, y - r - earH, earW, earH, earW/2, color);
    // Right ear outer
    tft.fillRoundRect(x + r/2 - earW/2, y - r - earH, earW, earH, earW/2, color);

    // Inner ears (pink)
    tft.fillRoundRect(x - r/2 - earW/4, y - r - earH + earW/4,
                      earW/2, earH - earW/2, earW/4, TFT_PINK);

    tft.fillRoundRect(x + r/2 - earW/4, y - r - earH + earW/4,
                      earW/2, earH - earW/2, earW/4, TFT_PINK);

    // Eyes
    int eyeOffsetX = r / 3;
    int eyeOffsetY = r / 4;
    int eyeR = r / 8;

    tft.fillCircle(x - eyeOffsetX, y - eyeOffsetY, eyeR, TFT_BLACK);
    tft.fillCircle(x + eyeOffsetX, y - eyeOffsetY, eyeR, TFT_BLACK);

    // Nose (small triangle)
    int noseY = y + r / 8;
    tft.fillTriangle(
        x, noseY,
        x - r/10, noseY + r/8,
        x + r/10, noseY + r/8,
        TFT_PINK
    );

    // Mouth (two little lines)
    tft.drawLine(x, noseY + r/8, x - r/6, y + r/3, TFT_BLACK);
    tft.drawLine(x, noseY + r/8, x + r/6, y + r/3, TFT_BLACK);
}

//------Dog face------
void drawDog(LGFX &tft, int x, int y, int size, uint16_t color) {
    int r = size;

    // Head
    tft.fillCircle(x, y, r, color);
    tft.drawCircle(x, y, r, TFT_BLACK);

    // Floppy ears (rounded rectangles hanging down)
    int earW = r / 2;
    int earH = r;

    // Left ear
    tft.fillRoundRect(x - r - earW/2, y - r/2, earW, earH, earW/2, color);
    // Right ear
    tft.fillRoundRect(x + r - earW/2, y - r/2, earW, earH, earW/2, color);

    // Eyes
    int eyeOffsetX = r / 3;
    int eyeOffsetY = r / 4;
    int eyeR = r / 8;

    tft.fillCircle(x - eyeOffsetX, y - eyeOffsetY, eyeR, TFT_BLACK);
    tft.fillCircle(x + eyeOffsetX, y - eyeOffsetY, eyeR, TFT_BLACK);

    // Snout (oval-ish using circle)
    int snoutY = y + r / 6;
    int snoutR = r / 3;
    tft.fillCircle(x, snoutY, snoutR, TFT_WHITE);
    tft.drawCircle(x, snoutY, snoutR, TFT_BLACK);

    // Nose
    tft.fillCircle(x, snoutY - r/10, r/10, TFT_BLACK);

    // Mouth
    tft.drawLine(x, snoutY, x - r/6, y + r/2, TFT_BLACK);
    tft.drawLine(x, snoutY, x + r/6, y + r/2, TFT_BLACK);
}


