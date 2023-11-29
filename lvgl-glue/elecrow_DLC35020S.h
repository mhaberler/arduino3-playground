#ifndef __ELECROW_DLC35020S_H
#define __ELECROW_DLC35020S_H

#ifdef ELECROW_DLC35020S

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

#define LCD_MOSI 13
#define LCD_MISO 14 // 12
#define LCD_SCK 12
#define LCD_CS 3
#define LCD_RST -1 // 26
#define LCD_DC 42  // 33
#define LCD_BL 46

#define SDA_FT6236 2
#define SCL_FT6236 1
#define TOUCH_I2C_ADD 0x38

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Bus_SPI _bus_instance;
  lgfx::Panel_ILI9488 _panel_instance;
  lgfx::Light_PWM _light_instance;
  lgfx::Touch_FT5x06 _touch_instance;

public:
  LGFX(void)
  {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI3_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 60000000;
      cfg.freq_read = 16000000;
      cfg.spi_3wire = false;
      cfg.use_lock = true;
      // cfg.dma_channel = 1;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk = LCD_SCK;
      cfg.pin_mosi = LCD_MOSI;
      cfg.pin_miso = LCD_MISO;
      cfg.pin_dc = LCD_DC;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();

      cfg.pin_cs = LCD_CS;
      cfg.pin_rst = LCD_RST;
      cfg.pin_busy = -1;
      cfg.memory_width = SCREEN_HEIGHT;
      cfg.memory_height = SCREEN_WIDTH;
      cfg.panel_width = SCREEN_HEIGHT;
      cfg.panel_height = SCREEN_WIDTH;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = true;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;
      _panel_instance.config(cfg);
    }
    {
      auto cfg = _light_instance.config();

      cfg.pin_bl = LCD_BL;

      _light_instance.config(cfg);
      _panel_instance.light(&_light_instance);
    }
    {
      auto cfg = _touch_instance.config();
      cfg.i2c_port = 0;
      cfg.i2c_addr = TOUCH_I2C_ADD;
      cfg.pin_sda = SDA_FT6236;
      cfg.pin_scl = SCL_FT6236;
      cfg.pin_int = -1;
      cfg.freq = 400000;
      cfg.x_min = 0;
      cfg.x_max = SCREEN_HEIGHT-1;
      cfg.y_min = 0;
      cfg.y_max = SCREEN_WIDTH-1;
      cfg.offset_rotation = 0;

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }
    setPanel(&_panel_instance);
  }
};

#endif // ELECROW_DLC35020S

#endif // __ELECROW_DLC35020S_H