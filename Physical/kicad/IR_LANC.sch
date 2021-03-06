EESchema Schematic File Version 4
LIBS:IR_LANC-cache
EELAYER 26 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 1
Title ""
Date "2020-04-30"
Rev "0.01"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_UNO_R3 A1
U 1 1 5EAB1C0F
P 3850 3825
F 0 "A1" H 3850 5003 50  0000 C CNN
F 1 "Arduino_UNO_R3" H 3850 4912 50  0000 C CNN
F 2 "Module:Arduino_UNO_R3" H 4000 2775 50  0001 L CNN
F 3 "https://www.arduino.cc/en/Main/arduinoBoardUno" H 3650 4875 50  0001 C CNN
	1    3850 3825
	-1   0    0    -1  
$EndComp
$Comp
L Interface_Optical:TSOP321xx U1
U 1 1 5EAB2912
P 7350 3425
F 0 "U1" H 7050 3750 50  0000 R CNN
F 1 "IR Rx module" H 7525 3125 50  0000 R CNN
F 2 "Opto-Devices:IRReceiver_Vishay_MINIMOLD-3pin" H 7300 3050 50  0001 C CNN
F 3 "http://www.vishay.com/docs/82490/tsop321.pdf" H 8000 3725 50  0001 C CNN
	1    7350 3425
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5EAB2AB3
P 4750 3700
F 0 "R1" H 4800 3825 50  0000 L CNN
F 1 "4k7" H 4800 3725 50  0000 L CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4680 3700 50  0001 C CNN
F 3 "~" H 4750 3700 50  0001 C CNN
	1    4750 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5EAB2B71
P 6350 3025
F 0 "C1" H 6375 3125 50  0000 L CNN
F 1 "4.7u" H 6375 2925 50  0000 L CNN
F 2 "Capacitors_THT:CP_Radial_D5.0mm_P2.50mm" H 6388 2875 50  0001 C CNN
F 3 "~" H 6350 3025 50  0001 C CNN
	1    6350 3025
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5EAB2D4C
P 3850 5125
F 0 "#PWR01" H 3850 4875 50  0001 C CNN
F 1 "GND" H 3855 4952 50  0000 C CNN
F 2 "" H 3850 5125 50  0001 C CNN
F 3 "" H 3850 5125 50  0001 C CNN
	1    3850 5125
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 4925 3750 5025
Wire Wire Line
	3750 5025 3850 5025
Wire Wire Line
	3850 5025 3850 5125
Wire Wire Line
	3850 4925 3850 5025
Connection ~ 3850 5025
Wire Wire Line
	3950 4925 3950 5025
Wire Wire Line
	3950 5025 3850 5025
$Comp
L power:+5V #PWR02
U 1 1 5EAB33DB
P 4600 2625
F 0 "#PWR02" H 4600 2475 50  0001 C CNN
F 1 "+5V" H 4615 2798 50  0000 C CNN
F 2 "" H 4600 2625 50  0001 C CNN
F 3 "" H 4600 2625 50  0001 C CNN
	1    4600 2625
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW1
U 1 1 5EAB35F3
P 5800 4025
F 0 "SW1" V 5846 4123 50  0000 L CNN
F 1 "SW_SPST" V 5755 4123 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5800 4025 50  0001 C CNN
F 3 "" H 5800 4025 50  0001 C CNN
	1    5800 4025
	0    1    -1   0   
$EndComp
$Comp
L Connector:AudioJack3_Ground J1
U 1 1 5EAB3C63
P 6525 4425
F 0 "J1" H 6292 4354 50  0000 R CNN
F 1 "2.5mm jack/plug" H 6775 4675 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6525 4425 50  0001 C CNN
F 3 "~" H 6525 4425 50  0001 C CNN
	1    6525 4425
	-1   0    0    1   
$EndComp
$Comp
L Device:D_Zener_ALT D1
U 1 1 5EAB401F
P 4750 4650
F 0 "D1" V 4650 4675 50  0000 L CNN
F 1 "5.1v Zener" V 4900 4225 50  0000 L CNN
F 2 "Diodes_THT:D_DO-15_P10.16mm_Horizontal" H 4750 4650 50  0001 C CNN
F 3 "~" H 4750 4650 50  0001 C CNN
	1    4750 4650
	0    1    1    0   
$EndComp
Wire Wire Line
	4600 2625 4600 2825
