#ifndef _DECODE_COMMAND_H_
#define _DECODE_COMMAND_H_


#include "Cooking/CookingController.h"
#include "Arduino.h"


// Float to Strinf  -   Конвертирование дроби в строку
String fToStr(float value);

// String to Float  -   Конвертирование строки в дробное число
float strToF(String str);

//Класс который декодирует этап приготовления
class DecodeActionBloc {
private:

  BaseCookingMode *_cookingMode = nullptr;
  MixerMode *_mixerMode = nullptr;

  // Декодоривание одной этапной команды 
  void actionCommandDecode(String input);

  //Генерирует и возвращает указатель на объект блока рецепта
  ActionBlock *generateActionBlock(String input);

  //Сгенерированный ActionBlock
  ActionBlock* _actionBlock = nullptr;
  
  // Забрали ли блок, если нет, то удалим его в деструкторе
  bool _getActionBlock = false;

public:
  DecodeActionBloc(String input);
  ~DecodeActionBloc();
  
  // Параметр который 
  String param = "";
  
  ActionBlock* getActionBlock();

};

// Делит строку на подстроки через каждый split_char, и вызывает func на каждую полученную подстроку
void splitAndRunForEach(String input, char split_char, void (*func)(String));

// //Генерирует и возвращает указатель на объект блока рецепта
// ActionBlock *generateActionBlock(String input);

// Декодирование одной общей команды
void oneCommandDecode(String input);

// Декодирование всей строки с общими командами
void decode(String input);

#endif