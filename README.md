# mikeometer
The Hackerspace Mike(s) (all of them) are terrible people. This drives a display to quantify just how terrible.

what do  
Throw the sketch on an Arduino Mega or similar, and put a http://adafru.it/420 on it.
On reset, the sketch reads a string from EEPROM. If not initialised, it'll probably give you interesting snowcrashy weirdness.

how do  
The sketch responds to three commands over serial:
@@reset@@ resets the counter.
@@a new message@@ sets "a new message" as the scrolling message, stashes it in EEPROM, and resets.
@@yo@@ is for Yobiquity™ integration.

where's that serial supposed to come from then?  
@dloman needs to fix the code for the accompanying ESP8266.

the front fell off  
ok, wiring this thing is kinda weird  
follow the guide for connecting it up at https://learn.adafruit.com/32x16-32x32-rgb-led-matrix/connecting-with-jumper-wires

i'm criminally impatient  
display pins (viewed from back)  
2  1  
4  3  
6  5  
9  8  
10 9  
12 11  
14 13  
16 15  
  
display	arduino	what  
1		24		R1  
2		25		G1  
3		26		B1  
4		GND		ground  	
5		27		R2  
6		28		G2  
7		29		B2  
8		GND		ground  
9		A0		A select  
10		A1		B select  
11		A2		C select  
12		GND		ground  
13		11		clock  
14		A3		latch  
15		9		row strobe  
16		GND		ground  
