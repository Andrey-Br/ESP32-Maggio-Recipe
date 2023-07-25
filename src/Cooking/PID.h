#ifndef _COOKING_PID_H_
#define _COOKING_PID_H_

#include "Settings.h"

// Максимальное 
#define PID_MAX_VALUE 20

#define PID_COUNT_STAGE 20 
#define PID_TIME 1500


class C_PID{
private:  
  int counter = 0;
  uint32_t last_time;

public:
  float culc(float need_temp, float k_milk, float k_water, float water_procent = 0);
  bool work(float need_temp, float k_milk, float k_water, float water_procent = 0, uint8_t min = 0);
  void tick();
  void setKoef (String input);
  String stringKoef();
};

extern C_PID PID;

#endif
