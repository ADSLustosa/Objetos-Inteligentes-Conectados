#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ============ WiFi ============
const char* WIFI_SSID     = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

// ============ MQTT ============
const char* MQTT_SERVER = "broker.hivemq.com";
const int   MQTT_PORT   = 1883;

const char* TOPIC_TEMP  = "/CasaViva/temperatura";
const char* TOPIC_HUM   = "/CasaViva/umidade";
const char* TOPIC_RELAY = "/CasaViva/relay";

#define DHTPIN   4
#define DHTTYPE  DHT22
DHT dht(DHTPIN, DHTTYPE);

#define MQ_PIN   33
#define SOUND_PIN 25
#define POT_PIN  34
#define JOY_VERT_PIN 32
#define JOY_SEL_PIN  14
#define RELAY_PIN 27
#define LED_R 2
#define LED_G 26
#define LED_B 5

inline void ledSet(bool r, bool g, bool b) {
  digitalWrite(LED_R, r);
  digitalWrite(LED_G, g);
  digitalWrite(LED_B, b);
}

void ledOff()     { ledSet(HIGH, HIGH, HIGH); }
void ledRed()     { ledSet(LOW,  HIGH, HIGH); }
void ledGreen()   { ledSet(HIGH, LOW,  HIGH); }
void ledBlue()    { ledSet(HIGH, HIGH, LOW ); }
void ledYellow()  { ledSet(LOW,  LOW,  HIGH); }
void ledCyan()    { ledSet(HIGH, LOW,  LOW ); }
void ledPurple()  { ledSet(LOW,  HIGH, LOW ); }

void ledRelayMode() {
  if (digitalRead(RELAY_PIN) == LOW) ledGreen();
  else                               ledRed();
}

// ============ MQTT ============
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastPublish    = 0;
unsigned long lastPageSwitch = 0;
unsigned long lastLcdDraw    = 0;

const unsigned long PUBLISH_INTERVAL_MS = 2500;
const unsigned long PAGE_INTERVAL_MS    = 6000;
const unsigned long LCD_MIN_REDRAW_MS   = 200;

unsigned long lastSoundEvent   = 0;
const unsigned long SOUND_DEBOUNCE_MS   = 2000;
const unsigned long SOUND_ALERT_MS      = 4000;


unsigned long lastBlinkToggle = 0;
bool blinkState = false;


float tempCur = 0, humCur = 0;
float tempMax = -999, tempMin = 999;
float humMax  = -999, humMin  = 999;
int   airPPM  = 0;
int   airThreshold = 1200;

bool wifiOk   = false;
bool mqttOk   = false;
bool sensorErro = false;
bool ecoMode  = false;

uint8_t currentPage = 0;
const uint8_t NUM_PAGES = 3;

// ============ Alertas ============
enum AlertType {
  ALERT_NONE = 0,
  ALERT_HEAT,
  ALERT_COLD,
  ALERT_DRY,
  ALERT_HUMID,
  ALERT_AIR,
  ALERT_SOUND
};

AlertType currentAlert = ALERT_NONE;
unsigned long alertUntil = 0;
const unsigned long ALERT_DURATION_MS = 8000;

// ============ Ícones customizados ============
byte iconTemp[8] = { B00100,B01010,B01010,B01110,B01110,B11111,B11111,B01110 };
byte iconHum[8]  = { B00100,B00100,B01010,B01010,B10001,B10001,B10001,B01110 };
byte iconWifi[8] = { B00000,B00000,B00100,B01010,B10001,B00000,B00100,B00000 };
byte iconMQTT[8] = { B00100,B01110,B10101,B10101,B10101,B01110,B00100,B00000 };
byte iconAlert[8]= { B00100,B00100,B00100,B01110,B11111,B11111,B00100,B00000 };
byte iconFan[8]  = { B00100,B01010,B10101,B00100,B10101,B01010,B00100,B00000 };

void lcdCreateIcons() {
  lcd.createChar(0, iconTemp);
  lcd.createChar(1, iconHum);
  lcd.createChar(2, iconWifi);
  lcd.createChar(3, iconMQTT);
  lcd.createChar(4, iconAlert);
  lcd.createChar(5, iconFan);
}

inline void lcdIcon(uint8_t idx) {
  lcd.write((uint8_t)idx);
}

// ============ Helpers LCD ============
void lcdStatus(const char* l1, const char* l2) {
  lcd.clear();
  lcd.setCursor(0,0); lcd.print(l1);
  lcd.setCursor(0,1); lcd.print(l2);
}

void lcdSplash() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  CasaViva IoT  ");
  lcd.setCursor(0,1);
  lcd.print(" Inicializando  ");
  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i,1);
    lcd.print(char(255));
    delay(30);
  }
}

