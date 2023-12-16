#include "CookingMode.h"
#include "Hardware.h"
#include "PID.h"
#include "Serial/decode.h"
#include "Serial/commands.h"
#include "Settings.h"

MixerMode::MixerMode(String input)
{
    int8_t c = input.indexOf(CHAR_NEXT_PARAM);
    if (c > 0)
    {
        speed = input.substring(0, c).toInt();
        seconds_autoreverse = input.substring(c + 1).toInt();
    }
    else
        speed = input.toInt();
};

//  MixerMode
String MixerMode::string()
{
    return COMMAND_MIXER_SPEED + String(speed) + CHAR_NEXT_PARAM + String(seconds_autoreverse);
};

//  BaseCookingMode
#pragma region BaseCookingMode

bool BaseCookingMode::isComplete()
{
    return true;
};
void BaseCookingMode::tick(){};
void BaseCookingMode::start(){};
void BaseCookingMode::end(){};
String BaseCookingMode::string() { return "[]"; };
BaseCookingMode::~BaseCookingMode(){};
#pragma endregion

#pragma region ManualMode
ManualMode::ManualMode(String input)
{
    if (input.length() < 4)
        throw "Argument";
    _bKlapan = (input[0] == 'O');
    _tens[0] = (input[1] == '1');
    _tens[1] = (input[2] == '1');
    _tens[2] = (input[3] == '1');
};
String ManualMode::string()
{
    String message = CM_MANUAL;
    message += _bKlapan ? "O" : "*";
    message += _tens[0] ? "1" : "0";
    message += _tens[1] ? "1" : "0";
    message += _tens[2] ? "1" : "0";
    return message;
};
bool ManualMode::isComplete() { return false; };
void ManualMode::tick()
{
    Hardware.klapanWork(_bKlapan || Hardware.b_waterError);
    Hardware.tenWork(_tens[0], _tens[1], _tens[2]);
};

#pragma endregion

//  EmptyMode
#pragma region EmptyMode

void EmptyMode::tick()
{
    Hardware.disableAll();
};
String EmptyMode::string() { return CM_EMPTY; };

#pragma endregion

//  Автоматический режим
#pragma region HeatingAutoMode

HeatingAutoMode::HeatingAutoMode(String input)
{
    int8_t c = input.indexOf(CHAR_NEXT_PARAM);
    if (c > 0)
    {
        temp = strToF(input.substring(0, c));
        ten = input.substring(c + 1).toInt();
    }
    else
        temp = strToF(input);
};
void HeatingAutoMode::start()
{
    bHeat = Temp::milk < temp;
};
bool HeatingAutoMode::isComplete()
{
    return bHeat ? Temp::milk >= temp : Temp::milk <= temp;
};
void HeatingAutoMode::tick()
{
    if (Temp::milk <= temp)
        Hardware.heat(ten, PID.work(temp, 0, settings.PID.HS_K2, settings.PID.waterHeatProcent));
    else
        Hardware.cold(PID.work(temp, settings.PID.CS_k1, settings.PID.CS_k2));
};

String HeatingAutoMode::string() { return CM_AUTO + fToStr(temp) + CHAR_NEXT_PARAM + String(ten); };

#pragma endregion

// HeatingFastMode
#pragma region HeatingFastMode

HeatingFastMode::HeatingFastMode(String input)
{
    int8_t c = input.indexOf(CHAR_NEXT_PARAM);
    if (c > 0)
    {
        temp = strToF(input.substring(0, c));
        ten = input.substring(c + 1).toInt();
    }
    else
        temp = strToF(input);
};
String HeatingFastMode::string() { return CM_H_FAST + fToStr(temp) + CHAR_NEXT_PARAM + String(ten); };
bool HeatingFastMode::isComplete() { return Temp::milk >= temp; };
void HeatingFastMode::tick()
{
    if (!isComplete())
        Hardware.heat(ten, PID.work(temp, settings.PID.HF_K1, settings.PID.HF_K1, settings.PID.waterHeatProcent));
    else
        Hardware.disableAll();
};

#pragma endregion

// HeatingSlowMode
#pragma region HeatingSlowMode

HeatingSlowMode::HeatingSlowMode(String input)
{
    int8_t c = input.indexOf(CHAR_NEXT_PARAM);
    if (c > 0)
    {
        temp = strToF(input.substring(0, c));
        ten = input.substring(c + 1).toInt();
    }
    else
        temp = strToF(input);
};
String HeatingSlowMode::string() { return CM_H_SLOW + fToStr(temp) + CHAR_NEXT_PARAM + String(ten); };
bool HeatingSlowMode::isComplete() { return Temp::milk >= temp; };
void HeatingSlowMode::tick()
{
    if (Temp::milk < temp)
    {
        Hardware.heat(ten, PID.work(temp, 0, settings.PID.HS_K2, settings.PID.waterHeatProcent));
    }
    else
        Hardware.disableAll();
};

#pragma endregion

