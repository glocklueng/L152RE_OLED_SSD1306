/*
 Demonstrate 0.96 in. OLED module (128x64/SPI). Revised for use with ST Nucleo L152RE
 
 Note:  On 16 pin OLED module from Taobao, MOSI connects to module D1
        and CLK (also called SCK) connects to module D0. Module RW and RD lines
        are connected to GND.
*/

#include "mbed.h"

#include "ssd1306.h"
#include "standard_font.h"
#include "bold_font.h"

/* definitions for ST Nucleo L152RE  */
#define CSEL  PA_4  // CS  D10
#define RST   PA_3  //     D9
#define DCMD  PA_2  // DC  D8
#define CLK   PA_5  // CLK D13
#define DATA  PA_7  // MOSI D11

// SSD1306 oled(p8 /* cs */, p9 /* reset */, p14 /* dc */, p13 /* clock */, p11 /* data */); // LPC1768
// SSD1306 oled(PTA13 /* cs */, PTD5 /* reset */, PTD0 /* dc */, PTD1 /* clock */, PTD2 /* data */); // KL25Z
// SSD1306 oled(D10 /* cs */, D9 /* reset */, D8 /* dc */, D13 /* clock */, D11 /* data */); // KL05Z or Arduino styles
SSD1306 oled(CSEL, RST, DCMD, CLK, DATA);   // STM32 Nucleo

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

static const unsigned char  logo16_glcd_bmp[] =
{ 0x00, 0xc0, // B00000000, B11000000,
  0x01, 0xc0, // B00000001, B11000000,
  0x01, 0xc0, // B00000001, B11000000,
  0x03, 0xe0, // B00000011, B11100000,
  0xf3, 0xe0, // B11110011, B11100000,
  0xfe, 0xf8, // B11111110, B11111000,
  0x7e, 0xff, // B01111110, B11111111,
  0x33, 0x9f, // B00110011, B10011111,
  0x1f, 0xfc, // B00011111, B11111100,
  0x0d, 0x70, // B00001101, B01110000,
  0x1b, 0xa0, // B00011011, B10100000,
  0x3f, 0xe0, // B00111111, B11100000,
  0x3f, 0xf0, // B00111111, B11110000,
  0x7c, 0xf0, // B01111100, B11110000,
  0x70, 0x70, // B01110000, B01110000,
  0x00, 0x30 }; // B00000000, B00110000 };

void testdrawbitmap(const unsigned char *bitmap, int w, int h) {
  uint8_t icons[NUMFLAKES][3];
  srand((unsigned int)time(NULL)); // srandom(123);     // whatever seed
  int i;
 
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = rand() % SSD1306_LCDWIDTH; // display.width();
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = rand() % 5 + 1;
#if 0
    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
#endif
  }

  i = 0;
  while (1) {
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      oled.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, 1); // WHITE);
    }
    oled.update();
    wait(0.2); // delay(200);
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      oled.drawBitmap(icons[f][XPOS], icons[f][YPOS],  logo16_glcd_bmp, w, h, 0); // BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > SSD1306_LCDHEIGHT) { // display.height()) {
        icons[f][XPOS] = rand() % SSD1306_LCDWIDTH; // display.width();
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = rand() % 5 + 1;
      }
    }
    if (i++ > 100) break;
   }

}

void testdrawline() {  
  for (int16_t i=0; i<SSD1306_LCDWIDTH; i+=4) {
    oled.line(0, 0, i, SSD1306_LCDHEIGHT-1);
    oled.update();
  }
  for (int16_t i=0; i<SSD1306_LCDHEIGHT; i+=4) {
    oled.line(0, 0, SSD1306_LCDWIDTH-1, i);
    oled.update();
  }
  wait(0.25); // delay(250);
  
  oled.clear();
  for (int16_t i=0; i<SSD1306_LCDWIDTH; i+=4) {
    oled.line(0, SSD1306_LCDHEIGHT-1, i, 0);
    oled.update();
  }
  for (int16_t i=SSD1306_LCDHEIGHT-1; i>=0; i-=4) {
    oled.line(0, SSD1306_LCDHEIGHT-1, SSD1306_LCDWIDTH-1, i);
    oled.update();
  }
  wait(0.25); // delay(250);
  
  oled.clear();
  for (int16_t i=SSD1306_LCDWIDTH-1; i>=0; i-=4) {
    oled.line(SSD1306_LCDWIDTH-1, SSD1306_LCDHEIGHT-1, i, 0);
    oled.update();
  }
  for (int16_t i=SSD1306_LCDHEIGHT-1; i>=0; i-=4) {
    oled.line(SSD1306_LCDWIDTH-1, SSD1306_LCDHEIGHT-1, 0, i);
    oled.update();
  }
  wait(0.25); // delay(250);

  oled.clear();
  for (int16_t i=0; i<SSD1306_LCDHEIGHT; i+=4) {
    oled.line(SSD1306_LCDWIDTH-1, 0, 0, i);
    oled.update();
  }
  for (int16_t i=0; i<SSD1306_LCDWIDTH; i+=4) {
    oled.line(SSD1306_LCDWIDTH-1, 0, i, SSD1306_LCDHEIGHT-1); 
    oled.update();
  }
  wait(0.25);
}

int main()
{
    oled.initialise();
    oled.clear();
    oled.set_contrast(255); // max contrast
    
    while(1)
    {
        
    oled.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16);
    oled.update();
    wait(3);
    
    testdrawline();
    
    oled.clear();
    testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);

    oled.set_font(bold_font, 8);
    oled.printf("Heading\r\n");

    oled.set_font(standard_font, 6);
    oled.printf("Hello World!\r\n");
    oled.printf("Some more text here...\r\n\r\n\r\n\r\n");
    // oled.set_font(bold_font, 8);
    oled.line(127, 0, 0, 63);

    oled.update();
    wait(1);

    int i = 10;
    while (i > 0)
    {
        wait(1);
        oled.printf("%d\r\n", i--);
        oled.update();
        oled.scroll_up();
    }
    
    oled.clear();
    
    } // end outside loop for OLED demo
}

// EOF
