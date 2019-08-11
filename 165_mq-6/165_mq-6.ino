#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "smartgas-c28af.firebaseio.com"
#define FIREBASE_AUTH "8B404Oscr4HTsFVDa98waTvVjiumgLQuUTfvaZQE"
#define WIFI_SSID "Semarang"
#define WIFI_PASSWORD "Semarang"

float sensorValue;
float sensorVolts;
const int solePin = D3;
const int buzzer = D4;
 
void setup()
{
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
  pinMode(solePin, OUTPUT);
  digitalWrite(solePin, HIGH);
  Serial.begin(9600);      // sets the serial port to 9600

  // CONNECT TO WIFI
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting!");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected : ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);

  Firebase.setString("SmartGas/smartgas1/name", "smartgas1");
  Firebase.setString("SmartGas/smartgas1/password", "123"); 
}

void kadar_gas()
{
    float sensorValue = analogRead(A0); // read analog input pin 0
    float persen = sensorValue/100;
    
    Serial.print("PPM = ");
    Serial.print(sensorValue);
    Serial.println(" ppm");
    Serial.print("Persen = ");
    Serial.println(persen);
    Serial.println("\n");

    Firebase.setFloat("SmartGas/smartgas1/Gas/kadar", persen);
 
    if(sensorValue < 250){
      digitalWrite(solePin, HIGH);
      digitalWrite(buzzer, HIGH);
      }
 
    if(sensorValue >= 250) {
      Serial.println("LPG GAS DETECTED!");
      digitalWrite(buzzer, LOW);
      digitalWrite(solePin, LOW);
      delay(750);
      digitalWrite(solePin, HIGH);
      delay(750);
      digitalWrite(solePin, LOW);
      delay(750);
      digitalWrite(solePin, HIGH);
      delay(750);
      digitalWrite(solePin, LOW);
      delay(750);
      digitalWrite(solePin, HIGH);
      delay(750);
      digitalWrite(solePin, LOW);
      delay(750);
      digitalWrite(solePin, HIGH);
      delay(750);
      }
    delay(1000); 
}

void regulator()
{
  String Reg = Firebase.getString("SmartGas/smartgas1/Regulator/status");
  if(Reg == "0"){
     digitalWrite(solePin, LOW);
      delay(750);
      digitalWrite(solePin, HIGH);
      delay(750);
      digitalWrite(solePin, LOW);
      delay(750);
      digitalWrite(solePin, HIGH);
      delay(750);
      digitalWrite(solePin, LOW);
      delay(750);
      digitalWrite(solePin, HIGH);
      delay(750);
      digitalWrite(solePin, LOW);
      delay(750);
      digitalWrite(solePin, HIGH);
      delay(750);
  }
}

void loop()
{
  kadar_gas();
  regulator();
}
