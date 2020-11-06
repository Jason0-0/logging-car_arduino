#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
extern Adafruit_SSD1306 oled;

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

/* --------------------- 卷王号 ------------------------------------------*/


void OledInit();
void Oled_PrintLogo(bool clear);
void Oled_PrintChar(int textSize,bool isWhite,int x,int y,String str,bool displayNow,bool clearNow);
void Oled_PrintChar(int textSize,bool isWhite,int x,int y,int intNum,bool displayNow,bool clearNow);