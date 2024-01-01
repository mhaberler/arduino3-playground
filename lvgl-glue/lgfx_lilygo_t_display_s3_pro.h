#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
    lgfx::Bus_SPI _bus_instance;
    lgfx::Panel_ST7796 _panel_instance;
    lgfx::Light_PWM _light_instance;
    lgfx::Touch_CST816S _touch_instance;

  public:
    // コンストラクタを作成し、ここで各種設定を行います。
    // クラス名を変更した場合はコンストラクタも同じ名前を指定してください。
    LGFX(void) {
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
            // 表示パネル制御の設定を行います。
            auto cfg = _panel_instance.config(); // 表示パネル設定用の構造体を取得します。

            cfg.pin_cs = 6;    // CS要拉低
            cfg.pin_rst = 5;   // RST和开发板RST相连
            cfg.pin_busy = -1; // BUSYが接続されているピン番号 (-1 = disable)

            // ※ 以下の設定値はパネル毎に一般的な初期値が設定されていますので、不明な項目はコメントアウトして試してみてください。

            cfg.memory_width = 240;   // ドライバICがサポートしている最大の幅
            cfg.memory_height = 320;  // ドライバICがサポートしている最大の高さ
            cfg.panel_width = 170;    // 実際に表示可能な幅
            cfg.panel_height = 320;   // 実際に表示可能な高さ
            cfg.offset_x = 35;        // パネルのX方向オフセット量
            cfg.offset_y = 0;         // パネルのY方向オフセット量
            cfg.offset_rotation = 0;  // 回転方向の値のオフセット 0~7 (4~7は上下反転)
            cfg.dummy_read_pixel = 8; // ピクセル読出し前のダミーリードのビット数
            cfg.dummy_read_bits = 1;  // ピクセル以外のデータ読出し前のダミーリードのビット数
            cfg.readable = true;      // データ読出しが可能な場合 trueに設定
            cfg.invert = true;        // パネルの明暗が反転してしまう場合 trueに設定
            cfg.rgb_order = false;    // パネルの赤と青が入れ替わってしまう場合 trueに設定
            cfg.dlen_16bit = false;   // データ長を16bit単位で送信するパネルの場合 trueに設定
            cfg.bus_shared = false;   // SDカードとバスを共有している場合 trueに設定(drawJpgFile等でバス制御を行います)

            _panel_instance.config(cfg);
        }

        {
            // バックライト制御の設定を行います。（必要なければ削除）
            auto cfg = _light_instance.config(); // バックライト設定用の構造体を取得します。

            cfg.pin_bl = 38;     // バックライトが接続されているピン番号
            cfg.invert = false;  // バックライトの輝度を反転させる場合 true
            cfg.freq = 44100;    // バックライトのPWM周波数
            cfg.pwm_channel = 7; // 使用するPWMのチャンネル番号

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance); // バックライトをパネルにセットします。
        }
        {
            auto cfg = _touch_instance.config();
            cfg.offset_rotation = 0;

            cfg.x_min = 0;
            cfg.x_max = 320;
            cfg.y_min = 0;
            cfg.y_max = 170;

            // I2C接続
            cfg.i2c_port = 0;
            cfg.pin_sda = 18;
            cfg.pin_scl = 17;
            cfg.pin_int = 16;
            cfg.pin_rst = 21;

            cfg.freq = 400000;
            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance);
        }
        setPanel(&_panel_instance); // 使用するパネルをセットします。
    }
};