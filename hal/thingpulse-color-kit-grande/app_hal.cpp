#include "app_hal.h"
#include "lvgl.h"
#include "settings.h"

TFT_eSPI tft = TFT_eSPI(TFT_SCREEN_WIDTH, TFT_SCREEN_HEIGHT); /* TFT instance */
FT6236 ts = FT6236(TFT_SCREEN_WIDTH, TFT_SCREEN_HEIGHT);

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ TFT_SCREEN_WIDTH * 10 ];

void initTft(TFT_eSPI *tft);
void initTouchScreen(FT6236 *ts);
uint8_t readRegister8(uint8_t reg);
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p );
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data );

void hal_setup(void) {
    Serial.begin(115200);
    initTouchScreen(&ts);
    initTft(&tft);


    lv_disp_draw_buf_init( &draw_buf, buf, NULL, TFT_SCREEN_WIDTH * 10 );

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = TFT_SCREEN_WIDTH;
    disp_drv.ver_res = TFT_SCREEN_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );
}

void hal_loop(void)
{
  lv_timer_handler(); 
}


/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{

    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}

void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX, touchY;

    TS_Point point = ts.getPoint(0);

    if( point.z == 0 )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = point.x;
        data->point.y = point.y;

    }
}

void initTft(TFT_eSPI *tft) {
  tft->init();
  tft->begin();          /* TFT init */
  tft->setRotation( TFT_ROTATION ); /* Landscape orientation, flipped */
  // We need to swap the colour bytes (endianess)
  // -> https://github.com/Bodmer/TJpg_Decoder/blob/master/examples/LittleFS/LittleFS_Jpg/LittleFS_Jpg.ino#L60
  tft->setSwapBytes(true);

  log_d("Configuring TFT backlight at pin %d.", TFT_BL);
  // Setup PWM channel, ledc is a LED Control Function
  ledcSetup(0, 5000, 8);
  ledcAttachPin(TFT_BL, 0);
  ledcWrite(0, TFT_LED_BRIGHTNESS);

  tft->fillScreen(TFT_BLACK);
}

void initTouchScreen(FT6236 *ts) {
  if (ts->begin(TOUCH_SENSITIVITY, TOUCH_SDA, TOUCH_SCL)) {
    log_i("Capacitive touch started.");
  } else {
    log_e("Failed to start the capacitive touchscreen.");
  }
  ts->setRotation(TOUCH_ROTATION);
}

uint8_t readRegister8(uint8_t reg) {
  uint8_t x;

  Wire.beginTransmission(FT6236_ADDR);
  Wire.write((byte)reg);
  Wire.endTransmission();

  Wire.requestFrom((byte)FT6236_ADDR, (byte)1);
  x = Wire.read();

  return x;
}

