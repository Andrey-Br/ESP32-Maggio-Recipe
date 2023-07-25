#include "Encoders.h"

#if (VERSION_PLATE == 4 || VERSION_PLATE == 3)

EncButton2 <EB_ENCBTN> encA (INPUT_PULLUP, PIN_ENC1_A, PIN_ENC1_B, PIN_ENC1_BTN);
EncButton2 <EB_ENCBTN> encB (INPUT_PULLUP, PIN_ENC2_A, PIN_ENC2_B, PIN_ENC2_BTN);

EncButton2<EB_ENCBTN> *arr_enc[COUNT_ENCODERS] = {&encA, &encB};
#endif

#if (VERSION_PLATE == 5)
EncButton2 <EB_ENCBTN> encA (INPUT_PULLUP, PIN_ENC1_A, PIN_ENC1_B, PIN_ENC1_BTN);
// EncButton2 <EB_ENCBTN> encB (INPUT_PULLUP, PIN_ENC2_A, PIN_ENC2_B, PIN_ENC2_BTN);

EncButton2<EB_ENCBTN> *arr_enc[COUNT_ENCODERS] = {&encA};

#endif

void oneEncoderCheck (uint8_t i){
  String c = String( (char)('A' + i) );
  EncButton2 <EB_ENCBTN> *enc = arr_enc[i];
  enc->tick();

  LedEncoder.hold(i, enc ->state());

  if (enc->releaseStep(enc->clicks)) serialSend(c+ "U");

  if (enc->right()) {
    serialSend(c + "+");
    LedEncoder.right(i);
  }

  if (enc->left()) {
    serialSend(c+ "-");
    LedEncoder.left(i);
  };

  if (enc->rightH()) {
    serialSend(c+ "R");
    LedEncoder.right(i);
  };

  if (enc->leftH()) {
    serialSend(c+ "L");
    LedEncoder.left(i);
  };

  if (enc->click()) {
    serialSend(c+ "C");
    LedEncoder.click(i);
  };

  if (enc->held()) {
    serialSend(c+ "H", String(enc->clicks+1));
    LedEncoder.click(i);
    };

  if (enc->hasClicks()) serialSend(c+ "N",String(enc->clicks));

}

void encodersInit(){
  for (uint8_t i = 0 ; i < COUNT_ENCODERS; i++)
    arr_enc[i]->setStepTimeout(32);
  // encA.setStepTimeout(32);
  // encB.setStepTimeout(32);
  // encC.setStepTimeout(32);
};

void encodersTick(){
  for (uint8_t i = 0; i < COUNT_ENCODERS; i++)
    oneEncoderCheck(i);
  // oneEncoderCheck(encB,"B");
  // oneEncoderCheck(encC,"C");
};
