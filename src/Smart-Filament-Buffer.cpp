/**
 * SFB Firmware
 * Copyright (C) 2019-2021 Technik Gegg
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
 Uncomment the  following line to compile for a Arduino PRO MICRO (LEONARDO).

 Be aware, that platformIO has issues with programming such devices!
 Hence, you have to copy the source file as "Smart-Filament-Buffer.ino" into an
 separate folder named "Smart-Filament-Buffer", copy the SFB.h into it as well
 and compile and upload this firmware using the Arduino IDE!
*/
//#define ARDUINO_PRO_MICRO 1   

#include <U8g2lib.h>
#include <EEPROM.h>
#include <Wire.h>
#include "SFB.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R1, /* reset=*/ U8X8_PIN_NONE);


bool          runout[MAX_SENSOR];       // state storage for run-out detection
int           runoutTime;               // timer value for latest run-out detected
uint8_t       sensorsIn[MAX_SENSOR] = { 
                  A0, A1, A2, A3, A6    // sensor inputs for T0..T4
              };
byte          runoutTrigger;            // time in seconds to trigger run-out on the controller board
int           gaussMin;                 // Gauss values min./max. for "filament run-out" detection
int           gaussMax;
int           screenSaver;              // counter for screen saver
volatile bool isPowerSave;              // screen saver ON flag 
bool          showMenu;                 // show menu flag
bool          lastTriggerState;         // last filament state
bool          isInit = true;            // flag for displaying params at start
int           seconds;                  // seconds counter
int8_t        tool = -1;                // current tool to monitor (-1 means all tools)

void setFont() {
  display.setFont(u8g2_font_9x15_tf);   // set default font
}

void setFont11() {
  display.setFont(u8g2_font_profont11_tr);  // set smaller font
}

int getAnalogValue(uint8_t port) {
  int val = analogRead(port);
  val &= 0x3FC;   // map out lower 2 bits
  long gauss = map(val, 0, 1023, -640, 640);
  return (int) gauss;
}

void showMainMenu() {

  bool      stopMenu = false;
  unsigned  startTime = millis();
  uint8_t   current_selection = 0;
  char      caption[40];
  char      buf[50];
  
  setFont();
  sprintf_P(caption, PSTR("Options"));

  do {
    // debounce Menu input
    while(digitalRead(MENU_PIN) == LOW)
      ;
    sprintf_P(buf, PSTR("Trigger\nSensors\n---\nDefaults\n---\nEXIT"));
    current_selection = display.userInterfaceSelectionList(caption, current_selection, buf);

    if(current_selection == 0)
      return;

    else {
      switch(current_selection) {
        case 1:
          showTriggerMenu();
          startTime = millis();
          break;
        
        case 2:
          showSensors();
          startTime = millis();
          break;

        case 4:
          setDefaults();
          startTime = millis();
          break;

        case 6:
          stopMenu = true;
          break;
      }
    }
    if(millis()-startTime > MENU_TIMEOUT)
      stopMenu = true;
  } while(!stopMenu);
  showMenu = false;
  isPowerSave = false;
}

