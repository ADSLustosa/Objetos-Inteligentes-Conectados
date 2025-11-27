# ⚡ Módulo Relé 5V – Controle de Cargas no CasaViva IoT
<p align="center"> 
<img src="../assets/Relay_Module.jpeg" width="330"><br>
</p>

## 📘 Visão Geral

O **Módulo relé 5V** é responsável por acionar dispositivos externos no sistema CasaViva IoT, como:

- Umidificador

- Luminária

- Ventilador

- Purificador de ar

- Qualquer carga AC/DC compatível

Ele funciona como um **“interruptor eletrônico”**, permitindo que o ESP32 controle cargas de maneira isolada e segura.
```
Importante: o módulo usado possui acionamento ativo em LOW, ou seja,
LOW = liga a carga
HIGH = desliga a carga
```

## 🔌 Pinagem e Especificações
### 📍 Pinos de controle (lado lógico)
| Pino | Função          | Descrição                    |
| ---- | --------------- | ---------------------------- |
| IN   | Entrada digital | Conectado ao ESP32 (GPIO 27) |
| VCC  | +5V             | Alimenta o relé              |
| GND  | Terra           | Referência de solo           |

### 📍 Pinos de saída (lado da carga)
| Terminal | Função                                             |
| -------- | -------------------------------------------------- |
| COM      | Comum                                              |
| NO       | Normalmente aberto (aciona quando o relé liga)     |
| NC       | Normalmente fechado (aciona quando o relé desliga) |

No **Projeto CasaViva** utilizamos COM + NO, que são os terminais mais comuns para automação.

## 🔧 Conexões no CasaViva IoT
```
IN  → GPIO 27  
VCC → VIN (5V do ESP32)  
GND → GND
```
**Motivo:**
    O relé precisa de 5V para ativação, e o ESP32 disponibiliza isso no pino VIN durante a simulação.

## 🧠 Lógica de Funcionamento no Firmware

O relé reage a mensagens MQTT:

### ▶ Assinatura
```
/CasaViva/relay
```

### ▶ Comandos aceitos
| Payload | Ação           |
| ------- | -------------- |
| ON      | Liga o relé    |
| OFF     | Desliga o relé |

### ▶ Exemplo (MQTTX)
```
Topic: /CasaViva/relay
Payload: ON
```

### ▶ Trecho de código no firmware
```
if (msg.equalsIgnoreCase("ON")) {
    digitalWrite(RELAY_PIN, LOW);   // Liga (ativo em LOW)
} else if (msg.equalsIgnoreCase("OFF")) {
    digitalWrite(RELAY_PIN, HIGH);  // Desliga
}
```