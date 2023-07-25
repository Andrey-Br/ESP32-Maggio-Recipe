#include "Bluetooth.h"

#ifndef _ANDROID_CHEESE_USE_BLE_

#include "BluetoothSerial.h"
// Объект bluetoth
extern BluetoothSerial ESP_BT;

// Инициализация Bluetoth
void bluetothInit();

// Обработка Bluetoth
void bluetothTick();

// Отправить по Bluetoth
void bluetothSend(String s);

#endif

