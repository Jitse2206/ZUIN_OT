// Testcode voor de LEDs van de hub
// Doel: controleren of beide LEDs (idle en action) correct werken
// Als beide LEDs knipperen zijn ze in orde, ligt het probleem elders in de code

const int ledAction = 4;
const int ledIdle = 6;

void setup() {
  Serial.begin(9600);
  pinMode(ledAction, OUTPUT);
  pinMode(ledIdle, OUTPUT);

  Serial.println("LED test gestart...");

  // Test 1: idle LED 5x knipperen
  Serial.println("Test 1: Idle LED...");
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledIdle, HIGH);  // Idle LED aan
    delay(500);
    digitalWrite(ledIdle, LOW);   // Idle LED uit
    delay(500);
    Serial.print("Idle knippering ");
    Serial.print(i + 1);
    Serial.println("/5");
  }

  delay(1000);

  // Test 2: action LED 5x knipperen
  Serial.println("Test 2: Action LED...");
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledAction, HIGH);  // Action LED aan
    delay(500);
    digitalWrite(ledAction, LOW);   // Action LED uit
    delay(500);
    Serial.print("Action knippering ");
    Serial.print(i + 1);
    Serial.println("/5");
  }

  delay(1000);

  // Test 3: beide LEDs tegelijk
  Serial.println("Test 3: Beide LEDs tegelijk...");
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledIdle, HIGH);
    digitalWrite(ledAction, HIGH);  // Beide aan
    delay(500);
    digitalWrite(ledIdle, LOW);
    digitalWrite(ledAction, LOW);   // Beide uit
    delay(500);
    Serial.print("Beide knippering ");
    Serial.print(i + 1);
    Serial.println("/5");
  }

  Serial.println("Test voltooid!");
}

void loop() {
  // Leeg — test gebeurt eenmalig in setup
}