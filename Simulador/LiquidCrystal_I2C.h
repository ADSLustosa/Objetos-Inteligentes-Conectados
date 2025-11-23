#pragma once
#include <Wire.h>

class LiquidCrystal_I2C {
public:
  LiquidCrystal_I2C(uint8_t address, uint8_t columns, uint8_t rows) {
    _address = address;
    _cols = columns;
    _rows = rows;
  }

  void init() { begin(); }
  void begin() {}

  void backlight() {}
  void clear() { Wire.beginTransmission(_address); Wire.write(0x01); Wire.endTransmission(); }

  void setCursor(uint8_t col, uint8_t row) {
    Wire.beginTransmission(_address);
    Wire.write(0x80 | (col + row * 0x40));
    Wire.endTransmission();
  }

  void print(const char *str) {
    while (*str) write(*str++);
  }

  void print(String str) {
    for (int i = 0; i < str.length(); i++) write(str[i]);
  }

  void print(float val) {
    print(String(val));
  }

  void print(int val) {
    print(String(val));
  }

private:
  void write(uint8_t value) {
    Wire.beginTransmission(_address);
    Wire.write(0x40);
    Wire.write(value);
    Wire.endTransmission();
  }

  uint8_t _address, _cols, _rows;
};
