#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#include <Wire.h>
#include <Adafruit_MLX90614.h>

// Set these to run example.
#define FIREBASE_HOST "suhu-4b447.firebaseio.com"
#define FIREBASE_AUTH "xjo8HaXBT9DMBgXtn28CKNl8rtgBRD7d8rycB2Bc"
#define WIFI_SSID "@wifi.id"
#define WIFI_PASSWORD "12345678!"

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

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
}

int n = 0;

void loop() {
  // set value
  Firebase.setFloat("suhu/suhu_ambient", mlx.readAmbientTempC());
  Firebase.setFloat("suhu/suhu_object", mlx.readObjectTempC());
  
  Firebase.pushInt("suhu/data_ambient", mlx.readAmbientTempC());
  Firebase.pushInt("suhu/data_object", mlx.readObjectTempC());
  delay(500);
}
