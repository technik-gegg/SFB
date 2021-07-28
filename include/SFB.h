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

#pragma once

#define EPR_TRIGGER_ADR 1
#define EPR_SENSMIN_ADR 2
#define EPR_SENSMAX_ADR 4

#define SERIAL_BAUDRATE 115200      // Baudrate for serial communication

#define MAX_SENSOR      5           // the number of Hall-sensors
#define MAX_SAMPLES     50          // number of samples for calibration

#if defined(ARDUINO_PRO_MICRO)
#define TRIGGER_PIN     6           // output pin for triggering filament runout
#define NEXT_PIN        8           // NEXT button pin
#define MENU_PIN        7           // MENU button pin
#define PREV_PIN        9           // PREV button pin 
#else
#define TRIGGER_PIN     9           // output pin for triggering filament runout (changed from 13 in previous version)
#define NEXT_PIN        11          // NEXT button pin
#define MENU_PIN        2           // MENU button pin
#define PREV_PIN        10          // PREV button pin 
#endif

#define LED_PIN         LED_BUILTIN // integrated LED pin
#define LOOP_DELAY      10          // delay for loop()
#define SCREEN_TIMEOUT  60*3        // turn screen off after 3 minutes to avoid OLED burn-in
#define MENU_TIMEOUT    10000
#define PRESCALER       256
#define SECOND_COUNTER  65536 - (F_CPU/PRESCALER) // usually 3036
#define DEFAULT_TRIGGER 150
#define DEFAULT_SENSMIN 50
#define DEFAULT_SENSMAX 500

void showTriggerMenu();
void showSensors();
void setDefaults();
void setupTimer();
void signalRunout(bool);
void draw();
void drawSamplingStat(char*);
void menuInterrupt();
void readSensors();
void reboot();
void setFont();
void setFont11();
void showParams();
int  getAnalogValue(uint8_t port);
