#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <LiquidCrystal.h>

// Configuración de la pantalla LCD
LiquidCrystal lcd(22,21,5,18,23,19);
//const char* ssid = "WiFi UVAQ";
//const char* password = "";
const char* ssid = "Megacable_2.4G_33C2";
const char* password = "j9r32F6j";

AsyncWebServer server(80);

// Variables globales para almacenar el texto ingresado y la IP del cliente
String textoIngresado;
String ipCliente;

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);

  // Conexión a la red WiFi
  Serial.println("Conectando a la red WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Conectado");
  // Imprimir IP local en el Monitor Serie
  Serial.println(WiFi.localIP());
    // Iniciar SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  // Inicialización del servidor web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Seleccionando archivo style.css
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/texto", HTTP_POST, [](AsyncWebServerRequest * request) {
    textoIngresado = request->arg("texto");
    ipCliente = request->client()->remoteIP().toString();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Texto ingresado:");
    lcd.setCursor(0, 1);
    lcd.print(textoIngresado);
    request->send(SPIFFS, "/index.html", "text/html");
    Serial.println(textoIngresado);
  });
  // Inicio del servidor web
  server.begin();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectado a WiFi!");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
}
void loop() {
}
