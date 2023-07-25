#include "LEDS.h"



cLedEncoder LedEncoder;

#ifdef ANDROID_CHEESE_LED_

void cLedEncoder:: init(){
  FastLED.addLeds<NEOPIXEL, PIN_LED_ENC1>(leds_enc[0], 8);
  // FastLED.addLeds<NEOPIXEL, PIN_LED_ENC2>(leds_enc[1], 8);
  FastLED.addLeds<NEOPIXEL, PIN_LED_STRIP>(leds_strip, 5);


  for (int i = 0; i < 8; i++)
    leds_enc[0][i] = CRGB::Blue;

  for (int i = 0; i < 8; i++)
    leds_enc[1][i] = CRGB::Red;

  FastLED.setBrightness(255);
};

void cLedEncoder:: tick(){
  culc();
  static uint64_t time_redraw = millis();

  if (millis() >= time_redraw) {
    time_redraw = millis() + TIME_LEDS_REDRAW;

    backLeds();
    frontLeds();

    FastLED.show();
  };
};

#else

void cLedEncoder:: init(){
};

void cLedEncoder:: tick(){
};
#endif




// leds 0..9 - leds encoders
// leds 10 - strip
void cLedEncoder:: fill(CRGB *leds, uint8_t count_leds, CRGB color){

  for ( uint8_t i = 0; i < count_leds; i++)
    leds[i] = color;

};

void cLedEncoder:: right(uint8_t i){
  param[i].count++;
  param[i].tim = millis();

  param[i].state = 2;
  param[i].tim_state = millis();
};

void cLedEncoder:: left(uint8_t i){
  param[i].count--;
  param[i].tim = millis();

  param[i].state = 3;
  param[i].tim_state = millis();
  
};

void cLedEncoder:: hold(uint8_t i, bool state){
  param[i].hold = state;
  if (state) param[i].tim = millis();
};

cLedEncoder:: cLedEncoder(){
  init();
};

void cLedEncoder:: click(uint8_t i){
  param[i].state = 1;
  param[i].tim_state = millis();
};

void cLedEncoder:: culc(){
  for (int i = 0; i < COUNT_ENCODERS; i++) {
    if (param[i].state != 0 && millis() - param[i].tim_state >= TIME_LEDS_ANIMATION) param[i].state = 0;
  }
};



//Фоновая подсветка этапа приготовления | i - энкодер
void cLedEncoder:: backLedsCooking(CRGB *leds, uint8_t leds_count){
  fill(leds,leds_count,CRGB::Black);
  // switch (Cooking._mode){
  //     case 0:
  //     fill(leds,leds_count,CRGB::Black);
  //     break;

  //     case 1:
  //     fill(leds,leds_count,CHSV(30,255,250));
  //     break;

  //     case 2:
  //     fill(leds,leds_count,CHSV(90,255,250));
  //     break;
      
  //     case 3:
  //     fill(leds,leds_count,CHSV(150,255,250));
  //     break;

  //     case 4:
  //     fill(leds,leds_count,CHSV(150,255,250));
  //     break;
  // }
};



//Фоновая подсветка работы миксера | i - энкодер
void cLedEncoder:: backLedsMixer(uint8_t i_leds){
  //поддерживаем параметр с 80 по 160 для корректной работы
  param[i_leds].count = (param[i_leds].count % 800) + 800;
  
  //Очищаем
  fill(leds_enc[i_leds],LED_ENCODER_COUNT,CRGB::Black);

  uint8_t speed = Mixer.getCurrentSpeed();

  static uint64_t time_mixer = millis();
  if (speed != 0){ 
    uint16_t length = speed * 5;
    if (length < 100) length = 100;

    smoothLine(leds_enc[i_leds],LED_ENCODER_COUNT,param[i_leds].count,param[i_leds].count + length, 200,255,100);

    if (millis() - time_mixer >= 15) {
      time_mixer = millis();

      uint8_t delta_count = speed / 10;
      if (delta_count == 0) delta_count = 1;

      if (Mixer.getCurrentDir()) param[i_leds].count-= delta_count;
      else param[i_leds].count+= delta_count;
    }
  }; 
}

