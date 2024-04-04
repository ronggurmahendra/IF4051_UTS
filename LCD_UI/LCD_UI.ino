#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library
MCUFRIEND_kbv tft;

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>

#include <FreeDefaultFonts.h>

#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define WHITE   0xFFFF
#define GREY    0x8410

void showReceivedString(String receivedString)
{
    tft.fillScreen(BLACK);

    // Split the received string into individual lines
    int yPos = 40; // Initial Y position for the first line
    int lineHeight = 20; // Adjust this according to your font size

    // Tokenize the received string based on 'q'
    int lineStart = 0;
    int lineEnd = receivedString.indexOf('q', lineStart);
    while (lineEnd != -1) {
        // Extract the line from the received string
        String line = receivedString.substring(lineStart, lineEnd);
        
        // Display the line on the LCD using showmsgXY()
        showmsgXY(0, yPos, 2, NULL, line.c_str());
        
        // Update Y position for the next line
        yPos += lineHeight;

        // Move to the next line
        lineStart = lineEnd + 1;
        lineEnd = receivedString.indexOf('q', lineStart);
    }

    // Display the last line (if any)
    if (lineStart < receivedString.length()) {
        String lastLine = receivedString.substring(lineStart);
        showmsgXY(0, yPos, 2, NULL, lastLine.c_str());
    }
}



void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg)
{
    int16_t x1, y1;
    uint16_t wid, ht;
    tft.drawFastHLine(0, y, tft.width(), WHITE);
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(GREEN);
    tft.setTextSize(sz);
    tft.print(msg);
}



void setup(void)
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    if (ID == 0xD3D3) ID = 0x9481; //force ID if write-only display
    tft.begin(ID);
    tft.setRotation(0);
}

void loop(void)
{
  // showReceivedString("STATE : 1  \n  NIM : 13519008  \n  Saldo : 15000  \n  Amount  :   \n ");
    if (Serial.available()) {
        String receivedString = Serial.readStringUntil('\n');
        // Call showReceivedString() to display the received string on the LCD
        showReceivedString(receivedString);
    }
}

