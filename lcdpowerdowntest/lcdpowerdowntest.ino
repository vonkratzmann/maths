/*
 * Simple test to power down the LCD TFT via software
 *
 */

#define POWER_CONTROL_1 0x1A
#define POWER_CONTROL_2 0x1B
#define POWER_CONTROL_3 0x1C
#define POWER_CONTROL_4 0x1D
#define POWER_CONTROL_5 0x1E
#define POWER_CONTROL_6 0x1F
#define DISPLAY_CONTROL_3 0x28
#define OSC_CONTROL_1 0x19

#define VCOMG_BIT 6
#define PON_BIT 4
#define DK_BIT 3
#define STB_BIT 0

#define GON_BIT 5
#define DTE_BIT 4
#define D1_BIT 3
#define D0_BIT 2

#define OSC_EN_BIT 0

#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

uint8_t Orientation = 1;                    //LANDSCAPE
const int XP = 8, XM = A2, YP = A3, YM = 9; //ID=0x9341
const int TS_LEFT = 148, TS_RT = 913, TS_TOP = 920, TS_BOT = 155;

uint16_t ID;

void setup()
{
    tft.reset();
    ID = tft.readID();
    tft.begin(ID);
    Serial.begin(9600);
    Serial.println("Started");
    initLcdWithMag("testing power down");
    delay(1000);

    int i = tft.readReg(0);
    Serial.print("Id: ");
    Serial.println(i, HEX);

    Serial.println("attempting displayOff");
    displayOff();
    delay(1000);
    Serial.println("attempting displayOn");
    displayOn();
    delay(1000);
    initLcdWithMag("Back after display on");
    delay(1000);
    Serial.println("displayOn finished");

    // powerSupplyHalt();
}

void loop()
{
    delay(10);

    //   pinMode(XM, OUTPUT); //restore shared pins
    //   pinMode(YP, OUTPUT);
}

void displayOff()
{
    uint8_t command;

    command = 0;
    command |= (1 << GON_BIT);
    command |= (1 << DTE_BIT);
    command |= (1 << D1_BIT);
    command &= ~(1 << D0_BIT);

    Serial.print("displayOff command: ");
    Serial.println(command, HEX);
    tft.WriteCmdData(DISPLAY_CONTROL_3, command);
    delay(500);

    command = 0;
    command |= (1 << D0_BIT);
    Serial.print("displayOff command: ");
    Serial.println(command, HEX);
    tft.WriteCmdData(DISPLAY_CONTROL_3, D0_BIT);
    delay(500);
}

void displayOn()
{
    uint8_t command;

    command = 0;
    command |= (1 << GON_BIT);
    command |= (1 << DTE_BIT);
    command |= (1 << D1_BIT);
    command &= ~(1 << D0_BIT);

    Serial.print("displayOn command: ");
    Serial.println(command, HEX);
    tft.WriteCmdData(DISPLAY_CONTROL_3, command);
    delay(500);

    command = 0;
    command |= (1 << GON_BIT);
    command |= (1 << DTE_BIT);
    command |= (1 << D1_BIT);
    command |= (1 << D1_BIT);
    command |= (1 << D0_BIT);
    Serial.print("displayOn command: ");
    Serial.println(command, HEX);
    tft.WriteCmdData(DISPLAY_CONTROL_3, D0_BIT);
    delay(500);
}

void powerSupplyHalt()
{
    uint8_t command;

    command = tft.readReg(POWER_CONTROL_6);
    command &= ~(1 << PON_BIT);
    command &= ~(1 << VCOMG_BIT);
    command |= (1 << DK_BIT);
    Serial.print("powerSupplyHalt  command: ");
    Serial.println(command, HEX);
    tft.WriteCmdData(POWER_CONTROL_6, command);
    delay(500);

    command = tft.readReg(POWER_CONTROL_6);
    command |= (1 << STB_BIT);
    Serial.print("powerSupplyHalt  command: ");
    Serial.println(command, HEX);
    tft.WriteCmdData(POWER_CONTROL_6, command);
    delay(500);

    command = tft.readReg(OSC_CONTROL_1);
    command &= ~(1 << OSC_EN_BIT);
    Serial.print("powerSupplyHalt  command: ");
    Serial.println(command, HEX);
    tft.WriteCmdData(OSC_CONTROL_1, command);
    delay(500);
}

void initLcdWithMag(char *msg)
{
    tft.setRotation(Orientation);
    tft.fillScreen(0x0000);
    tft.setCursor(0, 0);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(2);
    tft.println(msg);
}