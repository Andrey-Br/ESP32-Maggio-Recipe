#ifndef _COMMANDS_H_
#define _COMMANDS_H_



// Символ разделения команд
#define CHAR_SPLIT_COMMAND '_'
// Символ ввода процесса приготовления
#define CHAR_ACTION_INPUT '>'
// Символ разделения этапа приготовления
#define CHAR_ACTION_SPLIT '~'
// Символ ввода следующего параметра
#define CHAR_NEXT_PARAM '/'


//  Изменить этапа рецепта под номером
#define COMMAND_RECIPE_INDEX  "R:"
//  Добавить этап в рецепт 
#define COMMAND_RECIPE_ADD    "R+"
//  Удалить этап из рецепта
#define COMMAND_RECIPE_SUB    "R-"
//  Узнать количество элементов
#define COMMAND_RECIPE_SIZE   "R*"
//  Текущий этап рецепта
#define COMMAND_RECIPE_COUNT  "R."
//  Начать приготовеление
#define COMMAND_RECIPE_START  "RC"
//  Останвить приготовление рецепта
#define COMMAND_RECIPE_STOP   "RS"
//  Приостановить приготовление рецепта
#define COMMAND_RECIPE_PAUSE  "RP"
//  Продолжить приготовление рецепта
#define COMMAND_RECIPE_CONTINUE "RG"
//  Вывести рецепт
#define COMMAND_RECIPE_WRITE  "RW"
//  Приготовление рецепта заверешенно
#define COMMAND_RECIPE_FINISH "RF"
//  Перейти к этапу под номером
#define COMMAND_RECIPE_NUMBER "RN"
//  Очистить рецепт
#define COMMAND_RECIPE_CLEAR  "R!"
//  Перейти на следующий этап
#define COMMAND_RECIPE_NEXT   "R^"
//  Ручное управление вне рецепта
#define COMMAND_RECIPE_MANUAL "RM"
//  Выключить ручной режим
#define COMMAND_RECIPE_AUTO   "RA"



// Вывести информацию
#define COMMAND_INFO_WRITE "??"
//  СТОП кнопка нажата
#define COMMAND_INFO_STOP_BUTTON_PRESS   "B!"
//  СТОП кнопка не нажата
#define COMMAND_INFO_STOP_BUTTON_RELEASE "B*"
//  Нет воды в рубашке
#define COMMAND_INFO_WATER_ERROR    "W!"
//  Рубашка полная
#define COMMAND_INFO_WATER_FULL     "W*"
//  Температура молока
#define COMMAND_INFO_TEMP_MILK      "TM"
//  Текмпература воды
#define COMMAND_INFO_TEMP_WATER     "TW"
//  Вывод неоткалиброванных значений с датчика
#define COMMAND_INFO_RAW_TEMPERATURE "T?"

//  Калибровка датчиков по строке
#define COMMAND_CALIBRATE_STRING    "S:"
//  Калибровка верхней границы датчика молока 
#define COMMAND_CALIBRATE_MILK_HIGH "S1"
//  Калибровка нижней границы датчика молока
#define COMMAND_CALIBRATE_MILK_LOW  "S2"
//  Калибровка верхней границы датчика воды
#define COMMAND_CALIBRATE_WATER_HIGH  "S3"
//  Калибровка нижней границы датчика воды
#define COMMAND_CALIBRATE_WATER_LOW   "S4"
//  Сбросить калибровку датчиков
#define COMMAND_CALIBRATE_RESET       "SR"
//  Вывести значения калибровки
#define COMMAND_CALIBRATE_WRITE       "S?"


//  Отключить мешалку
#define COMMAND_MIXER_FREEZE  "MF"
//  Включить мешалку
#define COMMAND_MIXER_RUN     "MR"
//  Скорость мешалки
#define COMMAND_MIXER_SPEED   "MS"
//  Напраление мешалки
#define COMMAND_MIXER_DIRECTION   "MD"
//  Время автореверса
#define COMMAND_MIXER_AUTOREVERSE "MA"
//  Измениьт направление мешалки
#define COMMAND_MIXER_CHANGE_DIRECTION "MC"

//  Настроить пид параметр
#define COMMAND_PID_SETTINGS "P:"
//  Настроить пид параметр
#define COMMAND_PID_WRITE "P?"
// Сохранить пид параметры в EEPROM
#define COMMAND_PID_SAVE "PS"
// Сбросить настройки PID
#define COMMAND_PID_RESET "PR"
#endif
