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

#include <Arduino.h>
#include <U8g2lib.h>


#define EEP_TRIGGER_ADR 1
#define EEP_SENSMIN_ADR 2
#define EEP_SENSMAX_ADR 4
#define EEP_DEVICE_ADR  6
#define EEP_DEVMODE_ADR 8
#define EEP_DEVLAST_ADR 10
#define EEP_TOOL_ADR    12
#define EEP_ROTATE_ADR  14

#define SERIAL_BAUDRATE 115200      // Baudrate for serial communication

#define MAX_SENSOR      5           // the number of Hall-sensors
#define MAX_TOOLS       20          // the max. number of tools supported
#define MAX_SAMPLES     150         // number of samples for calibration
#define MIN_GAUSS       -640        // min. Gauss value the Hall sensor reads
#define MAX_GAUSS       640         // max. Gauss value the Hall sensor reads

#if defined(ARDUINO_PRO_MICRO)
#define TRIGGER_PIN     6           // output pin for triggering filament runout
#define TRIGGERIN_PIN   1           // input pin for triggering filament runout (for daisy-chaining)
#define NEXT_PIN        8           // NEXT button pin
#define MENU_PIN        7           // MENU button pin
#define PREV_PIN        9           // PREV button pin 
#define LED_PIN         0           // no built-in LED
#else
#define TRIGGER_PIN     9           // output pin for triggering filament runout (changed from 13 in previous version)
#define TRIGGERIN_PIN   1           // input pin for triggering filament runout (for daisy-chaining)
#define NEXT_PIN        11          // NEXT button pin
#define MENU_PIN        2           // MENU button pin
#define PREV_PIN        10          // PREV button pin 
#define LED_PIN         LED_BUILTIN // built-in LED pin
#endif

#define LOOP_DELAY      10          // delay for loop()
#define SCREEN_TIMEOUT  60*3        // turn screen off after 3 minutes to avoid OLED burn-in
#define MENU_TIMEOUT    10000
#define SENSORS_TIMEOUT 30000       // leave Sensors screen after this idle timeout
#define INFO_TIMEOUT    7           // timeout for settings summary after startup (in seconds)
#define PRESCALER       256
#define SECOND_COUNTER  65536 - (F_CPU/PRESCALER) // usually 3036
#define DEFAULT_TRIGGER 120
#define DEFAULT_SENSMIN 50
#define DEFAULT_SENSMAX 500
#define MAX_SECONDS     3600

#define ArraySize(arr) (sizeof(arr) / sizeof(arr[0]))

void    showTriggerMenu();
void    showSensors();
void    showDeviceMenu();
void    showModeMenu();
void    showRotateTools();
void    setDefaults();
bool    setChained();
void    setupTimer();
void    signalRunout(bool);
void    draw();
void    drawSamplingStat(char*);
void    menuInterrupt();
void    readSensors();
void    reboot();
void    setFontBig();
void    setFontSmall();
void    showParams();
int     getAnalogValue(uint8_t port);
uint8_t scanI2C();
void    resetScreenSaver();
void    debounceButtons();
void    __log(const char *fmt, ...);

