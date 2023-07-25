#include "CookingController.h"
#include "Arduino.h"
#include "Hardware.h"
#include "Mixer/Mixer.h"
#include "Serial/commands.h"

// Объект контроллера, с помощью которого производим все манипуляции связанные с приготовлением по рецепту
CRecipeController RecipeController;

ActionBlock::ActionBlock(BaseCookingMode *cookingMode, MixerMode *mixerMode)
{
  if (cookingMode == nullptr)
    this->_cookingMode = new EmptyMode();
  else
    this->_cookingMode = cookingMode;
  this->_mixerMode = mixerMode;
}

void ActionBlock::start()
{
  if (_cookingMode != nullptr)
    _cookingMode->start();

  if (_mixerMode != nullptr)
  {
    Mixer.setSpeed(_mixerMode->speed);
    Mixer.setTimeAutoReverse(_mixerMode->seconds_autoreverse);
  };
}

void ActionBlock::end() {
  if (_cookingMode != nullptr)
    _cookingMode->end();
}

void ActionBlock::tick()
{
  if (_cookingMode != nullptr)
    _cookingMode->tick();
}

bool ActionBlock::isComplete()
{
  // Если режима нагрева не было считается, что завершен
  if (_cookingMode == nullptr)
    return true;
  else
    return _cookingMode->isComplete();
};

ActionBlock::~ActionBlock()
{
  if (_cookingMode != nullptr)
    delete _cookingMode;

  if (_mixerMode != nullptr)
    delete _mixerMode;
}

String ActionBlock::stringCooking()
{
  if (_cookingMode != nullptr)
  {
    return _cookingMode->string();
  }
  else
    return "";
};

String ActionBlock::stringMixer()
{
  if (_mixerMode != nullptr)
  {
    return _mixerMode->string();
  }
  else
    return "";
};

String ActionBlock::stringAll()
{
  String result = stringMixer();
  if (result.length() > 0)
    result += CHAR_ACTION_SPLIT;
  result += stringCooking();

  return result;
}

// ####################################################################################################
// ####################################################################################################
// ####################################################################################################

void CRecipeController::finishCooking()
{
  bAutoCooking = false;
  countStage = 0;
  Serial.println(COMMAND_RECIPE_FINISH);
  Hardware.disableAll();
};

void CRecipeController::startCooking()
{
  manualDisable();

  if (arrStage.length() == 0)
  {
    Serial.println(COMMAND_RECIPE_SIZE + String("0"));
    bAutoCooking = false;
    return;
  };

  bAutoCooking = true;
  bPause = false;
  countStage = 0;
  Serial.println(COMMAND_RECIPE_START);
  setStage(countStage);
};

void CRecipeController::setStage(int index)
{
  if ((countStage < arrStage.length()) && (countStage >= 0)) arrStage[countStage].end();
  
  if (index >= arrStage.length())
  {
    finishCooking();
    return;
  };

  Serial.println(COMMAND_RECIPE_NUMBER + String(index));
  countStage = index;
  if (bAutoCooking && !bPause)
    arrStage[countStage].start();
};

void CRecipeController::nextStage()
{
  // Serial.println("R^");
  setStage(countStage + 1);
};

void CRecipeController::pause(bool bEnable)
{
  bPause = bEnable;

  if (bPause)
  {
    Hardware.disableAll();
  }
  else
  {
    manualDisable();
    // setStage(countStage);
  }
};

void CRecipeController::tick()
{
  // Если не автоматическое приготовление или пауза, то переходим в ручной режим
  if (!bAutoCooking || (bAutoCooking && bPause))
    return manualMode();

  arrStage[countStage].tick();
  if (arrStage[countStage].isComplete())
    nextStage();
};

void CRecipeController::addLast(ActionBlock *actionBlock)
{
  arrStage.addLast(actionBlock);
};

void CRecipeController::deleteLast()
{
  deleteIndex(size() - 1);
}

void CRecipeController::addIndex(int index, ActionBlock *actionBlock)
{
  arrStage.addTo(index, actionBlock);

  // Если добавили перед текущим этапом, то текущий этап сместился, поэтому поправляем
  if (index <= countStage)
    countStage++;
};

void CRecipeController::deleteIndex(int index)
{
  arrStage.deleteFrom(index);

  // Если удалили текущий этап, следующий сместился и встал на сесто текущего, поэтому просто стартуем его
  if (index == countStage)
    setStage(countStage);

  // Если удалили перед текущим этапом, то нужный этап сместился, поэтому смещаем и курсор
  if (index < countStage)
    countStage--;
};

void CRecipeController::changeIndex(int index, ActionBlock *newActionBlock)
{
  arrStage.change(index, newActionBlock);
};

void CRecipeController::clear()
{
  if (bAutoCooking)
    finishCooking();
  arrStage.clear();
};

uint8_t CRecipeController::size()
{
  return arrStage.length();
};

void CRecipeController::printRecipe()
{

  Serial.print(COMMAND_RECIPE_WRITE);
  for (int i = 0; i < arrStage.length(); i++)
  {
    Serial.print(CHAR_SPLIT_COMMAND);
    Serial.print(COMMAND_RECIPE_INDEX + String(i) + ">" + arrStage[i].stringAll());
  };
  Serial.println();
};

// Начать приготовление вне рецепта
void CRecipeController::manualStart(ActionBlock *manual)
{
  if (bAutoCooking && !bPause)
  {
    if (manual != nullptr)
      delete manual;
    bManualModeEnable = false;
    return;
  };

  bManualModeEnable = true;

  if (manualAction != nullptr)
    delete manualAction;

  manualAction = manual;
  manualAction->start();
}

void CRecipeController::manualMode()
{
  if (bManualModeEnable && manualAction != nullptr)
  {
    manualAction->tick();
  } else Hardware.disableAll();
};

void CRecipeController::manualDisable()
{
  bManualModeEnable = false;

  if (manualAction != nullptr)
  {
    delete manualAction;
    manualAction = nullptr;
  };
};

String CRecipeController::stringCurrentMode()
{
  return arrStage[countStage].stringCooking();
}