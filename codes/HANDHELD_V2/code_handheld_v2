#define USE_TFT_ESPI_LIBRARY
// Dit laadt de nodige libraries
#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <I2C_BM8563.h>
#include <Wire.h>
#include "lv_xiao_round_screen.h"
#include "ui.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEClient.h>

I2C_BM8563 rtc(I2C_BM8563_DEFAULT_ADDRESS, Wire);
lv_obj_t* images[3];

// De unieke UUID's van de BLE service en characteristic van de hub. Deze moeten exact overeenkomen met die van de hub.

static BLEUUID serviceUUID("95ff7bf8-aa6f-4671-82d9-22a8931c5387");
static BLEUUID charUUID("95ff7bf8-aa6f-4671-82d9-22a8931c5388");

// doConnect is een vlag die aangeeft dat er een verbinding gemaakt moet worden. foundDevice slaat het gevonden hub device op. Dit systeem is nodig omdat de ESP32 geen BLE verbinding mag maken vanuit de scan callback — dat moet vanuit de hoofdloop.

static BLEClient* pClient = nullptr;
static BLERemoteCharacteristic* pRemoteChar = nullptr;
static BLEScan* pBLEScan = nullptr;
static bool connected = false;
static bool signalActive = false;
static int activeFireIndex = -1;
static bool scanning = false;
static bool doConnect = false;
static BLEAdvertisedDevice* foundDevice = nullptr;

static uint32_t lastScanTime = 0;
static uint32_t lastClockTime = 0;
static uint32_t lastLvglTick = 0;

// -----------------------------------------------
// Detecteert een swipe gebaar op het touchscreen.
// -----------------------------------------------
void onSwipe(lv_event_t* e) {
  lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
  lv_obj_t* currentScreen = lv_disp_get_scr_act(NULL);

  if (dir == LV_DIR_LEFT) {
    if (currentScreen == ui_Screen1) lv_disp_load_scr(ui_Screen2);
    else if (currentScreen == ui_Screen2) lv_disp_load_scr(ui_Screen3);
    else if (currentScreen == ui_Screen3) lv_disp_load_scr(ui_Screen4);
    else if (currentScreen == ui_Screen4) lv_disp_load_scr(ui_Screen1);
  } else if (dir == LV_DIR_RIGHT) {
    if (currentScreen == ui_Screen1) lv_disp_load_scr(ui_Screen4);
    else if (currentScreen == ui_Screen2) lv_disp_load_scr(ui_Screen1);
    else if (currentScreen == ui_Screen3) lv_disp_load_scr(ui_Screen2);
    else if (currentScreen == ui_Screen4) lv_disp_load_scr(ui_Screen3);
  }
}

// -----------------------------------------------
// Dit is de touchscreen driver voor LVGL. Het leest de aanraakcoördinaten van de CHSC6x touch controller en geeft ze door aan LVGL zodat het interface op aanraking kan reageren.
// -----------------------------------------------
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  lv_coord_t touchX = 0, touchY = 0;
  if (digitalRead(TOUCH_INT) == LOW) {
    chsc6x_get_xy(&touchX, &touchY);
    if (touchX > 0 && touchX < 240 && touchY > 0 && touchY < 240) {
      data->state = LV_INDEV_STATE_PR;
      data->point.x = touchX;
      data->point.y = touchY;
      return;
    }
  }
  data->state = LV_INDEV_STATE_REL;
}

// -----------------------------------------------
// Deze functie wordt automatisch aangeroepen wanneer de hub een BLE notificatie stuurt. Bij "SIGNAL" kiest het willekeurig een eiland, laadt dat scherm en toont het vlammetje. Bij "IDLE" verbergt het het vlammetje en gaat terug naar het hoofdscherm met de klok.
// -----------------------------------------------
void notifyCallback(BLERemoteCharacteristic* pChar, uint8_t* pData, size_t length, bool isNotify) {
  String value = "";
  for (int i = 0; i < length; i++) {
    value += (char)pData[i];
  }

  Serial.print("Ontvangen: ");
  Serial.println(value);

  if (value == "SIGNAL" && !signalActive) {
    signalActive = true;
    activeFireIndex = random(0, 3);

    if (activeFireIndex == 0) lv_disp_load_scr(ui_Screen2);
    else if (activeFireIndex == 1) lv_disp_load_scr(ui_Screen3);
    else lv_disp_load_scr(ui_Screen4);

    lv_obj_clear_flag(images[activeFireIndex], LV_OBJ_FLAG_HIDDEN);
    Serial.print("Vlammetje op eiland ");
    Serial.println(activeFireIndex);

  } else if (value == "IDLE" && signalActive) {
    signalActive = false;

    if (activeFireIndex >= 0) {
      lv_obj_add_flag(images[activeFireIndex], LV_OBJ_FLAG_HIDDEN);
      activeFireIndex = -1;
    }

    lv_disp_load_scr(ui_Screen1);
    Serial.println("Vlammetje gedoofd");
  }
}