void cLedEncoder::strip_start(){
  
  for (uint8_t i = 0; i < COUNT_ENCODERS; i++) fill(leds_enc[i], LED_ENCODER_COUNT, CRGB::Black);

  static uint16_t count = 0;

  static uint8_t step = 50;

  CHSV sup_led[LED_STRIP_COUNT];

  for (int8_t i = 0; i < LED_STRIP_COUNT; i++) sup_led[i] = CHSV(50,0,0);

  for (int8_t i = 0; i < 25;  i++) {
    
    for (int jj = 0; jj < 5; jj++) sup_led[jj].val = i * 10;
    for (int jj = 0; jj < 5; jj++) leds_strip[jj] = sup_led[jj];
  
    FastLED.show();

    delay(20);
  };

  uint8_t digit = 0;
  uint8_t ss = 0;

  while (digit < 5) {
    sup_led[digit].sat += 10;

    if ( sup_led[digit].sat >= 250) {
      sup_led[digit].sat = 255;
      digit++;
    }

    for (int jj = 0; jj < 5; jj++) leds_strip[jj] = sup_led[jj];  
    FastLED.show();
    delay(10);
  }

  for (int8_t i = 25; i != 0;  i--) {
    
    for (int jj = 0; jj < 5; jj++) sup_led[jj].val = i * 10;
    for (int jj = 0; jj < 5; jj++) leds_strip[jj] = sup_led[jj];
  
    FastLED.show();

    delay(40);
  };

}

void cLedEncoder::backLedsStrip(){
  static uint8_t count = 0;
  fill(leds_strip,LED_STRIP_COUNT,CHSV(30,255,250));
  // switch (Cooking._mode) {
  //   case 0:
  //     static unsigned long tim = millis();
  //     if (tim <= millis()) {
  //       tim = millis() + 20;   
  //       fill(leds_strip,LED_STRIP_COUNT,CRGB::Black);
  //       smoothLine(leds_strip, LED_STRIP_COUNT,count, count + 100, 50, 255, 50);
  //       count = ++count % 250;
  //     };
  //     break;

  //   case 1:
  //   fill(leds_strip,LED_STRIP_COUNT,CHSV(30,255,250));
  //   break;

  //   case 2:
  //   fill(leds_strip,LED_STRIP_COUNT,CHSV(90,255,250));
  //   break;
    
  //   case 3:
  //   fill(leds_strip,LED_STRIP_COUNT,CHSV(150,255,250));
  //   break;

  //   case 4:
  //   fill(leds_strip,LED_STRIP_COUNT,CHSV(200,255,250));
  //   break;
  // }
};

void cLedEncoder:: smoothLine(CRGB *leds, uint8_t count_leds, uint16_t start, uint16_t end, uint8_t hue, uint8_t max_bright, uint8_t step, uint8_t s) {
  uint8_t left = start / step;
  uint8_t right = end / step ;
  uint8_t bright_left = step - ( start % step);
  uint8_t bright_right = end % step;

  if (left == right) {
    leds[right % count_leds] = CHSV(hue, s, map(right - left, 0 , step, 0, max_bright) );
  }
  else {
  //Рисуем правый светодиод
  leds[right % count_leds] = CHSV(hue, s, map(bright_right, 0 , step, 0, max_bright) );
  
  //Рисуем левый светодиод
  leds[left % count_leds] = CHSV(hue, s, map(bright_left, 0, step, 0, max_bright));

  //Прорисовываем полностью заполненные 
  for (uint8_t i = left + 1  ; i < right; i++) 
    leds[i % count_leds]=CHSV(hue,255,max_bright);
  };
};

// Фоновая подсветка (прорисовка нижнего слоя)
void cLedEncoder::backLeds(){   
  if (Hardware.b_redBtnPress) {
    // for (uint8_t i = 0; i < COUNT_ENCODERS; i++)    
    //   fill(leds_enc[i],LED_ENCODER_COUNT,CHSV(0,255,200));

    fill(leds_strip,LED_STRIP_COUNT,CHSV(0,255,200));
  }
  else {
    // strip_start();
    backLedsStrip();
    // backLedsCooking(leds_enc[0] , LED_ENCODER_COUNT);
    // backLedsMixer(0);
  };
};


// Подсветка действий энкодера (прорисовка верхнего слоя)
void cLedEncoder::frontLeds(){

  // Если время анимации вышло сбрасывае состояние
  for (int i = 0; i < COUNT_ENCODERS; i++) 
    if (param[i].state != 0 && millis() - param[i].tim_state >= TIME_LEDS_ANIMATION) param[i].state = 0;
  
  //  Рисуем
  for (uint8_t i = 0; i<COUNT_ENCODERS ; i++) {
    //Click
    if (param[i].state == 1) fill(leds_enc[i],LED_ENCODER_COUNT,CRGB::White);
    
    //right
    if (param[i].state == 2) {
      leds_enc[i][1] = CRGB::White;
      leds_enc[i][2] = CRGB::White;
      leds_enc[i][3] = CRGB::White;
    }

    //left
    if (param[i].state == 3) {
      leds_enc[i][5] = CRGB::White;
      leds_enc[i][6] = CRGB::White;
      leds_enc[i][7] = CRGB::White;
    }

    //hold
    if (param[i].hold) {
      leds_enc[i][0] = CRGB::White;
      leds_enc[i][4] = CRGB::White;        
    }
  };
};
