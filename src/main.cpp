#include <Arduino.h>
#include "ST7567S.h"
#include "Console.h"
#include <EncButton.h>

Console<ST7567S<0x3F>> lcd;
EncButton<EB_TICK, 4,3,2> enc;  // энкодер с кнопкой
//EncButton2<EB_ENC> enc(INPUT, 2, 3);        // просто энкодер
//EncButton2<EB_BTN> enc(INPUT, 4);           // просто кнопка
// для изменения направления энкодера поменяй A и B при инициализации

// по умолчанию пины настроены в INPUT_PULLUP
// Если используется внешняя подтяжка - лучше перевести в INPUT
//EncButton<EB_TICK, 2, 3, 4> enc(INPUT);

void setup() {
  Serial.begin(115200);
  Serial.println();

  lcd.init();
  if (! lcd.begin()) {
    Serial.println(F("ST7567S display not found!"));
    Serial.flush();
    ESP.deepSleep(0);
  }
//  lcd.flip(true);
  lcd.drawHLine(0, 0, lcd.width(), 1);
  lcd.drawHLine(0, lcd.height() - 1, lcd.width(), 1);
  lcd.drawVLine(0, 0, lcd.height(), 1);
  lcd.drawVLine(lcd.width() - 1, 0, lcd.height(), 1);
  delay(1000);
  lcd.clear();
  lcd.println();
  lcd.println();
  lcd.println(F("*********************"));
  lcd.println(F("*Зарядное устройство*"));
  lcd.println(F("*   Интеллект v1.0  *"));
  lcd.println(F("*********************"));
  for (uint8_t i = 0; i < 4; ++i) {
    vTaskDelay(500/portTICK_PERIOD_MS);
    lcd.inverse((i & 0x01) == 0);
  }
  lcd.clear();
 /* lcd.print(F("\fКакой-то там непонятный процесс "));
   for (uint8_t i = 0; i < 20; ++i) {
      static const char PROGRESS[] PROGMEM = { '-', '\\', '|', '/' };

    lcd.print((char)pgm_read_byte(&PROGRESS[i % 4]));
    lcd.print('\b');
    delay(100);
  }
  lcd.println(F("OK"));
  lcd.println(F("А теперь спать..."));
  for (int8_t contrast = 29; contrast >= 0; contrast -= 2) {
    delay(100);
    lcd.contrast(contrast);
  }
 // lcd.powerSave(true);
 // ESP.deepSleep(0);
*/ 
}

void loop() {
    enc.tick();                       // опрос происходит здесь

  // =============== ЭНКОДЕР ===============
  // обычный поворот
  if (enc.turn()) {
    lcd.println("turn");


    // можно опросить ещё:
    //Serial.println(enc.counter);  // вывести счётчик
    //Serial.println(enc.fast());   // проверить быстрый поворот
    lcd.println(enc.dir()); // направление поворота

  }

  // "нажатый поворот"
  if (enc.turnH()) {
    lcd.println("hold + turn");

    // можно опросить ещё:
    //Serial.println(enc.counter);  // вывести счётчик
    //Serial.println(enc.fast());   // проверить быстрый поворот
    //Serial.println(enc.dir()); // направление поворота
    lcd.println(enc.dir());
  }

  if (enc.left()){ lcd.println("left");}     // поворот налево
  if (enc.right()){ lcd.println("right");}  // поворот направо
  if (enc.leftH()){ lcd.println("leftH");} // нажатый поворот налево
  if (enc.rightH()){ lcd.println("rightH");}// нажатый поворот направо

  // =============== КНОПКА ===============
  if (enc.press()) {lcd.println("press");}
  if (enc.click()) {lcd.println("click");}
  if (enc.release()) {lcd.println("release");}

  if (enc.held()) lcd.println("held");     // однократно вернёт true при удержании
  //if (enc.hold()) Serial.println("hold");   // будет постоянно возвращать true после удержания
  if (enc.step()) lcd.println("step");     // импульсное удержание

  // проверка на количество кликов
  if (enc.hasClicks(1)) lcd.println("action 1 clicks");
  if (enc.hasClicks(2)) lcd.println("action 2 clicks");
  if (enc.hasClicks(3)) lcd.println("action 3 clicks");
  if (enc.hasClicks(5)) lcd.println("action 5 clicks");

  // вывести количество кликов
  if (enc.hasClicks()) {
    lcd.println("has clicks ");
    lcd.println(enc.clicks);
  }
}
