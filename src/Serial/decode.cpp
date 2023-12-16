#include "decode.h"
#include "Arduino.h"
#include "Mixer/Mixer.h"
#include "Hardware.h"
#include "Temp.h"
#include "commands.h"
#include "Cooking/PID.h"

// макрос для преобразовывания символов в число (нужен для switch)
#define S(x) ((uint16_t)(((uint8_t)x[0] << 8) + ((uint8_t)x[1])))

// Float to Strinf - Конвертирование дроби в строку
String fToStr(float value)
{
    return String((int)(value * 10));
}

// String to Float  -   Конвертирование строки в дробное число
float strToF(String str)
{
    return (float)(str.toInt()) / 10;
}


void decode(String input)
{
  input.toUpperCase();
  Serial.println(">>" + input);
  splitAndRunForEach(input, CHAR_SPLIT_COMMAND, oneCommandDecode);
}

void splitAndRunForEach(String input, char split_char, void (*func)(String))
{
  int8_t search;
  String sup_s;

  do
  {
    search = input.indexOf(split_char);
    if (search == -1)
      sup_s = input;
    else
      sup_s = input.substring(0, search);
    func(sup_s);
    input = input.substring(search + 1);

  } while (search > -1);
}

DecodeActionBloc::DecodeActionBloc(String input)
{
  int countInputChar = input.indexOf(CHAR_ACTION_INPUT);

  if (countInputChar < -1)
  {
    param = input;
  }
  else
  {
    param = input.substring(0, countInputChar);                             // input.substring(0, countInputChar - 1);
    _actionBlock = generateActionBlock(input.substring(countInputChar + 1)); // String s2 = input.substring(countInputChar + 1);
  };
};

ActionBlock *DecodeActionBloc::generateActionBlock(String input)
{

  // Посимвольно проходим, ищем ключевые символы и декодируем

  int8_t search = 0;
  String sup_s;

  while (search < input.length())
  {
    if (input[search] == CHAR_ACTION_SPLIT)
    {
      actionCommandDecode(input.substring(0, search));
      input = input.substring(search + 1);
      search = 0;
    }
    else if (input[search] == CHAR_ACTION_INPUT)
    {
      actionCommandDecode(input);
      return new ActionBlock(_cookingMode, _mixerMode);
    }
    else
      search++;
  }

  actionCommandDecode(input);
  return new ActionBlock(_cookingMode, _mixerMode);
};

ActionBlock* DecodeActionBloc:: getActionBlock() {
    _getActionBlock = true;
    return _actionBlock;
  }

DecodeActionBloc::~DecodeActionBloc(){
  if (!_getActionBlock) {
    if (_actionBlock != nullptr) {
      delete _actionBlock;
    }
  }
};

// ###################################################################################################
// ###################################################################################################
// ###################################################################################################
// ###################################################################################################
// ###################################################################################################

// Декодоривание одной этапной команды
void DecodeActionBloc::actionCommandDecode(String input)
{
  char command[3];
  input.toCharArray(command, 3);
  String param = input.substring(2);

  // Ст
  switch (S(command))
  {
  case S(CM_MIXER):
    if (_mixerMode == nullptr)
      _mixerMode = new MixerMode(param);
    break;

  case S(CM_H_FAST):
    if (_cookingMode == nullptr)
      _cookingMode = new HeatingFastMode(param);
    break;

  case S(CM_H_SLOW):
    if (_cookingMode == nullptr)
      _cookingMode = new HeatingSlowMode(param);
    break;
    break;

  case S(CM_H_WATER):
    if (_cookingMode == nullptr)
      _cookingMode = new HeatingWaterMode(param);
    break;
    break;

  case S(CM_H_MAX):
    if (_cookingMode == nullptr)
      _cookingMode = new HeatingMaxMode(param);
    break;
    break;

  case S(CM_C_FAST):
    if (_cookingMode == nullptr)
      _cookingMode = new CoolingFastMode(param);
    break;

  case S(CM_C_SLOW):
    if (_cookingMode == nullptr)
      _cookingMode = new CoolingSlowMode(param);
    break;

  case S(CM_T_TIME):
  {
    DecodeActionBloc decode(param);
    if (_cookingMode == nullptr)
      _cookingMode = new DelayMode(param);
  };
  break;

  case S(CM_T_INFINITY):
  {
    DecodeActionBloc decode(param);
    if (_cookingMode == nullptr)
      _cookingMode = new InfinityMode(param);
  };
  break;

  case S(CM_MANUAL):
    if (_cookingMode == nullptr)
      _cookingMode = new ManualMode(param);
    break;

  case S(CM_EMPTY):
    if (_cookingMode == nullptr)
      _cookingMode = new EmptyMode();
    break;

  default:
  {
    if (_cookingMode != nullptr)
      delete _cookingMode;
    if (_mixerMode != nullptr)
      delete _mixerMode;
    throw "Unknown Mode";
  }
  }
};

