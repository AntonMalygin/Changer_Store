#pragma once

#include <Arduino.h>
#include <Wire.h>

#define ST7567S_USE_UTF8 // Comment this define to use only ASCII chars

#ifdef ST7567S_USE_UTF8
typedef uint16_t utf8_t;
#endif

template<const uint8_t ADDR, TwoWire &WIRE = Wire>
class ST7567S {
public:
  static void init(bool fast = true);
#if defined(ESP8266) || defined(ESP32)
  static bool init(uint8_t sda, uint8_t scl, bool fast = true);
#endif

  bool begin();
  uint8_t width() const;
  uint8_t height() const;
  bool powerSave(bool enable);
  bool flip(bool enable);
  bool inverse(bool enable);
  bool contrast(uint8_t value);
  bool clear();
  bool fill(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color);
  bool drawPattern(uint8_t x, uint8_t y, uint8_t pattern, uint8_t repeat = 1);
  bool drawPattern(uint8_t x, uint8_t y, const uint8_t *pattern, uint8_t size);
  bool drawPattern_P(uint8_t x, uint8_t y, const uint8_t *pattern, uint8_t size);
  bool drawPixel(uint8_t x, uint8_t y, bool color);
  bool drawHLine(uint8_t x, uint8_t y, uint8_t w, bool color);
  bool drawVLine(uint8_t x, uint8_t y, uint8_t h, bool color);
  bool scrollUp(uint8_t pages);

  uint8_t charWidth() const;
  uint8_t charHeight() const;
#ifdef ST7567S_USE_UTF8
  bool printChar(uint8_t x, uint8_t y, utf8_t c);
#else
  bool printChar(uint8_t x, uint8_t y, char c);
#endif
  bool printStr(uint8_t x, uint8_t y, const char *str);
  bool printStr_P(uint8_t x, uint8_t y, const char *str);

protected:
  static const uint8_t WIDTH = 128;
  static const uint8_t HEIGHT = 64;
  static const uint8_t FLIP_OFFSET = 4;

  static const uint8_t FONT_WIDTH = 5;
  static const uint8_t FONT_HEIGHT = 8;
  static const uint8_t FONT_GAP = 1;

  bool sendCommand(uint8_t cmd);
  bool sendCommand(const uint8_t *cmds, uint8_t count);
  bool sendCommand_P(const uint8_t *cmds, uint8_t count);
  bool sendData(uint8_t data);
  bool sendData(uint8_t data, uint8_t count);
  bool sendData(const uint8_t *data, uint8_t count);
  bool sendData_P(const uint8_t *data, uint8_t count);
  bool setPointer(uint8_t x, uint8_t y);

#ifdef ST7567S_USE_UTF8
  const uint8_t *charPattern(utf8_t c);
#else
  const uint8_t *charPattern(char c);
#endif

  uint8_t _startPage : 4;
  bool _flipped : 1;
};

