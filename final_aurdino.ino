
#include <WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;

const char* ssid ="AEIE_AP"; //Your Network SSID
const char* password = "12345678"; //Your Network Password

WiFiClient client;
unsigned long myChannelNumber = 2237283;
const char* myWriteAPIKey = "UJNR6LH8E6MJ2CRY";  //  Enter your Write API key from ThingSpeak
const char * myReadAPIKey= "JOSAN7USB22WPHTJ";
const char* host = "api.thingspeak.com";

int ldrValue;
int LDRpin = 32;
// Threshold values to determine night or day
const int nightThreshold = 300;  // Adjust this value based on your environment
const int dayThreshold = 800;    // Adjust this value based on your environment

const int soilMoisturePin = 34; // Analog input pin for soil moisture sensor

// Calibration constants (example values; adjust as needed)
const int dryValue = 4095;   // Raw sensor reading in dry air or air + sensor resistance
const int wetValue = 1000;   // Raw sensor reading in fully submerged water

#include <DHT.h>;
#include "DHT.h"
#define DHTPIN 4      //pin where the dht11 is connected
#define LED 5
#define RelayPin 2
#define DHTTYPE DHT11

DHT dht (DHTPIN, DHT11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);
  dht.begin();
  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
   Serial.println("");
    Serial.println("Wifi connected");
  ThingSpeak.begin(client);
  delay(100);
  pinMode(LDRpin,INPUT);
  pinMode(LED, OUTPUT);
  pinMode(RelayPin, OUTPUT);
}

void upload_data()
{
      float h = dht.readHumidity();
      float t = dht.readTemperature();
   
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }
 
                         if (client.connect(host,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            String myString = String(myWriteAPIKey);
                             String postStr = myString;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+myString+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" °C, Humidity: ");
                             Serial.print(h);
                             Serial.println("%. Publishing to Thingspeak.");
                             ThingSpeak.setField(1, t);
                             ThingSpeak.setField(2, h);
                             delay(2000);
                        }
          client.stop();
 
          Serial.println("Waiting...");
}
void loop() {
  // put your main code here, to run repeatedly:
  upload_data();
  
  ldrValue = analogRead (LDRpin);
  float value = ThingSpeak.readFloatField(myChannelNumber, 5, myReadAPIKey);;
  Serial.print("Required soil moisture value: ");
  Serial.println(value);

  float curr_moisture = read_moisture();  // Read moisture data
  
  float lightIntensity = map(ldrValue, 0, 4095, 0, 100);  // Assuming a 12-bit ADC (0-4095 range)

  // Print the LDR value and light intensity
  Serial.print("LDR Value: ");
  Serial.print(ldrValue);
  Serial.print("\tLight Intensity: ");
  Serial.println(lightIntensity);
  ThingSpeak.setField(3, ldrValue);
  if(curr_moisture<value){
     Serial.print("Current soil moisture value is less than required soil mositure.");
    // Determine if it's night or day
    if (ldrValue < nightThreshold) {
      Serial.println("It is night");
      digitalWrite(LED, LOW);
      digitalWrite(RelayPin, LOW);
      Serial.println("Its night no need for irrigation....... MOTOR OFF!!!!!!!");
    } 
    else {
      Serial.println("It is day");
      digitalWrite(LED, HIGH); // turn the LED on.
      digitalWrite(RelayPin, HIGH);
      Serial.println("!!!!! MOTOR ON !!!!!!!");
    }
  }
  else{
      Serial.print("Current soil moisture value is not less than required soil mositure.");
      Serial.println("Sufficient water in the field ...... MOTOR OFF!!!!!!!");
      digitalWrite(LED, LOW);            //Initially LED is turned off
      digitalWrite(RelayPin, LOW);
  }
  delay(400);

  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
}

float read_moisture(){
    int sensorValue = analogRead(soilMoisturePin);

    // Calculate moisture percentage based on linear interpolation
    float moisturePercentage = map(sensorValue, dryValue, wetValue, 0, 100);
    moisturePercentage = constrain(moisturePercentage, 0, 100);

    Serial.print("Sensor Value: ");
    Serial.print(sensorValue);
    Serial.print("\tMoisture Percentage: ");
    Serial.print(moisturePercentage, 2);
    Serial.println("%");
    ThingSpeak.setField(4, moisturePercentage);
    delay(1000);
    return moisturePercentage;
    
}