// ============ MQTT CALLBACK ============
void mqttCallback(char* topic, byte* payload, unsigned int len) {
  String msg;
  msg.reserve(len);
  for (unsigned int i=0; i<len; i++) msg += (char)payload[i];

  Serial.print("[MQTT RX] "); Serial.print(topic);
  Serial.print(" => "); Serial.println(msg);

  if (String(topic) == TOPIC_RELAY) {
    if (msg.equalsIgnoreCase("ON"))  digitalWrite(RELAY_PIN, LOW);
    if (msg.equalsIgnoreCase("OFF")) digitalWrite(RELAY_PIN, HIGH);

    if (currentAlert == ALERT_NONE) {
      ledRelayMode();
    }
  }
}

// ============ WIFI ============
void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  wifiOk = true;
}

// ============ MQTT ============
void connectMQTT() {
  mqttOk = false;
  while (!client.connected()) {
    if (client.connect("CasaVivaESP32_INTER")) {
      mqttOk = true;
      client.subscribe(TOPIC_RELAY);
    } else {
      delay(400);
    }
  }
}

// ============ Leitura Joystick ============
void lerJoystick(unsigned long now) {
  int v = analogRead(JOY_VERT_PIN);

  static unsigned long lastJoyMove = 0;
  const unsigned long JOY_DEBOUNCE = 500;

  if (now - lastJoyMove > JOY_DEBOUNCE) {
    if (v < 1400) {
      currentPage = (currentPage + 1) % NUM_PAGES;
      lastPageSwitch = now;
      lastJoyMove = now;
    } else if (v > 2600) {
      if (currentPage == 0) currentPage = NUM_PAGES - 1;
      else currentPage--;
      lastPageSwitch = now;
      lastJoyMove = now;
    }
  }

  static bool lastSel = HIGH;
  bool sel = digitalRead(JOY_SEL_PIN);

  if (lastSel == HIGH && sel == LOW) {
    ecoMode = !ecoMode;
    if (!ecoMode) {
      lcd.backlight();
    }
  }
  lastSel = sel;
}

// ============ Leitura do potenciômetro ============
void lerPotenciometro() {
  int raw = analogRead(POT_PIN);
  airThreshold = map(raw, 0, 4095, 600, 1800);
}

// ============ Leitura do som ============
void lerSom(unsigned long now) {
  int soundLevel = digitalRead(SOUND_PIN);

  if (soundLevel == HIGH && (now - lastSoundEvent > SOUND_DEBOUNCE_MS)) {
    lastSoundEvent = now;
    currentAlert   = ALERT_SOUND;
    alertUntil     = now + SOUND_ALERT_MS;
  }
}

// ============ PÁGINAS LCD ============
void lcdPageDashboard() {
  lcd.clear();

  lcd.setCursor(0,0);
  lcdIcon(0); lcd.print(":");
  lcd.print(tempCur,1); lcd.print("C");

  lcd.setCursor(9,0);
  lcdIcon(1); lcd.print(":");
  lcd.print(humCur,0); lcd.print("%");

  lcd.setCursor(0,1);
  lcd.print("MQ:");
  lcd.print(airPPM);

  lcd.setCursor(9,1);
  lcd.print("R:");
  lcd.print(digitalRead(RELAY_PIN)==LOW ? "ON" : "OF");

  lcd.setCursor(13,1);
  lcd.print(ecoMode ? "E" : " ");
}

void lcdPageStats() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tmx:");
  lcd.print(tempMax,1);
  lcd.setCursor(9,0);
  lcd.print("Tmn:");
  lcd.print(tempMin,1);

  lcd.setCursor(0,1);
  lcd.print("Hmx:");
  lcd.print(humMax,0);
  lcd.setCursor(9,1);
  lcd.print("Hmn:");
  lcd.print(humMin,0);
}

void lcdPageAir() {
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("MQ:");
  lcd.print(airPPM);
  lcd.print("ppm");

  lcd.setCursor(0,1);
  lcd.print("Lim:");
  lcd.print(airThreshold);
  lcd.print(" Wi:");
  lcd.print(wifiOk ? "Y" : "N");
  lcd.print("/");
  lcd.print(mqttOk ? "Y" : "N");
}

void lcdPageAlert() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcdIcon(4);
  lcd.print(" ALERTA ATIVO ");

  lcd.setCursor(0,1);
  switch (currentAlert) {
    case ALERT_HEAT:   lcd.print("Calor extremo");      break;
    case ALERT_COLD:   lcd.print("Frio intenso");       break;
    case ALERT_DRY:    lcd.print("Ar muito seco");      break;
    case ALERT_HUMID:  lcd.print("Umidade elevada");    break;
    case ALERT_AIR:    lcd.print("Ar poluido (MQ)");    break;
    case ALERT_SOUND:  lcd.print("Som muito alto");     break;
    default:           lcd.print("Estado indefinido");  break;
  }
}

