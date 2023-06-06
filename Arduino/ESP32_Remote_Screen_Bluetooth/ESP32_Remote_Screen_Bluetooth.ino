//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_ADDR 0x3C // OLED 显示屏的 I2C 地址

Adafruit_SSD1306 display(128, 64, &Wire);

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

// ------------------------------------------------------------------------------

static const unsigned char PROGMEM wen[] =
{
/*--  文字:  温  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x23,0xF8,0x12,0x08,0x12,0x08,0x83,0xF8,0x42,0x08,0x42,0x08,0x13,0xF8,
0x10,0x00,0x27,0xFC,0xE4,0xA4,0x24,0xA4,0x24,0xA4,0x24,0xA4,0x2F,0xFE,0x00,0x00
};
static const unsigned char PROGMEM du[] =
{
/*--  文字:  度  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x01,0x00,0x00,0x80,0x3F,0xFE,0x22,0x20,0x22,0x20,0x3F,0xFC,0x22,0x20,0x22,0x20,
0x23,0xE0,0x20,0x00,0x2F,0xF0,0x24,0x10,0x42,0x20,0x41,0xC0,0x86,0x30,0x38,0x0E
};
static const unsigned char PROGMEM shi[] =
{
/*--  文字:  湿  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x27,0xF8,0x14,0x08,0x14,0x08,0x87,0xF8,0x44,0x08,0x44,0x08,0x17,0xF8,
0x11,0x20,0x21,0x20,0xE9,0x24,0x25,0x28,0x23,0x30,0x21,0x20,0x2F,0xFE,0x00,0x00
};
static const unsigned char PROGMEM qi[] =
{
/*--  文字:  气  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x10,0x00,0x10,0x00,0x3F,0xFC,0x20,0x00,0x4F,0xF0,0x80,0x00,0x3F,0xF0,0x00,0x10,
0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x0A,0x00,0x0A,0x00,0x06,0x00,0x02
};
static const unsigned char PROGMEM ya[] =
{
/*--  文字:  压  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x3F,0xFE,0x20,0x00,0x20,0x80,0x20,0x80,0x20,0x80,0x20,0x80,0x2F,0xFC,
0x20,0x80,0x20,0x80,0x20,0x90,0x20,0x88,0x20,0x88,0x40,0x80,0x5F,0xFE,0x80,0x00
};
static const unsigned char PROGMEM guang[] =
{
/*--  文字:  光  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x01,0x00,0x21,0x08,0x11,0x08,0x09,0x10,0x09,0x20,0x01,0x00,0xFF,0xFE,0x04,0x40,
0x04,0x40,0x04,0x40,0x04,0x40,0x08,0x42,0x08,0x42,0x10,0x42,0x20,0x3E,0xC0,0x00
};
static const unsigned char PROGMEM zhao[] =
{
/*--  文字:  照  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x7D,0xFC,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x94,0x7D,0x08,0x44,0xFC,
0x44,0x84,0x44,0x84,0x44,0x84,0x7C,0xFC,0x00,0x00,0x48,0x88,0x44,0x44,0x84,0x44
};
static const unsigned char PROGMEM sheshidu[] =
{
/*--  文字:  ℃  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x60,0x00,0x91,0xF4,0x96,0x0C,0x6C,0x04,0x08,0x04,0x18,0x00,0x18,0x00,0x18,0x00,
0x18,0x00,0x18,0x00,0x18,0x00,0x08,0x00,0x0C,0x04,0x06,0x08,0x01,0xF0,0x00,0x00
};
static const unsigned char PROGMEM sheshiduWithoutC[] =
{
/*--  文字:  ℃  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x60,0x00,0x90,0x00,0x90,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


static const unsigned char PROGMEM baifenhao[] =
{
/*--  文字:  ％  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x18,0x04,0x24,0x08,0x24,0x10,0x24,0x20,0x24,0x40,0x24,0x80,0x19,0x00,
0x02,0x60,0x04,0x90,0x08,0x90,0x10,0x90,0x20,0x90,0x40,0x90,0x00,0x60,0x00,0x00
};

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

int led = 13;

String temperature = "66.66";
String humidity = "66";
String pressure = "666.66";
String light = "66666";

void setup() {
  Serial.begin(9600);
  SerialBT.begin("HotConnect32"); //Bluetooth device name
  SerialBT.setPin("0106");
  Serial.println("The device started, now you can pair it with bluetooth!");
  
  // ----------------------------------------
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(led, OUTPUT);
  //------------------------------------------
  Wire.begin(21, 22); // 设置 SDA 和 SCL 引脚

  // 初始化 OLED 显示屏
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("无法连接到 OLED 显示屏");
    while (1);
  }

  // Clear the buffer.
  display.clearDisplay();
  display.display(); 

}

void loop() {
  // print the string when a newline arrives:
  if (SerialBT.available()){
    serialEvent();
  }
  if (stringComplete) {
    inputString.trim();
    if (inputString.equals("w")) {
      digitalWrite(led, HIGH);
      SerialBT.println("ESP32:the led is On.");
    }

    if (inputString.equals("s")) {
      digitalWrite(led, LOW);  // turn the LED on (HIGH is the voltage level)
      SerialBT.println("ESP32:the led is Off.");
    }

   if (inputString.startsWith("temperature")) {
      inputString.replace("temperature", "");
      if(isNumeric(inputString) == true){
        temperature = inputString;
      }
    }
    if (inputString.startsWith("humidity")) {
      inputString.replace("humidity", "");
      if(isNumeric(inputString) == true){
        humidity = inputString;
      }
    }
    if (inputString.startsWith("pressure")) {
      inputString.replace("pressure", "");
      if(isNumeric(inputString) == true){
        pressure = inputString;
      }
    }

    if (inputString.startsWith("light")) {
      inputString.replace("light", "");
      if(isNumeric(inputString) == true){
        light = inputString;
      }
    }

    SerialBT.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

  display.clearDisplay();
  // text display tests
  display.drawBitmap(0, 0, wen, 16, 16, 1); //画出字符对应点阵数据
  display.drawBitmap(16, 0, du, 16, 16, 1); //画出字符对应点阵数据
  display.drawBitmap(72, 3, sheshiduWithoutC, 16, 16, 1); //画出字符对应点阵数据
  // display.drawBitmap(64, 0, sheshidu, 16, 16, 1); //画出字符对应点阵数据
  display.drawBitmap(0, 16, shi, 16, 16, 1); //画出字符对应点阵数据
  display.drawBitmap(16, 16, du, 16, 16, 1); //画出字符对应点阵数据
  // display.drawBitmap(64, 16, baifenhao, 16, 16, 1); //画出字符对应点阵数据
  display.drawBitmap(0, 32, qi, 16, 16, 1); //画出字符对应点阵数据
  display.drawBitmap(16, 32, ya, 16, 16, 1); //画出字符对应点阵数据
  display.drawBitmap(0, 48, guang, 16, 16, 1); //画出字符对应点阵数据
  display.drawBitmap(16, 48, zhao, 16, 16, 1); //画出字符对应点阵数据
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(40, 4);
  display.print(temperature);
  display.println(" C");
  display.setCursor(40, 20);
  display.print(humidity);
  display.println("%");
  display.setTextSize(1);
  display.setCursor(40, 36);
  display.print(pressure);
  display.println(" hPa");
  display.setCursor(40, 52);
  display.print(light);
  display.println(" lux");
  // display.println("It's a fine day!");
  display.setCursor(0, 0);
  display.display(); // actually display all of the above
  
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (SerialBT.available()) {
    // get the new byte:
    char inChar = (char)SerialBT.read();
    // add it to the inputString:
    // inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }else{
      inputString += inChar;
    }
  }
}

bool isNumeric(String str) {
  bool hasDecimalPoint = false;
  
  for (int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    
    if (c == '.') {
      // 如果已经有小数点了，或者小数点是第一个或最后一个字符，返回 false
      if (hasDecimalPoint || i == 0 || i == str.length() - 1) {
        return false;
      }
      
      hasDecimalPoint = true;
    } else if (!isDigit(c)) {
      // 如果字符既不是数字也不是小数点，返回 false
      return false;
    }
  }
  
  return true;
}