/*
  Shows the raw values of each sensor.
  Menu button leaves this mode.
  Next button starts sampling for the highest value (gaussMin), which is used as run-out trigger value.
  Prev button stores the gaussMin in EEPROM.
*/
void showSensors() {
  setFont();
  display.setFontPosTop();

  bool stopMenu = false;
  bool isSampling = false;
  int x, y, samples = 0;
  int maxGauss;
  int minGauss;
  char buf[25];
  
  while(digitalRead(MENU_PIN) == LOW)
    ;

  do {
    display.clearBuffer();
    if(digitalRead(NEXT_PIN) == LOW && !isSampling) {
      isSampling = true;
      samples = 0;
      minGauss = gaussMin;
      maxGauss = gaussMax;
    }
    if(digitalRead(PREV_PIN) == LOW && !isSampling && maxGauss != 0) {
      EEPROM.put(EPR_SENSMIN_ADR, minGauss);
      EEPROM.put(EPR_SENSMAX_ADR, maxGauss);
      gaussMin = minGauss;
      gaussMax = maxGauss;
      sprintf_P(buf,PSTR("STORED!"));
      drawSamplingStat(buf);
      samples = 0;
    }
    
    for(int8_t i=0; i< MAX_SENSOR; i++) {
      int gauss = getAnalogValue(sensorsIn[i]);
      if(isSampling) {
        if(samples < MAX_SAMPLES) {
          if(abs(gauss) > minGauss)
            minGauss = (int)((float)(abs(gauss)/10+1) * 10);
        }
      }
      y = i*20+12; x = 5;
      sprintf_P(buf,PSTR("T%d"), i);
      display.setCursor(x, y); display.print(buf);
      sprintf_P(buf, PSTR("%4d"), gauss);
      display.setCursor(x+22, y); display.print(buf);
      delay(25);
    }
    if(isSampling) {
      if(samples < MAX_SAMPLES)
        sprintf_P(buf,PSTR("SAMPLE MIN"));
      drawSamplingStat(buf);
      samples++;
    }
    if(samples >= MAX_SAMPLES) {
      if(minGauss > 0) {
        sprintf_P(buf,PSTR("%3d/%3d"), minGauss, maxGauss);
        drawSamplingStat(buf);
      }
      isSampling = false;
    }
    display.sendBuffer();
    delay(100);
    if(digitalRead(MENU_PIN) == LOW)
      stopMenu = true;
  } while(!stopMenu);

}

void showTriggerMenu() {

  char caption[20];
  char buf[36];
  sprintf_P(caption, PSTR("Trigger"));
  uint8_t current_selection;
  bool stopMenu = false;

  switch(runoutTrigger) {
    case 15:  current_selection = 1; break;
    case 30:  current_selection = 2; break;
    case 45:  current_selection = 3; break;
    case 60:  current_selection = 4; break;
    case 90:  current_selection = 5; break;
    case 120: current_selection = 6; break;
    default:  current_selection = 0; break; 
  }
  do {
    setFont();
    sprintf_P(buf, PSTR("15s\n30s\n45s\n60s\n90s\n120s\nEXIT"));
    current_selection = display.userInterfaceSelectionList(caption, current_selection, buf);

    switch(current_selection) {
      case 1:
        runoutTrigger = 15;
        break;
      
      case 2:
        runoutTrigger = 30;
        break;

      case 3:
        runoutTrigger = 45;
        break;

      case 4:
        runoutTrigger = 60;
        break;

      case 5:
        runoutTrigger = 90;
        break;

      case 6:
        runoutTrigger = 120;
        break;
      
      default:
        stopMenu = true;
        break;
    }
    if(current_selection >= 1 && current_selection <=6) {
      EEPROM.update(EPR_TRIGGER_ADR, runoutTrigger);
    }
  } while(!stopMenu);
}

void reboot() {
  display.clearDisplay();
  display.clearBuffer();
  delay(500);
  asm volatile ("jmp 0");
}

void drawSamplingStat(char* buf) {
  setFont11();
  int y = display.getDisplayHeight()-10;
  display.setDrawColor(0);
  display.drawBox(0, y-2, display.getDisplayWidth(), display.getDisplayHeight()-y-1);
  display.setDrawColor(1);
  int x = display.getDisplayWidth() - display.getStrWidth(buf) - 2;
  display.setCursor(x, y); display.print(buf);
  setFont();
}

void showParams() {
  int x, y;
  char buf[30];
  // this can be interrupted by either Next or Prev buttons
  if(digitalRead(PREV_PIN) == LOW || digitalRead(NEXT_PIN) == LOW) {
    isInit = false;
    return;
  }
  y = 10; x = 5;
  setFont11();
  display.setCursor(x, y); display.print(F("*Smart-FB*"));  y+= 26;
  display.setCursor(x, y); display.print(F("Timeout"));     y+= 16;
  sprintf_P (buf, PSTR("%8d s"), runoutTrigger);
  display.setCursor(x, y); display.print(buf);              y+= 16;
  display.setCursor(x, y); display.print(F("Sensor MIN"));  y+= 16;
  sprintf_P (buf, PSTR("%7d Gs"), gaussMin);
  display.setCursor(x, y); display.print(buf);              y+= 16;
  display.setCursor(x, y); display.print(F("Sensor MAX"));  y+= 16;
  sprintf_P (buf, PSTR("%7d Gs"), gaussMax);
  display.setCursor(x, y); display.print(buf);
}

