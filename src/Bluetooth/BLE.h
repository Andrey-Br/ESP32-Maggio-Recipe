#ifndef _ANDROID_CHEESE_BLE_H
#define _ANDROID_CHEESE_BLE_H

#include "Bluetooth.h"

#ifdef _ANDROID_CHEESE_USE_BLE_

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "Serial/USB.h"

#define UUID_SERVICE "f4aaf878-43cb-11ed-b878-0242ac120002"

#define UUID_pChr_INPUT     "f4aafca6-43cb-11ed-b878-0242ac120002"
#define UUID_pChr_OUTPUT    "f4ab143e-43cb-11ed-b878-0242ac120002"
#define UUID_pChr_PID       "f4ab1664-43cb-11ed-b878-0242ac120002"


// Инициализация Bluetoth
void bluetothInit();

// Обработка Bluetoth
void bluetothTick();

// Отправить по Bluetoth
void bluetothSend(String s);

#endif

#endif