# ZUIN — Software
 
🛠️ Built by `Sam Verkimpe` & `Maxim Depever` & `Jitse Van Laer`  
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
 
## Functioneel schema
 
Onderstaand schema toont de volledige logica van het systeem: links de hub, rechts de handheld. De groene stippellijnen geven de BLE-communicatie weer tussen beide apparaten.
 
<p align="center">
  <img src="Img/zuin_systeem_flowchart.svg" width="88%">