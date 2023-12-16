#include "Temp.h"
#include "Serial/commands.h"
#include "Serial/decode.h"

// оригинальная функция map, переделанная под float. Нужна для калибровки датчика
float map_f(float x, float in_min, float in_max, float out_min, float out_max)
{
  const float run = in_max - in_min;
  if (run == 0)
  {
    return -1; // AVR returns -1, SAM returns 0
  }
  const float rise = out_max - out_min;
  const float delta = x - in_min;
  return (delta * rise) / run + out_min;
}


OneWire Temp::ds_milk = OneWire(PIN_TEMP_MILK);
OneWire Temp::ds_water = OneWire(PIN_TEMP_WATER);

float Temp::milk = 0;
float Temp::water = 0;
float Temp::milkRawData = 0;
float Temp::waterRawData = 0;

float Temp::getDallas(OneWire &ds)
{
  ds.reset();        // сброс шины
  ds.write(0xCC, 0); // пропуск ROM
  ds.write(0xBE, 0); // команда чтения памяти датчика
  uint8_t bufData[9];
  ds.read_bytes(bufData, 9); // чтение памяти датчика, 9 байтов

  delayMicroseconds(500);

  ds.reset();        // сброс шины
  ds.write(0xCC, 0); // пропуск ROM
  ds.write(0x44, 0); // инициализация измерения

  uint64_t stitch = ((bufData[1]) << 8) | bufData[0];

  // Если отрицательное, то переводим из дополнительного кода
  if (bufData[1] & B10000000)
  {
    stitch = (0xFFFF << 16) + stitch;
  }

  float temp = (float)(int)stitch * 0.0625 + 0.03125;

  int temp_i = (int)(temp * 100);

  if (temp_i % 10 >= 5)
    temp_i += 10;

  temp_i = temp_i / 10;

  temp = (float)temp_i / 10;

  return temp;
};

void Temp::updateTemp(bool guard)
{

  milkRawData = getDallas(ds_milk);
  milk = map_f(milkRawData, settings.CalibrateMilk[0].in, settings.CalibrateMilk[1].in, settings.CalibrateMilk[0].out, settings.CalibrateMilk[1].out);

  waterRawData = getDallas(ds_water);
  water = map_f(waterRawData, settings.CalibrateWater[0].in, settings.CalibrateWater[1].in, settings.CalibrateWater[0].out, settings.CalibrateWater[1].out);
};

void Temp::tick()
{
  static int64_t time_temp = millis();

  if (time_temp < millis())
  {
    time_temp = millis() + TIME_RESET_TEMP_SENSOR;

    updateTemp(false);
  };
};

void Temp::calibrate(str_Calibrate &Calibr, float inputTemp, float needTemp)
{
  Calibr.in = inputTemp;
  Calibr.out = needTemp;

  updateSettingsInEEPROM();
};

void Temp::calibrate(String input)
{
  String command = input.substring(0, 2);
  String param = input.substring(2);
  int8_t c = param.indexOf(CHAR_NEXT_PARAM);
  String param1;
  String param2;

  if (c > 0)
  {
    param1 = param.substring(0, c);
    param2 = param.substring(c + 1);
  }

  if (command.length() < 2)
    throw "Argument";

  switch (S(command))
  {
  case S("ML"):
    if (c > 0)
    {
      calibrate(settings.CalibrateMilk[0], strToF(param1), strToF(param2));
    }
    else
    {
      calibrate(settings.CalibrateMilk[0], Temp::milkRawData, strToF(param));
    }
    break;

  case S("MH"):
    if (c > 0)
    {
      calibrate(settings.CalibrateMilk[1], strToF(param1), strToF(param2));
    }
    else
    {
      calibrate(settings.CalibrateMilk[1], Temp::milkRawData, strToF(param));
    }
    break;

  case S("WL"):
    if (c > 0)
    {
      calibrate(settings.CalibrateWater[0], strToF(param1), strToF(param2));
    }
    else
    {
      calibrate(settings.CalibrateWater[0], Temp::waterRawData, strToF(param));
    }
    break;

  case S("WH"):
    if (c > 0)
    {
      calibrate(settings.CalibrateWater[1], strToF(param1), strToF(param2));
    }
    else
    {
      calibrate(settings.CalibrateWater[1], Temp::waterRawData, strToF(param));
    }
    break;
  };
};

void Temp::resetCalibrate()
{
  str_settings standart;

  settings.CalibrateMilk[0].in = standart.CalibrateMilk[0].in;
  settings.CalibrateMilk[0].out = standart.CalibrateMilk[0].out;
  settings.CalibrateMilk[1].in = standart.CalibrateMilk[1].in;
  settings.CalibrateMilk[1].out = standart.CalibrateMilk[1].out;

  settings.CalibrateWater[0].in = standart.CalibrateWater[0].in;
  settings.CalibrateWater[0].out = standart.CalibrateWater[0].out;
  settings.CalibrateWater[1].in = standart.CalibrateWater[1].in;
  settings.CalibrateWater[1].out = standart.CalibrateWater[1].out;

  updateSettingsInEEPROM();
};

String Temp::toString()
{
  String message;

  message += COMMAND_INFO_TEMP_MILK;
  message += (int32_t)(milk * 10);
  message += CHAR_SPLIT_COMMAND;
  message += COMMAND_INFO_TEMP_WATER;
  message += (int32_t)(water * 10);

  return message;
};

String Temp::stringCalibrate() {
      String str;
      str += "MH";
      str += fToStr(settings.CalibrateMilk[1].in);
      str += CHAR_NEXT_PARAM;
      str += fToStr(settings.CalibrateMilk[1].out);
      str += CHAR_SPLIT_COMMAND;

      str += "ML";
      str += fToStr(settings.CalibrateMilk[0].in);
      str += CHAR_NEXT_PARAM;
      str += fToStr(settings.CalibrateMilk[0].out);
      str += CHAR_SPLIT_COMMAND;

      str += "WH";
      str += fToStr(settings.CalibrateWater[1].in);
      str += CHAR_NEXT_PARAM;
      str += fToStr(settings.CalibrateWater[1].out);
      str += CHAR_SPLIT_COMMAND;

      str += "WL";
      str += fToStr(settings.CalibrateWater[0].in);
      str += CHAR_NEXT_PARAM;
      str += fToStr(settings.CalibrateWater[0].out);
      str += CHAR_SPLIT_COMMAND;

      return str;
}

String Temp::stringRaw(){
  String message;

  message += "M?";
  message += fToStr(milkRawData);
  message += CHAR_SPLIT_COMMAND;
  message += "W?";
  message += fToStr(waterRawData);

  return message;
}