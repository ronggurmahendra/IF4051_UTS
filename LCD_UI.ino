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

void setup(void)
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    Serial.println("Example: Font_simple");
    Serial.print("found ID = 0x");
    Serial.println(ID, HEX);
    if (ID == 0xD3D3) ID = 0x9481; //force ID if write-only display
    tft.begin(ID);
    tft.setRotation(0);
}

void loop(void)
{
    if (Serial.available()) {
        String receivedString = Serial.readStringUntil('\n');
        receivedString.replace("\\n", "\n"); // Replace '\n' with newline character
        if (receivedString == "clear") {
            tft.fillScreen(BLACK);
        } else {
            tft.fillScreen(BLACK);
            showReceivedString(receivedString);
        }
    }
}

void showReceivedString(String receivedString)
{
    tft.setFont(&FreeSans9pt7b);
    tft.setTextColor(GREEN);
    tft.setTextSize(2);
    tft.setCursor(0, 40);
    tft.print(receivedString);
}
