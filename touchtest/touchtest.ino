/*
 * Simple test touch on LCD TFT
 * 
 * Displays touch points on the serial interface
 * 
 */
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;       
#include <TouchScreen.h>

TSPoint tp;

//uint8_t Orientation = 0;    //PORTRAIT
//const int XP = 8, XM = A2, YP = A3, YM = 9; //ID=0x9341
//const int TS_LEFT = 173, TS_RT = 913 TS_TOP = 148, TS_BOT = 913;


uint8_t Orientation = 1;    //LANDSCAPE
const int XP = 8, XM = A2, YP = A3, YM = 9; //ID=0x9341
const int TS_LEFT = 148, TS_RT = 913, TS_TOP = 920, TS_BOT = 155;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define MINPRESSURE 200
#define MAXPRESSURE 1000

uint16_t ID;

void setup() {
  tft.reset();
  ID = tft.readID();
  tft.begin(ID);
  Serial.begin(9600);
  Serial.println("Started");

  tft.setRotation(Orientation);
  tft.fillScreen(0x0000);

  tft.setCursor(0, 0);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.println("Started - touch screen");
}

void loop() {
  uint16_t xpos, ypos;  //screen coordinates
  tp = ts.getPoint();   //tp.x, tp.y are ADC values

  pinMode(XM, OUTPUT);  //restore shared pins
  pinMode(YP, OUTPUT);

  if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
    // most mcufriend have touch (with icons) that extends below the TFT
    // screens without icons need to reserve a space for "erase"
    // scale the ADC values from ts.getPoint() to screen values e.g. 0-239
    

//for portrait
//    xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
//    ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());
    
//for landscape - had to swap x and y to make it work
    ypos = map(tp.x, TS_RT, TS_LEFT, 0, tft.height());
    xpos = map(tp.y, TS_BOT, TS_TOP, 0, tft.width());

    Serial.print ("xpos:= ");   Serial.print (xpos);   Serial.print ("  ypos:= ");   Serial.println (ypos);
  }
}
