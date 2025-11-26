<<<<<<< HEAD
📑 Sumário

📘 Visão Geral

🎯 Objetivos do Projeto

🧩 Funcionalidades Principais

🏗 Arquitetura do Sistema

🔧 Hardware Utilizado

💻 Software e Tecnologias

📡 Integração MQTT

🎤 Integração com Alexa

📂 Estrutura do Repositório

🚀 Como Executar o Projeto

📊 Dashboard Local (MQTTX)

📚 Documentação Completa

📎 Referências

👥 Autores

📘 Visão Geral

O CasaViva é um sistema IoT completo que transforma ambientes residenciais em espaços mais inteligentes, seguros e saudáveis.
Ele monitora continuamente:

Temperatura

Umidade

Qualidade do ar

Ruído ambiente

E integra-se com:

Alexa (comandos de voz)

MQTT (HiveMQ / MQTTX)

Relés inteligentes

Interface física (LCD + Joystick)

Indicadores visuais (LED RGB)

Com foco no ODS 3 – Saúde e Bem-Estar, o projeto demonstra como tecnologias acessíveis podem melhorar a qualidade de vida em ambientes internos.

🎯 Objetivos do Projeto

Monitorar condições ambientais em tempo real

Emitir alertas inteligentes e compreensíveis

Automatizar dispositivos domésticos via MQTT e Alexa

Criar um sistema modular, escalável e de baixo custo

Contribuir para ambientes mais saudáveis, confortáveis e seguros

🧩 Funcionalidades Principais
📡 Monitoramento Contínuo

Temperatura e umidade (DHT22)

Qualidade do ar (MQ-135)

Ruído ambiente (KY-037)

🌐 Comunicação MQTT

Envio periódico de métricas ambientais

Controle remoto do relé via tópicos MQTT

Integração com ferramentas de monitoramento (MQTTX, dashboards etc.)

🖥 Interface Física

LCD 16x2 com páginas dinâmicas (Dashboard, Estatísticas, Qualidade do Ar, Rede, Alertas, Modo ECO)

Joystick para navegação entre telas e ações rápidas

Potenciômetro para ajuste de limiar do MQ-135

🔔 Alertas Inteligentes

LED RGB com feedback visual por cor (verde/normal, amarelo/atenção, vermelho/crítico, roxo/qualidade do ar, azul/som, branco/relé ativo)

Mensagens no LCD para alertas ambientais e de sistema

Priorização de eventos (som > qualidade do ar > temperatura/umidade)

🏗 Arquitetura do Sistema
┌────────────┐      MQTT TX       ┌───────────────┐
│   ESP32     │──────────────────▶│  HiveMQ Cloud  │
│             │◀──────────────────┤  MQTT Broker   │
└─────┬──────┘      MQTT RX       └───────────────┘
      │
      ▼
┌──────────┐    ┌──────────┐    ┌───────────┐
│  DHT22   │    │ MQ-135   │    │  KY-037   │
└──────────┘    └──────────┘    └───────────┘
      │                │                │
      ▼                ▼                ▼
           ┌─────────────────────────┐
           │     CasaViva Core       │
           │  Processamento lógico   │
           └──────────┬──────────────┘
                      ▼
   ┌─────────────────────────────────────────────┐
   │ LCD I2C │ LED RGB │ Relé │ Joystick │ Alexa │
   └─────────────────────────────────────────────┘

🔧 Hardware Utilizado

ESP32 DevKit V1 – microcontrolador principal

DHT22 – sensor de temperatura e umidade

MQ-135 – sensor de qualidade do ar (gases/poluentes)

KY-037 – sensor de som/ruído

Potenciômetro 10k – ajuste de limiar para MQ-135

Joystick analógico – controle da interface no LCD

LCD 16x2 com I2C – interface de visualização local

Módulo relé 5V – acionamento de carga (ex.: umidificador, lâmpada)

LED RGB cátodo comum – indicador visual do estado do sistema

💻 Software e Tecnologias

Linguagem C++ (Arduino Framework)

Simulação com Wokwi

MQTT via PubSubClient

Broker MQTT: HiveMQ (público)

Cliente MQTT: MQTTX

Integração planejada com Alexa Smart Home

Ferramentas de desenvolvimento: Arduino IDE ou PlatformIO

📡 Integração MQTT
Publicações automáticas

O ESP32 publica periodicamente:

/CasaViva/temperatura   → valor em °C
/CasaViva/umidade       → valor em %RH
/CasaViva/qualidade_ar  → valor analógico/normalizado do MQ-135

Assinaturas (controle remoto)

O ESP32 assina:

/CasaViva/relay


Mensagens aceitas:

"ON" → ativa o relé (liga o dispositivo)

"OFF" → desativa o relé

Exemplo de uso no MQTTX

Conectar em: broker.hivemq.com:1883

Publicar:

Topic: /CasaViva/relay
Payload: ON
QoS: 0

🎤 Integração com Alexa

A arquitetura de integração com a Alexa segue o fluxo:

Usuário emite comando de voz para a Alexa

A Alexa aciona uma Skill CasaViva configurada

