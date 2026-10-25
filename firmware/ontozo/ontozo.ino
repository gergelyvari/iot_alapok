#include "AdafruitIO_WiFi.h"
#include "DHT.h"

#define IO_USERNAME "*****"
#define IO_KEY      "*****"
#define WIFI_SSID   "******"
#define WIFI_PASS   "******"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
AdafruitIO_Feed *feedTemp  = io.feed("noveny.homerseklet");
AdafruitIO_Feed *feedHum   = io.feed("noveny.para");
AdafruitIO_Feed *feedSoil  = io.feed("noveny.nedvesseg");
AdafruitIO_Feed *feedWater = io.feed("noveny.vizszint");
AdafruitIO_Feed *feedLastWater = io.feed("noveny.utolsolocsolas");

//Lábak
const int PIN_SOIL = 4;
const int PIN_PUMP = 5;
const int PIN_LED  = 6;
const int PIN_TRIG = 7;
const int PIN_ECHO = 15;
const int PIN_DHT  = 16;

DHT dht(PIN_DHT, DHT22);

//Beállítások
const int   SOIL_DRY = 2000;   // efölött száraz 
const float TANK_LOW_CM    = 20.0;   // ennél nagyobb távolság = alacsony víz
const int   PUMP_SECONDS   = 15;     // öntözés hossza
const float   TANK_FULL_CM   = 25.0; //tartály teljes mérete a mérés alapján

// Egy körforgás 5 mp.

int   tick = 350;  //Változó a földszárazság méréshez(fél óránként)
int   tack = 0; //Változó az adafruit feltöltéshez(percenként)
int   soilRaw = 0;
float tankCm = 0, temp = NAN, hum = NAN;
bool  tankLow = false;

float readTankCm() {
  digitalWrite(PIN_TRIG, LOW);  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  long us = pulseIn(PIN_ECHO, HIGH, 30000);
  if (us == 0) return -1;
  return us * 0.0343f / 2.0f;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_PUMP, OUTPUT); digitalWrite(PIN_PUMP, LOW);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  analogReadResolution(12);
  dht.begin();

  io.connect();
  while (io.status() < AIO_CONNECTED) { Serial.print("."); delay(500); }
  Serial.println(io.statusText());
}

void loop() {
  io.run();   // kapcsolat életben tartása, ezt rendszeresen hívni kell

  // Tartály, LED, hőmérséklet
  float d = readTankCm();  
  if (d > 0) {  //Alacsony vízszint ellenőrzés
    tankCm = d; 
    if (d > TANK_LOW_CM) { 
      tankLow = true;
    } else {
      tankLow = false;
      }
  }

  if (tankLow == true) {   //LED kapcsolás alacsony vízszintnél
    digitalWrite(PIN_LED, HIGH);
  } else {
    digitalWrite(PIN_LED, LOW);
  }

  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (!isnan(t)) temp = t;
  if (!isnan(h)) hum = h;

  // Talaj félóránként, ha száraz és van víz, 15 mp öntözés
  if (tick >= 360) {
    soilRaw = analogRead(PIN_SOIL);
    Serial.printf("Talaj: %d\n", soilRaw);
    tick = 0;
    if (soilRaw > SOIL_DRY && !tankLow) {
      Serial.println("Szivattyu: BE");
      digitalWrite(PIN_PUMP, HIGH);
      delay(PUMP_SECONDS * 1000);
      digitalWrite(PIN_PUMP, LOW);
      Serial.println("Szivattyu: KI");
      feedLastWater->save(soilRaw);
    }
  }
  
  // Feltöltés percenként
  if (tack >= 12) {
    if (!isnan(temp)) feedTemp->save(temp);
    if (!isnan(hum))  feedHum->save(hum);
    feedSoil->save(soilRaw);
    if (tankCm > 0)   feedWater->save(TANK_FULL_CM - tankCm);
    tack=0;
  }
  tick++;
  tack++; 
  delay(5000);
}
