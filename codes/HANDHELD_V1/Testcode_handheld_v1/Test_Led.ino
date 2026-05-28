// Testcode voor de LED van handheld V1
// Doel: controleren of de LED correct werkt
// Als de LED 5x knippert is de LED in orde, ligt het probleem elders in de code

const int ledPin = 4;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  Serial.println("LED test gestart...");

  // LED 5x laten knipperen
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, HIGH);  // LED aan
    delay(500);
    digitalWrite(ledPin, LOW);   // LED uit
    delay(500);
    Serial.print("Knippering ");
    Serial.print(i + 1);
    Serial.println("/5");
  }

  Serial.println("Test voltooid!");
}

void loop() {
  // Leeg — test gebeurt eenmalig in setup
}