static const uint8_t FONT_5X8[] PROGMEM = {
#ifdef ST7567S_USE_UTF8
  0x20, 0x00, 0x7E, 0x00, // From 0x0020 (' ') to 0x007E ('~')
#else
  0x20, 0x7E, // From 0x20 (' ') to 0x7E ('~')
#endif
  0x00, 0x00, 0x00, 0x00, 0x00, // ' '
  0x00, 0x00, 0x5F, 0x00, 0x00, // '!'
  0x00, 0x07, 0x00, 0x07, 0x00, // '"'
  0x14, 0x3E, 0x14, 0x3E, 0x14, // '#'
  0x24, 0x2A, 0x7F, 0x2A, 0x12, // '$'
  0x23, 0x13, 0x08, 0x64, 0x62, // '%'
  0x36, 0x49, 0x55, 0x22, 0x50, // '&'
  0x00, 0x04, 0x02, 0x01, 0x00, // '\''
  0x00, 0x1C, 0x22, 0x41, 0x00, // '('
  0x00, 0x41, 0x22, 0x1C, 0x00, // ')'
  0x14, 0x08, 0x3E, 0x08, 0x14, // '*'
  0x08, 0x08, 0x3E, 0x08, 0x08, // '+'
  0x00, 0xA0, 0x60, 0x00, 0x00, // ','
  0x08, 0x08, 0x08, 0x08, 0x08, // '-'
  0x00, 0x60, 0x60, 0x00, 0x00, // '.'
  0x20, 0x10, 0x08, 0x04, 0x02, // '/'
  0x3E, 0x51, 0x49, 0x45, 0x3E, // '0'
  0x04, 0x42, 0x7F, 0x40, 0x00, // '1'
  0x62, 0x51, 0x49, 0x49, 0x46, // '2'
  0x22, 0x41, 0x49, 0x49, 0x36, // '3'
  0x18, 0x14, 0x12, 0x7F, 0x10, // '4'
  0x27, 0x45, 0x45, 0x45, 0x39, // '5'
  0x3C, 0x4A, 0x49, 0x49, 0x30, // '6'
  0x01, 0x71, 0x09, 0x05, 0x03, // '7'
  0x36, 0x49, 0x49, 0x49, 0x36, // '8'
  0x06, 0x49, 0x49, 0x29, 0x1E, // '9'
  0x00, 0x36, 0x36, 0x00, 0x00, // ':'
  0x00, 0x56, 0x36, 0x00, 0x00, // ';'
  0x08, 0x14, 0x22, 0x41, 0x00, // '<'
  0x14, 0x14, 0x14, 0x14, 0x14, // '='
  0x41, 0x22, 0x14, 0x08, 0x00, // '>'
  0x02, 0x01, 0x51, 0x09, 0x06, // '?'
  0x32, 0x49, 0x79, 0x41, 0x3E, // '@'
  0x7C, 0x12, 0x11, 0x12, 0x7C, // 'A'
  0x41, 0x7F, 0x49, 0x49, 0x36, // 'B'
  0x3E, 0x41, 0x41, 0x41, 0x22, // 'C'
  0x41, 0x7F, 0x41, 0x41, 0x3E, // 'D'
  0x7F, 0x49, 0x49, 0x49, 0x41, // 'E'
  0x7F, 0x09, 0x09, 0x09, 0x01, // 'F'
  0x3E, 0x41, 0x41, 0x51, 0x72, // 'G'
  0x7F, 0x08, 0x08, 0x08, 0x7F, // 'H'
  0x00, 0x41, 0x7F, 0x41, 0x00, // 'I'
  0x20, 0x40, 0x41, 0x3F, 0x01, // 'J'
  0x7F, 0x08, 0x14, 0x22, 0x41, // 'K'
  0x7F, 0x40, 0x40, 0x40, 0x40, // 'L'
  0x7F, 0x02, 0x0C, 0x02, 0x7F, // 'M'
  0x7F, 0x04, 0x08, 0x10, 0x7F, // 'N'
  0x3E, 0x41, 0x41, 0x41, 0x3E, // 'O'
  0x7F, 0x09, 0x09, 0x09, 0x06, // 'P'
  0x3E, 0x41, 0x51, 0x21, 0x5E, // 'Q'
  0x7F, 0x09, 0x19, 0x29, 0x46, // 'R'
  0x26, 0x49, 0x49, 0x49, 0x32, // 'S'
  0x01, 0x01, 0x7F, 0x01, 0x01, // 'T'
  0x3F, 0x40, 0x40, 0x40, 0x3F, // 'U'
  0x1F, 0x20, 0x40, 0x20, 0x1F, // 'V'
  0x3F, 0x40, 0x38, 0x40, 0x3F, // 'W'
  0x63, 0x14, 0x08, 0x14, 0x63, // 'X'
  0x07, 0x08, 0x70, 0x08, 0x07, // 'Y'
  0x61, 0x51, 0x49, 0x45, 0x43, // 'Z'
  0x00, 0x7F, 0x41, 0x41, 0x00, // '['
  0x02, 0x04, 0x08, 0x10, 0x20, // '\\'
  0x00, 0x41, 0x41, 0x7F, 0x00, // ']'
  0x04, 0x02, 0x01, 0x02, 0x04, // '^'
  0x80, 0x80, 0x80, 0x80, 0x80, // '_'
  0x00, 0x01, 0x02, 0x04, 0x00, // '`'
  0x20, 0x54, 0x54, 0x54, 0x78, // 'a'
  0x7F, 0x48, 0x44, 0x44, 0x38, // 'b'
  0x38, 0x44, 0x44, 0x44, 0x20, // 'c'
  0x38, 0x44, 0x44, 0x48, 0x7F, // 'd'
  0x38, 0x54, 0x54, 0x54, 0x18, // 'e'
  0x08, 0x7E, 0x09, 0x01, 0x02, // 'f'
  0x18, 0xA4, 0xA4, 0xA4, 0x7C, // 'g'
  0x7F, 0x08, 0x04, 0x04, 0x78, // 'h'
  0x00, 0x48, 0x7D, 0x40, 0x00, // 'i'
  0x20, 0x40, 0x44, 0x3D, 0x00, // 'j'
  0x7F, 0x10, 0x28, 0x44, 0x00, // 'k'
  0x00, 0x41, 0x7F, 0x40, 0x00, // 'l'
  0x7C, 0x04, 0x78, 0x04, 0x78, // 'm'
  0x7C, 0x08, 0x04, 0x04, 0x78, // 'n'
  0x38, 0x44, 0x44, 0x44, 0x38, // 'o'
  0xFC, 0x24, 0x24, 0x24, 0x18, // 'p'
  0x18, 0x24, 0x24, 0x28, 0xFC, // 'q'
  0x7C, 0x08, 0x04, 0x04, 0x08, // 'r'
  0x48, 0x54, 0x54, 0x54, 0x20, // 's'
  0x04, 0x3F, 0x44, 0x40, 0x20, // 't'
  0x3C, 0x40, 0x40, 0x20, 0x7C, // 'u'
  0x1C, 0x20, 0x40, 0x20, 0x1C, // 'v'
  0x3C, 0x40, 0x30, 0x40, 0x3C, // 'w'
  0x44, 0x28, 0x10, 0x28, 0x44, // 'x'
  0x1C, 0xA0, 0xA0, 0x90, 0x7C, // 'y'
  0x44, 0x64, 0x54, 0x4C, 0x44, // 'z'
  0x00, 0x08, 0x36, 0x41, 0x00, // '{'
  0x00, 0x00, 0x7F, 0x00, 0x00, // '|'
  0x00, 0x41, 0x36, 0x08, 0x00, // '}'
  0x08, 0x04, 0x04, 0x08, 0x04, // '~'
#ifdef ST7567S_USE_UTF8
  0xB0, 0x00, 0xB0, 0x00, // From 0x00B0 ('°') to 0x00B0 ('°')
  0x00, 0x02, 0x05, 0x02, 0x00, // '°'
  0x01, 0x04, 0x01, 0x04, // From 0x0401 ('Ё') to 0x0401 ('Ё')
  0x7C, 0x55, 0x54, 0x55, 0x44, // 'Ё'
  0x10, 0x04, 0x4F, 0x04, // From 0x0410 ('А') to 0x044F ('я')
  0x7E, 0x11, 0x11, 0x11, 0x7E, // 'А'
  0x7F, 0x49, 0x49, 0x49, 0x33, // 'Б'
  0x7F, 0x49, 0x49, 0x49, 0x36, // 'В'
  0x7F, 0x01, 0x01, 0x01, 0x03, // 'Г'
  0x70, 0x29, 0x27, 0x21, 0x7F, // 'Д'
  0x7F, 0x49, 0x49, 0x49, 0x41, // 'Е'
  0x77, 0x08, 0x7F, 0x08, 0x77, // 'Ж'
  0x41, 0x41, 0x49, 0x49, 0x36, // 'З'
  0x7F, 0x10, 0x08, 0x04, 0x7F, // 'И'
  0x7C, 0x21, 0x12, 0x09, 0x7C, // 'Й'
  0x7F, 0x08, 0x14, 0x22, 0x41, // 'К'
  0x20, 0x41, 0x3F, 0x01, 0x7F, // 'Л'
  0x7F, 0x02, 0x0C, 0x02, 0x7F, // 'М'
  0x7F, 0x08, 0x08, 0x08, 0x7F, // 'Н'
  0x3E, 0x41, 0x41, 0x41, 0x3E, // 'О'
  0x7F, 0x01, 0x01, 0x01, 0x7F, // 'П'
  0x7F, 0x09, 0x09, 0x09, 0x06, // 'Р'
  0x3E, 0x41, 0x41, 0x41, 0x22, // 'С'
  0x01, 0x01, 0x7F, 0x01, 0x01, // 'Т'
  0x47, 0x28, 0x10, 0x08, 0x07, // 'У'
  0x1C, 0x22, 0x7F, 0x22, 0x1C, // 'Ф'
  0x63, 0x14, 0x08, 0x14, 0x63, // 'Х'
  0x7F, 0x40, 0x40, 0x7F, 0xC0, // 'Ц'
  0x07, 0x08, 0x08, 0x08, 0x7F, // 'Ч'
  0x7F, 0x40, 0x7F, 0x40, 0x7F, // 'Ш'
  0x7F, 0x40, 0x7F, 0x40, 0xFF, // 'Щ'
  0x01, 0x7F, 0x48, 0x48, 0x30, // 'Ъ'
  0x7F, 0x48, 0x30, 0x00, 0x7F, // 'Ы'
  0x7F, 0x48, 0x48, 0x48, 0x30, // 'Ь'
  0x22, 0x49, 0x45, 0x49, 0x3E, // 'Э'
  0x7F, 0x08, 0x3E, 0x41, 0x3E, // 'Ю'
  0x46, 0x29, 0x19, 0x09, 0x7F, // 'Я'
  0x20, 0x54, 0x54, 0x54, 0x78, // 'а'
  0x3C, 0x4A, 0x49, 0x49, 0x31, // 'б'
  0x7C, 0x54, 0x54, 0x54, 0x28, // 'в'
  0x7C, 0x04, 0x04, 0x04, 0x04, // 'г'
  0x60, 0x38, 0x24, 0x24, 0x7C, // 'д'
  0x38, 0x54, 0x54, 0x54, 0x18, // 'е'
  0x6C, 0x10, 0x7C, 0x10, 0x6C, // 'ж'
  0x44, 0x54, 0x54, 0x54, 0x28, // 'з'
  0x7C, 0x20, 0x10, 0x08, 0x7C, // 'и'
  0x78, 0x42, 0x24, 0x12, 0x78, // 'й'
  0x7C, 0x10, 0x28, 0x44, 0x00, // 'к'
  0x40, 0x3C, 0x04, 0x04, 0x7C, // 'л'
  0x7C, 0x08, 0x10, 0x08, 0x7C, // 'м'
  0x7C, 0x10, 0x10, 0x10, 0x7C, // 'н'
  0x38, 0x44, 0x44, 0x44, 0x38, // 'о'
  0x7C, 0x04, 0x04, 0x04, 0x7C, // 'п'
  0xFC, 0x24, 0x24, 0x24, 0x18, // 'р'
  0x38, 0x44, 0x44, 0x44, 0x20, // 'с'
  0x04, 0x04, 0x7C, 0x04, 0x04, // 'т'
  0x44, 0x28, 0x10, 0x08, 0x04, // 'у'
  0x18, 0x24, 0xFE, 0x24, 0x18, // 'ф'
  0x44, 0x28, 0x10, 0x28, 0x44, // 'х'
  0x7C, 0x40, 0x40, 0x7C, 0xC0, // 'ц'
  0x0C, 0x10, 0x10, 0x10, 0x7C, // 'ч'
  0x7C, 0x40, 0x7C, 0x40, 0x7C, // 'ш'
  0x7C, 0x40, 0x7C, 0x40, 0xFC, // 'щ'
  0x04, 0x7C, 0x50, 0x50, 0x20, // 'ъ'
  0x7C, 0x50, 0x20, 0x00, 0x7C, // 'ы'
  0x7C, 0x50, 0x50, 0x50, 0x20, // 'ь'
  0x44, 0x54, 0x54, 0x54, 0x38, // 'э'
  0x7C, 0x10, 0x38, 0x44, 0x38, // 'ю'
  0x48, 0x54, 0x34, 0x14, 0x7C, // 'я'
  0x51, 0x04, 0x51, 0x04, // From 0x0451 ('ё') to 0x0451 ('ё')
  0x38, 0x55, 0x54, 0x55, 0x18, // 'ё'
#endif
};

