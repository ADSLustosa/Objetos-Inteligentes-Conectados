# Arquitetura Geral do Sistema – CasaViva IoT

O CasaViva IoT foi projetado seguindo princípios modernos de arquitetura orientada a eventos, baixo acoplamento e escalabilidade. Ele combina sensores físicos, firmware embarcado, comunicação MQTT e integração com assistentes de voz (Alexa).

---

# 🌐 Visão Geral da Arquitetura

```
flowchart TD
Sensores[Sensores<br>DHT22, MQ-135, KY-037] --> ESP32
Joystick[Joystick & Potenciômetro] --> ESP32
LCD[LCD 16x2 I2C] --> ESP32
LED[LED RGB] --> ESP32

ESP32 -->|Publica Telemetria| MQTT[(Broker MQTT)]
MQTT --> MQTTX[MQTTX Dashboard]
MQTT --> NodeRED[Node-RED]
MQTT --> Alexa[AWS Lambda + Smart Home Skill]

MQTT -->|Comandos de Ação| ESP32
ESP32 --> Relay[Relé AC]
```

# ⚙ Camadas do Sistema

O sistema é dividido em cinco camadas principais:

1. Camada de Sensoriamento

Responsável por captar dados ambientais:

- DHT22: temperatura e umidade

- MQ-135: qualidade do ar

- KY-037: ruído

- Potenciômetro: threshold de poluição

2. Camada Embarcada (ESP32)

Responsável por:

- Leitura dos sensores

- Processamento leve

- Controle do LCD

- Operação do relé

- Atualização do LED RGB

- Lógica de estado ambiental

- Publicação/consumo MQTT

3. Camada de Comunicação (MQTT)

Protocolo leve que garante:

- Baixo consumo de energia

- Baixa latência

- Suporte a milhares de assinantes

Broker utilizado:

- HiveMQ Public

- Ou AWS IoT (modo avançado seguro com TLS)

4. Camada de Apresentação

Inclui:

- MQTTX para monitoramento

- Dashboards (Node-RED)

- LCD 16×2 embarcado

- LED RGB como indicador

5. Camada de Automação (Alexa / Nuvem)

- Alexa Smart Home Skill

- AWS Lambda

- AWS IoT Core

- Comandos de voz sincronizados com MQTT

# 🧠 Padrões de Projeto Utilizados

    | Padrão                        | Aplicação              |
    | ----------------------------- | ---------------------- |
    | **Publish/Subscribe**         | MQTT                   |
    | **State Machine**             | Páginas do LCD         |
    | **Debouncing lógico**         | Joystick               |
    | **Driver Isolation**          | Abstração de sensores  |
    | **Event-driven architecture** | Telemetria por gatilho |
    | **Fail-safe reconnection**    | Wi-Fi e MQTT           |

# 🚀 Possibilidades de Expansão

- Adição de sensores (luminosidade, presença, CO₂ dedicado)

- Dashboards WebSocket em tempo real

- Controle remoto por app móvel

- Machine learning local (TinyML)