A Skill chama uma função AWS Lambda

A Lambda publica mensagens em tópicos MQTT (HiveMQ ou AWS IoT)

O ESP32 recebe o comando via MQTT e aciona:

relé

alertas

atualização do LCD

Exemplos de comandos planejados:

“Alexa, ligar o umidificador da sala”

“Alexa, qual é a temperatura da casa?”

“Alexa, como está a qualidade do ar?”

📂 Estrutura do Repositório
CasaViva/
│
├── docs/
│   ├── hardware/
│   ├── software/
│   └── README.md
│
├── firmware/
│   └── main.cpp
│
├── simulation/
│   └── diagram.json
│
├── assets/
│   └── imagens-do-projeto/
│
└── README.md

🚀 Como Executar o Projeto
1. Execução no Wokwi

Abra o Wokwi (web)

Crie um novo projeto baseado em ESP32

Substitua o conteúdo pelo diagram.json em simulation/

Cole o código do main.cpp em firmware/ no editor do Wokwi

Clique em Run

Na simulação, é possível:

Ajustar temperatura/umidade no DHT22

Simular poluição no MQ-135

Gerar ruído no sensor de som

Ajustar o potenciômetro para mudar o limiar de alerta

Navegar pelas telas com o joystick

2. Uso com MQTTX

Abra o MQTTX

Crie uma nova conexão com:

Host: broker.hivemq.com

Porta: 1883

Protocolo: MQTT 3.1.1

Assine os tópicos:

/CasaViva/temperatura
/CasaViva/umidade
/CasaViva/qualidade_ar


Para controlar o relé, publique:

Topic: /CasaViva/relay
Payload: ON

3. Execução em Hardware Real (ESP32)

Instalar o Arduino IDE ou PlatformIO

Selecionar a placa ESP32 DevKit V1

Instalar as bibliotecas:

PubSubClient

DHT sensor library

LiquidCrystal I2C

Configurar a rede WiFi no código (WIFI_SSID, WIFI_PASSWORD)

Fazer upload do main.cpp para o ESP32

Monitorar via Serial (115200 baud)

📊 Dashboard Local (MQTTX)

Com o MQTTX, é possível:

Visualizar temperatura e umidade em tempo real

Ver o comportamento da qualidade do ar

Monitorar logs de mudança de estado do relé

Testar cenários de automação com diferentes payloads

Essa abordagem simula um painel local de monitoramento sem necessidade imediata de front-end web dedicado.

📚 Documentação Completa

A documentação técnica detalhada está na pasta:

/docs
  /hardware   → Descrição de cada componente físico
  /software   → Arquitetura, fluxos lógicos e MQTT


Inclui:

Diagrama de ligação dos sensores

Descrição dos tópicos MQTT

Lógica de alertas e estados do LED RGB

Papéis de cada módulo (sensores, atuadores, interface)

📎 Referências

WHO. Ambient air pollution guidelines. World Health Organization, 2018.

IPEA. ODS 3 – Saúde e Bem-Estar no Brasil: indicadores e análises. 2022.

SMITH, J. et al. Indoor environmental quality and health. Journal of Smart Buildings, 2022.

NEWBURY, R. et al. Smart home sensing for well-being. Sensors & Health Informatics Review, 2024.

👥 Autores
Nome	RA	Contato
João Pedro Lima Lustosa Amorim	10289920	joaopedrolima.amorim1@mackenzista.com.br

Islas Matheus Coelho Azevedo	10423586	islas.azevedo@mackenzista.com.br
=======
# 🌱 CasaViva IoT
### Sistema Inteligente de Monitoramento Ambiental Integrado à Alexa e MQTT

[![Status](https://img.shields.io/badge/status-ativo-brightgreen)]()
[![ESP32](https://img.shields.io/badge/ESP32-supported-blue)]()
[![MQTT](https://img.shields.io/badge/MQTT-HiveMQ%20Cloud-purple)]()
[![License](https://img.shields.io/badge/license-MIT-lightgrey)]()

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

    /CasaViva/temperatura
    /CasaViva/umidade
    /CasaViva/qualidade_ar


### Assinatura (controle):

    /CasaViva/relay → ON / OFF

### Exemplo (MQTTX):

    Topic: /CasaViva/relay
    Payload: ON


---

# 🎤 Integração com Alexa

Fluxo planejado:

1. Alexa recebe comando →  
2. Skill CasaViva →  
3. AWS Lambda →  
4. Publica via MQTT →  
5. ESP32 aciona relé / alertas / LCD  

Exemplos:
- "Alexa, ligar o umidificador"  
- "Alexa, como está a temperatura da sala?"  
- "Alexa, como está o ar da casa?"  

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
3. Cole `main.cpp`  
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

    broker.hivemq.com
    porta: 1883


**Assinar:**

    /CasaViva/temperatura
    /CasaViva/umidade
    /CasaViva/qualidade_ar


**Publicar comando:**

    /CasaViva/relay → ON


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

    /docs/hardware
    /docs/software


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
>>>>>>> 8e8cd24d63251f03b0974ea350cf43f6fe03c474
