# Arquitetura Geral do Sistema CasaViva IoT

## Visão Geral
A arquitetura do CasaViva IoT segue um modelo distribuído baseado em:
- Dispositivo embarcado (ESP32)
- Barramento de telemetria MQTT
- Camada de nuvem (HiveMQ ou AWS IoT)
- Opcional: Alexa Smart Home Skill + AWS Lambda
- Aplicações clientes (MQTTX, dashboards, mobile)

A comunicação é orientada a eventos e utiliza o padrão Publish/Subscribe.

---

## Componentes Principais

### 1. ESP32 – Dispositivo IoT
Responsável por:
- Aquisição de dados dos sensores
- Processamento embarcado
- Publicação MQTT
- Recepção de comandos MQTT para atuadores
- Interface com o usuário via LCD e joystick
- Sinalização de status com LED RGB

### 2. Broker MQTT
O projeto pode operar em dois modos:
- **HiveMQ Public Broker**
- **AWS IoT Core (modo seguro, TLS)**

Ambos seguem a estrutura de tópicos:
    /CasaViva/temperatura
    /CasaViva/umidade
    /CasaViva/qualidade_ar
    /CasaViva/ruido
    /CasaViva/relay


### 3. Consumidores MQTT
- Aplicativos como MQTTX
- Dashboards IoT locais
- Node-RED
- Alexa via AWS Lambda (modo avançado)

---

## Diagrama Geral em Alto Nível (Mermaid)

```mermaid
flowchart TD

    ESP[ESP32<br>CasaViva Firmware] -->|Publica Telemetria| MQTT[(Broker MQTT)]
    MQTT --> Consumer1[MQTTX<br>Dashboard]
    MQTT --> Lambda[AWS Lambda<br>Skill Backend]
    Lambda --> Alexa[Amazon Alexa<br>Smart Home]

    ESP <-->|Comandos ON/OFF| MQTT

## Características Arquiteturais

- Event-driven: atualizações de sensores disparam telemetria.

- Baixo acoplamento: dispositivos independentes via MQTT.

- Fail-safe: reconexões automáticas de Wi-Fi e MQTT.

- Escalável: novos sensores podem ser adicionados sem impacto.

- Compatível com nuvem: suporta AWS IoT com certificates + TLS.

## Padrões de Projeto Utilizados

- Publisher/Subscriber

- State Machine (estado do LCD)

- Driver Abstraction (camada de hardware separada do firmware principal)

- Debounce lógico para joystick

- Non-blocking loop (tempo baseado em millis())

## Requisitos Não Funcionais

    | Categoria      | Detalhamento                                   |
    | -------------- | ---------------------------------------------- |
    | Performance    | telemetria a cada 5 segundos                   |
    | Confiabilidade | reconexão automática Wi-Fi/MQTT                |
    | Segurança      | suporte a TLS no modo AWS                      |
    | Portabilidade  | código compatível com PlatformIO e Arduino IDE |
    | Escalabilidade | novos sensores sem impacto estrutural          |
