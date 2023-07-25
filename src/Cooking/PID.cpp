#include "Temp.h"
#include "PID.h"
#include "CookingMode.h"
#include "Settings.h"
#include "Serial/decode.h"
#include "Serial/commands.h"


C_PID PID;

float C_PID::culc(float need_temp, float k_milk, float k_water, float water_procent){
  // Какая должна быть рубашка
  float need_water = Temp::water * ((100 + water_procent) / 100);
  
  float t1 =  need_temp - Temp::milk;
  float t2 =  need_temp - need_water;

  return ( t1*k_milk + t2*k_water );
};

bool C_PID::work(float need_temp, float k_milk, float k_water, float water_procent, uint8_t min)
{
  float pid_value = culc(need_temp, k_milk, k_water, water_procent);
   
  if (pid_value < min) pid_value = 0;
  if (pid_value > PID_MAX_VALUE) pid_value = PID_MAX_VALUE;
  
  return counter <= map(pid_value, 0, PID_MAX_VALUE, 0, PID_COUNT_STAGE);
}

void C_PID::tick() {
  if (millis() - last_time >= PID_TIME) {
    last_time = millis();
    counter++;
    if (counter > PID_COUNT_STAGE) counter = 1;
  }
}


void C_PID::setKoef(String input){
  if (input.length() < 2) throw "Param error";
  
  char command[3];
  input.toCharArray(command, 3);
  String param = input.substring(2);

  String param1;
  String param2;

  int8_t c = param.indexOf(CHAR_NEXT_PARAM);
    if (c > 0)
    {
        param1 = param.substring(0, c);
        param2 = param.substring(c + 1);
    }

  switch( S(command) ) {
    case S("W:"):
      if (c>=0) throw "Argument";
      settings.PID.waterHeatProcent = strToF(param);
    break;

    case S(CM_H_FAST) :
      if (c < 0) throw "Argument";
      settings.PID.HF_K1 = strToF(param1);
      settings.PID.HF_K2 = strToF(param2);
    break;

    case S(CM_H_SLOW):
      if (c>=0) throw "Argument";
      settings.PID.HS_K2 = strToF(param);
    break;

    case S(CM_AUTO) :
      if (c < 0) throw "Argument";
      settings.PID.HA_Cooling_K1 = strToF(param1);
      settings.PID.HA_Cooling_K2 = strToF(param2);
    break;

    case S(CM_C_SLOW):
      if (c < 0) throw "Argument";
      settings.PID.CS_k1 = strToF(param1);
      settings.PID.CS_k2 = strToF(param2);
    break;
    
  };

  


}


String C_PID::stringKoef(){
  String message;

  message += COMMAND_PID_WRITE;
  message += CHAR_SPLIT_COMMAND;

  message += "W:";
  message += fToStr(settings.PID.waterHeatProcent);
  
  message += CHAR_SPLIT_COMMAND;
  
  message += CM_H_FAST;
  message += fToStr(settings.PID.HF_K1);
  message += CHAR_NEXT_PARAM;
  message += fToStr(settings.PID.HF_K2);

  message += CHAR_SPLIT_COMMAND;

  message += CM_H_SLOW;
  message += fToStr(settings.PID.HS_K2);

  message += CHAR_SPLIT_COMMAND;

  message += CM_AUTO;
  message += fToStr(settings.PID.HA_Cooling_K1);
  message += CHAR_NEXT_PARAM;
  message += fToStr(settings.PID.HA_Cooling_K2);

  message += CHAR_SPLIT_COMMAND;

  message += CM_C_SLOW;
  message += fToStr(settings.PID.CS_k1);
  message += CHAR_NEXT_PARAM;
  message += fToStr(settings.PID.CS_k2);

  return message;
}