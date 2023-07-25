#include "Mixer.h"

C_Mixer Mixer;

C_Mixer::C_Mixer(){


  #if (VERSION_PLATE <= 2)
    pinMode(PIN_MIXER1_IN, OUTPUT);
    pinMode(PIN_MIXER2_IN, OUTPUT);
    pinMode(PIN_MIXER1_SD, OUTPUT);
    pinMode(PIN_MIXER2_SD, OUTPUT);

    ledcSetup(LEDC_MIXER_SPEED, 40000, 8);
    // ledcSetup(LEDC_MIXER2,40000,8);
    ledcWrite(LEDC_MIXER_SPEED, 0);
    // ledcWrite(LEDC_MIXER2,0);
    ledcAttachPin(PIN_MIXER1_SD, LEDC_MIXER_SPEED);
    ledcAttachPin(PIN_MIXER2_SD, LEDC_MIXER_SPEED);

  #elif (VERSION_PLATE == 3)

  #elif (VERSION_PLATE == 4) 
    
    pinMode(PIN_MIXER1_IN, OUTPUT);
    pinMode(PIN_MIXER2_IN, OUTPUT);
    pinMode(PIN_MIXER_SD, OUTPUT);

    ledcSetup(LEDC_MIXER_SPEED, MIXER_FREQUENCY, MIXER_PWM_BITS);
    // ledcSetup(LEDC_MIXER2,40000,8);
    ledcWrite(LEDC_MIXER_SPEED, 0);
    // ledcWrite(LEDC_MIXER2,0);
    ledcAttachPin(PIN_MIXER_SD, LEDC_MIXER_SPEED);

  #elif (VERSION_PLATE == 5)

    pinMode(PIN_MIXER_HIGH1,OUTPUT);
    pinMode(PIN_MIXER_LOW1,OUTPUT);
    pinMode(PIN_MIXER_HIGH2,OUTPUT);
    pinMode(PIN_MIXER_LOW2,OUTPUT);

    ledcSetup(LEDC_MIXER_1, MIXER_FREQUENCY, MIXER_PWM_BITS);
    ledcWrite(LEDC_MIXER_1, 0);
    ledcAttachPin(PIN_MIXER_HIGH1,LEDC_MIXER_1);
    ledcAttachPin(PIN_MIXER_LOW2,LEDC_MIXER_1);

    ledcSetup(LEDC_MIXER_2, MIXER_FREQUENCY, MIXER_PWM_BITS);
    ledcWrite(LEDC_MIXER_2, 0);
    ledcAttachPin(PIN_MIXER_HIGH2,LEDC_MIXER_2);
    ledcAttachPin(PIN_MIXER_LOW1,LEDC_MIXER_2);

  #endif
};

bool C_Mixer::isStop()
{
  return !_mixer_work || Hardware.b_redBtnPress;
}

void C_Mixer::work(int8_t speed, bool rev)
{

  #if (VERSION_PLATE == 5)

  if (speed > 100) speed = 100;

  if (speed < 0) speed = 0;

  // Если красная кнопка нажата
  if (Hardware.b_redBtnPress)
  {
    
    ledcWrite(LEDC_MIXER_1, 0);
    ledcWrite(LEDC_MIXER_2, 0);    
    // ledcWrite(LEDC_MIXER_SPEED, 00);

    _current_speed = 0;
    _mode = 0;
    return;
  }

  else
  {
    uint16_t pwm; 
    if (speed <= 0 ) pwm = 0;
    else if (speed >= 99 ) pwm = 255;
    else pwm = map(speed, 0, 100, MIXER_START_SPEED, MIXER_END_SPEED);
    
    if (rev) {
      ledcWrite(LEDC_MIXER_2, 0);
      ledcWrite(LEDC_MIXER_1, pwm);
    
    } else {
      ledcWrite(LEDC_MIXER_1, 0);
      ledcWrite(LEDC_MIXER_2, pwm);
    }

  }

  #endif //Version_PLATE 5

};

