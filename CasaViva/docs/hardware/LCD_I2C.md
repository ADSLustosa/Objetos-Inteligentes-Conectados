
---

# 📂 **/docs/hardware/LCD_I2C.md**

```md
# Display LCD 16x2 com Interface I2C

## Descrição
O LCD apresenta em tempo real:
- Temperatura e umidade
- Estado do relé
- Qualidade do ar
- Indicadores de alerta
- Estado Wi-Fi e MQTT

## Pinagem
    | Pino LCD | ESP32 |
    |----------|--------|
    | GND      | GND |
    | VCC      | 3V3 |
    | SDA      | GPIO 21 |
    | SCL      | GPIO 22 |

## Biblioteca Usada
    LiquidCrystal_I2C

## Recursos Utilizados
- Animações de progresso
- Ícones customizados
- Troca de páginas via joystick
- Modo ECO

## Código Base
    ```cpp
    LiquidCrystal_I2C lcd(0x27, 16, 2);
    lcd.begin(16, 2);
    lcd.print("CasaViva IoT");
