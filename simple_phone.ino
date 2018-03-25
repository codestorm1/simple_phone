// from Adafruit oled feather test

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
//#include <Fonts/FreeSerif9pt7b.h>
//#include <Fonts/FreeMono18pt7b.h>
#include <Fonts/Arial8pt.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display = Adafruit_SSD1306();

#if defined(ESP8266)
  #define BUTTON_A 0
  #define BUTTON_B 16
  #define BUTTON_C 2
  #define LED      0
#elif defined(ESP32)
  #define BUTTON_A 15
  #define BUTTON_B 32
  #define BUTTON_C 14
  #define LED      13
#elif defined(ARDUINO_STM32F2_FEATHER)
  #define BUTTON_A PA15
  #define BUTTON_B PC7
  #define BUTTON_C PC5
  #define LED PB5
#elif defined(TEENSYDUINO)
  #define BUTTON_A 4
  #define BUTTON_B 3
  #define BUTTON_C 8
  #define LED 13
#elif defined(ARDUINO_FEATHER52)
  #define BUTTON_A 31
  #define BUTTON_B 30
  #define BUTTON_C 27
  #define LED 17
#else // 32u4, M0, and 328p
  #define BUTTON_A 9
  #define BUTTON_B 6
  #define BUTTON_C 5
  #define LED      13
#endif

#if (SSD1306_LCDHEIGHT != 32)
 #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

enum status {
  browsing,
  dialing,
  hanging_up
};

typedef struct {
  char* name;
  char* phone;
} phone_entry;

const int num_entries = 6;
phone_entry entries[num_entries] = {
  {"Bryan", "323 606-2999"},
  {"Guada", "323 251-4812"},
  {"Lujan", "310 926-8044"},
  {"Janet", "425 501-9757"},
  {"Village Pizza", "323 465-5566"},
  {"Numero Uno", "323 969-7661"}
};

int currentPhone = 2;
status cur_status = browsing;

void show_entries(int curPhone) {
  if (curPhone < 1) {
    curPhone == 1;
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Call ");
  display.println(entries[curPhone > 0 ? curPhone - 1 : num_entries - 1].name);
  display.print("Call ");
  display.println(entries[curPhone].name);
  display.print("Call ");
  display.println(entries[curPhone < num_entries - 1 ? curPhone + 1 : 0].name);

  display.setCursor(0,0);
  display.display(); // actually display all of the above
  
}

void show_dialing(int curPhone) {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println();
  display.print("Dialing ");
  display.println(entries[currentPhone].phone);
  display.setCursor(0,0);
  display.display(); // actually display all of the above
}

int decEntry(int curPhone) {
  int temp = curPhone > 0 ? curPhone - 1 : num_entries - 1;
  Serial.println(temp);
  return curPhone > 0 ? curPhone - 1 : num_entries - 1;
}

int incEntry(int curPhone) {
  int temp = curPhone < num_entries - 1 ? curPhone + 1 : 0;
  Serial.println(temp);
  return curPhone < num_entries - 1 ? curPhone + 1 : 0;
}

void setup() {  
  Serial.begin(9600);

  Serial.println("OLED FeatherWing test");
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  Serial.println("OLED begun");
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  display.setFont(&Arial8pt7b);

//  display.setFont();
  // Clear the buffer.
  display.display();
  
  Serial.println("IO test");

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  // text display tests
  display.setTextSize(1);
}

void loop() {
  if (!digitalRead(BUTTON_A)) { 
    cur_status = browsing;
    currentPhone = decEntry(currentPhone);
    Serial.print("Current phone: ");
    Serial.println(currentPhone);
  }

  if (!digitalRead(BUTTON_B)) {
    cur_status = dialing;
  }

  if (!digitalRead(BUTTON_C)) { 
    cur_status = browsing;
    currentPhone = incEntry(currentPhone);
    Serial.print("Current phone: ");
    Serial.println(currentPhone);
  }
  switch (cur_status) {
    case browsing:
      show_entries(currentPhone);
      break;
    case dialing:
      show_dialing(currentPhone);
      break;
  }
  delay(10);
  yield();
  display.display();
}

