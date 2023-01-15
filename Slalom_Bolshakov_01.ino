// переменная для слалома, значения: 1 - объехать препятствие справа, 2 - объехать препятствие слева
int slalom = 1;

// ультразвуковой датчик - используется библиотека HC-SR04 by Martin Sosic
#include <HCSR04.h>
UltraSonicDistanceSensor distanceSensor(11, 12);  // Датчик расстояния на пинах 12 и 13. Если не работает (не выводит в Монитор порта) - поменять 11 и 12 местами
float distance;      // переменная для хранения значения расстояния с датчика

// датчики линии - 5 штук. Взято из кода движения по линии Line_5_sensors.ide. Все скорости надо подбирать экспериментально,
// особенно если левый и правый моторы крутятся с разной скоростью
int LF = 10;             // при сигнале на контакте, левое колесо крутится вперед (Left forward)
int LB = 9;             // при сигнале на контакте, левое колесо крутится вперед (Left backward)
int RF = 6;             // при сигнале на контакте, правое колесо крутится вперед (Right forward)
int RB = 5;             // при сигнале на контакте, правое колесо крутится назад (Right backward)

int speed_left = 150;                 // скорость левого мотора базовая
int speed_right = 150;                 // скорость правого мотора базовая

int slow_left = 20;                   // скорость медленного поворота для левого мотора
int slow_right = 20;                  // скорость медленного поворота для правого мотора

int brake_left = 0;                   // скорость быстрого поворота для левого мотора
int brake_right = 0;                  // скорость быстрого поворота для правого мотора

int back_left = 40;                   // скорость медленного разворота для левого мотора
int back_right = 10;                  // скорость медленного разворота для правого мотора

int fast_back_left = 50;              // скорость быстрого разворота для левого мотора
int fast_back_right = 50;             // скорость быстрого разворота для правого мотора

// Датчики линии
int left_pin = 2;                // пин, к которому подключен датчик линии левый
int right_pin = 3;               // пин, к которому подключен датчик линии правый
int center_pin = 4;             // пин, к которому подключен датчик линии центральный
int left_left_pin = 7;          // пин, к которому подключен датчик линии крайний левый
int right_right_pin = 8;        // пин, к которому подключен датчик линии крайний правый

int left = 0;                    // переменная для хранения состояния датчика линии левого (ближнего к центральному)
int right = 0;                   // переменная для хранения состояния датчика линии правого (ближнего к центральному)
int center = 0;                  // переменная для хранения состояния датчика линии центрального
int left_left = 0;               // переменная для хранения состояния датчика линии крайнего левого
int right_right = 0;             // переменная для хранения состояния датчика линии крайнего правого

// Для облегчения написания сравнений пропишем статусы датчиков
// на черной линии используемые датчики выдают высокий уровень
// на белом полотне - низкий
#define BLACK HIGH
#define WHITE LOW

void setup() {
  Serial.begin(9600);   // запускаем связь с компьютером, Монитор порта
}


// основной код, вызывающий остальные функции - см. теорию о них https://arduino.ru/Reference/FunctionDeclaration
void loop() {
  measureLine();        // измеряем расстояние, записываем в distance и выводим в Монитор порта
  measureDistance();    // опрашиваем датчики линии

  if (distance > 20) {
    lineFollow();       // двигаемся по линии на 5 датчиках, см. теорию на странице https://cxem.net/uprav/uprav40.php
  }
  else {
    obyezd();         // если впереди препятствие - выполняем маневр объезда
  }
  delay (10);       // пауза, чтобы дать роботу проехать некоторое время (можно поэкспериментировать со значением 1...10 или вообще убрать)
}


// функция опроса датчиков линии (5 датчиков)
void measureLine() {
  left = digitalRead(left_pin);                   // читаем значение с датчика левого
  right = digitalRead(right_pin);                 // читаем значение с датчика правого
  center = digitalRead(center_pin);               // читаем значение с датчика центрального
  left_left = digitalRead(left_left_pin);         // читаем значение с датчика самого левого
  right_right = digitalRead(right_right_pin);     // читаем значение с датчика самого правого
}


