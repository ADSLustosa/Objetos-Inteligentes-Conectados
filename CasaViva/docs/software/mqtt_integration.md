# Integração MQTT – Projeto CasaViva IoT

A integração MQTT é o núcleo da comunicação entre o ESP32, dashboards, Alexa e sistemas externos.  
O CasaViva utiliza o protocolo **MQTT 3.1.1**, operando principalmente por meio do **HiveMQ Public Broker**.

---

# 📡 1. Arquitetura da Comunicação

```
flowchart TD
ESP[ESP32 CasaViva] -->|Publica Telemetria| BrokerMQTT[(Broker MQTT)]
BrokerMQTT --> MQTTX[MQTTX Dashboard]
BrokerMQTT --> NodeRED[Node-RED]
BrokerMQTT --> Alexa[AWS IoT / Alexa Skill]
BrokerMQTT --> Mobile[Aplicativos Mobile]
Mobile --> BrokerMQTT
```

A comunicação é orientada a eventos. O ESP32 publica periodicamente dados ambientais, enquanto recebe comandos para atuadores como o módulo relé.

# 📁 2. Estrutura dos Tópicos MQTT

O projeto utiliza a seguinte estrutura hierárquica:

Publicação (ESP32 → Nuvem)
    | Tópico                   | Conteúdo                | Tipo  |
    | ------------------------ | ----------------------- | ----- |
    | `/CasaViva/temperatura`  | Temperatura atual em °C | float |
    | `/CasaViva/umidade`      | Umidade relativa (%)    | float |
    | `/CasaViva/qualidade_ar` | Leitura bruta MQ-135    | int   |
    | `/CasaViva/ruido`        | Intensidade do som      | int   |

Comandos recebidos (Nuvem → ESP32)
    | Tópico            | Comando          | Ação                       |
    | ----------------- | ---------------- | -------------------------- |
    | `/CasaViva/relay` | `"ON"` / `"OFF"` | Liga/desliga o módulo relé |


# 💬 3. Payloads Padrão
Exemplo de telemetria enviada pelo ESP32:
```
    {
    "temperatura": 24.8,
    "umidade": 58.2,
    "qualidade_ar": 712,
    "ruido": 180
    }
```

Exemplo de comando para acionamento:
```
    Topic: /CasaViva/relay
    Message: "ON"
```

# 🔧 4. Implementação no Firmware
Configuração do cliente MQTT no ESP32:
```
    client.setServer("broker.hivemq.com", 1883);
    client.setCallback(mqttCallback);
    client.connect("CasaVivaESP32");
    client.subscribe("/CasaViva/relay");
```
Publicação:

```
    client.publish("/CasaViva/temperatura", String(temp).c_str());
    client.publish("/CasaViva/umidade",     String(umid).c_str());
```

Recepção:

```
    void mqttCallback(char* topic, byte* payload, unsigned int len) {
        String msg = "";
        for (int i = 0; i < len; i++) msg += (char)payload[i];

        if (String(topic) == "/CasaViva/relay") {
            if (msg == "ON") digitalWrite(RELAY_PIN, LOW);
            else digitalWrite(RELAY_PIN, HIGH);
        }
    }
```

# 🧪 5. Testes com MQTTX

1. Abra o MQTTX
2. Criar nova conexão:
```
    URL: broker.hivemq.com
    Porta: 1883
```
3. Criar assinaturas:
```
/CasaViva/#
```
4. Publicar comandos:
```
    Topic: /CasaViva/relay
    Payload: "ON"
```

# ⚙ 6. Boas Práticas de Publicação

- Utilizar QoS 0 para dispositivos low-power.

- Publicar somente o necessário (evitar excesso).

- Usar nomes de tópicos autoexplicativos.

- Evitar enviar grande volume de dados em cada mensagem.

- Utilizar JSON quando necessário, mas preferir valores simples para telemetria.

# 🛡 7. Integração com AWS IoT (Modo Seguro)

- Para produção, recomenda-se:

- MQTT com TLS 1.2

- Certificados X.509

- Shadow Device para sincronização

- Rate limiting entre 3–5s

# 🚀 8. Possibilidades de Expansão

- Retenção de mensagens

- Log histórico em banco de dados

- Dashboard web com WebSockets

- Automação com Node-RED

- Trigger de rotinas Alexa por regra de qualidade do ar
