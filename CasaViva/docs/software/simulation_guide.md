# ▶ Iniciando a Simulação

Acesse:
```
https://wokwi.com
```

E importe:

- `diagram.json`
- `sketch.ino`

---

# 🧩 Componentes Interativos

### **DHT22**
- Clique no sensor e ajuste:
  - Temperatura
  - Umidade

### **MQ-135**
- Ajuste o controle deslizante (Analog Value)

### **KY-037**
- Use o controle "Sound Level"

### **Potenciômetro**
- Gire com o mouse para ajustar threshold do MQ-135

### **Joystick**
- Setas direcionais
- Botão central (SEL)

### **LED RGB**
- Cor varia conforme o ambiente:
  - Verde → ar saudável
  - Amarelo → moderado
  - Vermelho → perigoso
  - Azul → ruído detectado

---

# 📟 LCD I2C
Mostra páginas:
- Dashboard
- Temperatura/Umidade
- Qualidade do ar
- Ruído
- Estado do relé
- Status MQTT/Wi-Fi

---

# 📡 Teste MQTT via MQTTX

Conecte-se:
```
broker.hivemq.com:1883
```

Assine:
```
/CasaViva/#
```

Comando:
```
Topic: /CasaViva/relay
Message: "ON"
```
