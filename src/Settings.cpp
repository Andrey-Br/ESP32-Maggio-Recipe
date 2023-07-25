#include "Settings.h"

str_settings settings;

bool loadSettingsFromEEPROM(){
  if (EEPROM.readByte(0) != 100) return false;
  if (EEPROM.readByte(1) != sizeof(settings)) return false;
  EEPROM.get(2,settings);
  return true;
};


void updateSettingsInEEPROM() {
  //обозначаем, что настройки сохранены
  EEPROM.writeByte(0,100);
  
  //Записываем размер данных (если в процессе отладки переделаем настройки то подстроится)
  EEPROM.writeByte(1,sizeof(settings));
  
  //Записываем сам файл настроек
  EEPROM.put(2, settings);

  EEPROM.commit();

};