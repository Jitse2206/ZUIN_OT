// Libraries voor I2C, LCD-scherm en BLE
#include <Wire.h>
#include "rgb_lcd.h"
#include <ArduinoBLE.h>

// LCD-object aanmaken
rgb_lcd lcd;

// Pinnen voor knop en LEDs
const int buttonPin = 2;
const int ledAction = 4;
const int ledIdle = 6;

// Bijhouden of er een signaal actief is en of er ooit verbinding was
bool signalActive = false;
bool wasConnected = false;

// Lijst van mogelijke apparaten die "aan" kunnen staan
String devices[] = { "TV", "Vaatwasser", "Licht", "Laptop", "Oven" };
int deviceIndex;

// Tijdsbeheer voor willekeurige wachttijd tussen signalen
unsigned long lastSignalTime = 0;
unsigned long waitTime = 5000;

// BLE service en characteristic aanmaken met vaste UUID's
BLEService hubService("95ff7bf8-aa6f-4671-82d9-22a8931c5387");
BLEStringCharacteristic signalChar("95ff7bf8-aa6f-4671-82d9-22a8931c5388", BLERead | BLENotify, 20);

void setup() {
  // Seriële communicatie starten voor debugging
  Serial.begin(9600);

  // Knop als input, LEDs als output instellen
  pinMode(buttonPin, INPUT);
  pinMode(ledAction, OUTPUT);
  pinMode(ledIdle, OUTPUT);

  // LCD initialiseren en opstartbericht tonen
  lcd.begin(16, 2);
  lcd.print("ZUIN HUB");

  // Willekeurige seed instellen op basis van analoge ruis
  randomSeed(analogRead(A0));

  // Idle LED aan, action LED uit bij opstart
  digitalWrite(ledIdle, HIGH);
  digitalWrite(ledAction, LOW);

  // BLE starten, foutmelding bij mislukking
  if (!BLE.begin()) {
    Serial.println("BLE start mislukt!");
    while (1);
  }

  // Hub configureren en beginnen adverteren als "ZuinHub"
  BLE.setLocalName("ZuinHub");
  BLE.setAdvertisedService(hubService);
  hubService.addCharacteristic(signalChar);
  BLE.addService(hubService);

  // Beginwaarde op IDLE zetten en adverteren starten
  signalChar.writeValue("IDLE");
  BLE.advertise();

  Serial.println("Advertising...");

  // Eerste willekeurige wachttijd instellen
  lastSignalTime = millis();
  waitTime = random(5000, 15000);
}

void loop() {
  // Controleren of er een handheld verbonden is
  BLEDevice central = BLE.central();

  if (central && central.connected()) {
    wasConnected = true;

    unsigned long currentMillis = millis();

    if (!signalActive) {
      // Idle-toestand: idle LED aan, wachten tonen op LCD
      digitalWrite(ledIdle, HIGH);
      digitalWrite(ledAction, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Wachten...   ");

      // Na de willekeurige wachttijd een nieuw signaal sturen
      if (currentMillis - lastSignalTime >= waitTime) {
        lastSignalTime = currentMillis;
        waitTime = random(5000, 15000);
        sendSignal();
      }
    }

    // Als signaal actief is en knop ingedrukt wordt: apparaat uitzetten
    if (signalActive && digitalRead(buttonPin) == HIGH) {
      turnOffDevice();
    }

  } else if (wasConnected) {
    // Verbinding verbroken: timer resetten
    Serial.println("Disconnected");
    wasConnected = false;
    lastSignalTime = millis();
  }

  // BLE-events verwerken
  BLE.poll();
}

void sendSignal() {
  // Signaal activeren en willekeurig apparaat kiezen
  signalActive = true;
  deviceIndex = random(0, 5);

  // Action LED aan, idle LED uit
  digitalWrite(ledIdle, LOW);
  digitalWrite(ledAction, HIGH);

  // LCD-melding tonen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Apparaat aan!");
  lcd.setCursor(0, 1);
  lcd.print("Druk om uit");

  // "SIGNAL" via BLE naar de handheld sturen
  signalChar.writeValue("SIGNAL");
  Serial.println("Signaal verstuurd");
}

void turnOffDevice() {
  // Action LED uit, idle LED aan
  digitalWrite(ledAction, LOW);
  digitalWrite(ledIdle, HIGH);

  // Naam van het uitgezette apparaat tonen op LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Uitgezet:");
  lcd.setCursor(0, 1);
  lcd.print(devices[deviceIndex]);

  // "IDLE" via BLE naar de handheld sturen
  signalChar.writeValue("IDLE");

  // 3 seconden wachten voor volgende cyclus
  delay(3000);
  signalActive = false;
  lastSignalTime = millis();
  waitTime = random(5000, 15000);
}