// -----------------------------------------------
// Maakt de effectieve BLE verbinding met de hub, zoekt de juiste service en characteristic op en registreert notifyCallback zodat signalen automatisch ontvangen worden.
// -----------------------------------------------
bool connectToHub() {
  if (pClient == nullptr) {
    pClient = BLEDevice::createClient();
  }

  Serial.println("Verbinden met ZuinHub...");
  if (!pClient->connect(foundDevice)) {
    Serial.println("Connectie mislukt!");
    return false;
  }

  Serial.println("Verbonden!");

  BLERemoteService* pService = pClient->getService(serviceUUID);
  if (!pService) {
    Serial.println("Service niet gevonden!");
    pClient->disconnect();
    return false;
  }

  pRemoteChar = pService->getCharacteristic(charUUID);
  if (!pRemoteChar) {
    Serial.println("Characteristic niet gevonden!");
    pClient->disconnect();
    return false;
  }

  if (pRemoteChar->canNotify()) {
    pRemoteChar->registerForNotify(notifyCallback);
    Serial.println("Notify geregistreerd!");
  }

  connected = true;
  return true;
}

// -----------------------------------------------
// De scan callback die uitgevoerd wordt voor elk gevonden BLE device. Als de naam "ZuinHub" is, slaat het het device op en zet doConnect op true.
// -----------------------------------------------
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == "ZuinHub") {
      Serial.println("ZuinHub gevonden!");
      pBLEScan->stop();
      scanning = false;
      foundDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
    }
  }
};

// -----------------------------------------------
// Setup
// -----------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("Opstarten...");

  pinMode(D0, OUTPUT);
  digitalWrite(D0, HIGH);

  Wire.begin(5, 6);
  rtc.begin();

  // Initialiseert LVGL, de round display en het touchscreen.

  lv_init();
  lv_xiao_disp_init();
  delay(100);
  lv_xiao_touch_init();

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  //Laadt de SquareLine Studio interface en toont Screen1 (het klokscherm) als beginscherm.

  ui_init();
  lv_disp_load_scr(ui_Screen1);

  //Koppelt de drie vlammetje objecten uit de SquareLine UI aan een array en verbergt ze allemaal bij het opstarten.

  images[0] = ui_Fire2;
  images[1] = ui_Fire3;
  images[2] = ui_Fire4;

  // Alle vlammetjes verbergen bij start
  for (int i = 0; i < 3; i++) {
    lv_obj_add_flag(images[i], LV_OBJ_FLAG_HIDDEN);
  }

  // Swipe events toevoegen aan alle schermen
  lv_obj_add_event_cb(ui_Screen1, onSwipe, LV_EVENT_GESTURE, NULL);
  lv_obj_add_event_cb(ui_Screen2, onSwipe, LV_EVENT_GESTURE, NULL);
  lv_obj_add_event_cb(ui_Screen3, onSwipe, LV_EVENT_GESTURE, NULL);
  lv_obj_add_event_cb(ui_Screen4, onSwipe, LV_EVENT_GESTURE, NULL);

  I2C_BM8563_TimeTypeDef t;
  rtc.getTime(&t);
  char timeBuf[10];
  sprintf(timeBuf, "%02d:%02d:%02d", t.hours, t.minutes, t.seconds);
  lv_label_set_text(ui_ClockLabel, timeBuf);

  //Configureert de BLE scan — setInterval en setWindow zorgen dat de scan niet te veel CPU gebruikt zodat LVGL vlot blijft draaien.

  BLEDevice::init("ZuinHandheld");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(50);

  Serial.println("Setup klaar!");
}

// -----------------------------------------------
// Dit is het hart van LVGL — lv_tick_inc vertelt LVGL hoeveel tijd er verstreken is en lv_timer_handler verwerkt alle animaties, events en hertekeningen. Dit moet zo snel mogelijk aangeroepen worden zonder delay().
// -----------------------------------------------
void loop() {
  uint32_t now = millis();

  lv_tick_inc(now - lastLvglTick);
  lastLvglTick = now;
  lv_timer_handler();

//Elke seconde wordt de tijd gelezen van de RTC klok en geüpdatet op het LCD label van Screen1.

  if (now - lastClockTime >= 1000) {
    lastClockTime = now;
    if (ui_ClockLabel != NULL) {
      I2C_BM8563_TimeTypeDef t;
      rtc.getTime(&t);
      char timeBuf[10];
      sprintf(timeBuf, "%02d:%02d:%02d", t.hours, t.minutes, t.seconds);
      lv_label_set_text(ui_ClockLabel, timeBuf);
      lv_obj_invalidate(ui_ClockLabel);
    }
  }

// Als de scan callback een hub gevonden heeft en doConnect op true gezet heeft, wordt hier de verbinding gemaakt vanuit de veilige context van de loop.

  if (doConnect) {
    doConnect = false;
    connectToHub();
  }

// Als er geen verbinding is herstart het scannen elke 5 seconden voor 3 seconden. Door intervallen te gebruiken in plaats van continu scannen blijft LVGL vlot draaien.

  if (!connected || (pClient && !pClient->isConnected())) {
    connected = false;
    if (!scanning && !doConnect && (now - lastScanTime >= 5000)) {
      lastScanTime = now;
      scanning = true;
      Serial.println("Scannen...");
      pBLEScan->start(3, false);
    }
  }
}