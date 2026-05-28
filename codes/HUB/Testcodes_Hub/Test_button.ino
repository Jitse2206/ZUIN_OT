// Testcode voor de knop van de hub
// Doel: controleren of de knop correct werkt
// Als "Knop ingedrukt!" verschijnt in de Serial Monitor is de knop in orde

const int buttonPin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  Serial.println("Knop test gestart...");
  Serial.println("Druk op de knop!");
}

void loop() {
  // Controleren of de knop ingedrukt is
  if (digitalRead(buttonPin) == HIGH) {
    Serial.println("Knop ingedrukt!");
    delay(300); // Debounce
  }
}