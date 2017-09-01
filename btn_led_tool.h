#define btnpin 4
#define ledpinbl 13
#define ledpinrt 15
#define ledpingn 12
#define LDRPin (A0)

int LDRValue;
int led_stat = 0;

const int CMD_WAIT = 0;
const int CMD_BUTTON_CHANGE = 1;
int cmd = CMD_WAIT;
int buttonState = HIGH;
static long startPress = 0;


void set_rgb(int iv_red, int iv_green, int iv_blue) {

  int lv_green = iv_green * 0.8;
  int lv_red = iv_red;
  int lv_blue = iv_blue;

  analogWrite(ledpinrt, lv_red);
  analogWrite(ledpingn, lv_green);
  analogWrite(ledpinbl, lv_blue);
}

void set_rgb(int iv_red, int iv_green, int iv_blue, int iv_LDRvalue) {

  int lv_green = iv_green * iv_LDRvalue / 1023;
  int lv_red = iv_red * iv_LDRvalue / 1023;
  int lv_blue = iv_blue * iv_LDRvalue / 1023;

  set_rgb(lv_red, lv_green, lv_blue);

}

void toggle() {
  led_stat++;
  if (led_stat > 5) {
    led_stat = 0;
  }

}

void restart() {
  ESP.reset();
  delay(1000);
}

void reset() {
  //reset wifi credentials
  WiFi.disconnect();
  delay(1000);
  ESP.reset();
  delay(1000);
}


void IntBtn() {
  cmd = CMD_BUTTON_CHANGE;
}

