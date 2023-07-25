#ifndef ANDROID_LED_H
#define ANDROID_LED_H

#include "Settings.h"
#include "FastLED.h"
#include "Hardware.h"

// Время анимации в мсек
#define TIME_LEDS_ANIMATION 200

//Время перересовки в мсек
#define TIME_LEDS_REDRAW 5

class cLedEncoder {
private:
  CRGB leds_enc[COUNT_ENCODERS][8];
  CRGB leds_strip[5];

  struct strParamEncoder{
    bool hold = false; 
    uint16_t count = 8;
    //0 - empty
    //1 - click
    //2 - right
    //3 - left
    uint8_t state;
    uint64_t tim;
    uint64_t tim_state; 
  } param[COUNT_ENCODERS];

public:
  cLedEncoder();
  
  void right(uint8_t i);
  void left(uint8_t i);

  void hold(uint8_t i, bool state);

  void click(uint8_t);

  void culc();

  void tick();

  void init();

  //Управляемая подсветка
  void frontLeds();
  
  //Фоновая подсветка
  void backLeds();

  //Фоновая подсветка этапа приготовления | i - энкодер
  void backLedsCooking(CRGB *leds, uint8_t leds_count);
  
  //Фоновая подсветка работы миксера | i - энкодер, работает только на энкодер
  void backLedsMixer(uint8_t i_leds);

  void backLedsStrip();

  void fill(CRGB *leds, uint8_t count_leds, CRGB color);

  /*Линия с плавными краями,
    i_leds- Лента на которую выводить
    start - начальная точка
    end   - конечная точка
    hue   - цвет HUE в HSV
    max_bright - максимальная яркость
    step  - шаг на один светодиод 
  */
  void smoothLine(CRGB *leds, uint8_t count_leds, uint16_t start, uint16_t end, uint8_t hue, uint8_t max_bright, uint8_t step = 10, uint8_t s = 255);

  void strip_start();
};

extern cLedEncoder LedEncoder;  

#endif