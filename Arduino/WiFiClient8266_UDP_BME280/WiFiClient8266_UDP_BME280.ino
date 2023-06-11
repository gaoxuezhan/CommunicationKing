#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "DFRobot_BME280.h"

typedef DFRobot_BME280_IIC    BME;    // ******** use abbreviations instead of full names ********

/**IIC address is 0x77 when pin SDO is high (BME280 sensor module)*/
/**IIC address is 0x76 when pin SDO is low  */
BME   bme(&Wire, 0x76);

#define SEA_LEVEL_PRESSURE    1015.0f   // sea level pressure
//----------------------------------------------------------------------

// Wi-Fi网络参数
const char* ssid = "ccc";
const char* password = "cCon8038";

// UDP 服务器参数
const char* udpServerIP = "192.168.1.101"; // UDP 服务器的IP地址
const char* udpServerIP2 = "192.168.1.18"; // UDP 服务器的IP地址
const int udpServerPort = 1234; // UDP 服务器的端口号

WiFiUDP udpClient;

int bufferIndex = 0;
char replyBuffer[255] = {};

void setup() {
  Serial.begin(9600);

  // 连接 Wi-Fi 网络
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // 初始化 UDP
  udpClient.begin(8888);

  //----------------------------------
  Serial.println("bme config test");
  while(bme.begin() != BME::eStatusOK) {
    Serial.println("bme begin faild");
    printLastOperateStatus(bme.lastOperateStatus);
    delay(2000);
  }
  Serial.println("bme begin success");

  bme.setConfigFilter(BME::eConfigFilter_off);        // set config filter
  bme.setConfigTStandby(BME::eConfigTStandby_125);    // set standby time
  bme.setCtrlMeasSamplingTemp(BME::eSampling_X8);     // set temperature over sampling
  bme.setCtrlMeasSamplingPress(BME::eSampling_X8);    // set pressure over sampling
  bme.setCtrlHumiSampling(BME::eSampling_X8);         // set humidity over sampling
  bme.setCtrlMeasMode(BME::eCtrlMeasMode_normal);     // set control measurement mode to make these settings effective

  delay(100);

}


void loop() {
  float   temp = bme.getTemperature();
  uint32_t    press = bme.getPressure();
  float   humi = bme.getHumidity();


  // 向 UDP 服务器发送数据
  udpClient.beginPacket(udpServerIP, udpServerPort);
  // udpClient.print("Hello, UDP Server!");

  udpClient.print("temperature:"); 
  udpClient.print(temp);
  udpClient.print("@");
  udpClient.print("pressure:"); 
  udpClient.print(press);
  udpClient.print("@");
  udpClient.print("humidity:"); 
  udpClient.print(humi);
  udpClient.print("@");

  udpClient.endPacket();

    // 向 UDP 服务器发送数据
  udpClient.beginPacket(udpServerIP2, udpServerPort);
  // udpClient.print("Hello, UDP Server!");

  udpClient.print("temperature:"); 
  udpClient.print(temp);
  udpClient.print("@");
  udpClient.print("pressure:"); 
  udpClient.print(press);
  udpClient.print("@");
  udpClient.print("humidity:"); 
  udpClient.print(humi);
  udpClient.print("@");

  udpClient.endPacket();

  delay(1000);

  // 接收服务器的回复
  int packetSize = udpClient.parsePacket();

  if (packetSize) {
    while (udpClient.available()) {
      char c = udpClient.read();
      if (c == '@') {
        Serial.println("get a @!!!");
        replyBuffer[bufferIndex] = '\0';  // 在字符串末尾添加空字符
        String replyMessage = String(replyBuffer);
        Serial.print("Received reply from server: ");
        Serial.println(replyMessage);

        // 缓冲区再初始化
        bufferIndex = 0;
        replyBuffer[255] = {};

        break;
      }else{
        replyBuffer[bufferIndex++] = c;
      }
    }
  }
}

// show last sensor operate status
void printLastOperateStatus(BME::eStatus_t eStatus)
{
  switch(eStatus) {
  case BME::eStatusOK:    Serial.println("everything ok"); break;
  case BME::eStatusErr:   Serial.println("unknow error"); break;
  case BME::eStatusErrDeviceNotDetected:    Serial.println("device not detected"); break;
  case BME::eStatusErrParameter:    Serial.println("parameter error"); break;
  default: Serial.println("unknow status"); break;
  }
}
