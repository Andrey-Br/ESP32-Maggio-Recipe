#ifndef _COOKING_MODE_H_
#define _COOKING_MODE_H_

#include "Arduino.h"
#include "CookingController.h"




// Сообщаем что где то будет ActionBlock
class ActionBlock;

// Класс позволяющий задать параметры мешалки в этапе приготовления
#define CM_MIXER "MS"
class MixerMode
{
public:
  MixerMode (String input);
  int speed = 0;
  int seconds_autoreverse = 0;

  String string();
};

// Базовый класс режима нагрева/охлаждения, от него наследуюся все режимы
class BaseCookingMode
{
public:
  virtual bool isComplete();
  virtual void tick();
  virtual void start();
  virtual String string();
  virtual void end();
};

//  Пустой режим
#define CM_EMPTY "HE"
class EmptyMode : public BaseCookingMode
{
  void tick() override;
  String string() override;
};

//  Ручной режим
#define CM_MANUAL "H!"
class ManualMode : public BaseCookingMode
{
private:
  bool _bKlapan = false;
  bool _tens[3] = {false, false, false};

public:
  ManualMode(String input);
  bool isComplete() override;
  void tick() override;
  String string() override;
};


// #####################################################################
// #########################    Нагрев    ##############################
// #####################################################################

//  Автоматический режим
#define CM_AUTO "HA"
class HeatingAutoMode : public BaseCookingMode
{
public:
  uint8_t ten = 3;
  bool bHeat;
  float temp;
  HeatingAutoMode(String input);
  bool isComplete() override;
  void tick() override;
  String string() override;
  void start() override;
};

//  Быстрый нагрев молока с перегревом рубашки
#define CM_H_FAST "HF"
class HeatingFastMode : public BaseCookingMode
{
public:
  uint8_t ten = 3;
  float temp;
  HeatingFastMode(String input);
  bool isComplete() override;
  void tick() override;
  String string() override;
};

//  Медленный нагрев молока без перегрева рубашки
#define CM_H_SLOW "HS"
class HeatingSlowMode : public BaseCookingMode
{
public:
  uint8_t ten = 3;
  float temp;
  HeatingSlowMode(String input);
  bool isComplete() override;
  void tick() override;
  String string() override;
};

//  Нагрев рубашки
#define CM_H_WATER "HW"
class HeatingWaterMode : public BaseCookingMode
{
public:
  uint8_t ten = 3;
  float temp;
  HeatingWaterMode(String input);
  bool isComplete() override;
  void tick() override;
  String string() override;
};

//  Максимальный нагрев
#define CM_H_MAX "HM"
class HeatingMaxMode : public BaseCookingMode
{
public:
  uint8_t ten = 3;
  float temp;
  HeatingMaxMode(String input);
  bool isComplete() override;
  void tick() override;
  String string() override;
};

// #####################################################################
// #####################################################################
// #####################################################################

//  Быстрое охлаждение
#define CM_C_FAST "CF"
class CoolingFastMode : public BaseCookingMode
{
public:
  float temp;
  CoolingFastMode(String input);
  bool isComplete() override;
  void tick() override;
  String string() override;
};

//  Медленное импульсное охлаждение
#define CM_C_SLOW "CS"
class CoolingSlowMode : public BaseCookingMode
{
public:
  float temp;
  CoolingSlowMode(String input);
  bool isComplete() override;
  void tick() override;
  String string() override;
};

// #####################################################################
// #####################################################################
// #####################################################################

//  Выдержка по времени
#define CM_T_TIME "HT"
class DelayMode : public BaseCookingMode
{
private:
  bool bRun = false;
  uint32_t _timeStart;
  uint32_t start_second = 0;

public:
  // DelayMode(ActionBlock *delayedStage, int seconds_delay = 0);
  DelayMode(String input);
  ActionBlock *delayedStage;
  uint32_t seconds_delay;
  bool isComplete() override;
  void tick() override;
  void start() override;
  String string() override;
  void end () override;
};

//  Бесконечный режим
#define CM_T_INFINITY "HI"
class InfinityMode : public BaseCookingMode
{
  ActionBlock *stage;
public:
  InfinityMode(String input);
  bool isComplete() override;
  void tick() override;
  void start() override;
  void end() override;
  String string() override;

};

#endif
