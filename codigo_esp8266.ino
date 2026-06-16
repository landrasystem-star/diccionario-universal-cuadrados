/*
  Diccionario Universal de Cuadrados (DUC)
  Código para ESP8266 con OTA y WiFiManager

  Este código permite:
  - Conectar a WiFi sin poner contraseñas fijas (WiFiManager)
  - Actualizar el ESP por WiFi (OTA)
  - Reconocer cuadrados y ejecutar acciones

  Licencia: MIT
  Autor: [Ramon Rey Dominguez]
  Fecha: 4 de junio de 2026
  Versión: 1.0
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>

// ========== CONFIGURACIÓN DE PINES ==========
const int PIN_LUZ = 2;
const int PIN_ALARMA = 4;
const int PIN_PUERTA = 5;
const int PIN_CALOR = 14;

// ========== SERVIDOR WEB ==========
ESP8266WebServer server(80);

// ========== DICCIONARIO DE CUADRADOS ==========
struct Comando {
  String cuadrado;
  String accion;
};

Comando comandos[] = {
  {"█  █\n████\n█  █\n█  █", "/luz_on"},
  {"████\n████\n████\n████", "/alarma"},
  {"███\n█ █\n███", "/ayuda"},
  {"█  █\n█  █\n████\n█  █", "/calor_on"},
  {"█  █\n████\n█  █\n█  █", "/ventilador_on"},
  {"██\n██", "/abrir_puerta"},
  {"████\n█  █\n█  █\n████", "/cerrar_todo"},
  {"█  █\n█  █\n█  █\n████", "/silenciar"}
};

const int numComandos = sizeof(comandos) / sizeof(comandos[0]);

// ========== CONFIGURACIÓN INICIAL ==========
void setup() {
  Serial.begin(115200);
  Serial.println("\n🔲 DUC - Diccionario Universal de Cuadrados");
  Serial.println("Versión 1.0 - 4 de junio de 2026\n");

  pinMode(PIN_LUZ, OUTPUT);
  pinMode(PIN_ALARMA, OUTPUT);
  pinMode(PIN_PUERTA, OUTPUT);
  pinMode(PIN_CALOR, OUTPUT);
  
  digitalWrite(PIN_LUZ, LOW);
  digitalWrite(PIN_ALARMA, LOW);
  digitalWrite(PIN_PUERTA, LOW);
  digitalWrite(PIN_CALOR, LOW);

  // ========== WIFIMANAGER ==========
  WiFiManager wifiManager;
  wifiManager.setConfigPortalTimeout(60);
  wifiManager.autoConnect("ESP-Cuadrados");

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("✅ WiFi conectado");
    Serial.print("📡 IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("⚠️ Sin WiFi - Modo local");
  }

  // ========== OTA ==========
  ArduinoOTA.setHostname("esp-cuadrados");
  ArduinoOTA.begin();
  Serial.println("✅ OTA listo");

  // ========== SERVIDOR WEB ==========
  server.on("/luz_on", []() {
    digitalWrite(PIN_LUZ, HIGH);
    server.send(200, "text/plain", "Luz encendida");
    Serial.println("💡 Luz encendida");
  });

  server.on("/luz_off", []() {
    digitalWrite(PIN_LUZ, LOW);
    server.send(200, "text/plain", "Luz apagada");
    Serial.println("💡 Luz apagada");
  });

  server.on("/alarma", []() {
    digitalWrite(PIN_ALARMA, HIGH);
    delay(1000);
    digitalWrite(PIN_ALARMA, LOW);
    server.send(200, "text/plain", "Alarma activada");
    Serial.println("🚨 Alarma activada");
  });

  server.on("/abrir_puerta", []() {
    digitalWrite(PIN_PUERTA, HIGH);
    delay(1000);
    digitalWrite(PIN_PUERTA, LOW);
    server.send(200, "text/plain", "Puerta abierta");
    Serial.println("🚪 Puerta abierta");
  });

  server.on("/calor_on", []() {
    digitalWrite(PIN_CALOR, HIGH);
    server.send(200, "text/plain", "Calefacción encendida");
    Serial.println("🔥 Calefacción encendida");
  });

  server.on("/calor_off", []() {
    digitalWrite(PIN_CALOR, LOW);
    server.send(200, "text/plain", "Calefacción apagada");
    Serial.println("🔥 Calefacción apagada");
  });

  server.on("/ayuda", []() {
    server.send(200, "text/plain", "Alerta de ayuda");
    Serial.println("🆘 ALERTA DE AYUDA");
  });

  server.on("/silenciar", []() {
    server.send(200, "text/plain", "Silencio activado");
    Serial.println("🔇 Silencio activado");
  });

  server.on("/cerrar_todo", []() {
    digitalWrite(PIN_PUERTA, LOW);
    server.send(200, "text/plain", "Todo cerrado");
    Serial.println("🔒 Todo cerrado");
  });

  server.on("/reset", []() {
    server.send(200, "text/plain", "Reiniciando...");
    Serial.println("🔄 Reiniciando ESP");
    delay(500);
    ESP.restart();
  });

  server.begin();
  Serial.println("✅ Servidor HTTP iniciado");

  Serial.println("\n========== INSTRUCCIONES ==========");
  Serial.println("📱 Desde el móvil (misma WiFi):");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("   http://");
    Serial.print(WiFi.localIP());
    Serial.println("/luz_on");
  } else {
    Serial.println("   (ESP sin WiFi)");
  }
  Serial.println("🔲 Para probar cuadrados:");
  Serial.println("   Copia y pega un cuadrado en el monitor serie");
  Serial.println("====================================\n");
}

// ========== BUCLE PRINCIPAL ==========
void loop() {
  server.handleClient();
  ArduinoOTA.handle();

  if (Serial.available()) {
    String entrada = Serial.readStringUntil('\n');
    entrada.trim();

    if (entrada.length() > 0) {
      Serial.println("📥 Recibido:");
      Serial.println(entrada);

      bool encontrado = false;
      for (int i = 0; i < numComandos; i++) {
        if (entrada == comandos[i].cuadrado) {
          Serial.print("✅ Reconocido: ");
          Serial.println(comandos[i].accion);

          if (comandos[i].accion == "/luz_on") {
            digitalWrite(PIN_LUZ, HIGH);
            Serial.println("💡 Luz encendida");
          } else if (comandos[i].accion == "/luz_off") {
            digitalWrite(PIN_LUZ, LOW);
            Serial.println("💡 Luz apagada");
          } else if (comandos[i].accion == "/alarma") {
            digitalWrite(PIN_ALARMA, HIGH);
            delay(1000);
            digitalWrite(PIN_ALARMA, LOW);
            Serial.println("🚨 Alarma activada");
          } else if (comandos[i].accion == "/abrir_puerta") {
            digitalWrite(PIN_PUERTA, HIGH);
            delay(1000);
            digitalWrite(PIN_PUERTA, LOW);
            Serial.println("🚪 Puerta abierta");
          } else if (comandos[i].accion == "/calor_on") {
            digitalWrite(PIN_CALOR, HIGH);
            Serial.println("🔥 Calefacción encendida");
          } else if (comandos[i].accion == "/calor_off") {
            digitalWrite(PIN_CALOR, LOW);
            Serial.println("🔥 Calefacción apagada");
          } else if (comandos[i].accion == "/ayuda") {
            Serial.println("🆘 ALERTA DE AYUDA");
          } else if (comandos[i].accion == "/silenciar") {
            Serial.println("🔇 Silencio activado");
          } else if (comandos[i].accion == "/cerrar_todo") {
            digitalWrite(PIN_PUERTA, LOW);
            Serial.println("🔒 Todo cerrado");
          }

          encontrado = true;
          break;
        }
      }

      if (!encontrado) {
        Serial.println("❌ Cuadrado no reconocido");
        Serial.println("   Escribe un cuadrado válido del diccionario");
      }
    }
  }

  delay(100);
}
