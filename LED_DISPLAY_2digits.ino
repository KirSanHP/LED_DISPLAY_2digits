//Скетч должен выводить две цифры, сменяемые по нажатию кнопок в порядке возрастания или убывания
//методом записи байта в порт D
// добавлено время игнорирования кнопки
// добавлено долгое нажатие с приращением или убавлением числа пока кнопка удерживается
// если нажаты обе кнопки одновременно, устанавливаются нули

//скетч работает нестабильно, как-то по настроению, сохранён ради интереса


#define DOT_TIME 300        //интервал мигания точки
#define BTN_IGNORE_TIME 100 //время игнорирования кнопки на время дребезга
#define SPEED1TIME 500      //время до начала прибавления числа при нажатой кнопке
#define SPEED2TIME 1500     //время до перехода на повышенную скорость изменения числа при нажатой кнопке
#define SPEED1MILLIS 200    //миллисекунды первой скорости
#define SPEED2MILLIS 100    //миллисекунды второй скорости

#define ZERO  B01000000
#define ONE   B01111001
#define TWO   B00100100
#define THREE B00110000
#define FOUR  B00011001
#define FIVE  B00010010
#define SIX   B00000010
#define SEVEN B01111000
#define EIGHT B00000000
#define NINE  B00010000
#define NONE  B11111111

#define ANODE1 8
#define ANODE2 9

#define BTN1_PIN 14 //A0
#define BTN2_PIN 15 //A1

bool dot1 = 0;
bool dot2 = 0;
unsigned long millisPrev = 0;
unsigned long switchTime = 0;
unsigned long dotPrevTime = 0;

struct Num {
  byte digit2;
  byte digit1;
  char change;
};

struct Btn {
  bool currentState;
  bool prevState;
  unsigned long prevTime;
  unsigned long pressTime;
};

Btn btn1 = {0, 0, 0, 0};
Btn btn2 = {0, 0, 0, 0};
Num num = {3, 6, 0};

void setup() {
  pinMode (ANODE1, OUTPUT);
  pinMode (ANODE2, OUTPUT);
  digitalWrite (ANODE1, HIGH);
  digitalWrite (ANODE2, HIGH);
  DDRD = B11111111;
  PORTD = EIGHT ^ (~dot1 << 7);
  delay (1000);
  PORTD = NONE;
  delay (1000);
  millisPrev = millis();
  pinMode (BTN1_PIN, INPUT_PULLUP);
  pinMode (BTN2_PIN, INPUT_PULLUP);
  digitalWrite (ANODE1, HIGH);
}

void loop() {
  btn1 = checkButton (BTN1_PIN, btn1);
  btn2 = checkButton (BTN2_PIN, btn2);

  if (btn1.currentState && btn2.currentState && (!btn1.prevState || !btn2.prevState) ) {
    num = {0, 0, 0};
    btn1.prevState = btn1.currentState;
    btn2.prevState = btn2.currentState;
  }
  else if (btn1.currentState && !btn1.prevState && !btn2.currentState) {
    num.change = 1;
    num = numCalc (num);
    btn1.prevState = btn1.currentState;
  }
  else if (btn2.currentState && !btn2.prevState && !btn1.currentState) {
    num.change = -1;
    num = numCalc (num);
  }

  if (millis() >= dotPrevTime + DOT_TIME) {
    dot1 = !dot1;
    dotPrevTime += DOT_TIME;
  }

  if (millis() >= switchTime + 5) {
    if (digitalRead (ANODE1) ) {
      digitalWrite (ANODE1, LOW);
      digitalWrite (ANODE2, HIGH);
      digitOut (num.digit2, dot2);
    }
    else {
      digitalWrite (ANODE1, HIGH);
      digitalWrite (ANODE2, LOW);
      digitOut (num.digit1, dot1);
    }
    switchTime += 5;
  }
delay (200);
}

Btn checkButton (byte _btnPin, Btn _btn) {
  bool _btnState = !digitalRead (_btnPin);
  unsigned long currentMillis = millis();
  if ( _btnState && !_btn.prevState && ( currentMillis >= _btn.prevTime + BTN_IGNORE_TIME) )
    return {1, _btn.prevState, currentMillis, 0};
  else if (!_btnState && _btn.prevState && ( currentMillis >= _btn.prevTime + BTN_IGNORE_TIME) ) {
    return {0, _btn.prevState, _btn.prevTime, (currentMillis - _btn.prevTime) };
  }
}

Num numCalc(Num _num) {
  _num.digit1 += _num.change;
  if (_num.digit1 == 10) {
    _num.digit1 = 0;
    _num.digit2++;
    if (_num.digit2 == 10)
      _num.digit2 = 0;
  }
  else if (num.digit1 == 255) {
    _num.digit1 = 9;
    _num.digit2 --;
    if (_num.digit2 == 255)
      _num.digit2 = 9;
  }
  _num.change = 0;
  return {_num.digit2, _num.digit1, _num.change};
}

void digitOut (byte _digit, bool _dot) {
  switch (_digit) {
    case 0:
      PORTD = ZERO ^ (~_dot << 7);
      break;
    case 1:
      PORTD = ONE ^ (~_dot << 7);
      break;
    case 2:
      PORTD = TWO ^ (~_dot << 7);
      break;
    case 3:
      PORTD = THREE ^ (~_dot << 7);
      break;
    case 4:
      PORTD = FOUR ^ (~_dot << 7);
      break;
    case 5:
      PORTD = FIVE ^ (~_dot << 7);
      break;
    case 6:
      PORTD = SIX ^ (~_dot << 7);
      break;
    case 7:
      PORTD = SEVEN ^ (~_dot << 7);
      break;
    case 8:
      PORTD = EIGHT ^ (~_dot << 7);
      break;
    case 9:
      PORTD = NINE ^ (~_dot << 7);
      break;
  }
}

