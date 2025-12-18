# Q65_STM32_Beacon
Q65 Data Mode tone generation using STM32 Blue Pill board.
All modes appear to work, closest tone selected when generating ARR register of timer.
Designed for 2010Hz Base Frequency, much higher than this will overflow the ARR register in the timer as it's only 16bit. Prescaling lowers the tuning step resolution.

Hard Coded 13 char message and Q65 mode paramaters in main.c  
PWM output (Square Wave) - Used for driving an LED in my optical beacon!

Pin Connections:
A10 - GPS 9600bd input (for timing)
A0 - PWM audio output

Q65 Message to Symbol encoding routines are from RFzero Beacon - see https://rfzero.net/
