EESchema Schematic File Version 4
LIBS:SFB-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Smart Filament Buffer"
Date "2019-06-24"
Rev "2"
Comp "Technik Gegg"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_Nano_v3.x U1
U 1 1 5D109B43
P 4450 3600
F 0 "U1" H 4450 2511 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 3937 2559 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 4600 2650 50  0001 L CNN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 4450 2600 50  0001 C CNN
	1    4450 3600
	1    0    0    -1  
$EndComp
$Comp
L Sensor_Magnetic:A1301KUA-T U2
U 1 1 5D10BC24
P 7550 3050
F 0 "U2" H 7321 3096 50  0000 R CNN
F 1 "A1326 LUA-T" H 7321 3005 50  0000 R CNN
F 2 "Package_TO_SOT_THT:TO-92Flat" H 7550 2700 50  0001 L CIN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/A1301-2-Datasheet.ashx" H 7450 3050 50  0001 C CNN
	1    7550 3050
	-1   0    0    1   
$EndComp
$Comp
L Sensor_Magnetic:A1301KUA-T U4
U 1 1 5D10F19C
P 7550 4100
F 0 "U4" H 7321 4146 50  0000 R CNN
F 1 "A1326 LUA-T" H 7321 4055 50  0000 R CNN
F 2 "Package_TO_SOT_THT:TO-92Flat" H 7550 3750 50  0001 L CIN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/A1301-2-Datasheet.ashx" H 7450 4100 50  0001 C CNN
	1    7550 4100
	-1   0    0    1   
$EndComp
$Comp
L Sensor_Magnetic:A1301KUA-T U6
U 1 1 5D10F50B
P 7550 5150
F 0 "U6" H 7321 5196 50  0000 R CNN
F 1 "A1326 LUA-T" H 7321 5105 50  0000 R CNN
F 2 "Package_TO_SOT_THT:TO-92Flat" H 7550 4800 50  0001 L CIN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/A1301-2-Datasheet.ashx" H 7450 5150 50  0001 C CNN
	1    7550 5150
	-1   0    0    1   
$EndComp
$Comp
L Sensor_Magnetic:A1301KUA-T U3
U 1 1 5D10F8EC
P 8900 3600
F 0 "U3" H 8671 3646 50  0000 R CNN
F 1 "A1326 LUA-T" H 8671 3555 50  0000 R CNN
F 2 "Package_TO_SOT_THT:TO-92Flat" H 8900 3250 50  0001 L CIN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/A1301-2-Datasheet.ashx" H 8800 3600 50  0001 C CNN
	1    8900 3600
	-1   0    0    1   
$EndComp
$Comp
L Sensor_Magnetic:A1301KUA-T U5
U 1 1 5D10FA77
P 8900 4700
F 0 "U5" H 8671 4746 50  0000 R CNN
F 1 "A1326 LUA-T" H 8671 4655 50  0000 R CNN
F 2 "Package_TO_SOT_THT:TO-92Flat" H 8900 4350 50  0001 L CIN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/A1301-2-Datasheet.ashx" H 8800 4700 50  0001 C CNN
	1    8900 4700
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5D10FB78
P 8550 5800
F 0 "#PWR0101" H 8550 5550 50  0001 C CNN
F 1 "GND" H 8555 5627 50  0000 C CNN
F 2 "" H 8550 5800 50  0001 C CNN
F 3 "" H 8550 5800 50  0001 C CNN
	1    8550 5800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5D11026E
P 4450 5300
F 0 "#PWR0102" H 4450 5050 50  0001 C CNN
F 1 "GND" H 4455 5127 50  0000 C CNN
F 2 "" H 4450 5300 50  0001 C CNN
F 3 "" H 4450 5300 50  0001 C CNN
	1    4450 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 3050 5400 3050
Wire Wire Line
	5400 3050 5400 3600
Wire Wire Line
	5400 3600 4950 3600
Wire Wire Line
	8600 3600 5600 3600
Wire Wire Line
	5600 3600 5600 3700
Wire Wire Line
	5600 3700 4950 3700
Wire Wire Line
	7250 4100 5600 4100
Wire Wire Line
	5600 4100 5600 3800
Wire Wire Line
	5600 3800 4950 3800
Wire Wire Line
	8600 4700 5500 4700
Wire Wire Line
	5500 4700 5500 3900
Wire Wire Line
	5500 3900 4950 3900
Wire Wire Line
	7250 5150 5400 5150
Wire Wire Line
	5400 5150 5400 4200
Wire Wire Line
	5400 4200 4950 4200
