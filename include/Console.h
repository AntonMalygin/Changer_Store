#pragma once

#include <Print.h>

template<class LCD>
class Console : public LCD, public Print {
public:
#ifdef ST7567S_USE_UTF8
  Console() : _wcharExtra(0), _col(0), _row(0) {}
#else
  Console() : _col(0), _row(0) {}
#endif

  bool begin();

  uint8_t columns() const;
  uint8_t rows() const;
  uint8_t col() const;
  uint8_t row() const;
  void position(uint8_t c, uint8_t r);
  void home();
  bool clear();

  size_t write(uint8_t c) override;
  using Print::write;

protected:
  static const uint8_t TAB_WIDTH = 4; // 8

#ifdef ST7567S_USE_UTF8
  bool writeChar(uint8_t c);
#else
  bool writeChar(char c);
#endif
  void preparePosition();

#ifdef ST7567S_USE_UTF8
  utf8_t _wchar;
  uint8_t _wcharExtra : 2;
#endif
  uint8_t _col : 5;
  uint8_t _row : 4;
};

template<class LCD>
bool Console<LCD>::begin() {
  return LCD::begin() && clear();
}

template<class LCD>
inline uint8_t Console<LCD>::columns() const {
  return this->width() / this->charWidth();
}

template<class LCD>
inline uint8_t Console<LCD>::rows() const {
  return this->height() / this->charHeight();
}

template<class LCD>
inline uint8_t Console<LCD>::col() const {
  return _col;
}

template<class LCD>
inline uint8_t Console<LCD>::row() const {
  return _row;
}

template<class LCD>
void Console<LCD>::position(uint8_t c, uint8_t r) {
  if ((c < columns()) && (r < rows())) {
    _col = c;
    _row = r;
  }
}

template<class LCD>
inline void Console<LCD>::home() {
  _col = _row = 0;
}

template<class LCD>
bool Console<LCD>::clear() {
  _col = _row = 0;
  return LCD::clear();
}

template<class LCD>
size_t Console<LCD>::write(uint8_t c) {
  if (writeChar(c))
    return sizeof(c);
  return 0;
}

template<class LCD>
#ifdef ST7567S_USE_UTF8
bool Console<LCD>::writeChar(uint8_t c) {
#else
bool Console<LCD>::writeChar(char c) {
#endif
  bool result = true;

  if (c == '\b') { // Backspace
    if (_col)
      --_col;
  } else if (c == '\t') { // Tab
    uint8_t spaces;

    preparePosition();
    spaces = ((_col + TAB_WIDTH) / TAB_WIDTH) * TAB_WIDTH - _col;
    while ((_col < columns()) && spaces--) {
      result = this->printChar(this->charWidth() * _col, this->charHeight() * _row, ' ');
      if (! result)
        break;
      ++_col;
    }
  } else if (c == '\f') { // Form feed (clear)
    result = clear();
  } else if (c == '\r') { // Carriage return
    _col = 0;
  } else if (c == '\n') { // New line
    if (_row < rows())
      ++_row;
    else // Scroll screen
      result = this->scrollUp(this->charHeight() / 8);
  } else if (c >= ' ') { // Ordinary char
    preparePosition();
#ifdef ST7567S_USE_UTF8
    if (c & 0B10000000) { // UTF8 code
      if (! _wcharExtra) { // First byte
        if ((c & 0B11100000) == 0B11000000) { // 2 bytes code
          _wchar = c & 0B00011111;
          _wcharExtra = 1;
        } else if ((c & 0B11110000) == 0B11100000) { // 3 bytes code
          _wchar = c & 0B00001111;
          _wcharExtra = 2;
        } else { // Error
          _wchar = ' '; // Error
          result = false;
        }
      } else { // Next byte
        if ((c & 0B11000000) == 0B10000000) {
          _wchar <<= 6;
          _wchar |= (c & 0B00111111);
          if (! --_wcharExtra) { // Last byte
            result = this->printChar(this->charWidth() * _col, this->charHeight() * _row, _wchar);
            ++_col;
          }
        } else {
          _wchar = ' '; // Error
          _wcharExtra = 0;
          result = false;
        }
      }
    } else { // ACSII char
#endif
      result = this->printChar(this->charWidth() * _col, this->charHeight() * _row, c);
      ++_col;
#ifdef ST7567S_USE_UTF8
    }
#endif
  }
  return result;
}

template<class LCD>
void Console<LCD>::preparePosition() {
  if (_col >= columns()) {
    _col = 0;
    ++_row;
  }
  if (_row >= rows()) {
    this->scrollUp(this->charHeight() / 8);
    _row = rows() - 1;
  }
}
