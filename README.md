# ZUIN — Software
 
🛠️ Built by `Maxim Depever` & `Jitse Van Laer`  
🔥 Supervised by `prof. dr. Bas Baccarne`, `Yannick Christiaens` & `Wouter Devriese`  
🌱 Grown at `Ghent University` 🏛️ `Industrial Design Engineering`
 
## Introductie
 
ZUIN is een systeem van interactieve producten dat kinderen en ouders helpt bewuster om te gaan met energieverbruik thuis. Het systeem bestaat uit twee hardware-onderdelen: een **hub** en een **handheld**. De hub staat centraal in de woning en communiceert draadloos via Bluetooth Low Energy (BLE) met de handheld, die het kind draagt als een horloge.
 
Wanneer de hub detecteert dat een apparaat in huis energie verbruikt, stuurt hij een signaal naar de handheld. Het kind wordt hiervan op de hoogte gebracht via trillingen, een LED en een schermmelding. Door naar de hub te gaan en op de knop te drukken, leert het kind actief om apparaten uit te zetten.
 
<p align="center">
  <img src="Img/ZUIN_render.jpeg" width="60%">
 
De software van dit project is opgesplitst in twee delen:
 
- **Hub** — een Arduino-gebaseerd systeem dat BLE-signalen uitzendt en de interactie met de knop verwerkt.
- **Handheld** — in twee versies ontwikkeld. Versie 1 is een minimalistisch prototype met een LCD-scherm en haptic feedback. Versie 2 is een verbeterde versie met een rond kleurentouchscreen, een grafische LVGL-interface en een echte RTC-klok.

---
 

## Opstarten van de code
 
De code is opgesplitst in drie mappen: één voor de hub, één voor handheld v1 en één voor handheld v2.
 
### Hub
 
Om de hub op te starten, open je [`code_hub`](./codes/HUB/code_hub) in de Arduino IDE. Zorg dat de volgende libraries geïnstalleerd zijn:
 
- `ArduinoBLE`
- `rgb_lcd` (Grove LCD library)
- `Wire`
Upload de code naar het Arduino-bordje. Bij het opstarten toont het LCD-scherm "ZUIN HUB" en begint het bordje te adverteren als `ZuinHub` via BLE.
 
### Handheld v1
 
Open [`code_handheld_V1`](./codes/HANDHELD_V1/code_handheld_V1) in de Arduino IDE. Zorg dat de volgende libraries geïnstalleerd zijn:
 
- `ArduinoBLE`
- `rgb_lcd` (Grove LCD library)
- `Wire`
Upload de code naar het Arduino-bordje. De handheld scant automatisch naar de hub en verbindt zodra `ZuinHub` gevonden wordt.
 
### Handheld v2
 
Open [`code_handheld_v2`](./codes/HANDHELD_V2/code_handheld_v2) in de Arduino IDE. Dit project maakt gebruik van meerdere gegenereerde bestanden vanuit SquareLine Studio die zich bevinden in de map [`extra_bestanden_interface/`](./codes/HANDHELD_V2/extra_bestanden_interface). Zorg dat deze map samen met het hoofdbestand gebruikt wordt. De volgende libraries zijn vereist:
 
- `TFT_eSPI`
- `lvgl`
- `I2C_BM8563` (RTC)
- `BLEDevice`, `BLEUtils`, `BLEScan`, `BLEClient` (ESP32 BLE stack)
- `lv_xiao_round_screen`
- `Wire`
Upload de code naar de Seeed XIAO ESP32S3. Het scherm toont bij het opstarten de huidige tijd en de handheld begint automatisch te scannen naar `ZuinHub`.
 
---
 
## Functioneel schema
 
Onderstaand schema toont de volledige logica van het systeem: links de hub, rechts de handheld. De groene stippellijnen geven de BLE-communicatie weer tussen beide apparaten.
 
<p align="center">
  <img src="Img/zuin_systeem_flowchart.svg" width="88%">