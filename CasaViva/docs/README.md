# 📘 Documentação Oficial – Projeto CasaViva IoT
**Sistema de Monitoramento Ambiental com ESP32, MQTT, Alexa e Automação Residencial**

Bem-vindo à pasta <mark>docs/</mark>, que reúne toda a documentação técnica, arquitetural e operacional do projeto **CasaViva IoT**.
Aqui você encontrará materiais completos sobre hardware, software, simulação, arquitetura e integrações em nuvem.

# 🧭 Estrutura Geral dos Documentos

A documentação está organizada para facilitar leitura e manutenção:
```
docs/
 ├── hardware/
 ├── software/
 ├── README.md  ← (este arquivo)
```

# 🔧 📂 hardware/

Contém a documentação detalhada de todos os sensores e atuadores do sistema:

**Sensores**

- **DHT22** – temperatura e umidade

- **MQ-135** – qualidade do ar (gases tóxicos)

- **KY-037** – captação de ruído ambiental

**Interfaces de Entrada**

- **Joystick analógico** – controle de navegação

- **Potenciômetro** – ajuste de thresholds

**Atuadores e Saída**

- **Relé AC** – controle de dispositivos (umidificadores, climatizadores etc.)

- **LED RGB** – feedback visual instantâneo

- **LCD 16x2 I2C** – dashboards embarcados com ícones e múltiplas páginas

# 💻 📂 software/

Documenta toda a camada lógica e embarcada do projeto:

## 📌 Arquivos de Software

- **arquitetura_geral.md**
    Visão completa do ecossistema IoT, camadas, módulos e padrões de projeto.

- **firmware_architecture.md**
    Design do firmware, loop não bloqueante, organização dos módulos, drivers e eventos.

- **mqtt_integration.md**
    Documentação completa da integração MQTT (HiveMQ / AWS IoT).

- **alexa_integration.md**
    Arquitetura da integração com Alexa, Fluxos AWS Lambda e Smart Home Skill.

- **simulation_guide.md**
    Guia completo de simulação no Wokwi, interação com sensores e MQTTX.

# 🌐 Tecnologias Empregadas
**🔹 IoT / Hardware**

- ESP32 DevKit V1

- DHT22 • KY-037 • MQ-135

- LCD I2C • LED RGB • Relé

**🔹 Comunicação e Rede**

- MQTT (HiveMQ / AWS IoT)

- Wi-Fi (Wokwi Guest / Rede local)

- MQTTX • Node-RED

- TLS (quando usado AWS IoT Core)

**🔹 Nuvem e Automação**

- AWS Lambda

- AWS IoT Core

- Alexa Smart Home Skill

**🔹 Simulação**

- Wokwi

- Monitoramento MQTT via MQTTX

# 🎯 Objetivo da Documentação

Esta pasta foi criada para:

    ✔ Documentar toda a arquitetura do sistema
    ✔ Permitir replicação do projeto por qualquer desenvolvedor
    ✔ Fornecer material acadêmico para avaliações
    ✔ Servir como referência para evolução futura
    ✔ Garantir escalabilidade e padronização técnica

# 📄 Guia Rápido de Acesso

| Categoria | Documento              | Descrição                                 |
| --------- | ---------------------- | ----------------------------------------- |
| Hardware  | `DHT22.md`             | Especificações técnicas + uso no firmware |
| Hardware  | `MQ135.md`             | Monitoramento de qualidade do ar          |
| Software  | `arquitetura_geral.md` | Diagrama global e camadas do projeto      |
| Software  | `mqtt_integration.md`  | Tópicos MQTT, comandos e fluxo            |
| Software  | `alexa_integration.md` | Comunicação ESP32 → AWS → Alexa           |
| Software  | `simulation_guide.md`  | Como simular sensores e MQTT              |

# 🖼 Diagrama Conceitual do Sistema
```
flowchart TD
Sensores --> ESP32
ESP32 --> LCD
ESP32 --> LED
ESP32 --> Relay

ESP32 --> MQTT[(Broker MQTT)]
MQTT --> MQTTX
MQTT --> NodeRED
MQTT --> Alexa[AWS IoT → Lambda → Alexa]
```

# 🏆 Sobre o Projeto CasaViva IoT

O **Projeto CasaViva** IoT foi desenvolvido como solução inteligente para promoção da saúde e bem-estar em ambientes residenciais, alinhado ao ODS 3 da ONU.
Integra sensores ambientais, automação e assistentes de voz para monitorar riscos e agir automaticamente, tudo com baixíssimo custo e código aberto.