$Comp
L power:+5V #PWR0103
U 1 1 5D1137CF
P 8150 5800
F 0 "#PWR0103" H 8150 5650 50  0001 C CNN
F 1 "+5V" H 8165 5973 50  0000 C CNN
F 2 "" H 8150 5800 50  0001 C CNN
F 3 "" H 8150 5800 50  0001 C CNN
	1    8150 5800
	-1   0    0    1   
$EndComp
Wire Wire Line
	7650 5550 8150 5550
Wire Wire Line
	8150 5550 8150 5800
Wire Wire Line
	9000 5100 9000 5550
Wire Wire Line
	9000 5550 8150 5550
Connection ~ 8150 5550
Wire Wire Line
	7650 4500 8150 4500
Wire Wire Line
	8150 4500 8150 5550
Wire Wire Line
	7650 3450 8150 3450
Wire Wire Line
	8150 3450 8150 4000
Connection ~ 8150 4500
Wire Wire Line
	9000 4000 8150 4000
Connection ~ 8150 4000
Wire Wire Line
	8150 4000 8150 4500
Wire Wire Line
	9000 4300 8550 4300
Wire Wire Line
	8550 4300 8550 4750
Wire Wire Line
	9000 3200 8550 3200
Wire Wire Line
	8550 3200 8550 3700
Connection ~ 8550 4300
Wire Wire Line
	7650 2650 8550 2650
Wire Wire Line
	8550 2650 8550 3200
Connection ~ 8550 3200
Wire Wire Line
	7650 3700 8550 3700
Connection ~ 8550 3700
Wire Wire Line
	8550 3700 8550 4300
Wire Wire Line
	7650 4750 8550 4750
Connection ~ 8550 4750
Wire Wire Line
	8550 4750 8550 5800
Wire Wire Line
	4550 4600 4550 5150
Wire Wire Line
	4550 5150 4450 5150
Wire Wire Line
	4450 5150 4450 5300
Wire Wire Line
	4450 4600 4450 5150
Connection ~ 4450 5150
$Comp
L power:+5V #PWR0104
U 1 1 5D119368
P 4650 2200
F 0 "#PWR0104" H 4650 2050 50  0001 C CNN
F 1 "+5V" H 4665 2373 50  0000 C CNN
F 2 "" H 4650 2200 50  0001 C CNN
F 3 "" H 4650 2200 50  0001 C CNN
	1    4650 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 4000 5300 4000
Wire Wire Line
	5300 4000 5300 6050
Wire Wire Line
	4950 4100 5200 4100
Wire Wire Line
	5200 4100 5200 5950
$Comp
L power:+5V #PWR0105
U 1 1 5D12279F
P 4400 6250
F 0 "#PWR0105" H 4400 6100 50  0001 C CNN
F 1 "+5V" H 4415 6423 50  0000 C CNN
F 2 "" H 4400 6250 50  0001 C CNN
F 3 "" H 4400 6250 50  0001 C CNN
	1    4400 6250
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_DPST SW2
U 1 1 5D126182
P 2150 3100
F 0 "SW2" H 2150 3425 50  0000 C CNN
F 1 "SW_DPST" H 2150 3334 50  0000 C CNN
F 2 "SFB:SW_Tactile_Straight_KSA0Axx1LFTR-4Pin" H 2150 3100 50  0001 C CNN
F 3 "~" H 2150 3100 50  0001 C CNN
	1    2150 3100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_DPST SW1
U 1 1 5D126B34
P 2150 3750
F 0 "SW1" H 2150 4075 50  0000 C CNN
F 1 "SW_DPST" H 2150 3984 50  0000 C CNN
F 2 "SFB:SW_Tactile_Straight_KSA0Axx1LFTR-4Pin" H 2150 3750 50  0001 C CNN
F 3 "~" H 2150 3750 50  0001 C CNN
	1    2150 3750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_DPST SW3
U 1 1 5D127C59
P 2150 4400
F 0 "SW3" H 2150 4725 50  0000 C CNN
F 1 "SW_DPST" H 2150 4634 50  0000 C CNN
F 2 "SFB:SW_Tactile_Straight_KSA0Axx1LFTR-4Pin" H 2150 4400 50  0001 C CNN
F 3 "~" H 2150 4400 50  0001 C CNN
	1    2150 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 3200 3950 3200
Wire Wire Line
	2350 3850 3250 3850
Wire Wire Line
	3250 3850 3250 4000
Wire Wire Line
	3250 4000 3950 4000
