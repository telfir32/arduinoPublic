// ультразвуковой датчик - используется библиотека HC-SR04 by Martin Sosic
#include <HCSR04.h>
UltraSonicDistanceSensor distanceSensor(11, 12);  // Датчик расстояния на пинах 12 и 13. Если не работает (не выводит в Монитор порта) - поменять 11 и 12 местами
float distance;      // переменная для хранения значения расстояния с датчика


void setup() {
  Serial.begin(9600);   // запускаем связь с компьютером, Монитор порта
}

void loop() {
  measureLine();
  measureDistance();
  
}

// функция опроса датчиков линии (5 датчиков)
void measureLine(){
  
}


// функция измерения расстояния и вывод в Монитор порта
void measureDistance(){
  distance = distanceSensor.measureDistanceCm();      // измеряем расстояние в см и записываем его в переменную distance
  Serial.println(distanceSensor.measureDistanceCm());   // чтобы можно было убедиться, что датчик работает - выводим значение в Монитор порта
}



void obyezd (){
  
}
