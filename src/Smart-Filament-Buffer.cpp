/**
 * SFB Firmware
 * Copyright (C) 2019 Technik Gegg
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

#include <U8g2lib.h>
#include <EEPROM.h>
#include <Wire.h>
#include "SFB.h"


U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R1, /* reset=*/ U8X8_PIN_NONE);


bool          runout[MAX_SENSOR];       // state storage for run-out detection
int           runoutTime;               // timer value for latest run-out detected
int           sensorsIn[MAX_SENSOR] = { 
                  A6, A3, A2, A1, A0    // sensor inputs for T0..T4
              };
byte          runoutTrigger;            // time in seconds to trigger runout on the controller board
int           gaussMin;                 // Gauss values min./max. for "filament runout" detection
int           gaussMax;
volatile int  screenSaver;              // counter for screen saver
volatile bool isPowerSave;              // screen saver ON flag 
bool          showMenu;                 // show menu flag
bool          lastTriggerState;         // last filament state
bool          isInit = true;            // flag for displaying params at start
char          buf[60];                  // generic buffer
volatile int  seconds;                  // seconds counter

void showMainMenu() {

  bool      stopMenu = false;
  unsigned  startTime = millis();
  uint8_t   current_selection = 0;
  char      caption[40];
  
  display.setFont(u8g2_font_unifont_t_symbols);
  sprintf_P(caption, PSTR("Options"));

  do {
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

void showSensors() {
  display.setFont(u8g2_font_unifont_t_symbols);
  display.setFontPosTop();

  bool stopMenu = false;
  bool isSampling = false;
  int x, y, samples = 0;
  int maxGauss;
  while(digitalRead(MENU_PIN) == LOW)
    ;

  do {
    display.clearBuffer();
    if(digitalRead(NEXT_PIN) == LOW && !isSampling) {
      isSampling = true;
      samples = 0;
      maxGauss = 0;
    }
    if(digitalRead(PREV_PIN) == LOW && !isSampling && maxGauss != 0) {
      EEPROM.put(EPR_SENSMIN_ADR, -maxGauss);
      EEPROM.put(EPR_SENSMAX_ADR, maxGauss);
      sprintf_P(buf,PSTR("STORED!"));
      drawSamplingStat(buf);
      samples = 0;
    }
    
    for(int i=0; i< MAX_SENSOR; i++) {
      int gauss = map(analogRead(sensorsIn[i]), 0, 1023, -640, 640);
      if(isSampling) {
        if(abs(gauss) > maxGauss)
          maxGauss = (int)((float)(abs(gauss)/10+1) * 10);
      }
      y = i*20+12; x = 5;
      sprintf_P(buf,PSTR("T%d"), i);
      display.setCursor(x, y); display.print(buf);
      sprintf_P(buf, PSTR("%4d"), gauss);
      display.setCursor(x+25, y); display.print(buf);
    }
    if(isSampling) {
      sprintf_P(buf,PSTR("SAMPLING..."));
      drawSamplingStat(buf);
      samples++;
    }
    if(samples > 20) {
      isSampling = false;
      if(maxGauss > 0) {
        sprintf_P(buf,PSTR("%3d/%3d"),-maxGauss, maxGauss);
        drawSamplingStat(buf);
      }
    }
    display.sendBuffer();
    delay(500);
    if(digitalRead(MENU_PIN) == LOW)
      stopMenu = true;
  } while(!stopMenu);

}

void showTriggerMenu() {

  char caption[40];
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
    display.setFont(u8g2_font_unifont_t_symbols);
    sprintf_P(buf, PSTR("15 s\n30 s\n45 s\n60 s\n90 s\n120 s\nEXIT"));
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
      reboot();
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
  display.setFont(u8g2_font_profont10_tf);
  int y = display.getDisplayHeight()-10;
  display.setDrawColor(0);
  display.drawBox(0, y-2, display.getDisplayWidth(), display.getDisplayHeight()-y-2);
  display.setDrawColor(1);
  int x = display.getDisplayWidth() - display.getStrWidth(buf) - 2;
  display.setCursor(x, y); display.print(buf);
  display.setFont(u8g2_font_unifont_t_symbols);
}

void draw() {
  int x, y;

  if(isPowerSave)
    return;

  if(seconds < 10 && isInit) {
    if(digitalRead(PREV_PIN) == LOW || digitalRead(NEXT_PIN) == LOW) {
      isInit = false;
      return;
    }
    y = 10; x = 5;
    display.setFont(u8g2_font_profont11_tf);
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
    return;
  }
  else {
    isInit = false;
  }
  display.setFont(u8g2_font_unifont_t_symbols);
  display.setFontPosTop();

  for(int i=0; i< MAX_SENSOR; i++) {
    y = i*20+12; x = 5;
    sprintf_P(buf, PSTR("T%d"), i);
    display.setCursor(x, y); display.print(buf);
    if(i%2 == 0)
      x = 34;
    else 
      x = 48;
    y = i*20+20;
    display.drawUTF8(x, y, runout[i] == true ? "\u25cb" : "\u25cf");
  }
  y += 16;
  display.setFont(u8g2_font_profont10_tf);
  if(runoutTime > 0) {
    if(runoutTrigger - (seconds - runoutTime) < 0)
      sprintf_P(buf, PSTR("No filament"));
    else
      sprintf_P(buf, PSTR("%d"), runoutTrigger - (seconds - runoutTime));
  }
  else {
    sprintf_P(buf, PSTR(" "));
    display.setDrawColor(0);
    display.drawBox(0, y-2, display.getDisplayWidth(), display.getDisplayHeight()-y-2);
    display.setDrawColor(1);
  }
  x = display.getDisplayWidth() - display.getStrWidth(buf) -2;
  display.setCursor(x, y); display.print(buf);
}

void readSensors() {
  //Serial.print(gaussMin); Serial.print(F(", "));    // uncomment this to draw sensor values on Serial Plotter
  for(int i=0; i< MAX_SENSOR; i++) {
    runout[i] = true;
    int gauss = map(analogRead(sensorsIn[i]), 0, 1023, -640, 640);
    if(gauss <= gaussMin || gauss >= gaussMax) {
      runout[i] = false;
    }
    delay(10);
    //Serial.print(gauss); Serial.print(F(","));      // uncomment this to draw sensor values on Serial Plotter
  }
  //Serial.println(gaussMax);                         // uncomment this to draw sensor values on Serial Plotter
  int runoutCnt = 0;
  for(int i=0; i< MAX_SENSOR; i++) {
    if(runout[i] == true) 
      runoutCnt++;
  }

  if(runoutCnt == MAX_SENSOR) {
    if(runoutTime == 0)
      runoutTime = seconds;
    if(seconds - runoutTime >= runoutTrigger)
      signalRunout(true);
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
  display.setFont(u8g2_font_profont11_tf);
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

void setup() {
  Serial.begin(57600);

  pinMode(MENU_PIN, INPUT_PULLUP);
  pinMode(NEXT_PIN, INPUT_PULLUP);
  pinMode(PREV_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  for(int i=0; i< MAX_SENSOR; i++) {
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
  int val1;
  EEPROM.get(EPR_SENSMIN_ADR, val1);
  gaussMin = val1;
  EEPROM.get(EPR_SENSMAX_ADR, val1);
  gaussMax = val1;
}

void loop() {
   while(digitalRead(MENU_PIN) == LOW)
    delay(10);

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
