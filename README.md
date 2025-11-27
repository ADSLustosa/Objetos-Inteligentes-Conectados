# 🌱 CasaViva IoT
### Sistema Inteligente de Monitoramento Ambiental Integrado à Alexa e MQTT

[![Status](https://img.shields.io/badge/status-ativo-brightgreen)]()
[![ESP32](https://img.shields.io/badge/ESP32-supported-blue)]()
[![MQTT](https://img.shields.io/badge/MQTT-HiveMQ%20Cloud-purple)]()

---

# 📑 Sumário
- [📘 Visão Geral](#-visão-geral)
- [🎯 Objetivos do Projeto](#-objetivos-do-projeto)
- [🧩 Funcionalidades Principais](#-funcionalidades-principais)
- [🏗 Arquitetura do Sistema](#-arquitetura-do-sistema)
- [🔧 Hardware Utilizado](#-hardware-utilizado)
- [💻 Software e Tecnologias](#-software-e-tecnologias)
- [📡 Integração MQTT](#-integração-mqtt)
- [🎤 Integração com Alexa](#-integração-com-alexa)
- [📂 Estrutura do Repositório](#-estrutura-do-repositório)
- [🚀 Como Executar o Projeto](#-como-executar-o-projeto)
- [📊 Dashboard Local (MQTTX)](#-dashboard-local-mqttx)
- [📚 Documentação Completa](#-documentação-completa)
- [📎 Referências](#-referências)
- [👥 Autores](#-autores)

---

# 📘 Visão Geral

O **CasaViva IoT** é um sistema completo de monitoramento ambiental e automação doméstica, projetado para melhorar a **saúde e o bem-estar** em ambientes internos, alinhado ao **ODS 3 (ONU)**.  
Ele monitora temperatura, umidade, ruído e qualidade do ar, além de permitir automações remotas via **MQTT** e controle por **Alexa**.

O sistema combina:
- Sensores ambientais  
- Interface física com LCD  
- Controle por joystick  
- Alertas inteligentes por LED RGB  
- Relé para automação  
- Recomendações de saúde baseadas em indicadores internos

---

# 🎯 Objetivos do Projeto

- Monitoramento contínuo da qualidade ambiental  
- Emissão de alertas automáticos com priorização  
- Automação por comandos MQTT e Alexa  
- Controle físico local por joystick e LCD  
- Criação de um ambiente mais seguro, confortável e moderno  
- Análise dos impactos na **saúde e bem-estar** de moradores  
- Construção de solução IoT acessível, modular e escalável  

---

# 🧩 Funcionalidades Principais

### 📡 Monitoramento Completo
- **Temperatura & Umidade** (DHT22)
- **Qualidade do Ar** (MQ-135)
- **Ruído / Vibração** (KY-037)
- **Controle de Limiar** (Potenciômetro)

### 🌐 Comunicação IoT
- **Publicação MQTT** (HiveMQ Cloud)
- **Controle remoto via MQTTX**
- **Integração com Alexa Smart Home**

### 🖥 Interface Local
- LCD 16x2 com páginas dinâmicas:
  - Dashboard
  - Estatísticas
  - Ar Interior
  - Rede
  - Alertas
  - Modo ECO
- Joystick para navegação
- Modo ECO automático

### 🔔 Alertas Inteligentes
- LED RGB com lógica de:
  - Verde → ambiente saudável
  - Amarelo → atenção
  - Vermelho → crítico
  - Roxo → poluição
  - Azul → alerta sonoro
  - Branco → relé ON

---

# 🏗 Arquitetura do Sistema

    ┌────────────┐ MQTT TX ┌───────────────┐
    │ ESP32 │──────────────────▶│ HiveMQ Cloud │
    │ │◀──────────────────┤ MQTT Broker │
    └─────┬──────┘ MQTT RX └───────────────┘
    │
    ▼
    ┌──────────┐ ┌──────────┐ ┌───────────┐
    │ DHT22 │ │ MQ-135 │ │ KY-037 │
    └──────────┘ └──────────┘ └───────────┘
    │ │ │
    ▼ ▼ ▼
    ┌─────────────────────────┐
    │ CasaViva Core │
    │ Processamento lógico │
    └──────────┬──────────────┘
    ▼
    ┌─────────────────────────────────────────┐
    │ LCD I2C │ LED RGB │ Relé │ Joystick │ Alexa │
    └─────────────────────────────────────────┘

---

# 🔧 Hardware Utilizado

| Componente | Função |
|-----------|--------|
| **ESP32 DevKit V1** | núcleo do sistema |
| **DHT22** | temperatura e umidade |
| **MQ-135** | qualidade do ar |
| **KY-037** | som/ruído |
| **Potenciômetro 10k** | ajuste do limiar MQ |
| **LCD 16x2 I2C** | interface física |
| **Joystick analógico** | navegação do LCD |
| **Relé 5V** | acionamento de carga |
| **LED RGB** | alertas visuais |

# 💻 Software e Tecnologias

- C++ / Arduino Framework  
- MQTT (PubSubClient)  
- HiveMQ Cloud  
- MQTTX  
- Wokwi Simulation  
- Arduino IDE / PlatformIO  
- Arquitetura orientada a eventos (EDA)  

---

# 📡 Integração MQTT

### Publicações:
```
    /CasaViva/temperatura
    /CasaViva/umidade
    /CasaViva/qualidade_ar
```

### Assinatura (controle):
```
    /CasaViva/relay → ON / OFF
```

### Exemplo (MQTTX):
```
    Topic: /CasaViva/relay
    Payload: ON
```

---

# 🎤 Integração com Alexa

Fluxo planejado:

1. Alexa recebe comando →  
2. Skill CasaViva →  
3. AWS Lambda →  
4. Publica via MQTT →  
5. ESP32 aciona relé / alertas / LCD  

Exemplos:
```
- "Alexa, ligar o umidificador"  
- "Alexa, como está a temperatura da sala?"  
- "Alexa, como está o ar da casa?"  
```
---

# 📂 Estrutura do Repositório

    CasaViva/
    │
    ├── docs/
    │ ├── hardware/
    │ ├── software/
    │ └── README.md
    │
    ├── firmware/
    │ └── main.cpp
    │
    ├── simulation/
    │ └── diagram.json
    │
    ├── assets/
    │ └── imagens-do-projeto/
    │
    └── README.md

---

# 🚀 Como Executar o Projeto

## ✔ 1. Wokwi (Simulação Completa)
1. Abra o Wokwi  
2. Cole `diagram.json`  
3. Cole `sketch.ino`  
4. Clique em **Run**  
5. Teste interativamente:
   - DHT22 (temperatura/umidade)
   - MQ-135 (poluição)
   - KY-037 (som)
   - Potenciômetro (limiar)
   - Joystick (navegação)
   - Relé (MQTTX)

---

## ✔ 2. MQTTX (Dashboard Local)

**Conectar ao broker:**
```
    broker.hivemq.com
    porta: 1883
```

**Assinar:**
```
    /CasaViva/temperatura
    /CasaViva/umidade
    /CasaViva/qualidade_ar
```

**Publicar comando:**
```
    /CasaViva/relay → ON
```

---

## ✔ 3. ESP32 físico

1. Arduino IDE  
2. Board: **ESP32 DevKit V1**  
3. Bibliotecas:
   - `PubSubClient`
   - `DHT sensor library`
   - `LiquidCrystal I2C`
4. Configurar WiFi no código  
5. Upload via USB  
6. Abrir monitor serial (115200)  

---

# 📊 Dashboard Local (MQTTX)

O MQTTX atua como painel:

- leitura em tempo real  
- gráfico histórico  
- logs de eventos  
- envio de comandos  

---

# 📚 Documentação Completa

Toda a documentação está em:
```
    /docs/hardware
    /docs/software
```

Inclui:
- arquitetura completa  
- lógica de alertas  
- documentação de sensores  
- fluxo MQTT  
- diagramas técnicos  

---

# 📎 Referências

- WHO. *Ambient air pollution guidelines*, 2018.  
- IPEA. *ODS 3 – Saúde e Bem-Estar no Brasil*, 2022.  
- SMITH, J. et al. *Indoor environmental quality and health*, 2022.  
- NEWBURY, R. et al. *Smart home sensing for well-being*, 2024.  

---

# 👥 Autores

| Nome | RA | Contato |
|------|------|---------|
| **João Pedro Lima Lustosa Amorim** | 10289920 | joaopedrolima.amorim1@mackenzista.com.br |
| **Islas Matheus Coelho Azevedo** | 10423586 | islas.azevedo@mackenzista.com.br |

---