// ###################################################################################################
// ###################################################################################################
// ###################################################################################################
// ###################################################################################################
// ###################################################################################################

// Декодирование одной общей команды
void oneCommandDecode(String input)
{
  try
  {

    char command[3];
    input.toCharArray(command, 3);
    String param = input.substring(2);

    switch (S(command))
    {

    case S(COMMAND_CALIBRATE_STRING):
      Temp::calibrate(param);
    break;

    // Calibrate MILK HIGH
    case S(COMMAND_CALIBRATE_MILK_HIGH):
      Temp::calibrate(settings.CalibrateMilk[1], Temp::milkRawData, strToF(param));
      // serialSend("CalibrateMilk HIGH in :", String(t));
    break;

    // Calibrate MILK LOW
    case S(COMMAND_CALIBRATE_MILK_LOW):
      Temp::calibrate(settings.CalibrateMilk[0], Temp::milkRawData, strToF(param));
      // serialSend("Calibrate Milk LOW in :", String(t));
    break;

    // Calibrate WATER HIGH
    case S(COMMAND_CALIBRATE_WATER_HIGH):
      Temp::calibrate(settings.CalibrateWater[1], Temp::waterRawData, strToF(param));
      // serialSend("Calibrate Water HIGH in :", String(t));
    break;

    // Calibrate WATER LOW
    case S(COMMAND_CALIBRATE_WATER_LOW):
      Temp::calibrate(settings.CalibrateWater[0], Temp::waterRawData, strToF(param));
      // serialSend("Calibrate Water LOW in :", String(t));
    break;

    // Sensors Reset
    case S(COMMAND_CALIBRATE_RESET):
      Temp::resetCalibrate();
      // serialSend("Calibrate Sensors Reset");
      break;

    case S(COMMAND_CALIBRATE_WRITE):
      serialSend(Temp::stringCalibrate());
    break;
    // OK (Соеденение платы и Android успешно)
    case S("OK"):
      timeConnect = millis() + TIME_LOST_CONNECT;
      break;


    case S(COMMAND_INFO_RAW_TEMPERATURE):
      serialSend(Temp::stringRaw());
    break;

    case S(COMMAND_PID_WRITE):
      serialSend(PID.stringKoef());
    break;

    case S(COMMAND_PID_SETTINGS):
      PID.setKoef(param);
    break;

    // Сохранить коэффиценты PID в  EEPROM
    case S(COMMAND_PID_SAVE):
      updateSettingsInEEPROM();
      serialSend("PID koef save in EEPROM");
      break;

    // Сбросить коэффиценты
    case S(COMMAND_PID_RESET):
    {
      str_settings s1;
      settings = s1;
      // serialSend("PID koef reset");
    }
    break;
    // Вывести информацию
    case S(COMMAND_INFO_WRITE):
      printInfo();
      break;

    // Mixer Freeze
    case S(COMMAND_MIXER_FREEZE):
      // Serial.println("Mixer Freeze");
      Mixer.stop();
      break;

    // Mixer Speed **
    case S(COMMAND_MIXER_SPEED):
    {
      int8_t p = (int8_t)RANGE(param.toInt(), 0, 100);
      Mixer.setSpeed(p);

      // serialSend("Mixer Speed set ", String(p));
    }
    break;

    // Mixer Run (start)
    case S(COMMAND_MIXER_RUN):
      // serialSend("Mixer Start");
      Mixer.start();
      break;

    // Mixer Change direction
    case S(COMMAND_MIXER_CHANGE_DIRECTION):
      // serialSend("Mixer Change direction");
      Mixer.changeDir();
      break;

    // Mixer Direction *
    case S(COMMAND_MIXER_DIRECTION):
    {
      bool p = (bool)param.toInt();
      // serialSend("Mixer Direction Set: ", String(p));

      Mixer.setDir(p);
    }
    break;

    // Mixer time AutoReverse ****
    case S(COMMAND_MIXER_AUTOREVERSE):
    {
      uint32_t t = (uint32_t)param.toInt();
      Mixer.setTimeAutoReverse(t);
      // serialSend("Mixer Autoreverse time = ", String((uint32_t)t));
    }
    break;

    // Добавить этап под заданным номером, если не написать номер, то добавится в конец
    case S(COMMAND_RECIPE_ADD):
    {
      DecodeActionBloc decode(param);
      if (decode.param == "")
        RecipeController.addLast(decode.getActionBlock());
      else
        RecipeController.addIndex(decode.param.toInt(), decode.getActionBlock());
    }
    break;

    // Удалить этап под заданным номером, если не написать номер, то удалиться последний
    case S(COMMAND_RECIPE_SUB):
      if (param == "")
        RecipeController.deleteLast();
      else
        RecipeController.deleteIndex(param.toInt());
      break;

    // Изменить этап под заданным номером
    case S(COMMAND_RECIPE_INDEX):
    {
      DecodeActionBloc decode(param);
      RecipeController.changeIndex(decode.param.toInt(), decode.getActionBlock());
    }
    break;

    // Вывести все этапы рецепта
    case S(COMMAND_RECIPE_WRITE):
      RecipeController.printRecipe();
      break;

    // Очистить рецепт
    case S(COMMAND_RECIPE_CLEAR):
      RecipeController.clear();
      break;

    // Вывести количество этапов в рецепте
    case S(COMMAND_RECIPE_SIZE):
      Serial.println(COMMAND_RECIPE_SIZE + String(RecipeController.size()));
      break;

    // Вывести текущий этап
    case S(COMMAND_RECIPE_COUNT):
      Serial.println(COMMAND_RECIPE_COUNT + String(RecipeController.countStage));
      break;

    // Перейти на следующий этап
    case S(COMMAND_RECIPE_NEXT):
      RecipeController.nextStage();
      break;

    // Остановить приготовление
    case S(COMMAND_RECIPE_STOP):
      RecipeController.finishCooking();
      break;

    // Начать приготовление
    case S(COMMAND_RECIPE_START):
      RecipeController.startCooking();
      break;

    // Поставить приготовление на паузу
    case S(COMMAND_RECIPE_PAUSE):
      RecipeController.pause(true);
      break;

    // Возобновить приготовление
    case S(COMMAND_RECIPE_CONTINUE):
      RecipeController.pause(false);
      break;

    // Перейти к этапу под номером
    case S(COMMAND_RECIPE_NUMBER):
      RecipeController.setStage(param.toInt());
      break;

    // Войти в ручной режим
    case S(COMMAND_RECIPE_MANUAL):{
      DecodeActionBloc decode(param);
      RecipeController.manualStart(decode.getActionBlock());
    };
      break;

    // Выйти из режима ручного управления
    case S(COMMAND_RECIPE_AUTO):
      RecipeController.manualDisable();
    break;


    default:
      throw "Unknown command";
    };
  }
  catch (const char *error_message)
  {
    Serial.println("ER: " + String(error_message) + " : " + input);
  }
  catch (...)
  {
    Serial.println("ER : Unknown Error " + input);
  };
};
