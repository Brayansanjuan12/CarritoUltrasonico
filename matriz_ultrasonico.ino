const int pines[] = {33, 25, 26, 27, 14, 12};  // IN1, IN2, IN3, IN4, enableA, enableB
const int trigPin = 21;  // Pin TRIG del sensor ultrasónico
const int echoPin = 18;  // Pin ECHO del sensor ultrasónico
const int distanciaSegura = 30;  // Distancia en cm para reducir la velocidad y girar
const int distanciaMinima = 25;  // Distancia en cm para detenerse y girar

const int movimientos[][6] = {
  {1, 0, 0, 1, 200, 200},  // Adelante
  {0, 1, 1, 0, 200, 200},  // Atrás
  {0, 0, 1, 0, 0, 200},    // Izquierda
  {1, 0, 0, 0, 200, 0},    // Derecha
  {1, 0, 0, 1, 100, 255},  // Esquinado Izquierda
  {0, 1, 1, 0, 255, 100},  // Esquinado Derecha
  {0, 0, 0, 0, 0, 0}       // Parar
};

void setup() {
  for (int i = 0; i < 6; i++) pinMode(pines[i], OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);  // Iniciar la comunicación serial para depuración
}

void moverMotores(const int mov[]) {
  for (int i = 0; i < 4; i++) digitalWrite(pines[i], mov[i]);
  analogWrite(pines[4], mov[4]);
  analogWrite(pines[5], mov[5]);
}

// Función para medir la distancia con el sensor ultrasónico
long medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH);
  long distancia = duracion * 0.034 / 2;  // Convertir duración a distancia en cm
  return distancia;
}

void loop() {
  long distancia = medirDistancia();
  Serial.print("Distancia: ");
  Serial.println(distancia);

  if (distancia < distanciaMinima) {
    // Si está demasiado cerca, detenerse y girar
    moverMotores(movimientos[6]);  // Parar
    delay(1000);
    moverMotores(movimientos[2]);  // Girar a la izquierda
    delay(2000);
  } else if (distancia < distanciaSegura) {
    // Si está dentro de la distancia segura, reducir velocidad
    int movReducido[] = {1, 0, 0, 1, 100, 100};  // Adelante con menor velocidad
    moverMotores(movReducido);
  } else {
    // Si no hay obstáculos, moverse normalmente
    moverMotores(movimientos[0]);  // Adelante a velocidad normal
  }

  delay(100);  // Pausa pequeña para evitar lecturas muy rápidas
}
