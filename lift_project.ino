// ===== Motor Pins (L298N) =====
int IN1 = 6;
int IN2 = 7;
int EN  = 5;

// ===== Ultrasonic Pins =====
#define TRIG 10
#define ECHO 9

// ===== Button Pins (PULLUP) =====
#define BTN1 13
#define BTN2 12
#define BTN3 11

// ===== Elevator State =====
int mevcutKat = 0;   // current floor (from sensor)
int hedefKat  = 0;   // target floor (from buttons)

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN, OUTPUT);

  stopMotor();

  Serial.begin(9600);
}

// ================= MAIN LOOP =================
void loop() {

  // ---------- READ DISTANCE ----------
  int mesafe = mesafeOlc();
  if (mesafe == -1) return; // sensor error protection

  Serial.print("Mesafe: ");
  Serial.println(mesafe);

  // ---------- DETECT CURRENT FLOOR ----------
  if (mesafe >= 3 && mesafe <= 5) {
    mevcutKat = 1;
  } 
  else if (mesafe >= 13 && mesafe <= 15) {
    mevcutKat = 2;
  } 
  else if (mesafe >= 24 && mesafe <= 26) {
    mevcutKat = 3;
  }

  Serial.print("Mevcut Kat: ");
  Serial.println(mevcutKat);

  // ---------- READ BUTTONS ----------
  if (digitalRead(BTN1) == LOW) hedefKat = 1;
  else if (digitalRead(BTN2) == LOW) hedefKat = 2;
  else if (digitalRead(BTN3) == LOW) hedefKat = 3;

  if (hedefKat != 0) {
    Serial.print("Hedef Kat: ");
    Serial.println(hedefKat);
  }

  // ---------- MOTOR CONTROL ----------
  if (hedefKat == 0 || mevcutKat == 0) {
    stopMotor();
    return;
  }

  if (mevcutKat < hedefKat) {
    moveUp();
  }
  else if (mevcutKat > hedefKat) {
    moveDown();
  }
  else {
    stopMotor();
    hedefKat = 0; // target reached
    Serial.println("Hedefe çatdı ✅");
  }
}

// ================= FUNCTIONS =================

int mesafeOlc() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long sure = pulseIn(ECHO, HIGH, 30000); // timeout protection
  if (sure == 0) return -1;

  return (sure * 0.034) / 2;
}

void moveUp() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(EN, 60);
}

void moveDown() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(EN, 60);
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(EN, 0);
}
// ===== END OF PROJECT =====
