#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// ========================= CONFIGURAÇÕES GLOBAIS =========================

// Wi-Fi
constexpr char WIFI_SSID[]     = "SEU_SSID";
constexpr char WIFI_PASS[]     = "SUA_SENHA";

// MQTT
constexpr char MQTT_HOST[]     = "test.mosquitto.org";
constexpr uint16_t MQTT_PORT   = 1883;
constexpr char MQTT_CLIENT_ID[]= "casaviva-esp32";
constexpr char TOPIC_T_DHT[]   = "casaviva/sala/sensor/dht22";
constexpr char TOPIC_T_SOM[]   = "casaviva/sala/sensor/ky037";
constexpr char TOPIC_T_GAS[]   = "casaviva/sala/sensor/mq135";
constexpr char TOPIC_CMD_LED[] = "casaviva/sala/cmd/led";
constexpr char TOPIC_ST_LED[]  = "casaviva/sala/estado/led";

// TIMERS
constexpr uint32_t T_SENSORES_MS = 2000;
constexpr uint32_t T_MQTT_MS     = 50;
constexpr uint32_t T_WIFI_RETRY  = 5000;

// ========================= MAPEAMENTO DE PINOS =========================

namespace Pins {
  // Sensores
  constexpr uint8_t DHT_DATA   = 4;
  constexpr uint8_t KY037_AOUT = 34;
  constexpr uint8_t MQ135_AOUT = 35;
  constexpr uint8_t MQ135_DOUT = 32;

  constexpr uint8_t I2C_SDA = 21;
  constexpr uint8_t I2C_SCL = 22;

  constexpr uint8_t BT_RX = 16;
  constexpr uint8_t BT_TX = 17;

  constexpr uint8_t LED_R = 25;
  constexpr uint8_t LED_G = 26;
  constexpr uint8_t LED_B = 27;
}

// ========================= PERIFÉRICOS =========================

DHT dht(Pins::DHT_DATA, DHT22);
LiquidCrystal_I2C lcd(0x27, 16, 2);
HardwareSerial BT(2);
WiFiClient wifi;
PubSubClient mqtt(wifi);


namespace Ledc {
  constexpr uint8_t CH_R = 0;
  constexpr uint8_t CH_G = 1;
  constexpr uint8_t CH_B = 2;
  constexpr uint32_t FREQ = 5000;
  constexpr uint8_t RES   = 8;
}

// ========================= ESTADO E BUFFERS =========================

struct Amostras {
  float tempC = NAN;
  float umid  = NAN;
  int   som   = 0;
  int   gas   = 0;
  int   gasD  = 0;
} s;

uint32_t t_lastSens = 0;
uint32_t t_lastMqtt = 0;

// ========================= UTILITÁRIOS =========================

inline void setRgb(uint8_t r, uint8_t g, uint8_t b) {
  ledcWrite(Ledc::CH_R, r);
  ledcWrite(Ledc::CH_G, g);
  ledcWrite(Ledc::CH_B, b);
}

void publicaEstadoLed(uint8_t r, uint8_t g, uint8_t b) {
  char msg[64];
  snprintf(msg, sizeof(msg), "{\"r\":%u,\"g\":%u,\"b\":%u}", r, g, b);
  mqtt.publish(TOPIC_ST_LED, msg, true);
}

void lcdSplash() {
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("CasaViva IoT");
  lcd.setCursor(0,1); lcd.print("Inicializando...");
}

void lcdAmostras() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("T:");  isnan(s.tempC) ? lcd.print("--") : lcd.print(s.tempC,1);
  lcd.print("C ");
  lcd.print("U:");  isnan(s.umid ) ? lcd.print("--") : lcd.print((int)s.umid);
  lcd.print("%");

  lcd.setCursor(0,1);
  lcd.print("G:"); lcd.print(s.gas/100);
  lcd.print(" S:"); lcd.print(s.som/100);
}

// ========================= MQTT ===========================


void onMqttMessage(char* topic, byte* payload, unsigned int len) {

  if (strcmp(topic, TOPIC_CMD_LED) == 0) {
    uint8_t r=0,g=0,b=0;
    
    for (unsigned i=0;i<len;i++) payload[i] = (payload[i]==0? ' ': payload[i]);
    String p = String((char*)payload).substring(0, len);
    int ir = p.indexOf("\"r\":"); if (ir>=0) r = (uint8_t) p.substring(ir+4).toInt();
    int ig = p.indexOf("\"g\":"); if (ig>=0) g = (uint8_t) p.substring(ig+4).toInt();
    int ib = p.indexOf("\"b\":"); if (ib>=0) b = (uint8_t) p.substring(ib+4).toInt();
    setRgb(r,g,b);
    publicaEstadoLed(r,g,b);
  }
}

