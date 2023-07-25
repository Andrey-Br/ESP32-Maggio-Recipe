#ifndef _ANDROID_CHEESE_SETTINGS_
#define _ANDROID_CHEESE_SETTINGS_

#define VERSION_PLATE 5

#include "Arduino.h"
#include "EEPROM.h"

//Структура коэффицентов PID
struct str_PIDkoef{
    float waterHeatProcent = 4;
    
    float HS_K2 = 4;

    float HF_K1 = 4;
    float HF_K2 = 6;

    float HA_Cooling_K1 = 3; 
    float HA_Cooling_K2 = 1;

    float CS_k1 = 4;
    float CS_k2 = 1; 
};

//Структура для калибровки датчика in - "сырых" значение с датчика, out - какое должно быть
struct str_Calibrate{
  float in, out;
};

struct str_settings {
  
  // Данные о калибровке молока
  str_Calibrate CalibrateMilk[2]= {{25, 22}, {69.2, 74}};

  // Данные о калибровке воды
  str_Calibrate CalibrateWater[2]= {{10, 10}, {65.7, 63}};

  //Значения необходимые для PID регулирования
  str_PIDkoef PID;
  
};

extern str_settings settings;

//Получить настройки находящиеся в EEPROM, true - если успешно
bool loadSettingsFromEEPROM();

//Записать текущие настройки в EEPROM
void updateSettingsInEEPROM();

// Макрос вывода значения переменной для отладки
#define Serial_DEBUG(x)      \
  do                         \
  {                          \
    Serial.print("DEBUG: "); \
    Serial.print(#x);        \
    Serial.print(" = ");     \
    Serial.println(x);       \
  } while (0)

// Макросс. Если число выходит за крайние границы приравниет к краям
#define RANGE(x, min, max) (((x) > (min)) ? (((x) < (max)) ? (x) : (max)) : (min))

/*  ____Регулирование мощности____
  переменная увеличивается на 1 каждые TIME_ONE_PERIOD_PID
  Если переменная больше нужной мощности ТЭН выключается, иначе включается

*/

// Время одного шага для регулирования мощности ТЭНа (мс)
#define PID_TIME_ONE_PERIOD 1500

// Максимальное количество регулирования
#define PID_COUNT_POWER_PERIOD 20

// Время отправки сообщений состояний всей системы
#define TIME_INFO 1000

extern uint8_t pin_ten[3];
extern bool btn_active_ten[3];

#if (VERSION_PLATE == 3)
#define PIN_ENC1_A 32
#define PIN_ENC1_B 35
#define PIN_ENC1_BTN 33

#define PIN_ENC2_A 34
#define PIN_ENC2_B 39
#define PIN_ENC2_BTN 36

#define PIN_MIXER_SD  17
#define PIN_MIXER1_IN 4

#define PIN_MIXER2_IN 16

#define PIN_TEN1 21
#define PIN_TEN2 22
#define PIN_TEN3 23

#define PIN_KLAPAN 13

#define PIN_WATER_DETECT 25
#define PIN_STOP_BTN 5

#define PIN_TEMP_WATER 19
#define PIN_TEMP_MILK 18

// Канал ШИМ сигналов на миксер
#define LEDC_MIXER_SPEED 0

//Количество светодиодов в ленте
#define LED_STRIP_COUNT 5
#define LED_ENCODER_COUNT 8
#define COUNT_ENCODERS 2 

#endif // VERSION_PLATE 3

#if (VERSION_PLATE == 4)

#define ANDROID_CHEESE_LED_

//Количество Энкодеров
#define COUNT_ENCODERS 2 

#define PIN_ENC1_A 36
#define PIN_ENC1_B 39
#define PIN_ENC1_BTN 34

#define PIN_ENC2_A 35
#define PIN_ENC2_B 32
#define PIN_ENC2_BTN 33

#define PIN_MIXER_SD  12
#define PIN_MIXER1_IN 14

#define PIN_MIXER2_IN 5

#define PIN_TEN1 17
#define PIN_TEN2 16
#define PIN_TEN3 4

#define PIN_KLAPAN 23

#define PIN_WATER_DETECT 18
#define PIN_STOP_BTN 22

#define PIN_TEMP_WATER 13
#define PIN_TEMP_MILK 15

// Канал ШИМ сигналов на миксер
#define LEDC_MIXER_SPEED 0

#define PIN_LED_ENC1    26
#define PIN_LED_ENC2    25
#define PIN_LED_STRIP   27

//Количество светодиодов в ленте
#define LED_STRIP_COUNT 5
#define LED_ENCODER_COUNT 8

#endif // VERSION_PLATE 4

#if (VERSION_PLATE == 5)

#define ANDROID_CHEESE_LED_

//Количество Энкодеров
#define COUNT_ENCODERS 1 

#define PIN_ENC1_A 16
#define PIN_ENC1_B 17
#define PIN_ENC1_BTN 5

#define PIN_MIXER_HIGH1 27
#define PIN_MIXER_LOW1  26

#define PIN_MIXER_HIGH2 12
#define PIN_MIXER_LOW2  14

// Канал ШИМ сигналов на миксер
#define LEDC_MIXER_1 0
#define LEDC_MIXER_2 1

#define PIN_TEN1 21
#define PIN_TEN2 22
#define PIN_TEN3 23

#define PIN_KLAPAN 2
#define PIN_WATER_DETECT 4
#define PIN_STOP_BTN 36

#define PIN_TEMP_WATER 32
#define PIN_TEMP_MILK 33


//Количество светодиодов в ленте
#define PIN_LED_ENC1    18
#define PIN_LED_STRIP   19
#define LED_STRIP_COUNT 5
#define LED_ENCODER_COUNT 8

#endif // VERSION_PLATE 5

#endif