// Моторы и скорости
int LF = 10; 			 // при сигнале на контакте, левое колесо крутится вперед (Left forward)
int RF = 6; 			 // при сигнале на контакте, правое колесо крутится вперед (Right forward)
int speedLeft = 175; 		 // скорость левого мотора
int speedRight= 150;            // скорость правого мотора

void setup() {
}

void loop() {
  analogWrite(LF, speedLeft);    // включаем левый мотор
  analogWrite(RF, speedRight);   // включаем правый мотор
}
