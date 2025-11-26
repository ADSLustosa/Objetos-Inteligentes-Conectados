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