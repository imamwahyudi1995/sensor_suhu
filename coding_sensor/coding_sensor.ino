#include <ESP8266WiFi.h>

#include <DHT.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

#include <FirebaseArduino.h>

#include <Wire.h>
#include <Adafruit_MLX90614.h>

// Set these to run example.
#define FIREBASE_HOST "suhu-4b447.firebaseio.com"
#define FIREBASE_AUTH "xjo8HaXBT9DMBgXtn28CKNl8rtgBRD7d8rycB2Bc"
#define WIFI_SSID "@wifi.id"
#define WIFI_PASSWORD "12345678!?"
#define LED D4

//#define DHTPIN 0 //D3 Pin DHT11
#define DHTPIN D5 //Pin DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();


const long utcOffsetInSeconds = 25200;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  Serial.println("Adafruit MLX90614 test");  
  mlx.begin();
  dht.begin(); //memulai pembacaan sensor dht11  

  pinMode(LED,OUTPUT);
  digitalWrite(LED,0);
  Firebase.setInt("StatusPenghangat",0);

  timeClient.begin();
}

void loop() {
  // set value
  timeClient.update();
  Firebase.setFloat("suhu/suhu_ambient", mlx.readAmbientTempC());
  Firebase.setFloat("suhu/suhu_object", mlx.readObjectTempC());


   float t = dht.readTemperature();
   float h = dht.readHumidity();
   if (isnan(h) || isnan(t)) {                                                // Check if any reads failed and exit early (to try again).
    Serial.println("Failed to read from DHT sensor! Ignoring it! ");
   
  }else {
  Firebase.setFloat("suhu/suhu_ruang", t);
  Firebase.setFloat("suhu/suhu_kelembaban", h);
  }

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["waktu"] = timeClient.getFormattedTime() ;
  root["suhu_ambang"] = mlx.readAmbientTempC() ;
  root["suhu_object"] = mlx.readObjectTempC() ;

  String name = Firebase.push("suhu/data_suhu", root);
  
  Serial.print(Firebase.getInt("StatusPenghangat"));

  if(Firebase.getInt("StatusPenghangat")){
    digitalWrite(LED,HIGH);
  }
  else {
    digitalWrite(LED,LOW);
  }
  delay(1000);
}