// HeatingWaterMode
#pragma region HeatingWaterMode

HeatingWaterMode::HeatingWaterMode(String input)
{
    int8_t c = input.indexOf(CHAR_NEXT_PARAM);
    if (c > 0)
    {
        temp = strToF(input.substring(0, c));
        ten = input.substring(c + 1).toInt();
    }
    else
        temp = strToF(input);
};
String HeatingWaterMode::string() { return CM_H_WATER + fToStr(temp) + CHAR_NEXT_PARAM + String(ten); };
bool HeatingWaterMode::isComplete() { return Temp::water >= temp; };
void HeatingWaterMode::tick()
{
    if (Temp::water < temp)
        Hardware.heat(ten, PID.work(temp, 0, settings.PID.HS_K2, 0, 1));
    else
        Hardware.cold(PID.work(temp, 0, 2));
};

#pragma endregion

// HeatingMaxMode
#pragma region HeatingMaxMode
HeatingMaxMode::HeatingMaxMode(String input)
{
    int8_t c = input.indexOf(CHAR_NEXT_PARAM);
    if (c > 0)
    {
        temp = strToF(input.substring(0, c));
        ten = input.substring(c + 1).toInt();
    }
    else
        temp = strToF(input);
};
String HeatingMaxMode::string() { return CM_H_MAX + fToStr(temp) + CHAR_NEXT_PARAM + String(ten); };
bool HeatingMaxMode::isComplete() { return Temp::milk >= temp; };
void HeatingMaxMode::tick() { Hardware.heat(ten, Temp::milk < temp); };

#pragma endregion

//  CoolingFastMode
#pragma region CoolingFastMode

CoolingFastMode::CoolingFastMode(String input)
{
    temp = strToF(input);
};
String CoolingFastMode::string() { return CM_C_FAST + fToStr(temp); };
void CoolingFastMode::tick() { Hardware.cold(!isComplete()); };
bool CoolingFastMode::isComplete() { return Temp::milk <= temp; };

#pragma endregion

// CoolingSlowMode
#pragma region CoolingSlowMode

CoolingSlowMode::CoolingSlowMode(String input)
{
    temp = strToF(input);
};
String CoolingSlowMode::string() { return CM_C_SLOW + fToStr(temp); };
void CoolingSlowMode::tick()
{
    if (!isComplete())
        Hardware.cold(PID.work(temp, settings.PID.CS_k1, settings.PID.CS_k2, 0, 1));
    else
        Hardware.disableAll();
};
bool CoolingSlowMode::isComplete() { return Temp::milk <= temp; };

#pragma endregion

//  DelayMode
#pragma region DelayMode
DelayMode::DelayMode(String input)
{
    DecodeActionBloc decode(input);
    this->delayedStage = decode.getActionBlock();

    int8_t c = decode.param.indexOf(CHAR_NEXT_PARAM);
    if (c > 0)
    {
        seconds_delay = decode.param.substring(0, c).toInt();
        start_second = decode.param.substring(c + 1).toInt();
    }
    else
        seconds_delay = decode.param.toInt();
};
bool DelayMode::isComplete() { return (millis() - _timeStart) >= (seconds_delay * 1000); };
void DelayMode::end()
{
    bRun = false;
    if (delayedStage != nullptr)
        delayedStage->end();
};
void DelayMode::start()
{
    bRun = true;
    _timeStart = millis() - (start_second * 1000);
    delayedStage->start();
};
void DelayMode::tick()
{
    if (delayedStage != nullptr)
        delayedStage->tick();
};
String DelayMode::string()
{
    String result = CM_T_TIME + String(seconds_delay);

    if (bRun)
    {
        result += CHAR_NEXT_PARAM;
        result += (millis() - _timeStart) / 1000;
    }
    else if (start_second != 0)
    {
        result += CHAR_NEXT_PARAM;
        result += start_second;
    };

    result += CHAR_ACTION_INPUT;
    if (delayedStage != nullptr)
        result += delayedStage->stringAll();

    return result;
};
DelayMode::~DelayMode()
{
    if (delayedStage != nullptr)
    {
        delete delayedStage;
    }
}

#pragma endregion

//  InfinityMode
#pragma region InfinityMode

InfinityMode::InfinityMode(String input)
{
    DecodeActionBloc decode(input);
    this->stage = decode.getActionBlock();
};
bool InfinityMode::isComplete()
{
    return false;
};
void InfinityMode::tick()
{
    if (stage != nullptr)
        stage->tick();
};
void InfinityMode::start()
{
    stage->start();
};
String InfinityMode::string()
{
    String result = CM_T_INFINITY;
    result += CHAR_ACTION_INPUT;
    if (stage != nullptr)
        result += stage->stringAll();
    return result;
};
void InfinityMode::end()
{
    if (stage != nullptr)
        stage->end();
}
InfinityMode::~InfinityMode()
{
    if (stage != nullptr)
    {
        delete stage;
    }
}
#pragma endregion