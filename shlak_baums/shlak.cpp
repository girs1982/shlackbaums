#include "shlak.h"
#define MAX_DELTA_1 200
String codan_shlak="0000";
boolean CheckValueSH(unsigned int base, unsigned int value)
{ ESP.wdtEnable(1000);
    yield();   ESP.wdtFeed();
  return ((value == base) || ((value > base) && ((value - base) < MAX_DELTA_1)) || ((value < base) && ((base - value) < MAX_DELTA_1)));
}
volatile unsigned long prevtime;
volatile unsigned int lolen, hilen, statesh;
volatile static byte cameCounter = 0;    // count of bits stored
volatile static long cameCode = 0;       // code itself
volatile static long lastCodesh = 0;
void grabshlak() {
    ////Serial.println("sacanon");
    ESP.wdtEnable(1000);
    yield();
   statesh = digitalRead(rxPin);
   Serial.println(statesh);
  if (statesh == HIGH){lolen = micros() - prevtime;} else{
    hilen = micros() - prevtime;
  prevtime = micros();} 
  if (statesh == LOW)
  {
    // последовательность закончилась
    if (CheckValueSH(320, hilen) && CheckValueSH(640, lolen)) // valid 1
    {
      cameCode = (cameCode << 1) | 1;
      cameCounter++;
    }
    else if (CheckValueSH(640, hilen) && CheckValueSH(320, lolen)) // valid 0
    {
      cameCode = (cameCode << 1) | 0;
      cameCounter++;
    }
    else {cameCounter = 0;}
  } else {
      if (lolen > 1000 &&(cameCounter == 12 || cameCounter == 13)&&((cameCode & 0xfff) != 0xfff))
  {
    lastCodesh = cameCode & 0xfff;
    codan_shlak="shlak-"+String(lastCodesh);
////   prefs.begin("esp32king", false);
  //// prefs.putULong("shlak0",lastCodesh);
  ////  prefs.end();
    cameCounter = 0;
    cameCode = 0;
   ESP.wdtFeed();
     
  }
  }
}
String code_shlak(){
String codon=codan_shlak;
return codon;
}