// ============ LÓGICA DE ALERTAS COM LED ============
void avaliarAlertas(float t, float h, int ppm, unsigned long now) {
  AlertType novo = ALERT_NONE;

  if (ppm >= airThreshold)       novo = ALERT_AIR;
  else if (t >= 40.0)            novo = ALERT_HEAT;
  else if (t <= 15.0)            novo = ALERT_COLD;
  else if (h < 30.0)             novo = ALERT_DRY;
  else if (h > 80.0)             novo = ALERT_HUMID;


  if (novo != ALERT_NONE) {
    currentAlert = novo;
    alertUntil   = now + ALERT_DURATION_MS;
  } else {
    if (currentAlert != ALERT_SOUND && currentAlert != ALERT_NONE && now > alertUntil) {
      currentAlert = ALERT_NONE;
    }
  }
}

void updateLEDForStatus(unsigned long now) {
  if (sensorErro) {
    ledYellow();
    return;
  }

  if (currentAlert == ALERT_SOUND && now < alertUntil) {
    if (now - lastBlinkToggle > 200) {
      lastBlinkToggle = now;
      blinkState = !blinkState;
      if (blinkState) ledBlue();
      else            ledOff();
    }
    return;
  }

  if (currentAlert != ALERT_NONE && now < alertUntil) {
    switch (currentAlert) {
      case ALERT_AIR:   ledPurple(); break;
      case ALERT_HEAT:  ledRed();    break;
      case ALERT_COLD:  ledBlue();   break;
      case ALERT_DRY:   ledCyan();   break;
      case ALERT_HUMID: ledYellow(); break;
      default:          ledRelayMode(); break;
    }
    return;
  }

  currentAlert = ALERT_NONE;
  ledRelayMode();
}

// ============ SETUP ============
void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  ledOff();

  pinMode(MQ_PIN, INPUT);
  pinMode(SOUND_PIN, INPUT);

  pinMode(POT_PIN, INPUT);

  pinMode(JOY_VERT_PIN, INPUT);
  pinMode(JOY_SEL_PIN, INPUT_PULLUP);

  Wire.begin(21,22);
  lcd.begin(16,2);
  lcd.backlight();
  lcdCreateIcons();
  lcdSplash();

  dht.begin();

  WiFi.mode(WIFI_STA);
  connectWiFi();

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(mqttCallback);
  connectMQTT();

  lcdStatus("Sistema Pronto", "CasaViva IoT");

  lastPublish    = millis();
  lastPageSwitch = millis();
  lastLcdDraw    = millis();
}

// ============ LOOP ============
void loop() {

  if (!client.connected()) connectMQTT();
  client.loop();

  unsigned long now = millis();

  lerJoystick(now);
  lerPotenciometro();
  lerSom(now);

  if (now - lastPublish >= PUBLISH_INTERVAL_MS) {
    lastPublish = now;

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (isnan(t) || isnan(h)) {
      sensorErro = true;
      lcdStatus("Erro DHT22", "Verifique sensor");
    } else {
      sensorErro = false;
      tempCur = t;
      humCur  = h;

      tempMax = max(tempMax, t);
      tempMin = min(tempMin, t);
      humMax  = max(humMax,  h);
      humMin  = min(humMin,  h);

      int raw = analogRead(MQ_PIN);
      airPPM  = map(raw, 0, 4095, 300, 2000);

      Serial.print("[SENS] T=");
      Serial.print(tempCur);
      Serial.print(" H=");
      Serial.print(humCur);
      Serial.print(" MQ=");
      Serial.print(airPPM);
      Serial.print(" (Lim=");
      Serial.print(airThreshold);
      Serial.println(")");

      client.publish(TOPIC_TEMP, String(tempCur).c_str());
      client.publish(TOPIC_HUM,  String(humCur).c_str());

      avaliarAlertas(tempCur, humCur, airPPM, now);
    }
  }

  if (ecoMode && !sensorErro && currentAlert == ALERT_NONE) {
    lcd.noBacklight();
  } else {
    lcd.backlight();
  }

  if (!sensorErro && currentAlert == ALERT_NONE &&
      !ecoMode && now - lastPageSwitch >= PAGE_INTERVAL_MS) {
    lastPageSwitch = now;
    currentPage = (currentPage + 1) % NUM_PAGES;
  }

  updateLEDForStatus(now);

  if (now - lastLcdDraw >= LCD_MIN_REDRAW_MS) {
    lastLcdDraw = now;

    if (sensorErro) {
    } else if (currentAlert != ALERT_NONE && now < alertUntil) {
      lcdPageAlert();
    } else {
      switch (currentPage) {
        case 0: lcdPageDashboard(); break;
        case 1: lcdPageStats();     break;
        case 2: lcdPageAir();       break;
        default: lcdPageDashboard(); break;
      }
    }
  }

  delay(40);
}
