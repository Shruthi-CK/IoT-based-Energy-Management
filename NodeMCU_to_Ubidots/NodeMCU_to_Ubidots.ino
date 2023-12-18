#include<SoftwareSerial.h>
#include<ArduinoJson.h>

SoftwareSerial s(10,11);
#include<UbidotsESPMQTT.h>


/****************************************
 * Define Constants
 ****************************************/
 
#define TOKEN "Your_Ubidots_Token" // Your Ubidots TOKEN
#define WIFINAME "Your_Wifi_Name" //Your Wifi SSID
#define WIFIPASS "Your_Wifi_Password" // Your Wifi Pass
Ubidots client(TOKEN);
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  s.begin(9600);
  while (!Serial) continue;
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);

}

void loop() {

  if(!client.connected()){
      client.reconnect();
      }
      
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& object = jsonBuffer.parseObject(s);
  if (object == JsonObject::invalid())
    return;
  Serial.println("JSON received and parsed");
  object.prettyPrintTo(Serial);
  Serial.println("--------------****---------------");  

  float value1 = object["data1"];
  float value2 = object["data2"];
  float value3 = object["data3"];
  float value4 = object["data4"];
  float value5 = object["data5"];
  float value6 = object["data6"];
  float value7 = object["data7"];
  float value8 = object["data8"];
  float value9 = object["data9"];
  float value10 = object["data10"];
  float value11 = object["data11"];
  float value12 = object["data12"];
  float value13 = object["data13"];
  float value14 = object["data14"];
  float value15 = object["data15"];
  
  client.add("Critical Load1", value1);
  client.add("Non Critical Load1", value2);
  client.add("Generation1", value3);
  client.add("Excess Power1", value4);
  client.add("Price1", value5);
   
  client.add("Critical Load2", value6);
  client.add("Non Critical Load2", value7);
  client.add("Generation2", value8);
  client.add("Excess Power2", value9);
  client.add("Price2", value10);

  client.add("Critical Load3", value11);
  client.add("Non Critical Load3", value12);
  client.add("Generation3", value13);
  client.add("Excess Power3", value14);
  client.add("Price3", value15);
   
  client.ubidotsPublish("Sending-End");
  delay(1000);
  client.loop();

}