Wire Wire Line
	3950 4100 2600 4100
$Comp
L power:GND #PWR0107
U 1 1 5D12BBE9
P 1600 4700
F 0 "#PWR0107" H 1600 4450 50  0001 C CNN
F 1 "GND" H 1605 4527 50  0000 C CNN
F 2 "" H 1600 4700 50  0001 C CNN
F 3 "" H 1600 4700 50  0001 C CNN
	1    1600 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 4700 1600 4500
Wire Wire Line
	1600 4500 1950 4500
Wire Wire Line
	1950 3850 1600 3850
Wire Wire Line
	1600 3850 1600 4500
Connection ~ 1600 4500
Wire Wire Line
	1950 3200 1600 3200
Wire Wire Line
	1600 3200 1600 3850
Connection ~ 1600 3850
Text Label 2650 3150 0    50   ~ 0
MENU
Text Label 2650 4050 0    50   ~ 0
NEXT
Text Label 2650 3800 0    50   ~ 0
PREV
Wire Wire Line
	2600 4500 2350 4500
Wire Wire Line
	2600 4100 2600 4500
$Comp
L Connector:Conn_01x05_Female J1
U 1 1 5D136CB2
P 3500 1300
F 0 "J1" V 3438 1012 50  0000 R CNN
F 1 "JST XH 1x5 angled" V 3347 1012 50  0000 R CNN
F 2 "Connector_JST:JST_XH_S5B-XH-A_1x05_P2.50mm_Horizontal" H 3500 1300 50  0001 C CNN
F 3 "~" H 3500 1300 50  0001 C CNN
	1    3500 1300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4650 2200 4650 2300
Connection ~ 4650 2300
Wire Wire Line
	4650 2300 4650 2600
Wire Wire Line
	3600 1500 3600 3000
Wire Wire Line
	3600 3000 3950 3000
Wire Wire Line
	3700 1500 3700 3100
Wire Wire Line
	3700 3100 3950 3100
Wire Wire Line
	3500 1500 3500 3900
Wire Wire Line
	4500 5950 4500 6450
Wire Wire Line
	4600 6050 4600 6450
Wire Wire Line
	4500 5950 5200 5950
Wire Wire Line
	4600 6050 5300 6050
$Comp
L power:GND #PWR0108
U 1 1 5D139CC1
P 3300 1650
F 0 "#PWR0108" H 3300 1400 50  0001 C CNN
F 1 "GND" H 3150 1550 50  0000 C CNN
F 2 "" H 3300 1650 50  0001 C CNN
F 3 "" H 3300 1650 50  0001 C CNN
	1    3300 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 1500 3400 2300
Wire Wire Line
	3400 2300 4650 2300
Wire Wire Line
	3300 1500 3300 1650
Wire Wire Line
	3950 3900 3500 3900
Text Notes 3200 1250 0    50   ~ 0
GND\n
Text Notes 3350 1150 0    50   ~ 0
+5V
Text Notes 3450 1250 0    50   ~ 0
SIG
Text Notes 3550 1150 0    50   ~ 0
RX
Text Notes 3650 1250 0    50   ~ 0
TX
Text Notes 7800 3350 0    98   ~ 20
T0
Text Notes 9150 3900 0    98   ~ 20
T1\n
Text Notes 7800 4400 0    98   ~ 20
T2
Text Notes 9150 5000 0    98   ~ 20
T3
Text Notes 7800 5450 0    98   ~ 20
T4
$Comp
L power:GND #PWR0106
U 1 1 5D123581
P 4300 6250
F 0 "#PWR0106" H 4300 6000 50  0001 C CNN
F 1 "GND" H 4305 6077 50  0000 C CNN
F 2 "" H 4300 6250 50  0001 C CNN
F 3 "" H 4300 6250 50  0001 C CNN
	1    4300 6250
	-1   0    0    1   
$EndComp
$Comp
L SFB-rescue:SSD1306 U7
U 1 1 5F288751
P 4450 6800
F 0 "U7" H 4800 6800 50  0000 L CNN
F 1 "SSD1306" H 4300 6450 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 4450 7050 50  0001 C CNN
F 3 "" H 4450 7050 50  0001 C CNN
	1    4450 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 6250 4300 6450
Wire Wire Line
	4400 6250 4400 6450
Text Notes 650  7700 0    98   ~ 20
Note: Signal Out (run-out pin) has moved from pin 16 to pin 9 in Rev. 2\nAlso: VCC and GND have been swapped on the I2C display (they're \nmore common these days)
$EndSCHEMATC