Wire Wire Line
	3950 2825 4600 2825
Connection ~ 4600 2825
Wire Wire Line
	4600 2825 4750 2825
$Comp
L power:GND #PWR03
U 1 1 5EAB4BB4
P 6350 3725
F 0 "#PWR03" H 6350 3475 50  0001 C CNN
F 1 "GND" H 6355 3552 50  0000 C CNN
F 2 "" H 6350 3725 50  0001 C CNN
F 3 "" H 6350 3725 50  0001 C CNN
	1    6350 3725
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 4325 4750 4325
Wire Wire Line
	4750 3550 4750 2825
Wire Wire Line
	3950 5025 4750 5025
Wire Wire Line
	4750 5025 4750 4800
Connection ~ 3950 5025
Wire Wire Line
	4750 4325 4750 4500
Connection ~ 4750 4325
$Comp
L Device:R R2
U 1 1 5EAB683D
P 5000 3925
F 0 "R2" V 4925 3925 50  0000 C CNN
F 1 "5k6" V 5000 3925 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4930 3925 50  0001 C CNN
F 3 "~" H 5000 3925 50  0001 C CNN
	1    5000 3925
	0    1    1    0   
$EndComp
Wire Wire Line
	4850 3925 4350 3925
Wire Wire Line
	5125 5025 5125 4850
Connection ~ 4750 5025
Wire Wire Line
	5125 4450 5125 4325
Wire Wire Line
	5425 4650 5525 4650
Wire Wire Line
	5525 4650 5525 3925
Wire Wire Line
	5150 3925 5525 3925
Wire Wire Line
	4750 3850 4750 4325
Wire Wire Line
	6100 5025 6100 4525
Wire Wire Line
	6100 4525 6325 4525
Wire Wire Line
	6325 4425 5800 4425
$Comp
L Device:R R3
U 1 1 5EAC1654
P 5350 3425
F 0 "R3" V 5275 3425 50  0000 C CNN
F 1 "100R" V 5350 3425 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 5280 3425 50  0001 C CNN
F 3 "~" H 5350 3425 50  0001 C CNN
	1    5350 3425
	0    1    1    0   
$EndComp
Wire Wire Line
	5500 3425 6050 3425
Wire Wire Line
	5200 3425 4350 3425
Wire Wire Line
	6350 2825 6350 2875
Wire Wire Line
	6350 3175 6350 3625
Wire Wire Line
	6950 3625 6350 3625
Connection ~ 6350 3625
Wire Wire Line
	6350 3625 6350 3725
Wire Wire Line
	6950 3225 6750 3225
Wire Wire Line
	6750 3225 6750 2825
Wire Wire Line
	6750 2825 6350 2825
Connection ~ 6350 2825
$Comp
L Device:R R4
U 1 1 5EAC5505
P 6050 3125
F 0 "R4" H 6025 2975 50  0000 R CNN
F 1 "12k" H 6000 3150 50  0000 R CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 5980 3125 50  0001 C CNN
F 3 "~" H 6050 3125 50  0001 C CNN
	1    6050 3125
	-1   0    0    1   
$EndComp
Wire Wire Line
	6050 3275 6050 3425
Connection ~ 6050 3425
Wire Wire Line
	6050 3425 6950 3425
Wire Wire Line
	6050 2975 6050 2825
Connection ~ 6050 2825
Wire Wire Line
	6050 2825 6350 2825
Wire Wire Line
	5800 4425 5800 4225
Wire Wire Line
	5800 3825 5800 2825
Connection ~ 5800 2825
Wire Wire Line
	5800 2825 6050 2825
Wire Wire Line
	4750 2825 5800 2825
Connection ~ 5125 4325
Wire Wire Line
	5125 4325 6325 4325
Wire Wire Line
	4750 4325 5125 4325
Wire Wire Line
	4750 5025 5125 5025
Wire Wire Line
	5125 5025 6100 5025
Connection ~ 5125 5025
Connection ~ 4750 2825
$Comp
L Device:Q_NPN_EBC Q1
U 1 1 5EB57DBB
P 5225 4650
F 0 "Q1" H 5100 4800 50  0000 L CNN
F 1 "2N3904/BC547" H 4625 4450 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow_Oval" H 5425 4750 50  0001 C CNN
F 3 "~" H 5225 4650 50  0001 C CNN
	1    5225 4650
	-1   0    0    -1  
$EndComp
$EndSCHEMATC
