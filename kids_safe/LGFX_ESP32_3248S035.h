#define LGFX_USE_V1
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7796  _panel_instance;
  lgfx::Bus_SPI       _bus_instance;
  lgfx::Light_PWM     _light_instance;
  lgfx::Touch_XPT2046 _touch_instance;

public:
  LGFX(void) {
    { // 1. Configure the Main SPI Bus
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI2_HOST;     // Bound to SPI3
      cfg.spi_mode = 0;             // SPI Mode 0
      cfg.freq_write = 40000000;    // 40 MHz clock speed for drawing
      cfg.freq_read  = 16000000;    // 16 MHz clock speed for reading
      cfg.pin_sclk = 14;            // Hardware Clock
      cfg.pin_mosi = 13;            // Hardware MOSI
      cfg.pin_miso = 12;            // Hardware MISO
      cfg.pin_dc   = 2;             // Data/Command pin
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    { // 2. Configure the Display Panel (ST7796)
      auto cfg = _panel_instance.config();
      cfg.pin_cs           = 15;    // Display Chip Select
      cfg.pin_rst          = -1;    // Linked directly to ESP32 EN/Reset pin
      cfg.panel_width      = 320;   // Physical hardware width
      cfg.panel_height     = 480;   // Physical hardware height
      cfg.offset_x         = 0;
      cfg.offset_y         = 0;
      cfg.invert           = false;
      cfg.rgb_order        = false;
      _panel_instance.config(cfg);
    }

    { // 3. Configure the Backlight Control
      auto cfg = _light_instance.config();
      cfg.pin_bl = 27;              // Backlight Pin
      cfg.freq   = 44100;           // PWM frequency
      cfg.pwm_channel = 1;          // ESP32 LEDC Channel
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }

    { // 4. Configure the Resistive Touch (XPT2046)
      auto cfg = _touch_instance.config();
      cfg.x_min      = 300;         // Initial calibration limits
      cfg.x_max      = 3900;
      cfg.y_min      = 200;
      cfg.y_max      = 3800;
      cfg.pin_cs     = 33;          // Touch Chip Select (Unique)
      cfg.pin_int    = 36;          // Touch Interrupt Pin
      cfg.bus_shared = true;        // Shares MISO/MOSI/SCLK with TFT
      cfg.spi_host   = SPI2_HOST;   // Bound to the same SPI peripheral

      // CRITICAL OVERRIDES FOR CORE 3.3.10
      cfg.pin_sclk   = 14;          // Force to CYD shared SCLK
      cfg.pin_mosi   = 13;          // Force to CYD shared MOSI
      cfg.pin_miso   = 12;          // Force to CYD shared MISO

      cfg.offset_rotation = 2;
      
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }

    setPanel(&_panel_instance);
  }
};
