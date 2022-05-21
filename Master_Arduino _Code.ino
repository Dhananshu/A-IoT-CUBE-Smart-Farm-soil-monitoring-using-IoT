#include <DHT.h> // Including library for dht
#include <ESP8266WiFi.h>
String apiKey = "****************";
const char *ssid = "******";
// Enter your Write API key from ThingSpeak
// replace with your wifi ssid and wpa2 key
const char *pass = "*******";
const char* server = "api.thingspeak.com";
#define DHTPIN D2
//pin where the dht11 is connected
DHT dht(DHTPIN, DHT11);
WiFiClient client;
void setup()
{
pinMode(A0, INPUT);
Serial.begin(115200);
delay(10);
dht.begin();
Serial.println("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, pass);
while (WiFi.status() != WL_CONNECTED)
{
delay(500);
40Serial.print("."); //printing “…” until WiFi is not connected
}
Serial.println("");
Serial.println("WiFi connected");
}
void loop()
{
float h = dht.readHumidity(); //reading humidity value
float t = dht.readTemperature(); //reading temperature value
float msvalue = analogRead(A0); //reading soil moisture value in range 0-1024
float msvalue1= ((1024-msvalue)/874)*100; //normalising based on calibration
if (isnan(h) || isnan(t))
{
Serial.println("Failed to read from DHT sensor!");
return;
}
if (isnan(msvalue) )
{
Serial.println("Failed to read from Soil Moisture Sensor!");
return;
}
if (client.connect(server,80)) // "184.106.153.149"
{
String postStr = apiKey;
postStr +="&field1=";
postStr += String(t);
41postStr +="&field2=";
postStr += String(h);
postStr +="&field3=";
postStr += String(msvalue1);
postStr += "\r\n\r\n\r\n";
client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n\n");
client.print(postStr);
Serial.print("Temperature: ");
Serial.print(t);
Serial.print(" degrees Celcius, Humidity: ");
Serial.print(h);
Serial.print("Soil Moisture: ");
Serial.print(msvalue1);
Serial.println("%. Send to Thingspeak.");
}
client.stop();
Serial.println("Waiting...");
// thingspeak needs minimum 15 sec delay between updates
delay(1000);
}