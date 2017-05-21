EESchema Schematic File Version 2
LIBS:ecc83_schlib
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "ECC Push-Pull"
Date "Sat 21 Mar 2015"
Rev "0.1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	2500 3400 2500 3700
Wire Wire Line
	6100 4650 6100 4850
Wire Wire Line
	2000 2000 6300 2000
Connection ~ 3400 2000
Wire Wire Line
	7050 3000 7800 3000
Wire Wire Line
	7300 3000 7300 3200
Wire Wire Line
	2000 3500 2600 3500
Wire Wire Line
	2400 3500 2400 3400
Wire Wire Line
	6200 3500 6200 3850
$Comp
L ECC83 U1
U 3 1 48B4F266
P 2500 2950
F 0 "U1" H 2500 3400 50  0000 C CNN
F 1 "ECC83" H 2500 3300 50  0000 C CNN
F 2 "Valves:VALVE-ECC-83-1" V 2250 2950 30  0000 C CNN
F 3 "" H 2500 2950 60  0001 C CNN
	3    2500 2950
	1    0    0    -1  
$EndComp
$Comp
L ECC83 U1
U 2 1 48B4F263
P 6200 4250
F 0 "U1" H 6350 4600 50  0000 C CNN
F 1 "ECC83" H 6400 3900 50  0000 C CNN
F 2 "Valves:VALVE-ECC-83-1" H 6400 3850 30  0000 C CNN
F 3 "" H 6200 4250 60  0001 C CNN
	2    6200 4250
	1    0    0    -1  
$EndComp
$Comp
L ECC83 U1
U 1 1 48B4F256
P 6300 2550
F 0 "U1" H 6450 2900 50  0000 C CNN
F 1 "ECC83" H 6050 2250 50  0000 C CNN
F 2 "Valves:VALVE-ECC-83-1" H 6570 2150 50  0001 C CNN
F 3 "" H 6300 2550 60  0001 C CNN
	1    6300 2550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 457DBAF8
P 7700 3300
F 0 "#PWR01" H 7700 3300 30  0001 C CNN
F 1 "GND" H 7700 3230 30  0001 C CNN
F 2 "" H 7700 3300 60  0001 C CNN
F 3 "" H 7700 3300 60  0001 C CNN
	1    7700 3300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 457DBAF5
P 7300 3550
F 0 "#PWR02" H 7300 3550 30  0001 C CNN
F 1 "GND" H 7300 3480 30  0001 C CNN
F 2 "" H 7300 3550 60  0001 C CNN
F 3 "" H 7300 3550 60  0001 C CNN
	1    7300 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 457DBAF1
P 6100 5200
F 0 "#PWR03" H 6100 5200 30  0001 C CNN
F 1 "GND" H 6100 5130 30  0001 C CNN
F 2 "" H 6100 5200 60  0001 C CNN
F 3 "" H 6100 5200 60  0001 C CNN
	1    6100 5200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 457DBAEF
P 5700 5200
F 0 "#PWR04" H 5700 5200 30  0001 C CNN
F 1 "GND" H 5700 5130 30  0001 C CNN
F 2 "" H 5700 5200 60  0001 C CNN
F 3 "" H 5700 5200 60  0001 C CNN
	1    5700 5200
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG05
U 1 1 457DBAC0
P 3000 2350
F 0 "#FLG05" H 3000 2620 30  0001 C CNN
F 1 "PWR_FLAG" H 3000 2580 30  0000 C CNN
F 2 "" H 3000 2350 60  0001 C CNN
F 3 "" H 3000 2350 60  0001 C CNN
	1    3000 2350
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P4
U 1 1 456A8ACC
P 1650 3600
F 0 "P4" V 1600 3600 40  0000 C CNN
F 1 "CONN_2" V 1700 3600 40  0000 C CNN
F 2 "connect:bornier2" H 1650 3600 60  0001 C CNN
F 3 "" H 1650 3600 60  0001 C CNN
	1    1650 3600
	-1   0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 4549F4BE
