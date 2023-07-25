#include "Serial/USB.h"

#include "Bluetooth/Bluetooth.h"
#include "Serial/commands.h"

uint64_t timeConnect = 0;

void serialSend(String s1, String s2)
{
  Serial.println(s1 + s2);
  bluetothSend(s1 + s2);
}

// Вывести информацию о датчиках и кнопках
void printInfo()
{
  String message = "";

  // if (Cooking.b_autoCooking == false) {
  //   if (Cooking.b_ten) {
  //     message+="T+";
  //     message+= CHAR_SPLIT_COMMAND;
  //   };
  //   if (Cooking.b_klapan){
  //     message+="KO";
  //     message+= CHAR_SPLIT_COMMAND;
  //   };
  // }

  // Температура
  message += Temp::toString();
  message += CHAR_SPLIT_COMMAND;

  // Уровень воды
  message += Hardware.b_waterError ? COMMAND_INFO_WATER_ERROR : COMMAND_INFO_WATER_FULL;
  message += CHAR_SPLIT_COMMAND;

  // СТОП Кнопка
  message += Hardware.b_redBtnPress ? COMMAND_INFO_STOP_BUTTON_PRESS : COMMAND_INFO_STOP_BUTTON_RELEASE;
  message += CHAR_SPLIT_COMMAND;

  // //ТЭН
  // message += "TN";
  // for (uint8_t i = 0; i < 3; i++)
  // {
  //   message += ten_power[i] ? '1' : '0';
  // }
  // message += CHAR_SPLIT_COMMAND;

  // Mixer
  message += COMMAND_MIXER_SPEED;
  message += Mixer.getNeedSpeed();
  message += CHAR_SPLIT_COMMAND;
  message += COMMAND_MIXER_AUTOREVERSE;
  message += (uint16_t)(Mixer.getPeriodAutoreverse() / 1000);
  message += CHAR_SPLIT_COMMAND;

  message += COMMAND_RECIPE_SIZE;
  message += RecipeController.size();
  message += CHAR_SPLIT_COMMAND;

  if (RecipeController.bAutoCooking)
  {
    message += COMMAND_RECIPE_START;
    message += CHAR_SPLIT_COMMAND;

    message += COMMAND_RECIPE_COUNT;
    message += RecipeController.countStage;
    message += CHAR_SPLIT_COMMAND;

    if (RecipeController.bPause)
    {
      message += COMMAND_RECIPE_PAUSE;
      message += CHAR_SPLIT_COMMAND;
    };

    message += RecipeController.stringCurrentMode();
  }
  else
  {
    message += COMMAND_RECIPE_FINISH;
  }

  if (((RecipeController.bAutoCooking == false) || (RecipeController.bPause == true)) && RecipeController.bManualModeEnable)
  {
    message += CHAR_SPLIT_COMMAND;

    message += COMMAND_RECIPE_MANUAL;
    message += CHAR_ACTION_INPUT;
    if ( RecipeController.manualAction != nullptr ) message += RecipeController.manualAction->stringAll();

  }

  // //Нагрев
  // // STate текущий режим платы
  // message += "ST";
  // message += Cooking._mode;
  // message += CHAR_SPLIT_COMMAND;

  // message += "HS";
  // message += (int16_t)(Cooking.need_temp_heat * 10);

  // message += CHAR_SPLIT_COMMAND;
  // message += "TS";
  // message += (uint16_t)(Cooking.need_delay_heat);

  // message += CHAR_SPLIT_COMMAND;
  // message += "KS";
  // message += (int16_t)(Cooking.need_temp_cooling * 10);

  // message += CHAR_SPLIT_COMMAND;
  // message += "HD";
  // message += (int16_t)(Cooking.need_temp_infinity_dealy * 10);

  // //Если выдержка
  // if (Cooking._mode == 2) {
  //   message += CHAR_SPLIT_COMMAND;
  //   message += "TL";
  //   message += (int16_t)((millis()  - Cooking._time_delay) / 1000);
  //   }

  // if (Cooking.b_autoCooking) {
  //   message += CHAR_SPLIT_COMMAND;
  //   message += "PV";
  //   message +=(int16_t)(Cooking._PID_result);
  // }

  serialSend(message);
}

void checkSerial()
{
  if (Serial.available() == 0)
    return;
  String data;
  while (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c != '\n')
      data += c;
    // чтобы не пропустить символы, без него были ложные деления на подстроки
    delayMicroseconds(100);
  }

  // serialSend(">>", data);
  decode(data);
}
