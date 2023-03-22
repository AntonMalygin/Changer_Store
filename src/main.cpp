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

#define ITEMS 24               // Общее кол во пунктов (больше 8 - нужно несколько страниц)
uint8_t data[ITEMS];
bool flag = true;

// Битмап с картинкой стрелочки (Если нужен)
const uint8_t ptr_bmp[] PROGMEM = {
  0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0xFF, 0xFF, 0x7E, 0x3C, 0x18,
};

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
   static int8_t pointer = 0; // Переменная указатель
    enc.tick();                       // опрос происходит здесь

  // =============== ЭНКОДЕР ===============
  // обычный поворот
  //  if (enc.turn()) {
  //  lcd.println("turn");
    // можно опросить ещё:
    //Serial.println(enc.counter);  // вывести счётчик
    //Serial.println(enc.fast());   // проверить быстрый поворот
  //  lcd.println(enc.dir()); // направление поворота
  //}

  // "нажатый поворот"
  //  if (enc.turnH()) {
  //  lcd.println("hold + turn");
    // можно опросить ещё:
    //Serial.println(enc.counter);  // вывести счётчик
    //Serial.println(enc.fast());   // проверить быстрый поворот
    //Serial.println(enc.dir()); // направление поворота
  //  lcd.println(enc.dir());
  //  }

  // if (enc.left()){ lcd.println("left");}     // поворот налево
  // if (enc.right()){ lcd.println("right");}  // поворот направо
  // if (enc.leftH()){ lcd.println("leftH");} // нажатый поворот налево
  // if (enc.rightH()){ lcd.println("rightH");}// нажатый поворот направо

  // =============== КНОПКА ===============
  // if (enc.press()) {lcd.println("press");}
  // if (enc.click()) {lcd.println("click");}
  // if (enc.release()) {lcd.println("release");}

  // if (enc.held()) lcd.println("held");     // однократно вернёт true при удержании
  // if (enc.hold()) Serial.println("hold");   // будет постоянно возвращать true после удержания
  // if (enc.step()) lcd.println("step");     // импульсное удержание

  // проверка на количество кликов
  // if (enc.hasClicks(1)) lcd.println("action 1 clicks");
  // if (enc.hasClicks(2)) lcd.println("action 2 clicks");
  // if (enc.hasClicks(3)) lcd.println("action 3 clicks");
  // if (enc.hasClicks(5)) lcd.println("action 5 clicks");

  // вывести количество кликов
  //if (enc.hasClicks()) {
  //  lcd.println("has clicks ");
  //  lcd.println(enc.clicks);
  // }

if (enc.left() or enc.leftH())  
  {                                           // Если энкодер повернули влево или нажатым повернули влево
    pointer = constrain(pointer-1,0,ITEMS-1); // Двигаем указатель в пределах дисплея
  }

if (enc.right() or enc.rightH())
  {
    pointer = constrain(pointer+1,0,ITEMS-1);
  }

if (enc.isClick()) {   // Нажатие на Энкодер" - переход в пункт меню
    switch (pointer) {  // По номеру указателей располагаем вложенные функции (можно вложенные меню)
      case 0: func(); break;  // По нажатию на ОК при наведении на 0й пункт вызвать функцию
      case 1: break;
      case 2: break;
      case 3: break;
      case 4: break;
      case 5: break;
      case 6: break;
      case 7: break;
        // И все остальные
    }
  }

/* меню */
  lcd.clear();           // Очищаем буфер
  lcd.home();            // Курсор в левый верхний угол

  if (pointer < 8) {      // Первая страница
    lcd.print            // Вывод всех пунктов
    (F(
       "  Parameter 0:\n" // Не забываем про '\n' - символ переноса строки
       "  Parameter 1:\n"
       "  Parameter 2:\n"
       "  Parameter 3:\n"
       "  Parameter 4:\n"
       "  Parameter 5:\n"
       "  Parameter 6:\n"
       "  Parameter 7:\n"
     ));
  } else if (pointer < 16) {// Вторая страница
    lcd.print
    (F(
       "  Parameter 8:\n"
       "  Parameter 9:\n"
       "  Parameter 10:\n"
       "  Parameter 11:\n"
       "  Parameter 12:\n"
       "  Parameter 13:\n"
       "  Parameter 14:\n"
       "  Parameter 15:\n"
     ));
  } else {                  // Последняя страница
    lcd.print
    (F(
       "  Parameter 16:\n"
       "  Parameter 17:\n"
       "  Parameter 18:\n"
       "  Parameter 19:\n"
       "  Parameter 20:\n"
       "  Parameter 21:\n"
       "  Parameter 22:\n"
       "  Parameter 23:\n"
     ));
  }

  printPointer(pointer);  // Вывод указателя

}
void printPointer(uint8_t pointer) {
  // Символьный указатель - лучший выбор в многостраничном меню
  // Указатель в начале строки
  lcd.position(0, pointer);
  lcd.print(">");
  // Можно еще в конце
  /*oled.setCursor(20, pointer);
    oled.print("<");*/
  // Указатель в виде прямоугольной рамки со скругленными углами
  /*oled.roundRect(0, pointer * 8, 127, (pointer * 8) + 6);*/
  // Указатель в виде битмапа (картинки)
  /*oled.drawBitmap(0, pointer * 8, ptr_bmp, 10, 8);*/
}
/* пример вложеной функции, которую можно вызвать из под меню */
void func(void) {
  lcd.clear();
  lcd.home();
  lcd.print(F("Press OK to return"));
  
  while (1) {
    enc.tick();
    if (enc.isClick()) return; // return возвращает нас в предыдущее меню
  }
}

