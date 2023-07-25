#ifndef _ANDROID_CHEESE_USB_
#define _ANDROID_CHEESE_USB_

#include "Hardware.h"
#include "Settings.h"
#include "Mixer/Mixer.h"
#include "Temp.h"

#define SPLIT_CHAR '_'

// Включить ли отключение всего при отключении платы
//  #define ACTIVATE_OK_COMMAND

// Время отсутсввия связи с платой
#define TIME_LOST_CONNECT 5000

extern uint64_t timeConnect;

// макрос для преобразовывания символов в число (нужен для switch)
#define S(x) ((uint16_t)(((uint8_t)x[0] << 8) + ((uint8_t)x[1])))

// Отправить сообщения по проводу и BT
void serialSend(String s1, String s2 = "");

// Формирование строки состояний
void printInfo();

// Проверка данных переданных по проводу
void checkSerial();

#endif