template<const uint8_t ADDR, TwoWire &WIRE>
void ST7567S<ADDR, WIRE>::init(bool fast) {
  WIRE.begin();
  if (fast)
    WIRE.setClock(400000);
}

#if defined(ESP8266) || defined(ESP32)
template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::init(uint8_t sda, uint8_t scl, bool fast) {
#ifdef ESP8266
  WIRE.begin(sda, scl);
  if (fast)
    WIRE.setClock(400000);
  return true;
#else // ESP32
  return WIRE.begin(sda, scl, fast ? 400000U : 100000U);
#endif
}
#endif

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::begin() {
  static const uint8_t CMDS[] PROGMEM = {
    0xE2, // Reset
    0xAE, // Display OFF
    0xA0, // MX = 0
    0xC8, // MY = 1
    0xA6, // Inverse OFF
    0xA4, // All Pixel OFF
    0x2F, // Power ON
    0xA3, // Bias 1/7
    0x24, // RR = 5.0
    0x81, 0x1F, // EV = 31 (1/2)
//    0xAF // Display ON
  };

  _startPage = 0;
  _flipped = false;
  return sendCommand_P(CMDS, sizeof(CMDS)) && clear() && sendCommand(0xAF); // Display ON
}

template<const uint8_t ADDR, TwoWire &WIRE>
inline uint8_t ST7567S<ADDR, WIRE>::width() const {
  return WIDTH;
}