// функция проверяет в каком режиме должен быть миксер
void C_Mixer::reculc()
{
  if (!Mixer.isStop())
  {
    if (_current_speed == 0)
    {
      _mode = 0;
      _current_dir = _need_dir;
    }
    else
      _mode = (_need_dir == _current_dir) ? 0 : 2;
  }
  else
    _mode = 2;
};

// Остановить миксер в обычном режиме
void C_Mixer::stop()
{
  _mixer_work = false;
  reculc();
};

// устновить направление (true, false)
void C_Mixer::setDir(bool direction)
{
  _need_dir = direction;
  // Serial.print("MIXER: Set direction : ");
  // Serial.println(direction ? "True" : "False");

  reculc();
};

void C_Mixer::setSpeed(int8_t procent)
{
    _need_speed = procent;

  reculc();
};

void C_Mixer::setTimeAutoReverse(uint64_t time)
{
  _period_autoreverse = time * 1000;
  // Serial.print("MIXER: Time autoreverse = ");
  // Serial.print(time);
  // Serial.println(" ms");
  reculc();
};

void C_Mixer::changeDir()
{
  _need_dir = !_need_dir;
  // Serial.println("MIXER: User change direction");
  reculc();
};

void C_Mixer::start()
{
  _mixer_work = true;
  reculc();
  // Serial.println("Mixer Start");
};

// Запуск миксера  (скорость(0..100), автореверс в с (0 если выключен))
void C_Mixer::start(int8_t sp, uint64_t time)
{
  setTimeAutoReverse(time);
  setSpeed(sp);
};

void C_Mixer::tick()
{
  if (_time_tick > millis())
    return;
  _time_tick = millis() + MIXER_DELTA_TIME;

  // Если миксер выключен, а скорость есть : гасим
  if ((_mixer_work == false) && (_current_speed > 0))
    _mode = 2;

  // Если миксер выключен и стоит -> выходим из функции
  if ((_mixer_work == false) && (_current_speed == 0))
    return;

  switch (_mode)
  {
  // Плавный набор скорости
  case 0:
    if (_current_speed == _need_speed)
    {
      _mode = 1;
      _time_autoreverse = millis() + _period_autoreverse;
    }
    else
    {
      if (_current_speed < _need_speed)
      {
        _current_speed += MIXER_DELTA_SPEED;
        if (_current_speed > _need_speed)
          _current_speed = _need_speed;
      }
      else
      {
        _current_speed -= MIXER_DELTA_SPEED;
        if (_current_speed < _need_speed)
          _current_speed = _need_speed;
      }
      work(_current_speed, _current_dir);
    };
    break;

  // Скорость набранна, поддержание
  case 1:
    // если включен автореверс и пришло время менять направление
    if ((_period_autoreverse != 0) && (_time_autoreverse <= millis()))
    {
      _mode = 2;
      _need_dir = !_need_dir;
    }
    break;

  // плавная остановка перед сменой направления
  case 2:

    // Если следующий тик последний
    if (_current_speed <= MIXER_DELTA_SPEED)
    {
      _current_speed = 0;
      _time_autoreverse = millis() + MIXER_PERIOD_PAUSE_AUTOREVERSE;
      _mode = 3;
    }
    else
      _current_speed -= MIXER_DELTA_SPEED;
    work(_current_speed, _current_dir);
    break;

  // Пауза между сменой направления,если mixer_work false тут и будет стопориться
  case 3:
    if (_time_autoreverse <= millis() && !Mixer.isStop())
    {

      // Меняем направление
      _current_dir = _need_dir;

      // в режим набирания скорости
      _mode = 0;
    };
    break;
  }
};

//получить рабочую скорость миксера в %
int8_t C_Mixer::getNeedSpeed(){
  return _need_speed;
};

//получить текущую скорость миксера в %
int8_t C_Mixer::getCurrentSpeed(){
  return _current_speed;
}

//получить текущее направление 
bool C_Mixer::getCurrentDir(){
  return _current_dir;
};

uint64_t C_Mixer::getPeriodAutoreverse(){
  return _period_autoreverse;
};