void draw() {
  int x, y;
  char buf[30];

  if(isPowerSave)
    return;

  // show parameters set in EEPROM after startup for 5 seconds
  if(seconds < 5 && isInit) {
    showParams();
    return;
  }
  else {
    isInit = false;
  }
  setFont();
  display.setFontPosTop();

  for(int8_t i=0; i< MAX_SENSOR; i++) {
    // draw tool (sensor) number
    y = i*20+12; x = 5;
    sprintf_P(buf, PSTR("T%d"), i);
    display.setCursor(x, y); display.print(buf);
    // draw status of filament presence
    x = (i % 2 == 0) ? 40 : 58;
    y = i*20+18;
    if(tool == -1) {
      if(!runout[i])
        display.drawDisc(x, y, 5, U8G2_DRAW_ALL);
      else
        display.drawCircle(x, y, 5, U8G2_DRAW_ALL);
    }
    else if(tool == i) {
      if(!runout[i])
        display.drawDisc(x, y, 5, U8G2_DRAW_ALL);
      else
        display.drawCircle(x, y, 5, U8G2_DRAW_ALL);
    }
    else {
      // not the currently monitored tool, then don't show status of this sensor at all
      display.setDrawColor(0);
      display.drawBox(x, y, 10, 10);
      display.setDrawColor(1);
    }
  }
  y += 18;
  // draw run-out if detected
  setFont11();
  if(runoutTime > 0) {
    if(runoutTrigger - (seconds - runoutTime) <= 0)
      sprintf_P(buf, PSTR("RUNOUT"));
    else
      sprintf_P(buf, PSTR("T-%d"), runoutTrigger - (seconds - runoutTime));
  }
  else {
    sprintf_P(buf, PSTR(" "));
    display.setDrawColor(0);
    display.drawBox(0, y-1, display.getDisplayWidth(), display.getDisplayHeight()-y-1);
    display.setDrawColor(1);
  }
  x = display.getDisplayWidth() - display.getStrWidth(buf) -2;
  display.setCursor(x, y); display.print(buf);
}

void readSensors() {
  
  for(int8_t i=0; i< MAX_SENSOR; i++) {
    runout[i] = true;
    int gauss = getAnalogValue(sensorsIn[i]);
    if(gauss >= (gaussMin * 1.75)) {
      runout[i] = false;
    }
    delay(2);
  }
  
  int runoutCnt = 0;
  if(tool == -1) {
    for(int8_t i=0; i< MAX_SENSOR; i++) {
      if(runout[i] == true) {
          runoutCnt++;
      }
    }
  }
  else {
    // monitoring only one sensor, check only that one
    if(runout[tool])
      runoutCnt = MAX_SENSOR;
  }
  // check if run-out has to be signalled
  if(runoutCnt == MAX_SENSOR) {
    if(runoutTime == 0)
      runoutTime = seconds;
    if(seconds - runoutTime >= runoutTrigger) {
      signalRunout(true);
    }
  }
  else {
    signalRunout(false);
    runoutTime = 0;
  }
}

void signalRunout(bool state) {
  if(state == lastTriggerState)
    return;
  if(state) {
    //digitalWrite(LED_PIN, HIGH);     // uncomment this if the LED used is not the built-in LED
    digitalWrite(TRIGGER_PIN, HIGH);
    isPowerSave = false;
  }
  else {
    //digitalWrite(LED_PIN, LOW);     // uncomment this if the LED used is not the built-in LED
    digitalWrite(TRIGGER_PIN, LOW);
  }
  lastTriggerState = state;
}

void setDefaults() {
  char t1[10], t2[10], t3[10], btn[15];
  sprintf_P(t1,PSTR("Reset to"));
  sprintf_P(t2,PSTR("DEFAULTS"));
  sprintf_P(t3,PSTR("Sure?"));
  sprintf_P(btn,PSTR(" Yes \n NO "));
  setFont11();
  display.clearDisplay();

  if(display.userInterfaceMessage(t1, t2, t3, btn)!=1) {
    return;
  }
  runoutTrigger = DEFAULT_TRIGGER;
  gaussMin = DEFAULT_SENSMIN;
  gaussMax = DEFAULT_SENSMAX;
  EEPROM.put(EPR_TRIGGER_ADR, runoutTrigger);
  EEPROM.put(EPR_SENSMIN_ADR, gaussMin);
  EEPROM.put(EPR_SENSMAX_ADR, gaussMax);
  reboot();
}

