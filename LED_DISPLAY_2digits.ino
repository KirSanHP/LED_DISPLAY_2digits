//Скетч должен выводить две цифры, сменяемые по нажатию кнопок в порядке возрастания
//методом записи байта в порт D

#define DOT_TIME 300

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

bool btn1Prev = 0;
bool btn2Prev = 0;
byte digit1 = 0;
byte digit2 = 0;
bool dot1 = 0;
bool dot2 = 0;
unsigned long millisPrev = 0;
unsigned long switchTime = 0;
unsigned long dotPrevTime = 0;

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
//  pinMode (DOT_PIN, OUTPUT);
  digitalWrite (ANODE1, HIGH);
}

int main() {
  bool btn1 = !digitalRead (BTN1_PIN);
  bool btn2 = !digitalRead (BTN2_PIN);
  if (btn1 == 1 && btn1Prev == 0) {
    digit1++;
    if (digit1 == 10) {
      digit1 = 0;
      digit2++;
      if (digit2 == 10)
        digit2 = 0;
    }
  }

  if (btn2 == 1 && btn2Prev == 0) {
    digit1--;
    if (digit1 == 255) {
      digit1 = 9;
      digit2--;
      if (digit2 == 255)
        digit2 = 9;
    }
  }
  btn1Prev = btn1;
  btn2Prev = btn2;

  if (millis() >= dotPrevTime + DOT_TIME) {
    dot1 = !dot1;
    dotPrevTime += DOT_TIME;
  }

  if (millis() >= switchTime + 5) {
    if (digitalRead (ANODE1) ) {
      digitalWrite (ANODE1, LOW);
      digitalWrite (ANODE2, HIGH);
      digitOut (digit2, dot2);
    }
    else {
      digitalWrite (ANODE1, HIGH);
      digitalWrite (ANODE2, LOW);
      digitOut (digit1, dot1);
    }
    switchTime += 5;
  }
  return 0;
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

