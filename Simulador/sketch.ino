#include <Arduino.h>
#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// ======================
// CONFIGURAÇÕES GERAIS
// ======================

// Wi-Fi do Wokwi (obrigatório)
constexpr const char* WIFI_SSID = "Wokwi-GUEST";
constexpr const char* WIFI_PASS = "";

// MQTT
constexpr const char* MQTT_HOST      = "test.mosquitto.org";
constexpr uint16_t    MQTT_PORT      = 1883;
constexpr const char* MQTT_CLIENT_ID = "casaviva-esp32-sim";

// Tópicos MQTT
constexpr const char* TOPIC_T_DHT   = "casaviva/sala/sensor/dht22";
constexpr const char* TOPIC_T_SOM   = "casaviva/sala/sensor/ky037";
constexpr const char* TOPIC_T_GAS   = "casaviva/sala/sensor/mq135";
constexpr const char* TOPIC_CMD_LED = "casaviva/sala/cmd/led";
constexpr const char* TOPIC_ST_LED  = "casaviva/sala/estado/led";

// Timers
constexpr uint32_t T_SENSORES_MS = 2000;
constexpr uint32_t T_MQTT_LOOP   = 50;

// ======================
// MAPEAMENTO DE PINOS
// ======================

namespace Pins {
  constexpr uint8_t DHT_DATA   = 4;    // DHT22
  constexpr uint8_t KY037_AOUT = 34;   // potenciômetro
  constexpr uint8_t MQ135_AOUT = 35;   // potenciômetro
  constexpr uint8_t I2C_SDA = 21;
  constexpr uint8_t I2C_SCL = 22;
  constexpr uint8_t LED_R = 25;
  constexpr uint8_t LED_G = 26;
  constexpr uint8_t LED_B = 27;
}

// ======================
// OBJETOS DO SISTEMA
// ======================

DHT dht(Pins::DHT_DATA, DHT22);
LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Dados dos sensores
struct Amostras {
  float tempC = NAN;
  float umid  = NAN;
  int   som   = 0;
  int   gas   = 0;
} s;

uint32_t lastSensorsMs  = 0;
uint32_t lastMqttLoopMs = 0;

// ======================
// FUNÇÕES AUXILIARES
// ======================

void setRgb(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(Pins::LED_R, r);
  analogWrite(Pins::LED_G, g);
  analogWrite(Pins::LED_B, b);
}

void publicaEstadoLed(uint8_t r, uint8_t g, uint8_t b) {
  char payload[64];
  snprintf(payload, sizeof(payload), "{\"r\":%u,\"g\":%u,\"b\":%u}", r, g, b);
  mqttClient.publish(TOPIC_ST_LED, payload, true);
}

void lcdSplash() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CasaViva IoT");
  lcd.setCursor(0, 1);
  lcd.print("Simulacao Wokwi");
}

void lcdAmostras() {
  lcd.clear();

  float temp  = isnan(s.tempC) ? -1.0f : s.tempC;
  int   umidV = isnan(s.umid) ? -1 : (int)s.umid;

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(String(temp, 1));
  lcd.print("C U:");
  lcd.print(String(umidV));
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("G:");
  lcd.print(s.gas / 100);
  lcd.print(" S:");
  lcd.print(s.som / 100);
}

// ======================
// MQTT CALLBACK
// ======================

void mqttCallback(char* topic, byte* payload, unsigned int len) {
  String msg;
  for (unsigned i = 0; i < len; i++) msg += (char)payload[i];

  if (String(topic) == TOPIC_CMD_LED) {
    uint8_t r = 0, g = 0, b = 0;

    int ir = msg.indexOf("\"r\":");
    int ig = msg.indexOf("\"g\":");
    int ib = msg.indexOf("\"b\":");

    if (ir >= 0) r = msg.substring(ir + 4).toInt();
    if (ig >= 0) g = msg.substring(ig + 4).toInt();
    if (ib >= 0) b = msg.substring(ib + 4).toInt();

    setRgb(r, g, b);
    publicaEstadoLed(r, g, b);
  }
}

// ======================
// CONEXÃO WI-FI
// ======================

void connectWifi() {
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }
}

// ======================
// CONEXÃO MQTT
// ======================

void connectMqtt() {
  if (mqttClient.connected()) return;

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);

  while (!mqttClient.connected()) {
    mqttClient.connect(MQTT_CLIENT_ID);
    delay(250);
  }

  mqttClient.subscribe(TOPIC_CMD_LED);
}

// ======================
// SENSORES
// ======================

void lerSensores() {
  s.tempC = dht.readTemperature();
  s.umid  = dht.readHumidity();
  s.som   = analogRead(Pins::KY037_AOUT);
  s.gas   = analogRead(Pins::MQ135_AOUT);
}

void publicarSensores() {
  char payload[96];

  if (!isnan(s.tempC) && !isnan(s.umid)) {
    snprintf(payload, sizeof(payload),
             "{\"temp_c\":%.1f,\"umid_pct\":%.0f}", s.tempC, s.umid);
    mqttClient.publish(TOPIC_T_DHT, payload);
  }

  snprintf(payload, sizeof(payload), "{\"adc\":%d}", s.som);
  mqttClient.publish(TOPIC_T_SOM, payload);

  snprintf(payload, sizeof(payload), "{\"adc\":%d}", s.gas);
  mqttClient.publish(TOPIC_T_GAS, payload);
}

// ======================
// SETUP E LOOP
// ======================

void setup() {
  Serial.begin(115200);

  Wire.begin(Pins::I2C_SDA, Pins::I2C_SCL);
  lcd.init();
  lcd.backlight();
  lcdSplash();

  dht.begin();

  // LED — versão compatível com Wokwi
  pinMode(Pins::LED_R, OUTPUT);
  pinMode(Pins::LED_G, OUTPUT);
  pinMode(Pins::LED_B, OUTPUT);

  connectWifi();
  connectMqtt();

  // =========================
  // TESTE AUTOMÁTICO DO LED RGB
  // =========================
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Testando LED...");
  lcd.setCursor(0,1);
  lcd.print("RGB sequencial");

  setRgb(255, 0, 0);   // Vermelho
  delay(800);

  setRgb(0, 255, 0);   // Verde
  delay(800);

  setRgb(0, 0, 255);   // Azul
  delay(800);

  setRgb(255, 255, 255);   // Branco
  delay(800);

  setRgb(0, 0, 0);     // Desliga
  delay(300);

  lcd.clear();
  lcd.print("CasaViva IoT");
  lcd.setCursor(0,1);
  lcd.print("Iniciando...");
  delay(1500);

  // Timers
  lastSensorsMs = millis();
  lastMqttLoopMs = millis();
}


void loop() {
  connectWifi();

  uint32_t now = millis();

  if (!mqttClient.connected()) {
    connectMqtt();
  }

  if (now - lastMqttLoopMs >= T_MQTT_LOOP) {
    mqttClient.loop();
    lastMqttLoopMs = now;
  }

  if (now - lastSensorsMs >= T_SENSORES_MS) {
    lerSensores();
    lcdAmostras();
    publicarSensores();

    if (s.gas > 2500)       setRgb(255, 0, 0);
    else if (s.som > 2500)  setRgb(255, 255, 0);
    else if (s.tempC > 30)  setRgb(255, 50, 0);
    else                    setRgb(0, 255, 0);

    lastSensorsMs = now;
  }
}
