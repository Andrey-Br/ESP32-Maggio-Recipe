
#include "Bluetooth_Classic.h"
#include "Serial/USB.h"

#ifndef _ANDROID_CHEESE_USE_BLE_

// Объект bluetoth
BluetoothSerial ESP_BT;

//иницализация Bluetoth
void bluetothInit()
{
  if (ESP_BT.begin("MAGGIO " + String(VERSION_PLATE)))
    Serial.println("Bluetoth inited!");
  else
    Serial.println("Bluetoth error!");
};

String bt_input_string = "";

// Обработка Bluetoth
void bluetothTick()
{
  while (ESP_BT.available()) // Check if we receive anything from Bluetooth
  {
    int input_bt = ESP_BT.read(); // Read what we recevive

    // конец строки
    if (input_bt != '\n')
      bt_input_string += (char)input_bt;
    else
    {
      serialSend("bt>>" , bt_input_string );
      decode(bt_input_string);

      bt_input_string = "";
    };
  };
};

// Отправить по Bluetoth
void bluetothSend(String s)
{
  ESP_BT.println(s);
}

#endif

