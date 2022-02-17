#include <GyverPortal.h>

#define relePin 2

// Загружать конфигурацию из памяти
// Автоматическое отключение через некоторый промежуток времени

bool releState = false;

void build() {
  String s;
  BUILD_BEGIN(s);
  add.THEME(GP_DARK);

  add.LABEL("Реле:");
  add.SWITCH("state", releState); add.BREAK();

  BUILD_END();
}

void debug() {
  Serial.print(" Rele: ");
  Serial.println(releState);
}

void GUI() {
  GyverPortal portal;
  portal.attachBuild(build);
  portal.start(WIFI_AP);

  while (portal.tick()) {
    if (portal.click("state"))
    {
      releState = not releState;
      digitalWrite(relePin, releState);
      debug();
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("init");

  pinMode(relePin, OUTPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("Smart Outlet", "secretpassword");
  GUI();
}


void loop() {}
