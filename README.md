# mikeometer
The Hackerspace Mike(s) (all of them) are terrible people. This drives a display to quantify just how terrible.

what do
Throw the sketch on an Arduino Mega or similar, and put a http://adafru.it/420 on it.
On reset, the sketch reads a string from EEPROM. If not initialised, it'll probably give you interesting snowcrashy weirdness.

how do
The sketch responds to three commands over serial:
@@reset@@ resets the counter.
@@message@@ sets a new scrolling message, stashes it in EEPROM, and resets the counter.
@@yo@@ is for Yobiquity™ integration.

where's that serial supposed to come from then?
@dloman needs to fix the code for the accompanying ESP8266.
