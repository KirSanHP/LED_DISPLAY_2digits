//Скетч должен выводить цифры, сменяемые по нажатию кнопки в порядке возрастания
//методом записи байта в порт D

#define BLINK_TIME 300

// два младших бита поменяны местами из-за несоответствия нумерации битов порта и выходов платы (??)
#define ZERO  B00111111
#define ONE   B00000101
#define TWO   B01011011
#define THREE B01001111
#define FOUR  B01100101
#define FIVE  B01101110
#define SIX   B01111110
#define SEVEN B00000111
#define EIGHT B01111111
#define NINE  B01101111
#define NONE  B00000000

#define BTN_PIN 14 //A0
//#define DOT_PIN 8
#define ANOD 8

bool btnPrev = 0;
byte digit = 0;
unsigned long millisPrev = 0;

void setup() {
  DDRD = B11111111;           // == pinMode (0-7, OUTPUT);
  PORTD = EIGHT;              // == digitOut (8);
  delay (1000);
  PORTD = NONE;               // выключить все сегменты
  delay (1000);
  millisPrev = millis();
  pinMode (BTN_PIN, INPUT_PULLUP);
//  pinMode (DOT_PIN, OUTPUT);
  PORTD = ZERO;               // == digitOut (0);
}

void loop() {
  bool btn = !digitalRead (BTN_PIN);
  if (btn == 1 && btnPrev == 0) {
    delay (30);
    digit++;
    if (digit == 10)
      digit = 0;
    digitOut (digit);
  }
  btnPrev = btn;

  if (millis() >= millisPrev + BLINK_TIME) {
    PORTD = PIND ^ (1 << 7);                  // инвертировать бит, отвечающий за точку и сразу отправить на выход
    millisPrev += BLINK_TIME;
  }
}

void digitOut (byte _digit) { 
  switch (_digit) {
    case 0:
      PORTD = (PIND & B10000000) | ZERO;
      break;
    case 1:
      PORTD = (PIND & B10000000) | ONE;
      break;
    case 2:
      PORTD = (PIND & B10000000) | TWO;
      break;
    case 3:
      PORTD = (PIND & B10000000) | THREE;
      break;
    case 4:
      PORTD = (PIND & B10000000) | FOUR;
      break;
    case 5:
      PORTD = (PIND & B10000000) | FIVE;
      break;
    case 6:
      PORTD = (PIND & B10000000) | SIX;
      break;
    case 7:
      PORTD = (PIND & B10000000) | SEVEN;
      break;
    case 8:
      PORTD = (PIND & B10000000) | EIGHT;
      break;
    case 9:
      PORTD = (PIND & B10000000) | NINE;
      break;
  }
}