template<const uint8_t ADDR, TwoWire &WIRE>
inline uint8_t ST7567S<ADDR, WIRE>::height() const {
  return HEIGHT;
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::powerSave(bool enable) {
  static const uint8_t CMDS[4] PROGMEM = {
    0xAE, // Display OFF
    0xA5, // All Pixel ON
    0xA4, // All Pixel OFF
    0xAF, // Display ON
  };

  return sendCommand_P(&CMDS[enable ? 0 : 2], 2);
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::flip(bool enable) {
  static const uint8_t CMDS[4] PROGMEM = {
    0xC0, // MY = 0
    0xA1, // MX = 1
    0xC8, // MY = 1
    0xA0, // MX = 0
  };

  _flipped = enable;
  return sendCommand_P(&CMDS[enable ? 0 : 2], 2);
}

template<const uint8_t ADDR, TwoWire &WIRE>
inline bool ST7567S<ADDR, WIRE>::inverse(bool enable) {
  return sendCommand(0xA6 | enable);
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::contrast(uint8_t value) {
  uint8_t cmds[2];

  cmds[0] = 0x81;
  cmds[1] = value & 0x3F;
  return sendCommand(cmds, sizeof(cmds));
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::clear() {
  for (uint8_t y = 0; y < HEIGHT; y += 8) {
    if ((! setPointer(0, y)) || (! sendData((uint8_t)0, WIDTH)))
      return false;
  }
  return true;
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::fill(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color) {
  if ((x < WIDTH) && (y < HEIGHT)) {
    uint8_t pattern = 0;

    if (x + w > WIDTH)
      w = WIDTH - x;
    if (y + h > HEIGHT)
      h = HEIGHT - y;
    for (uint8_t _y = y; _y < y + h; _y = (_y + 8) & 0xF8) {
      if (color) {
        pattern = 0xFF;
        if (y + h - _y < 8)
          pattern >>= (8 - (y + h - _y));
        pattern <<= (_y & 0x07);
      }
      if ((! setPointer(x, _y)) || (! sendData(pattern, w)))
        return false;
    }
    return true;
  }
  return false;
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::drawPattern(uint8_t x, uint8_t y, uint8_t pattern, uint8_t repeat) {
  if ((x < WIDTH) && (y < HEIGHT)) {
    if (x + repeat > WIDTH)
      repeat = WIDTH - x;
    return setPointer(x, y) && sendData(pattern, repeat);
  }
  return false;
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::drawPattern(uint8_t x, uint8_t y, const uint8_t *pattern, uint8_t size) {
  if ((x < WIDTH) && (y < HEIGHT)) {
    if (x + size > WIDTH)
      size = WIDTH - x;
    return setPointer(x, y) && sendData(pattern, size);
  }
  return false;
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::drawPattern_P(uint8_t x, uint8_t y, const uint8_t *pattern, uint8_t size) {
  if ((x < WIDTH) && (y < HEIGHT)) {
    if (x + size > WIDTH)
      size = WIDTH - x;
    return setPointer(x, y) && sendData_P(pattern, size);
  }
  return false;
}

template<const uint8_t ADDR, TwoWire &WIRE>
inline bool ST7567S<ADDR, WIRE>::drawPixel(uint8_t x, uint8_t y, bool color) {
  return drawPattern(x, y, color << (y & 0x07));
}

template<const uint8_t ADDR, TwoWire &WIRE>
inline bool ST7567S<ADDR, WIRE>::drawHLine(uint8_t x, uint8_t y, uint8_t w, bool color) {
  return drawPattern(x, y, color << (y & 0x07), w);
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::drawVLine(uint8_t x, uint8_t y, uint8_t h, bool color) {
  if ((x < WIDTH) && (y < HEIGHT)) {
    uint8_t pattern = 0;

    if (y + h > HEIGHT)
      h = HEIGHT - y;
    for (uint8_t _y = y; _y < y + h; _y = (_y + 8) & 0xF8) {
      if (color) {
        pattern = 0xFF;
        if (y + h - _y < 8)
          pattern >>= (8 - (y + h - _y));
        pattern <<= (_y & 0x07);
      }
      if ((! setPointer(x, _y)) || (! sendData(pattern)))
        return false;
    }
    return true;
  }
  return false;
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::scrollUp(uint8_t pages) {
  _startPage = (_startPage + pages) & 0x07;
  return sendCommand(0x40 | (_startPage * 8)) && fill(0, HEIGHT - pages * 8, WIDTH, pages * 8, 0);
}

template<const uint8_t ADDR, TwoWire &WIRE>
inline uint8_t ST7567S<ADDR, WIRE>::charWidth() const {
  return FONT_WIDTH + FONT_GAP;
}

template<const uint8_t ADDR, TwoWire &WIRE>
inline uint8_t ST7567S<ADDR, WIRE>::charHeight() const {
  return FONT_HEIGHT;
}

template<const uint8_t ADDR, TwoWire &WIRE>
#ifdef ST7567S_USE_UTF8
bool ST7567S<ADDR, WIRE>::printChar(uint8_t x, uint8_t y, utf8_t c) {
#else
bool ST7567S<ADDR, WIRE>::printChar(uint8_t x, uint8_t y, char c) {
#endif
  const uint8_t *pattern = charPattern(c);

  if (pattern)
    return drawPattern_P(x, y, pattern, FONT_WIDTH) && drawPattern(x + FONT_WIDTH, y, (uint8_t)0, FONT_GAP);
  else
    return drawPattern(x, y, (uint8_t)0, FONT_WIDTH + FONT_GAP);
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::printStr(uint8_t x, uint8_t y, const char *str) {
  uint8_t b;

  b = *str++;
  while (b && (x < WIDTH)) {
#ifdef ST7567S_USE_UTF8
    utf8_t c = 0;

    if (b & 0x80) {
      if ((b & 0B11100000) == 0B11000000) {
        c = (uint16_t)(b & 0B00011111) << 6;
        b = *str++;
        if ((b & 0B11000000) == 0B10000000)
          c |= (b & 0B00111111);
        else
          return false;
      } else if ((b & 0B11110000) == 0B11100000) {
        c = (uint16_t)(b & 0B00001111) << 12;
        b = *str++;
        if ((b & 0B11000000) == 0B10000000) {
          c |= (uint16_t)(b & 0B00111111) << 6;
          b = *str++;
          if ((b & 0B11000000) == 0B10000000)
            c |= (b & 0B00111111);
          else
            return false;
        } else
          return false;
      }
    } else
      c = b;
    if (! printChar(x, y, c))
#else
    if (! printChar(x, y, b))
#endif
      return false;
    x += charWidth();
    b = *str++;
  }
  return true;
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::printStr_P(uint8_t x, uint8_t y, const char *str) {
  uint8_t b;

  b = pgm_read_byte(str++);
  while (b && (x < WIDTH)) {
#ifdef ST7567S_USE_UTF8
    utf8_t c = 0;

    if (b & 0x80) {
      if ((b & 0B11100000) == 0B11000000) {
        c = (uint16_t)(b & 0B00011111) << 6;
        b = pgm_read_byte(str++);
        if ((b & 0B11000000) == 0B10000000)
          c |= (b & 0B00111111);
        else
          return false;
      } else if ((b & 0B11110000) == 0B11100000) {
        c = (uint16_t)(b & 0B00001111) << 12;
        b = pgm_read_byte(str++);
        if ((b & 0B11000000) == 0B10000000) {
          c |= (uint16_t)(b & 0B00111111) << 6;
          b = pgm_read_byte(str++);
          if ((b & 0B11000000) == 0B10000000)
            c |= (b & 0B00111111);
          else
            return false;
        } else
          return false;
      }
    } else
      c = b;
    if (! printChar(x, y, c))
#else
    if (! printChar(x, y, b))
#endif
      return false;
    x += charWidth();
    b = pgm_read_byte(str++);
  }
  return true;
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::sendCommand(uint8_t cmd) {
  WIRE.beginTransmission(ADDR);
  WIRE.write(0x00);
  WIRE.write(cmd);
  return WIRE.endTransmission() == 0;
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::sendCommand(const uint8_t *cmds, uint8_t count) {
  WIRE.beginTransmission(ADDR);
  while (count--) {
    WIRE.write(count > 0 ? 0x80 : 0x00);
    WIRE.write(*cmds++);
  }
  return WIRE.endTransmission() == 0;
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::sendCommand_P(const uint8_t *cmds, uint8_t count) {
  WIRE.beginTransmission(ADDR);
  while (count--) {
    WIRE.write(count > 0 ? 0x80 : 0x00);
    WIRE.write(pgm_read_byte(cmds++));
  }
  return WIRE.endTransmission() == 0;
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::sendData(uint8_t data) {
  WIRE.beginTransmission(ADDR);
  WIRE.write(0x40);
  WIRE.write(data);
  return WIRE.endTransmission() == 0;
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::sendData(uint8_t data, uint8_t count) {
  WIRE.beginTransmission(ADDR);
  WIRE.write(0x40);
  while (count--) {
    if (! WIRE.write(data)) {
      if (WIRE.endTransmission() != 0)
        return false;
      WIRE.beginTransmission(ADDR);
      WIRE.write(0x40);
      WIRE.write(data);
    }
  }
  return WIRE.endTransmission() == 0;
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::sendData(const uint8_t *data, uint8_t count) {
  WIRE.beginTransmission(ADDR);
  WIRE.write(0x40);
  while (count--) {
    if (! WIRE.write(*data)) {
      if (WIRE.endTransmission() != 0)
        return false;
      WIRE.beginTransmission(ADDR);
      WIRE.write(0x40);
      WIRE.write(*data);
    }
    ++data;
  }
  return WIRE.endTransmission() == 0;
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::sendData_P(const uint8_t *data, uint8_t count) {
  WIRE.beginTransmission(ADDR);
  WIRE.write(0x40);
  while (count--) {
    if (! WIRE.write(pgm_read_byte(data))) {
      if (WIRE.endTransmission() != 0)
        return false;
      WIRE.beginTransmission(ADDR);
      WIRE.write(0x40);
      WIRE.write(pgm_read_byte(data));
    }
    ++data;
  }
  return WIRE.endTransmission() == 0;
}

template<const uint8_t ADDR, TwoWire &WIRE>
bool ST7567S<ADDR, WIRE>::setPointer(uint8_t x, uint8_t y) {
  uint8_t cmds[3];

  if (_flipped)
    x += FLIP_OFFSET;
  cmds[0] = 0x10 | (x >> 4);
  cmds[1] = x & 0x0F;
  cmds[2] = 0xB0 | ((y / 8 + _startPage) & 0x07);
  return sendCommand(cmds, sizeof(cmds));
}

template<const uint8_t ADDR, TwoWire &WIRE>
#ifdef ST7567S_USE_UTF8
const uint8_t *ST7567S<ADDR, WIRE>::charPattern(utf8_t c) {
  uint16_t offset;
  uint16_t from, to;
#else
const uint8_t *ST7567S<ADDR, WIRE>::charPattern(char c) {
  uint16_t offset;
  uint8_t from, to;
#endif

  offset = 0;
  from = pgm_read_byte(&FONT_5X8[offset++]);
#ifdef ST7567S_USE_UTF8
  from |= ((uint16_t)pgm_read_byte(&FONT_5X8[offset++]) << 8);
#endif
  to = pgm_read_byte(&FONT_5X8[offset++]);
#ifdef ST7567S_USE_UTF8
  to |= ((uint16_t)pgm_read_byte(&FONT_5X8[offset++]) << 8);
#endif
  while ((c < from) || (c > to)) {
    offset += ((to - from + 1) * (FONT_WIDTH * ((FONT_HEIGHT + 7) / 8)));
    if (offset >= sizeof(FONT_5X8))
      return nullptr;
    from = pgm_read_byte(&FONT_5X8[offset++]);
#ifdef ST7567S_USE_UTF8
    from |= ((uint16_t)pgm_read_byte(&FONT_5X8[offset++]) << 8);
#endif
    to = pgm_read_byte(&FONT_5X8[offset++]);
#ifdef ST7567S_USE_UTF8
    to |= ((uint16_t)pgm_read_byte(&FONT_5X8[offset++]) << 8);
#endif
  }
  return &FONT_5X8[offset + (c - from) * (FONT_WIDTH * ((FONT_HEIGHT + 7) / 8))];
}
