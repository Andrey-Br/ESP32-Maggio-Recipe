#ifndef _ANROID_CHEESE_TEMP_H
#define _ANROID_CHEESE_TEMP_H

#include <OneWire.h>

#include "Settings.h"
#include "Serial/USB.h"

//С какой периодичностью оправшиваем датчики температуры
#define TIME_RESET_TEMP_SENSOR 1000

//объект датчика молока
extern OneWire ds_milk;
//объект датчика воды
extern OneWire ds_water;

// //Структура для калибровки датчика in - "сырых" значение с датчика, out - какое должно быть
// struct str_Calibrate{
//   float in, out;
// };


// Класс для взаимодействия с температурами
class Temp {
private:
  //  Получить температуру от датчика
  static float getDallas(OneWire &ds);
  //  Обновить температуры, false Если без защиты от помех
  static void updateTemp(bool guard = true);
  //  Объект датчика молока 
  static  OneWire ds_milk;
  //  Объект датчика воды
  static OneWire ds_water;

public:
  //  Откалибровання температура молока
  static float milk;
  //  Откалибровання температура воды
  static float water;
  //  Неоткалиброванная температура с датчика молока
  static float milkRawData;
  //  Неоткалиброванная температура с датчика воды
  static float waterRawData;
  //  Обновление данных по истечению времени
  static void tick();
  //  Перезаписать структуру калибровки
  static void calibrate(str_Calibrate &Calibr, float inputTemp, float needTemp);
  //  Декодирование и перезапись структуры калибровки
  static void calibrate(String input);
  //  Сбросить калиброку на стандартную
  static void resetCalibrate();
  //  Преобразование температур в строку
  static String toString();
  //  Строка значений калибровки
  static String stringCalibrate();
  //  Вывести сыре данные с датчков
  static String stringRaw();
};

/*

class C_Temp {
private:
//Получить температуру от датчика
float getDallas(OneWire &ds);
//Обновить темпенратуры, false Если без защиты от помех
void updateTemp(bool guard = true);

public:
  float milk = 0;
  float water = 0;
  float milk_non_calibrate = 0;
  float water_nono_calibrate = 0;

  void tick();
  void calibrate(str_Calibrate &Calibr, float inputTemp, float needTemp);
  void calibrate(String s);
  void resetCalibrate();
  String toString();
}

extern C_Temp Temp;

*/

// //текущая температура воды в рубашке
// extern float Temp::water;
// //текущая откалиброванная температура молока
// extern float Temp::milk;

// //"Сырые" значения с датчиков
// extern float Temp::milk_non_calibrate, Temp::water_non_calibrate;

// //Сбросить калибровку датчиков
// void resetCalibrate();

// // Изменить калибровачное значение. При inputtemp будет needtemp
// void setCalibrate(str_Calibrate &Calibr, float inputtemp, float needtemp);


// // оригинальная функция map, переделанная под float. Нужна для калибровки датчика
// float map_f(float x,float in_min, float in_max, float out_min, float out_max);

// //сгенерировать строку для выдачи температуры
// String stringTemp();

// //Получить температуру от датчика
// float getDallas(OneWire &ds);

// //Обновить темпенратуры, false Если без защиты от помех
// void updateTemp(bool guard = true);

// //Получаем темперауры от датчиков
// void temptick();

#endif