// Testcode voor de haptic motor
// Doel: controleren of de haptic motor correct trilt
// Als de motor 5x trilt is de motor in orde, ligt het probleem elders in de code

const int hapticPin = 6;

void setup() {
  Serial.begin(9600);
  pinMode(hapticPin, OUTPUT);
  
  Serial.println("Haptic motor test gestart...");

  // Motor 5x laten trillen
  for (int i = 0; i < 5; i++) {
    digitalWrite(hapticPin, HIGH);  // Motor aan
    delay(300);
    digitalWrite(hapticPin, LOW);   // Motor uit
    delay(200);
    Serial.print("Trilling ");
    Serial.print(i + 1);
    Serial.println("/5");
  }

  Serial.println("Test voltooid!");
}

void loop() {
  // Leeg — test gebeurt eenmalig in setup
}