P 3400 2250
F 0 "C1" H 3450 2350 50  0000 L CNN
F 1 "10uF" H 3150 2350 50  0000 L CNN
F 2 "discret:C2V10" H 3400 2250 60  0001 C CNN
F 3 "" H 3400 2250 60  0001 C CNN
	1    3400 2250
	-1   0    0    -1  
$EndComp
$Comp
L CONN_2 P3
U 1 1 4549F4A5
P 1650 2100
F 0 "P3" V 1600 2100 40  0000 C CNN
F 1 "POWER" V 1700 2100 40  0000 C CNN
F 2 "connect:bornier2" H 1650 2100 60  0001 C CNN
F 3 "" H 1650 2100 60  0001 C CNN
	1    1650 2100
	-1   0    0    -1  
$EndComp
$Comp
L CONN_2 P2
U 1 1 4549F46C
P 8150 3100
F 0 "P2" V 8100 3100 40  0000 C CNN
F 1 "OUT" V 8200 3100 40  0000 C CNN
F 2 "connect:bornier2" H 8150 3100 60  0001 C CNN
F 3 "" H 8150 3100 60  0001 C CNN
	1    8150 3100
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P1
U 1 1 4549F464
P 5000 4350
F 0 "P1" V 4950 4350 40  0000 C CNN
F 1 "IN" V 5050 4350 40  0000 C CNN
F 2 "connect:bornier2" H 5000 4600 50  0000 C CNN
F 3 "" H 5000 4350 60  0001 C CNN
	1    5000 4350
	-1   0    0    1   
$EndComp
$Comp
L C C2
U 1 1 4549F3BE
P 6900 3000
F 0 "C2" V 6750 3000 50  0000 C CNN
F 1 "680nF" V 7050 3000 50  0000 C CNN
F 2 "discret:C2" H 6900 3000 60  0001 C CNN
F 3 "" H 6900 3000 60  0001 C CNN
	1    6900 3000
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 4549F3AD
P 7300 3350
F 0 "R3" H 7200 3550 50  0000 C CNN
F 1 "100K" V 7300 3350 50  0000 C CNN
F 2 "discret:R3" H 7300 3350 60  0001 C CNN
F 3 "" H 7300 3350 60  0001 C CNN
	1    7300 3350
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 4549F3A2
P 5700 5000
F 0 "R4" H 5600 5200 50  0000 C CNN
F 1 "47K" V 5700 5000 50  0000 C CNN
F 2 "discret:R3" H 5700 5000 60  0001 C CNN
F 3 "" H 5700 5000 60  0001 C CNN
	1    5700 5000
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 4549F39D
P 6100 5000
F 0 "R2" H 6000 5200 50  0000 C CNN
F 1 "1.5K" V 6100 5000 50  0000 C CNN
F 2 "discret:R3" H 6100 5000 60  0001 C CNN
F 3 "" H 6100 5000 60  0001 C CNN
	1    6100 5000
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 4549F38A
P 6200 3350
F 0 "R1" H 6300 3150 50  0000 C CNN
F 1 "1.5K" V 6200 3350 50  0000 C CNN
F 2 "discret:R3" H 6200 3350 60  0001 C CNN
F 3 "" H 6200 3350 60  0001 C CNN
	1    6200 3350
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR06
U 1 1 53B6F370
P 3400 2450
F 0 "#PWR06" H 3400 2450 30  0001 C CNN
F 1 "GND" H 3400 2380 30  0001 C CNN
F 2 "" H 3400 2450 60  0001 C CNN
F 3 "" H 3400 2450 60  0001 C CNN
	1    3400 2450
	-1   0    0    -1  
