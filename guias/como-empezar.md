# Cómo empezar con el Diccionario Universal de Cuadrados

## Lo que necesitas

- ESP8266 (NodeMCU, Wemos D1, etc.)
- LED con resistencia (para prueba inicial)
- Cable USB para programar
- Arduino IDE instalado

---

## Paso 1: Configurar Arduino IDE para ESP8266

1. Abre Arduino IDE
2. Archivo → Preferencias
3. Añade esta URL en "Gestor de URLs adicionales":
https://arduino.esp8266.com/stable/package_esp8266com_index.json

text
4. Herramientas → Placa → Gestor de tarjetas
5. Busca "esp8266" e instala "esp8266 by ESP8266 Community"

---

## Paso 2: Conectar el LED

| ESP8266 | LED |
|---------|-----|
| GPIO2 (D4) | Ánodo (pata larga) → Resistencia 330Ω → LED |
| GND | Cátodo (pata corta) |

---

## Paso 3: Cargar el código

1. Abre `codigo_esp8266.ino` en Arduino IDE
2. Conecta el ESP8266 por USB
3. Selecciona la placa: **NodeMCU 1.0 (ESP-12E Module)**
4. Selecciona el puerto COM correcto
5. Haz clic en **Subir** (flecha derecha)

---

## Paso 4: Probar por monitor serie

1. Abre el monitor serie (herramientas → monitor serie)
2. Velocidad: 115200 baudios
3. Copia y pega este cuadrado:
█ █
████
█ █
█ █

text
4. Presiona Enter
5. ¡El LED debería encenderse!

---

## Paso 5: Usar WiFiManager

1. Si no hay WiFi configurado, al arrancar el ESP crea una red: `ESP-Cuadrados`
2. Conéctate a ella con tu móvil
3. Abre el navegador en `192.168.4.1`
4. Selecciona tu WiFi y escribe la contraseña
5. El ESP se conecta y guarda los datos

---

## Paso 6: Probar desde el móvil

1. Una vez conectado a tu WiFi, el ESP tendrá una IP
2. Desde el móvil (misma WiFi), abre:
`http://IP_DEL_ESP/luz_on`
3. ¡El LED se enciende!

---

## Solución de problemas

| Problema | Solución |
|----------|----------|
| No aparece la IP | Revisa que el ESP está conectado al WiFi |
| El LED no se enciende | Revisa la polaridad del LED |
| No conecta al WiFi | Reinicia y vuelve a configurar con WiFiManager |

---

