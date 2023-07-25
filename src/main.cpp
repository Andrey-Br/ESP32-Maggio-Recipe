#ifndef _ANDROID_CHEESE_MAIN_
#define _ANDROID_CHEESE_MAIN_

#include "Settings.h"
#include "Mixer/Mixer.h"
#include "Serial/USB.h"
#include "Hardware.h"
#include "Mixer/Mixer.h"
#include "Encoders.h"
#include "Temp.h"
#include <EEPROM.h>
#include "Bluetooth/Bluetooth.h"
#include "LEDS.h"
#include "Cooking/CookingController.h"



void setup()
{

  Serial.begin(115200);
  EEPROM.begin(255);
  bluetothInit();

  pinMode(PIN_TEN1, OUTPUT);
  pinMode(PIN_TEN2, OUTPUT);
  pinMode(PIN_TEN3, OUTPUT);

  pinMode(PIN_KLAPAN, OUTPUT);

  pinMode(PIN_WATER_DETECT, INPUT);
  pinMode(PIN_STOP_BTN, INPUT);

  pinMode(PIN_TEMP_MILK, INPUT);
  pinMode(PIN_TEMP_WATER, INPUT);


  if ( ! loadSettingsFromEEPROM() ) {
    Serial.println("Settings not loaded");
    updateSettingsInEEPROM();
  }

  // Cooking.disableAll();
  Hardware.disableAll();

  LedEncoder.strip_start();
}

// uint64_t time_temp = millis();

void loop()
{
  Temp::tick();
  //encodersTick();
  Hardware.parametrsCheck();
  LedEncoder.tick();

  checkSerial();
  bluetothTick();

  Mixer.tick();
  // Cooking.tick();
  RecipeController.tick();

  // // устраняет дребезг кнопок, возможно не нужен, при аппаратной защите
  delay(1);
}

#endif