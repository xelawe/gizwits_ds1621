#define serdebug
#ifdef serdebug
#define DebugPrint(...) {  Serial.print(__VA_ARGS__); }
#define DebugPrintln(...) {  Serial.println(__VA_ARGS__); }
#else
#define DebugPrint(...) { }
#define DebugPrintln(...) { }
#endif

#include <Ticker.h>
#include "cy_wifi.h"
#include "cy_ota.h"
#include "cy_weather.h"
#include "ds1621_tool.h"
#include "btn_led_tool.h"

const char* gv_hostname = "gizwitsds1621";

Ticker senstick;
boolean gv_senstick;

void do_senstick() {
  gv_senstick = true;
}

void setup() {
  // put your setup code here, to run once:

#ifdef serdebug
  Serial.begin(115200);
#endif

  set_rgb(255, 255, 255);
  delay(500);


  wifi_init(gv_hostname);
  delay(500);

  init_ota(gv_hostname);

  init_ds1621();
  do_sensor();
  gv_senstick = false;
  senstick.attach(60, do_senstick);

  set_rgb(0, 0, 0);
  delay(500);

  //setup button
  pinMode(btnpin, INPUT);
  attachInterrupt(btnpin, IntBtn, CHANGE);
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:

  check_ota();

  if (gv_senstick == true) {
    do_sensor();
    gv_senstick = false;
  }

  LDRValue = analogRead(LDRPin);

  switch (cmd) {
    case CMD_WAIT:
      break;
    case CMD_BUTTON_CHANGE:
      int currentState = digitalRead(btnpin);
      if (currentState != buttonState) {
        if (buttonState == LOW && currentState == HIGH) {
          long duration = millis() - startPress;
          if (duration < 1000) {
            DebugPrintln("short press - toggle LED");
            toggle();
          } else if (duration < 5000) {
            DebugPrintln("medium press - reset");
            restart();
          } else if (duration < 60000) {
            DebugPrintln("long press - reset settings");
            reset();
          }
        } else if (buttonState == HIGH && currentState == LOW) {
          startPress = millis();
        }
        buttonState = currentState;
      }
      break;
  }


  switch (led_stat) {
    case 0:
      set_rgb(0, 0, 0);
      break;
    case 1:
      set_rgb(255, 0, 0);
      break;
    case 2:
      set_rgb(0, 255, 0);
      break;
    case 3:
      set_rgb(0, 0, 255);
      break;
    case 4:
      set_rgb(255, 255, 255);
      break;
    case 5:
      set_rgb(255, 255, 255, LDRValue);
      DebugPrintln(LDRValue);
      break;
  }

  delay(100);

}

void do_sensor() {

  get_ds1621();

}

