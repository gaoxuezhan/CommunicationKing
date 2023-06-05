/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.

  A good test for this is to try it with a GPS receiver that sends out
  NMEA 0183 sentences.

  NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
  other ATmega32U4 based boards.

  created 9 May 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/SerialEvent
*/

#include <GaoxzSuperServo.h>
#include <EEPROM.h>
#include <BH1750.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
#define BUTTON_A  9
#define BUTTON_B  6
#define BUTTON_C  5
#define WIRE Wire

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &WIRE);

BH1750 lightMeter;

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

int led = 13;
GaoxzSuperServo gaoxz;
GaoxzSuperServo gaoxz2;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  // display.display();
  // delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  // Serial.println("IO test");

  // text display tests
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("hello!");
  display.println("It's a fine day!");
  display.setCursor(0,0);
  display.display(); // actually display all of the above

  //---------------------------------------------------------------------- 
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  //----------------------------------------------------------------------
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
  //-----------------------------------------------------------------------
  pinMode(led, OUTPUT);
  //-----------------------------------------------------------------------
  gaoxz.attach(9, 650, 2500);
  gaoxz.write(EEPROM.read(0));

  gaoxz2.attach(10, 550, 2500);
  gaoxz2.write(EEPROM.read(1));  
  //-----------------------------------------------------------------------
  
}

void loop() {

  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);

    display.clearDisplay();
    display.setCursor(0,0);
    display.println(inputString);
    display.display();

    inputString.trim();
    if (inputString.equals("w")) {
      digitalWrite(led, HIGH);
      Serial.println("UNO:the led is On.");
    }
    if (inputString.equals("s")) {
      digitalWrite(led, LOW);  // turn the LED on (HIGH is the voltage level)
      Serial.println("UNO:the led is Off.");
    }
    if (inputString.equals("1")) {
      gaoxz.write(gaoxz.read() + 45);
      gaoxz2.write(gaoxz.read());
      Serial.println(gaoxz.read());
    }
    if (inputString.equals("2")) {
      gaoxz.write(gaoxz.read() - 45);
      gaoxz2.write(gaoxz.read());
      Serial.println(gaoxz.read());
    }
    if (inputString.equals("3")) {
      gaoxz.slowMotion(gaoxz.read() + 45);
      gaoxz2.slowMotion(gaoxz.read());
      Serial.println(gaoxz.read());
    }
    if (inputString.equals("4")) {
      gaoxz.slowMotion(gaoxz.read() - 45);
      gaoxz2.slowMotion(gaoxz.read());
      Serial.println(gaoxz.read());
    } 
    EEPROM.write(0, gaoxz.read());
    EEPROM.write(1, gaoxz2.read());
    delay(100);

    // clear the string:
    inputString = "";
    stringComplete = false;
  }else{
    delay(1000);
    while (!lightMeter.measurementReady(true)) {
      yield();
    }
    float lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");

    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Light: ");
    display.print(lux);
    display.println(" lx");
    display.display();

    lightMeter.configure(BH1750::ONE_TIME_HIGH_RES_MODE);

    if(lux < 13000){
      if(gaoxz.read() != 0){
        gaoxz.slowMotion(0);
        gaoxz2.slowMotion(gaoxz.read());
        EEPROM.write(0, gaoxz.read());
        EEPROM.write(1, gaoxz2.read());
      }
    }
    
    if(lux > 15000){
      if(gaoxz.read() != 180){
        gaoxz.slowMotion(180);
        gaoxz2.slowMotion(gaoxz.read());
        EEPROM.write(0, gaoxz.read());
        EEPROM.write(1, gaoxz2.read());
      }
    }

  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    //inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }else{
      inputString += inChar;
    }
  }
}

