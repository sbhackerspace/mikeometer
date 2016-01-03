// http://www.adafruit.com/products/420

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#define F2(progmem_ptr) (const __FlashStringHelper *)progmem_ptr
#include <EEPROM.h>
//#include <avr/wdt.h>
/*
  #define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
  #define LAT 10
  #define OE  9
  #define A   A0
  #define B   A1
  #define C   A2
  #define resetpin 5
  #define NEWLINE '\n'
*/

//Or, if you're using a Mega
#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, true);

//There's no RAM left on an Uno.
//BITCH WE MEGA NOW
String str;
const char fuckyou1[] PROGMEM = "BLAME";
const char fuckyou2[] PROGMEM = "MIKE";
String strbuf = "";
int    textX   = matrix.width(),
       textMin = sizeof(str) * -16, //a safe default
       hue     = 0,
       hue2 = 128;
float millidays = 0;
bool iwasreset = true;
//String ohbill = "1234567890ABCDEF Dan Sucks Ordinary Normal Dongs"; //debug
String fuckyou;
int stringlength = 7;
//char serbuf[64];

typedef struct{
  char currentChar;
  char previousChar;
  bool headerReceived;
  bool tailerReceived = false;
  String incommingString = "";
  unsigned long headerTimestamp;
  int headerTimeout = 500;
} parserstate_t;

parserstate_t parser;


boolean hasData(void){
  return parser.tailerReceived;
}

void clearState(void){
  parser.tailerReceived = false;
  parser.headerReceived = false;
  parser.currentChar = 0;
  parser.previousChar = 0;
}

//Serial RX Buffer
void serialEvent(){
  //Update state
  parser.previousChar = parser.currentChar;
  parser.currentChar = Serial.read();

  //Serial.print(parser.currentChar);
  //Serial.println(parser.previousChar);

  //Header has been received
  if(parser.currentChar == '@' && parser.previousChar == '@' && parser.headerReceived == false) {
    //Serial.println("Header Received");
    parser.headerTimestamp = millis();
    parser.headerReceived = true;
    parser.tailerReceived = false;
    parser.currentChar = '\0';
    parser.previousChar = '\0';
    parser.incommingString = ""; 
    
  }
  
  //Tail has been received
  else if(parser.headerReceived && !parser.tailerReceived){
    parser.incommingString += parser.currentChar;
    if(parser.currentChar == '@' && parser.previousChar == '@') {
      //Serial.println("Tailer Received");
      parser.tailerReceived = true;
      parser.headerReceived = false;
      parser.currentChar = '\0';
      parser.previousChar = '\0';
    }
  }

  //Garbage
  else{
}
}

void EEPROMWriteString(String& str){
  
  if(str.length() >= EEPROM.length() || str.length() > 255) {
    Serial.println("EEROR: String longer than EEPROM");
    return;
  }
  else{
    const char* c_string;
    char stringLength = str.length();
    Serial.println(stringLength, DEC);
    c_string = str.c_str();
    EEPROM.write(0, stringLength);
    for(int i = 0; i < stringLength; i++){
      EEPROM.write(i+1, *(c_string  + i));
    }
  }
}

String EEPROMReadString(void){
  String stringBuffer;
  char stringLength = EEPROM.read(0);
  if(stringLength < 1) {
    Serial.println("Error: No length String Found");
    return stringBuffer;
  }
  else{
    for(int i=0; i < stringLength; i++){
      stringBuffer += (char)EEPROM.read(i+1);
    }
    return stringBuffer;
  }
}

void setup() {
  pinMode(7, OUTPUT); //no, i'm worse than that
  digitalWrite(7, HIGH);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setTextSize(1);
  Serial.begin(9600);
  Serial.print("WHY ARE WE IN SETUP\n");
  //EEPROM.put(0,ohbill);
  delay(100);
  fuckyou = EEPROMReadString();
  delay(100);
  //Serial.print(fuckyou);
  textMin = sizeof(fuckyou) * -(fuckyou.length()); //i am a bad person
  clearState();
}

void loop() {
  byte i;

  // Clear background
  matrix.fillScreen(0);
  matrix.setTextColor(matrix.ColorHSV(1023, 255, 255, true));
  matrix.fillScreen(0);

  //scroll
  matrix.setTextColor(matrix.ColorHSV(hue, 255, 255, true)); //blue
  matrix.setCursor(textX, 8);
  matrix.print(fuckyou);

  //counter
  matrix.setTextColor(matrix.ColorHSV(hue2, 255, 255, true)); //red
  matrix.setCursor(0, 0);
  millidays = ((float)millis() / 1000 / 86400); //86.4 seconds
  matrix.print(millidays, 3);

  //Scrolls the text. Looks dirty because it is.
  if ((--textX) < textMin) textX = matrix.width();
  // Update display
  matrix.swapBuffers(false);
  hue += 14;
  if (hue >= 1536) hue -= 1536;
  hue2 += 30;
  if (hue2 >= 1536) hue -= 1536;
  delay(50); //slows down the scroll
  // if (digitalRead(2) == HIGH)
  //{
  if (Serial.available() > 0) {
    if (hasData()) {
      if (parser.incommingString.substring(0, parser.incommingString.length() - 2) == "reset") {
        clearState();
        reset();
      }
      else if (parser.incommingString.substring(0, parser.incommingString.length() - 2) == "yo") {
        clearState();
        yo();
      }
      else{
        clearState();
        fuckyou = parser.incommingString.substring(0, parser.incommingString.length() - 2);
        EEPROMWriteString("days since " + fuckyou);
        reset();
      }
    }
  }
  //if (strbuf.equals("reset")){
  //  Serial.print("Reset found");
  //reset();
  //}
  // }

}

void reset() {
  asm volatile ("  jmp 0");
}

//Yobiquiti framework compliance
void yo() {
  matrix.fillScreen(0b1001100000011010); //branding compliance
  matrix.setTextColor(matrix.ColorHSV(400, 512, 512, false));
  matrix.setCursor(0, 0);
  matrix.setTextSize(2);
  matrix.print("Yo!");
  matrix.swapBuffers(false);
  matrix.setTextSize(1);
  delay(2600);
}
//20151227_2145
