#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimerOne.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire1);

//-------------------------------------------------------------------
#include "DFRobot_BMP280.h"
typedef DFRobot_BMP280_IIC    BMP;    // ******** use abbreviations instead of full names ********
BMP   bmp(&Wire, BMP::eSdoLow);
#define SEA_LEVEL_PRESSURE    1015.0f   // sea level pressure
//-------------------------------------------------------------------

void setup() {

  Wire.setSDA(0);
  Wire.setSCL(1);

  Wire1.setSDA(2);
  Wire1.setSCL(3);

  Serial.begin(9600);
//--------------------------------------------------
  bmp.reset();
  Serial.println("bmp read data test");
  while(bmp.begin() != BMP::eStatusOK) {
    Serial.println("bmp begin faild");
    printLastOperateStatus(bmp.lastOperateStatus);
    delay(2000);
  }
  Serial.println("bmp begin success");
  delay(100);
//----------------------------------------------------
  Serial.println("OLED FeatherWing test");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  Serial.println("OLED begun");
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  //-------------------------------------------------------
  pinMode(25, OUTPUT);  // 将 LED 管脚设置为输出模式
  
  Timer1.initialize(500000);  // 设置定时器的计数周期为 0.5 秒
  Timer1.attachInterrupt(blinkLED);  // 将中断处理函数关联到 Timer1，并在定时器溢出时调用 blinkLED 函数
}

void loop() {

  float   temp = bmp.getTemperature();
  uint32_t    press = bmp.getPressure();
  float   alti = bmp.calAltitude(SEA_LEVEL_PRESSURE, press);

  display.clearDisplay();
  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  display.println();
  display.println("WELCOME");
  display.print("temperature:"); 
  display.println(temp);
  display.print("pressure   :"); 
  display.println(press);
  // display.print("altitude   :"); 
  // display.println(alti);

  display.setCursor(0,0);
  display.display(); // actually display all of the above

  delay(1000);
}

// show last sensor operate status
void printLastOperateStatus(BMP::eStatus_t eStatus)
{
  switch(eStatus) {
  case BMP::eStatusOK:    Serial.println("everything ok"); break;
  case BMP::eStatusErr:   Serial.println("unknow error"); break;
  case BMP::eStatusErrDeviceNotDetected:    Serial.println("device not detected"); break;
  case BMP::eStatusErrParameter:    Serial.println("parameter error"); break;
  default: Serial.println("unknow status"); break;
  }
}

void blinkLED() {
  static bool ledState = false;  // LED 的状态（亮/灭）
  
  ledState = !ledState;  // 切换 LED 的状态
  
  digitalWrite(ledPin, ledState);  // 设置 LED 管脚的电平
}