void ensureWifi() {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  uint32_t t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis()-t0 < T_WIFI_RETRY) delay(100);
}

void ensureMqtt() {
  if (mqtt.connected()) return;
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMqttMessage);
  while (!mqtt.connected()) {
    if (mqtt.connect(MQTT_CLIENT_ID)) {
      mqtt.subscribe(TOPIC_CMD_LED);
      publicaEstadoLed(0,255,0);
      break;
    }
    delay(500);
  }
}

// ================= SENSORES ===========================

void lerSensores() {
  s.tempC = dht.readTemperature();
  s.umid  = dht.readHumidity();
  s.som   = analogRead(Pins::KY037_AOUT);
  s.gas   = analogRead(Pins::MQ135_AOUT);
  s.gasD  = digitalRead(Pins::MQ135_DOUT);
}

void publicarSensores() {
  char buf[96];

  if (!isnan(s.tempC) && !isnan(s.umid)) {
    snprintf(buf, sizeof(buf), "{\"temp_c\":%.1f,\"umid_pct\":%.0f}", s.tempC, s.umid);
    mqtt.publish(TOPIC_T_DHT, buf, false);
  }
  snprintf(buf, sizeof(buf), "{\"adc\":%d}", s.som);
  mqtt.publish(TOPIC_T_SOM, buf, false);

  snprintf(buf, sizeof(buf), "{\"adc\":%d,\"digital\":%d}", s.gas, s.gasD);
  mqtt.publish(TOPIC_T_GAS, buf, false);
}

// ========================= BLUETOOTH =========================

void btEnviar() {
  BT.printf("T:%.1fC U:%.0f%% Gas:%d Som:%d\n",
            s.tempC, s.umid, s.gas, s.som);
}

// ========================= CONFIGURAÇÃO ========================

void setup() {
  // GPIO
  pinMode(Pins::KY037_AOUT, INPUT);
  pinMode(Pins::MQ135_AOUT, INPUT);
  pinMode(Pins::MQ135_DOUT, INPUT);

  // LEDC
  ledcSetup(Ledc::CH_R, Ledc::FREQ, Ledc::RES);
  ledcSetup(Ledc::CH_G, Ledc::FREQ, Ledc::RES);
  ledcSetup(Ledc::CH_B, Ledc::FREQ, Ledc::RES);
  ledcAttachPin(Pins::LED_R, Ledc::CH_R);
  ledcAttachPin(Pins::LED_G, Ledc::CH_G);
  ledcAttachPin(Pins::LED_B, Ledc::CH_B);
  setRgb(0, 0, 0);

  // Serial e Bluetooth
  Serial.begin(115200);
  BT.begin(9600, SERIAL_8N1, Pins::BT_RX, Pins::BT_TX);

  // I2C + LCD
  Wire.begin(Pins::I2C_SDA, Pins::I2C_SCL);
  lcd.init(); lcd.backlight(); lcdSplash();

  // DHT
  dht.begin();

  // Rede
  ensureWifi();
  ensureMqtt();

  t_lastSens = millis();
  t_lastMqtt = millis();
}

// ========================= LOOP =========================

void loop() {
  
  // Conectividade
  ensureWifi();
  if (millis() - t_lastMqtt >= T_MQTT_MS) {
    ensureMqtt();
    mqtt.loop();
    t_lastMqtt = millis();
  }

  if (millis() - t_lastSens >= T_SENSORES_MS) {
    lerSensores();
    lcdAmostras();
    publicarSensores();
    btEnviar();

    // Regras locais simples de controle do LED RGB
    if (s.gas > 2000)        setRgb(255,   0,   0);   // alerta gás
    else if (s.som > 2000)   setRgb(255, 255,   0);   // ruído alto
    else if (!isnan(s.tempC) && s.tempC > 30.0)
                            setRgb(255,  50,   0);   // quente
    else                    setRgb(  0, 255,   0);   // normal

    t_lastSens = millis();
  }
}
