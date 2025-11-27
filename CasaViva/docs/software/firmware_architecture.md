# Arquitetura do Firmware – CasaViva IoT

O firmware do **Projeto CasaViva** é responsável por gerenciar sensores, atuadores, comunicação MQTT, renderização no LCD e lógica de decisão ambiental.

---

# 🧱 Organização do Projeto
    firmware/
    ├── src/
    │ └── main.cpp
    ├── include/
    │ └── configs.h
    ├── lib/
    │ └── LiquidCrystal_I2C/
    └── platformio.ini

---

# 🔄 Ciclo Principal (Loop)

```
flowchart TD
Start[Setup] --> Wifi[Conectar Wi-Fi]
Wifi --> MQTT[Conectar MQTT]
MQTT --> Loop[Loop Principal]

Loop --> Sensores{Atualiza Sensores}
Loop --> LCD{Renderiza LCD}
Loop --> LED{Estado RGB}
Loop --> Relé{Ação do Relé}
Loop --> Joystick{Input do Joystick}
Loop --> Telemetria{Publicar MQTT}
Loop --> Loop
```

# 🔧 Principais Módulos do Firmware
1. Módulo de Sensores
- DHT22 (temp/umidade)

- MQ-135 (qualidade)

- KY-037 (som)

- Potenciômetro

2. Módulo de Exibição
- LCD 16×2

- Ícones personalizados

- Navegação por páginas

- Modo ECO

3. Módulo de Comunicação
- Cliente MQTT

- Reconexão automática

- Tratamento de mensagens

4. Módulo de Ação
- Relé (ativo em LOW)

- LED RGB (status do ambiente)

5. Módulo de Interação
- Joystick analógico

- Debounce via software

# 🧪 Estratégias de Implementação
- Loop não bloqueante (millis)

- Estados persistentes

- LCD com redraw inteligente (evita flicker)

- Sistema de telemetria periódica

- Reconexão robusta

