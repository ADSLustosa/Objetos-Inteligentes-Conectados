# 🔌 Firmware – Projeto CasaViva IoT
**Código-fonte embarcado para ESP32 com suporte a MQTT, LCD, sensores e automação residencial**

Bem-vindo ao diretório /firmware, onde está localizado todo o código embarcado responsável pela operação inteligente do **Projeto CasaViva IoT**.
Aqui você encontrará o firmware principal **(main.cpp)**, bibliotecas auxiliares e a configuração do **PlatformIO** utilizada para compilar, organizar e expandir o sistema.

O firmware foi projetado com foco em:

    ✔ Alta estabilidade
    ✔ Baixo consumo
    ✔ Loop não bloqueante
    ✔ Modularidade
    ✔ Integração IoT completa
    ✔ Compatibilidade com Wokwi e dispositivos reais

# 🧱 Arquivos presentes nesta pasta
```
firmware/
 ├── main.cpp
 ├── platformio.ini
 ├── .gitignore
```

# 📌 📄 main.cpp – Firmware principal

O arquivo **main.cpp** implementa:

## 🧩 Principais funcionalidades
**🔹 1. Leitura de Sensores**

- DHT22 (temperatura/umidade)

- MQ-135 (qualidade do ar – futura expansão)

- KY-037 (nível de som – futura expansão)

- Joystick (interação do usuário)

- Potenciômetro

**🔹 2. Lógica de estados + LCD Dashboard**

- Páginas com ícones customizados

- Indicadores de saúde do sistema (Wi-Fi, MQTT, Relay etc.)

- Alternância automática de telas

**🔹 3. Comunicação IoT**

- Conexão com HiveMQ / AWS IoT (simulado ou real)

- Publicação periódica de temperatura e umidade

- Recebimento de comandos via MQTT

- Atualização de estado de relé em tempo real

**🔹 4. Controle de Atuadores**

- Relé AC (umidificador, ventilador, iluminação etc.)

- LED RGB interativo (feedback de eventos)

**🔹 5. Compatibilidade com Wokwi**

- Compila sem erros

- Integração 100% com o diagram.json

- Sensores simuláveis

- MQTT em tempo real via MQTTX

# ⚙️ 📄 platformio.ini – Configuração do Ambiente

Este arquivo define:
    | Configuração          | Propósito                                   |
    | --------------------- | ------------------------------------------- |
    | PlatformIO env        | Configurações para ESP32 DevKit V1          |
    | Monitor speed         | Serial@115200 para debug                    |
    | Type                  | Firmware C++17                              |
    | Bibliotecas incluídas | DHT sensor, PubSubClient, LiquidCrystal I2C |
    | Flags                 | Otimizações para estabilidade no Wokwi      |

Exemplo de estrutura:
```
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
lib_deps =
  adafruit/DHT sensor library
  knolleary/PubSubClient
  marcoschwartz/LiquidCrystal_I2C
```

# 🎯 Objetivos do Firmware CasaViva

O firmware foi elaborado para:

**✔ Monitorar variáveis ambientais**

- Temperatura

- Umidade

- Ruído

- Qualidade do ar


**✔ Tomar decisões inteligentes**

- Ação automática com relé

- Feedback visual via LED RGB

- Exibição organizada no LCD


**✔ Integrar com ecossistemas IoT**

- MQTT Broker público (HiveMQ)

- AWS IoT Core

- Alexa Smart Home Skill

- Node-RED


**✔ Garantir operação contínua**

- Reconexão automática Wi-Fi

- Reconexão automática MQTT

- Fail-safe de sensores

- Watchdog via lógica de timeout


# 🔍 Fluxo Geral do Firmware
```
flowchart TD
A[Boot ESP32] --> B[Inicializa LCD + Ícones]
B --> C[Wi-Fi Connect]
C --> D[MQTT Connect]
D --> E[Loop Principal]

E --> F{Leitura Sensores}
F -->|OK| G[Publica via MQTT]
F -->|Falha| H[Mensagem de erro no LCD]

G --> I[Atualiza Dashboard]
H --> I

I --> J{Comandos RECEBIDOS via MQTT}
J -->|RELAY ON| K[Ativa Relé]
J -->|RELAY OFF| L[Desativa Relé]
```

# 🧪 Testes suportados pelo firmware

| Teste               | Como fazer                                   |
| ------------------- | -------------------------------------------- |
| Simular temperatura | Ajustar slider do DHT22 no Wokwi             |
| Simular umidade     | Usar painel do DHT22                         |
| Testar MQTT TX      | Acompanhar no MQTTX → Received               |
| Testar MQTT RX      | Publicar em `/CasaViva/relay` com "ON"/"OFF" |
| Testar LCD          | Ver alternância de páginas                   |
| Testar relé         | Enviar comando MQTT                          |

# 🚀 Como executar
**No Wokwi**

1. Abra <mark>simulation/wokwi-project.txt</mark>

2. Rode o simulador

3. Verifique LCD, sensores e MQTT

**No hardware real**

1. Instale PlatformIO

2. Conecte ESP32 via USB

3. Execute:
```
pio run --target upload
pio device monitor
```

# 🧩 Integrações Futuras

- Node-RED + Dashboard Web

- Automação local via ESPHome

- Banco de dados Timeseries (InfluxDB)

- Machine Learning embarcado (TinyML)