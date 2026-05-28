// Libraries voor BLE, I2C communicatie en het LCD-scherm
#include <ArduinoBLE.h>
#include <Wire.h>
#include "rgb_lcd.h"

// LCD-object aanmaken
rgb_lcd lcd;

// Pinnen voor haptic motor en LED
const int hapticPin = 6;
const int ledPin = 4;

// Naam en UUID van de hub waarmee verbonden wordt
const char* targetName = "ZuinHub";
const char* charUuid = "95ff7bf8-aa6f-4671-82d9-22a8931c5388";

// Bijhouden of er momenteel een signaal actief is
bool signalActive = false;

void setup() {
  // Seriële communicatie starten voor debugging
  Serial.begin(9600);

  // Haptic motor en LED instellen als output, standaard uitgeschakeld
  pinMode(hapticPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(hapticPin, LOW);
  digitalWrite(ledPin, LOW);

  // LCD initialiseren en opstartbericht tonen
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("ZUIN HANDHELD");

  // BLE starten, foutmelding bij mislukking
  if (!BLE.begin()) {
    Serial.println("BLE MISLUKT");
    while (1);
  }

  // Beginnen met scannen naar BLE-apparaten in de buurt
  BLE.scan();
  Serial.println("Scanning...");
}

void loop() {
  // BLE-events verwerken
  BLE.poll();

  // Controleren of er een BLE-apparaat gevonden is
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    Serial.print("Found: ");
    Serial.print(peripheral.localName());
    Serial.print(" (");
    Serial.print(peripheral.address());
    Serial.println(")");

    // Alleen verdergaan als het gevonden apparaat de hub is
    if (peripheral.localName() != targetName) return;

    // Stoppen met scannen en verbinding maken
    BLE.stopScan();

    if (peripheral.connect()) {
      Serial.println("Connected");

      // Attributen van de hub ophalen
      if (peripheral.discoverAttributes()) {
        BLECharacteristic characteristic = peripheral.characteristic(charUuid);

        // Subscriben op de characteristic zodat updates binnenkomen
        if (characteristic && characteristic.subscribe()) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Verbonden!");

          // Zolang verbonden blijven luisteren naar updates
          while (peripheral.connected()) {
            BLE.poll();

            // Nieuwe waarde ontvangen van de hub
            if (characteristic.valueUpdated()) {
              byte buffer[20];
              int length = characteristic.readValue(buffer, 20);

              // Ontvangen bytes omzetten naar een leesbare string
              String value = "";
              for (int i = 0; i < length; i++) {
                value += (char)buffer[i];
              }

              Serial.print("Ontvangen: ");
              Serial.println(value);

              // Bij "SIGNAL": signaal activeren
              if (value == "SIGNAL" && !signalActive) {
                signalActive = true;
                activateSignal();
              // Bij "IDLE": signaal deactiveren
              } else if (value == "IDLE" && signalActive) {
                signalActive = false;
                deactivateSignal();
              }
            }
          }
        }
      }

      // Verbinding verbroken: alles resetten en opnieuw scannen
      Serial.println("Disconnected");
      peripheral.disconnect();
      deactivateSignal();
      signalActive = false;
    }

    // Opnieuw beginnen scannen na disconnect
    BLE.scan();
  }
}

void activateSignal() {
  // LCD-melding tonen dat een apparaat aan staat
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Apparaat aan!");
  lcd.setCursor(0, 1);
  lcd.print("Ga naar hub!");

  // LED aanzetten
  digitalWrite(ledPin, HIGH);

  // Haptic motor 3x kort laten trillen als tactiele melding
  for (int i = 0; i < 3; i++) {
    digitalWrite(hapticPin, HIGH);
    delay(300);
    digitalWrite(hapticPin, LOW);
    delay(200);
  }
}

void deactivateSignal() {
  // LCD terugzetten naar beginscherm
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ZUIN HANDHELD");

  // LED en haptic motor uitschakelen
  digitalWrite(ledPin, LOW);
  digitalWrite(hapticPin, LOW);
}