$EndComp
$Comp
L CONN_1 P5
U 1 1 54A5890A
P 6250 7000
F 0 "P5" H 6330 7000 40  0000 L CNN
F 1 "CONN_1" H 6250 7055 30  0001 C CNN
F 2 "connect:1pin" H 6250 7000 60  0001 C CNN
F 3 "" H 6250 7000 60  0000 C CNN
	1    6250 7000
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P6
U 1 1 54A58C65
P 6250 7100
F 0 "P6" H 6330 7100 40  0000 L CNN
F 1 "CONN_1" H 6250 7155 30  0001 C CNN
F 2 "connect:1pin" H 6250 7100 60  0001 C CNN
F 3 "" H 6250 7100 60  0000 C CNN
	1    6250 7100
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P7
U 1 1 54A58C8A
P 6250 7200
F 0 "P7" H 6330 7200 40  0000 L CNN
F 1 "CONN_1" H 6250 7255 30  0001 C CNN
F 2 "connect:1pin" H 6250 7200 60  0001 C CNN
F 3 "" H 6250 7200 60  0000 C CNN
	1    6250 7200
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P8
U 1 1 54A58CA3
P 6250 7300
F 0 "P8" H 6330 7300 40  0000 L CNN
F 1 "CONN_1" H 6250 7355 30  0001 C CNN
F 2 "connect:1pin" H 6250 7300 60  0001 C CNN
F 3 "" H 6250 7300 60  0000 C CNN
	1    6250 7300
	1    0    0    -1  
$EndComp
Connection ~ 2400 3500
Wire Wire Line
	2500 3700 2000 3700
Wire Wire Line
	2600 3500 2600 3400
Wire Wire Line
	5350 4250 5900 4250
Wire Wire Line
	5700 4250 5700 4850
Connection ~ 5700 4250
Wire Wire Line
	6200 2950 6200 3200
Connection ~ 6200 3700
Wire Wire Line
	6000 2550 5900 2550
Wire Wire Line
	5900 2550 5900 3700
Wire Wire Line
	5900 3700 6200 3700
Connection ~ 7300 3000
Wire Wire Line
	6750 3000 6200 3000
Connection ~ 6200 3000
Wire Wire Line
	6300 2000 6300 2150
Wire Wire Line
	3400 2100 3400 2000
Wire Wire Line
	3400 2450 3400 2400
$Comp
L PWR_FLAG #FLG07
U 1 1 550EA992
P 3000 1950
F 0 "#FLG07" H 3000 2220 30  0001 C CNN
F 1 "PWR_FLAG" H 3000 2180 30  0000 C CNN
F 2 "" H 3000 1950 60  0001 C CNN
F 3 "" H 3000 1950 60  0001 C CNN
	1    3000 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 1950 3000 2000
Connection ~ 3000 2000
$Comp
L GND #PWR08
U 1 1 550EAB37
P 3000 2550
F 0 "#PWR08" H 3000 2550 30  0001 C CNN
F 1 "GND" H 3000 2480 30  0001 C CNN
F 2 "" H 3000 2550 60  0001 C CNN
F 3 "" H 3000 2550 60  0001 C CNN
	1    3000 2550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3000 2350 3000 2550
Wire Wire Line
	2000 2200 2800 2200
Wire Wire Line
	2800 2200 2800 2450
Wire Wire Line
	2800 2450 3000 2450
Connection ~ 3000 2450
$Comp
L GND #PWR09
U 1 1 550EAF5A
P 5450 4600
F 0 "#PWR09" H 5450 4600 30  0001 C CNN
F 1 "GND" H 5450 4530 30  0001 C CNN
F 2 "" H 5450 4600 60  0000 C CNN
F 3 "" H 5450 4600 60  0000 C CNN
	1    5450 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 4600 5450 4450
Wire Wire Line
	5450 4450 5350 4450
Wire Wire Line
	6100 5150 6100 5200
Wire Wire Line
	5700 5150 5700 5200
Wire Wire Line
	7800 3200 7700 3200
Wire Wire Line
	7700 3200 7700 3300
Wire Wire Line
	7300 3550 7300 3500
NoConn ~ 6100 7000
NoConn ~ 6100 7100
NoConn ~ 6100 7200
NoConn ~ 6100 7300
$EndSCHEMATC