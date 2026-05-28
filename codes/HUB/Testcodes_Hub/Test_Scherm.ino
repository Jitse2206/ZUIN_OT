// Testcode voor het LCD-scherm van de hub
// Doel: controleren of het scherm correct werkt

#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  Serial.println("LCD test gestart...");

  // Test 1: opstartbericht
  lcd.setCursor(0, 0);
  lcd.print("ZUIN HUB");
  lcd.setCursor(0, 1);
  lcd.print("Test gestart...");
  Serial.println("Test 1: opstartbericht");
  delay(2000);

  // Test 2: alle 16 karakters per rij
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1234567890123456");
  lcd.setCursor(0, 1);
  lcd.print("ABCDEFGHIJKLMNOP");
  Serial.println("Test 2: alle karakters");
  delay(2000);

  // Test 3: hubberichten zoals in echte code
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Apparaat aan!");
  lcd.setCursor(0, 1);
  lcd.print("Druk om uit");
  Serial.println("Test 3: hubbericht");
  delay(2000);

  // Test 4: uitgezet bericht
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Uitgezet:");
  lcd.setCursor(0, 1);
  lcd.print("TV");
  Serial.println("Test 4: uitgezet bericht");
  delay(2000);

  // Afsluiten
  lcd.clear();
  lcd.print("Test voltooid!");
  Serial.println("Test voltooid!");
}

void loop() {
  // Leeg — test gebeurt eenmalig in setup
}