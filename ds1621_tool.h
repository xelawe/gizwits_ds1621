#include <DS1621_esp.h>

byte addr = (0x90 >> 1) | 0;  // replace the 0 with the value you set on pins A2, A1 and A0
DS1621 sensor = DS1621(addr, 5, 14);

void get_ds1621() {
  int tC, tFrac;

  tC = sensor.getHrTemp();                             // read high-resolution temperature

  if (tC < 0) {
    tC = -tC;                                   // fix for integer division
    DebugPrint("-");                          // indicate negative
  }

  tFrac = tC % 100;                             // extract fractional part
  tC /= 100;                                    // extract whole part

  DebugPrint(tC);
  DebugPrint(".");
  if (tFrac < 10)
    DebugPrint("0");
  DebugPrintln(tFrac);

}

void init_ds1621()
{
  sensor.startConversion(false);                       // stop if presently set to continuous
  sensor.setConfig(DS1621::POL | DS1621::ONE_SHOT);                    // Tout = active high; 1-shot mode
  sensor.setThresh(DS1621::ACCESS_TH, 27);                     // high temp threshold = 80F
  sensor.setThresh(DS1621::ACCESS_TL, 24);                     // low temp threshold = 75F

  DebugPrintln("DS1621 Init");

  int tHthresh = sensor.getTemp(DS1621::ACCESS_TH);
  DebugPrint("High threshold = ");
  DebugPrintln(tHthresh);

  int tLthresh = sensor.getTemp(DS1621::ACCESS_TL);
  DebugPrint("Low threshold = ");
  DebugPrintln(tLthresh);
}