void setupTimer() {
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = SECOND_COUNTER;
  TCCR1B |= (1 << CS12);    // prescaler 256
  TIMSK1 |= (1 << TOIE1);
  interrupts();
}

ISR(TIMER1_OVF_vect)        
{
  if(seconds < 3600)
    seconds++;
  else
    seconds = 0;
  TCNT1 = SECOND_COUNTER;
}

void menuInterrupt() {
  showMenu = false;  
  if(isPowerSave) {
    screenSaver = seconds;
    isPowerSave = false;
    return;
  }
  if(seconds - screenSaver > 2)
    showMenu = true;  
}

void handleSerial(String& in) {
  // Serial.print("[IN]\t"); Serial.println(in);
  if(in.charAt(0) == 'T') {
    // either set or query active tool
    if(in.charAt(1) != '?') {
      long t = in.substring(1).toInt();
      if(t >= -1 && t < MAX_SENSOR)
        tool = (int8_t)t;
    }
    Serial.println(tool);
  }
  else if(in.charAt(0) == 'N') {
    // either set or query Gauss Min value
    if(in.charAt(1) == '?') {
      Serial.print(F("Gauss Min.: "));
      Serial.println(gaussMin);
    }  
    else {
      long g = in.substring(1).toInt();
      if(g >= DEFAULT_SENSMIN)
        gaussMin = (int)g;      
      EEPROM.put(EPR_SENSMIN_ADR, gaussMin);
    }
  }
  else if(in.charAt(0) == 'X') {
    // either set or query Gauss Max value
    if(in.charAt(1) == '?') {
      Serial.print(F("Gauss Max.: "));
      Serial.println(gaussMax);
    }  
    else {
      long g = in.substring(1).toInt();
      if(g < DEFAULT_SENSMAX)
        gaussMax = (int)g;      
      EEPROM.put(EPR_SENSMAX_ADR, gaussMax);
    }
  }
}

void serialEvent() {
  String in = Serial.readString();
  in.toUpperCase();
  handleSerial(in);
}

void setup() {
  Serial.begin(SERIAL_BAUDRATE);

  pinMode(MENU_PIN, INPUT_PULLUP);
  pinMode(NEXT_PIN, INPUT_PULLUP);
  pinMode(PREV_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(TRIGGER_PIN, LOW);
  
  for(int8_t i=0; i< MAX_SENSOR; i++) {
    pinMode(sensorsIn[i], INPUT);
  }
  
  display.begin(/*Select=*/ MENU_PIN,  /* menu_next_pin= */ NEXT_PIN, /* menu_prev_pin= */ PREV_PIN, /* menu_up_pin= */ U8X8_PIN_NONE, /* menu_down_pin= */ U8X8_PIN_NONE, /* menu_home_pin= */ U8X8_PIN_NONE);
  attachInterrupt(digitalPinToInterrupt(MENU_PIN), menuInterrupt, FALLING);
  setupTimer();
  
  screenSaver = 0;
  isPowerSave = false;
  signalRunout(true);
  
  byte val = EEPROM.read(EPR_TRIGGER_ADR);
  if(val == 0xFF) {
    setDefaults();
  }
  else
    runoutTrigger = val;
  EEPROM.get(EPR_SENSMIN_ADR, gaussMin);
  EEPROM.get(EPR_SENSMAX_ADR, gaussMax);
}

void loop() {

  if(Serial.available()) {
    serialEvent();
  }
  if(showMenu) {
    if(!isInit)
      showMainMenu();
  }
  else {
    display.setPowerSave(isPowerSave ? 1 : 0);
    readSensors();
    display.clearBuffer();
    draw();
    display.sendBuffer();
  }
  delay(LOOP_DELAY);
  if(seconds - screenSaver >= SCREEN_TIMEOUT) {
    isPowerSave = true;
  }
}
