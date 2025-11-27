# 🧪 Simulation – CasaViva IoT
**Ambiente completo de simulação Wokwi para testes rápidos, MQTT e depuração visual**

A pasta <mark>simulation/</mark> contém tudo o que você precisa para **executar, testar e validar** o sistema CasaViva IoT diretamente no **Wokwi**, sem necessidade de hardware físico.
É o ambiente perfeito para experimentação, depuração e demonstração acadêmica.

# 🚀 O que você pode simular
**🔥 Sensores**

- **DHT22** → temperatura e umidade

- **MQ-135** → qualidade do ar

- **Microfone KY-037** → nível de ruído

- **Joystick analógico** → interação manual

- **Potenciômetro** → ajuste de valores

# 💡 Atuadores

- **Relé** (controlado via MQTT ou pelo firmware)

- **LED RGB** (catodo comum)

- **LCD 16x2 I2C** com ícones customizados

📂 Arquivos incluídos
```
simulation/
 ├── diagram.json        # Esquemático completo no Wokwi
 ├── sketch.ino          # Código para simulação (versão Wokwi)
 ├── libraries.txt       # Lista de libs carregadas automaticamente
 ├── LiquidCrystal_I2C.h # Biblioteca do LCD compatível com Wokwi
 ├── README.md           # (este arquivo)
 └── wokwi-project.txt   # Link direto do projeto
```

# ▶️ Como executar a simulação

1. Abra o arquivo <mark>wokwi-project.txt</mark>

2. Clique no link

3. O Wokwi vai abrir o projeto com todos os componentes montados

4. Clique em **Start Simulation**

        Pronto! LCD, sensores, MQTT e LED estarão funcionando em tempo real.

# 🌡️ Como simular valores dos sensores

**DHT22 → Temp/Umidade**

- Clique no sensor

- Ajuste os sliders de Temperature e Humidity

**MQ-135 → Qualidade do ar**

- Clique no MQ-135

- Ajuste a concentração de gases

**Microfone → Ruído**

- Clique no microfone

- Ajuste o nível de som

**Joystick**

- Arraste o stick para qualquer direção

# 🛰️ MQTT com HiveMQ + MQTTX
    
✔ Receber dados (temp/umidade)

Assine:
```
/CasaViva/temperatura
/CasaViva/umidade
```
✔ Controlar o relé pelo MQTTX

Publique em:
```
/CasaViva/relay
```

Com payload:
```
ON
```

ou

```
OFF
```

Relé e LCD respondem instantaneamente!

# 🖥️ Visualização do sistema no LCD

O display mostra:

- Temperatura e umidade

- Status do Wi-Fi

- Status do MQTT

- Estado do relé

- Máx/Mín registrados

- Telas alternando automaticamente

# 🧠 Por que este ambiente é poderoso?

**✔ Permite testar toda a lógica sem hardware**
**✔ Ideal para avaliação acadêmica (UPM/Mackenzie)**
**✔ Simula eventos complexos (MQTT + sensores)**
**✔ Atualizações rápidas**
**✔ Compatível com seu firmware real**