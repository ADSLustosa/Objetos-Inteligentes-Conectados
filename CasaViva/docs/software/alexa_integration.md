# Integração com Amazon Alexa – CasaViva IoT

A integração com a Alexa leva o CasaViva ao nível de automação residencial inteligente, permitindo controle por voz, rotinas e comunicação em nuvem usando AWS IoT e AWS Lambda.

---

# 🧩 Arquitetura Alexa + CasaViva

```
flowchart TD
Alexa --> Skill[Amazon Alexa Smart Home Skill]
Skill --> Lambda[AWS Lambda]
Lambda --> AWSIoT[AWS IoT Core]
AWSIoT --> MQTTBroker[(MQTT Broker)]
MQTTBroker --> ESP32[ESP32 CasaViva]
```

# 🗂 Componentes Envolvidos
1. Alexa Smart Home Skill

- A skill recebe comandos como:

    "Ligar umidificador"

    "Desligar ventilação"

- Usa o padrão Alexa Discovery JSON

- Utiliza intents nativas (TurnOn / TurnOff)

2. AWS Lambda

- Funções principais:

    Traduzir intents da Alexa em comandos MQTT

    Publicar no tópico:
    ```
    /CasaViva/relay
    ```

- Validar tokens / autenticação

3. AWS IoT Core

- Gerencia certificados X.509

- Fornece broker MQTT seguro (porta 8883 TLS)

- Mantém Shadow do dispositivo

# 🛰 Fluxo de Execução
```
sequenceDiagram
Alexa->>Skill: TurnOnIntent
Skill->>Lambda: Directive JSON
Lambda->>AWSIoT: Publish /CasaViva/relay = "ON"
AWSIoT->>ESP32: Comando MQTT
ESP32->>Relay: Ativa relé
```

# 🔧 JSON de Comando de Voz

- Alexa envia:
```
{
  "directive": {
    "header": {
      "namespace": "Alexa.PowerController",
      "name": "TurnOn"
    }
  }
}
```

- Lambda publica:
```
Topic: /CasaViva/relay
Payload: "ON"
```

# 📦 Entrada de Descoberta (Alexa Discovery)

- O dispositivo é registrado assim:
```
{
  "endpoints": [
    {
      "endpointId": "casaviva_relay",
      "displayName": "Umidificador CasaViva",
      "capabilities": [
        {
          "type": "AlexaInterface",
          "interface": "Alexa.PowerController",
          "version": "3"
        }
      ]
    }
  ]
}
```

# ✔ Benefícios da Integração Alexa

- Controle automático de umidificador

- Rotinas personalizadas

- Comandos de voz hands-free

- Integração com app Alexa

- Comportamento inteligente baseado nos sensores