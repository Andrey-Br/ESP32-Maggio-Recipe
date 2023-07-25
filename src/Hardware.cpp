#include "Hardware.h"
#include "Serial/commands.h"

CHardware Hardware;

void CHardware::parametrsCheck()
{

// Плата отключена от Android
#ifdef ACTIVATE_OK_COMMAND
  if (timeConnect <= millis())
  {
    Cooking.disableAll();
    Mixer.stop();
  }
#endif

  bool b;
  // инверсия, потому что при отсутсвии сигнала должно быть true

  // b = !digitalRead(PIN_WATER_DETECT);

  // Защита от дребезга
  b = bounceWaterError(!digitalRead(PIN_WATER_DETECT), false, 2000);

  if (b != b_waterError)
  {
    b_waterError = b;
    serialSend(b_waterError ? COMMAND_INFO_WATER_ERROR: COMMAND_INFO_WATER_FULL);
  }

  // инверсия, потому что INPUT_PULLUP, если digitalRead = false - значит кнопка нажата
  b = !bounceRedButton(!digitalRead(PIN_STOP_BTN), true, 1500);
  if (b != b_redBtnPress)
  {
    b_redBtnPress = b;
    Mixer.reculc();

    serialSend(b_redBtnPress ? COMMAND_INFO_STOP_BUTTON_PRESS : COMMAND_INFO_STOP_BUTTON_RELEASE);
  };

  static uint64_t info_time = millis() + TIME_INFO;
  if (info_time <= millis())
  {
    info_time = millis() + TIME_INFO;
    // TODO:Раскоментировать
    // printInfo();
  };
};

// Функция защиты от ложных сигналов/  now стремится к need и при достежении ждет время защиты
bool CHardware::bounceWaterError(bool now, bool need, uint16_t time)
{
  static uint32_t time_bounce = 0;

  if (now != need)
  {
    time_bounce = millis();
    return !need;
  };

  if (millis() - time_bounce >= time)
    return need;

  return !need;
}

// Функция защиты от ложных сигналов/  now стремится к need и при достежении ждет время защиты
bool CHardware::bounceRedButton(bool now, bool need, uint16_t time)
{
  static uint32_t time_bounce = 0;

  if (now != need)
  {
    time_bounce = millis();
    return !need;
  };

  if (millis() - time_bounce >= time)
    return need;

  return !need;
}

// Установить стандартные значения ТЭНов
void CHardware::setTen(bool t1, bool t2, bool t3)
{
  tens_enable[0] = t1;
  tens_enable[1] = t2;
  tens_enable[2] = t3;
};

// Работа (открыть/ Закрыть клапан)
void CHardware::klapanWork(bool enable)
{
  if (b_redBtnPress)
    digitalWrite(PIN_KLAPAN, false);
  else
    digitalWrite(PIN_KLAPAN, enable);
};

// Включить / выключить ТЭН с текущими настройками
void CHardware::tenWork(bool work)
{
  if (work)
    tenWork(tens_enable[0], tens_enable[1], tens_enable[2]);
  else
    tenWork(false, false, false);
};

// Включить ТЭНы
void CHardware::tenWork(bool t1, bool t2, bool t3)
{
  // Если нажата СТОП или нет воды кнопка ничего не включаем
  if (b_redBtnPress || b_waterError)
  {
    digitalWrite(PIN_TEN1, false);
    digitalWrite(PIN_TEN2, false);
    digitalWrite(PIN_TEN3, false);
    return;
  }

  digitalWrite(PIN_TEN1, t1);
  digitalWrite(PIN_TEN2, t2);
  digitalWrite(PIN_TEN3, t3);
};

// Включить количество ТЭНов
void CHardware::tenWork(uint8_t power)
{
  switch (power)
  {
  case (0):
    tenWork(false, false, false);
    break;
  case (1):
    tenWork(true, false, false);
    break;
  case (2):
    tenWork(false, true, true);
    break;
  case (3):
    tenWork(true, true, true);
    break;
  default:
    tenWork(false, false, false);
  }
};

// Выключить все системы
void CHardware::disableAll()
{
  tenWork(false);
  klapanWork(false);
};

void CHardware::heat(uint8_t power, bool enable)
{
  if (enable)
  {
    if (b_waterError)
    {
      klapanWork(true);
      tenWork(false);
    }
    else
      tenWork(power);
  };
  tenWork(false);
}

// Охлаждение с выключенными ТЭНамми
void CHardware::cold(bool enable)
{
  tenWork(false);
  klapanWork(enable);
};
