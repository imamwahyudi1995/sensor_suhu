#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <FirebaseArduino.h>

// set firebase and wifi
#define FIREBASE_HOST "smartgas-c28af.firebaseio.com"
#define FIREBASE_AUTH "8B404Oscr4HTsFVDa98waTvVjiumgLQuUTfvaZQE"
#define WIFI_SSID "Semarang"
#define WIFI_PASSWORD "Semarang"

//Millis
unsigned long interval = 1000;
unsigned long previousMillis = 0;

// Define NTP Client to get time
const long utcOffsetInSeconds = 7 * 60 * 60;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// sensor pressure
const int analogInPin = A0;  

//Data Grafik
int akhir;
int awal;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

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

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  timeClient.begin();

  Firebase.setString("SmartGas/smartgas1/name", "smartgas1");
  Firebase.setString("SmartGas/smartgas1/password", "123");
}

void LPG(){
  //DateTime
  timeClient.update();
  String formattedDate = timeClient.getFormattedDate();
  String dayStamp = formattedDate.substring(0, formattedDate.indexOf("T"));
  unsigned long Date = timeClient.getEpochTime();
  String DateMillis = String(Date) + "000";

  unsigned long currentMillis = millis();

  if((unsigned long) (currentMillis - previousMillis) >= interval){
    //First data & last data 1 day
    const int numReading = 86399;   //1 day array
    int reading[numReading];
    int index;

    //looping array 0 - 86399
    for(int index=0; index<numReading; index++){
    reading[index] = analogRead(analogInPin);
    float voltage = (reading[index]*3.3)/1024;
    float KPa = (voltage-0.475)*350;
    float bar = KPa/100;
    float MPA = bar/10;
    int persen = MPA/0.8*100;
    reading[index] = persen;
    Firebase.setInt("SmartGas/smartgas1/Regulator/value", persen); //realtime
    awal = (reading[0]);    //first array - detik ke 1
    Serial.print("Nilai Awal :");
    Serial.println(awal);
    delay(1000);
    }
    akhir = (reading[86398]);   //last array - detik ke 9
    Serial.print("Nilai Akhir : ");
    Serial.println(akhir);
    Serial.print("Tanggal   : ");
    Serial.println(dayStamp);
    Serial.print("Tanggal   : ");
    Serial.println(DateMillis);
    Serial.print("\n");
    
    previousMillis = millis();

    //detik ke 10 mengirim ke firebase
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["date"] = dayStamp ;
    root["tgl"] = DateMillis ;
    root["firstValue"] = awal ;
    root["lastValue"] = akhir ;
  
    String name = Firebase.push("SmartGas/smartgas1/Grafik", root);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  LPG();
}
