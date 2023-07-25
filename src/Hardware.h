/*
  В этом файле описаны функции для работы с датчиками и для приготовления
*/

#ifndef _ANDROID_CHESSE_COOCKINH_H_
#define _ANDROID_CHESSE_COOCKINH_H_

#include "Serial/USB.h"
#include "Settings.h"

// Класс для управления аппаратной частью нагрева / охлажденния
class CHardware
{
private:
  bool tens_enable[3] = {true, true, true};

  // Функция защиты от ложных сигналов датчика уровня воды/  now стремится к need и при достежении ждет время защиты
  //  need - Если time времени подряд было need
  //. !need - Если было несоотвествие хотя бы при одном срабатывании
  bool bounceWaterError(bool now, bool need, uint16_t time = 1000);

  // Функция защиты от ложных сигналов красной кнопки/  now стремится к need и при достежении ждет время защиты
  //   need - Если time времени подряд было need
  //. !need - Если было несоотвествие хотя бы при одном срабатывании
  bool bounceRedButton(bool now, bool need, uint16_t time);

public:
  // Ошибка воды в рубашке (true если воды нет,  false - вода в рубашке есть)
  bool b_waterError;

  // Нажата ли красная кнопка
  bool b_redBtnPress;

  // Проверка всех кнопок, соеденения с платой и датчика уровня воды
  void parametrsCheck();
  
  // Установить стандартные значения ТЭНов
  void setTen(bool t1, bool t2, bool t3);

  // Работа (открыть/ Закрыть клапан)
  void klapanWork(bool work);

  // Включить / выключить ТЭН с текущими настройками
  void tenWork(bool work);

  // Включить ТЭНы 
  void tenWork(bool t1, bool t2, bool t3);
  
  // Нагревать с учетом датчика уровня воды
  void heat(uint8_t power, bool enable);

  // Охлаждение с выключенными ТЭНамми
  void cold(bool enable);

  // Включить количество ТЭНов
  void tenWork(uint8_t power);

  // Выключить все системы
  void disableAll();
};

extern CHardware Hardware;


#endif