void lineFollow () {
  // Линия по центру: левый и правый датчики на белом (WHITE) и центральный - на черном (BLACK)
  // ехать прямо
  if (left_left == WHITE && left == WHITE && center == BLACK && right == WHITE && right_right == WHITE) {
    analogWrite (LF, speed_left);
    analogWrite (LB, 0);
    analogWrite (RF, speed_right);
    analogWrite (RB, 0);
  }

  // Линия чуть слева: левый - черный, центральный - черный, правый - белый)
  // повернуть слегка влево
  if (left_left == WHITE && left == BLACK && center == BLACK && right == WHITE && right_right == WHITE) {
    analogWrite (LF, slow_left);
    analogWrite (LB, 0);
    analogWrite (RF, speed_right);
    analogWrite (RB, 0);
  }

  // Линия чуть справа: левый - белый, центральный - черный, правый - черный)
  // повернуть слегка вправо
  if (left_left == WHITE && left == WHITE && center == BLACK && right == BLACK && right_right == WHITE) {
    analogWrite (LF, speed_left);
    analogWrite (LB, 0);
    analogWrite (RF, slow_right);
    analogWrite (RB, 0);
  }

  // Линия сильно слева: левый - черный, центральный - белый, правый - белый)
  // повернуть сильно влево
  if (left_left == WHITE && left == BLACK && center == WHITE && right == WHITE  && right_right == WHITE) {
    analogWrite (LF, brake_left);
    analogWrite (LB, 0);
    analogWrite (RF, speed_right);
    analogWrite (RB, 0);
  }

  // Линия сильно справа: левый - белый, центральный - белый, правый - черный)
  // повернуть сильно вправо
  if (left_left == WHITE && left == WHITE && center == WHITE && right == BLACK && right_right == WHITE) {
    analogWrite (LF, speed_left);
    analogWrite (LB, 0);
    analogWrite (RF, brake_right);
    analogWrite (RB, 0);
  }


  // Линия очень сильно слева: самый левый - черный, левый - черный, центральный - белый, правый - белый, самый правый - белый)
  // развернуться слабо влево
  if (left_left == BLACK && left == BLACK && center == WHITE && right == WHITE  && right_right == WHITE) {
    analogWrite (LF, 0);
    analogWrite (LB, back_left);
    analogWrite (RF, speed_right);
    analogWrite (RB, 0);
  }

  // Линия очень сильно справа: самый левый - белый,левый - белый, центральный - белый, правый - черный, самый правый - черный)
  // повернуть сильно вправо
  if (left_left == WHITE && left == WHITE && center == WHITE && right == BLACK && right_right == BLACK) {
    analogWrite (LF, speed_left);
    analogWrite (LB, 0);
    analogWrite (RF, 0);
    analogWrite (RB, back_right);
  }

  // Сход с линии влево: самый левый - черный, остальные белые
  // развернуться сильно влево
  if (left_left == BLACK && left == WHITE && center == WHITE && right == WHITE  && right_right == WHITE) {
    analogWrite (LF, 0);
    analogWrite (LB, fast_back_left);
    analogWrite (RF, speed_right);
    analogWrite (RB, 0);
  }

  // Сход с линии вправо: самый правый - черный, остальные белые
  // развернуться сильно вправо
  if (left_left == WHITE && left == WHITE && center == WHITE && right == WHITE && right_right == BLACK) {
    analogWrite (LF, speed_left);
    analogWrite (LB, 0);
    analogWrite (RF, 0);
    analogWrite (RB, fast_back_right);
  }
  delay (10);       // пауза, чтобы дать роботу проехать некоторое время
}


// функция измерения расстояния и вывод в Монитор порта
void measureDistance() {
  distance = distanceSensor.measureDistanceCm();      // измеряем расстояние в см и записываем его в переменную distance
  Serial.println(distanceSensor.measureDistanceCm());   // чтобы можно было убедиться, что датчик работает - выводим значение в Монитор порта
}


// функция объезда препятствия по правилам слалома: первое справа, второе слева, третье справа и т.д.
void obyezd() {
  if (slalom == 1) {
    obyezd_right();
    slalom = 2;   // следующее препятствие объезжать слева
  }
  else {
    obyezd_left();
    slalom = 1;   // следующее препятствие объезжать справа
  }
}

// объезд препятствия справа. Все delay и некоторые скорости необходимо подстроить экспериментально
void obyezd_right() {
  // развернуться на месте направо
  analogWrite (LF, speed_left);
  analogWrite (LB, 0);
  analogWrite (RF, 0);
  analogWrite (RB, speed_right);
  delay (500);

  // поехать полукруг вокруг препяствия справа
  analogWrite (LF, speed_left - 50);
  analogWrite (LB, 0);
  analogWrite (RF, speed_right);
  analogWrite (RB, 0);
  delay (1000);

  // развернуться на месте направо, чтобы вернуться на линию и поехать по ней дальше
  analogWrite (LF, speed_left);
  analogWrite (LB, 0);
  analogWrite (RF, 0);
  analogWrite (RB, speed_right);
  delay (500);
}

// объезд препятствия справа. Все delay и некоторые скорости необходимо подстроить экспериментально
void obyezd_left() {
  // развернуться на месте налево
  analogWrite (LF, 0);
  analogWrite (LB, speed_left);
  analogWrite (RF, speed_right);
  analogWrite (RB, 0);
  delay (500);

  // поехать полукруг вокруг препяствия слева
  analogWrite (LF, speed_left);
  analogWrite (LB, 0);
  analogWrite (RF, speed_right - 50);
  analogWrite (RB, 0);
  delay (1000);

  // развернуться на месте слева, чтобы вернуться на линию и поехать по ней дальше
  analogWrite (LF, 0);
  analogWrite (LB, speed_left);
  analogWrite (RF, speed_right);
  analogWrite (RB, 0);
  delay (500);
}
