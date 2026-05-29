# Marlin 2.1.2.7 — Creality E3 Free-runs (CR4NT220801C10) + Ender-5 Pro

Guía completa para compilar y flashear Marlin en la placa **Creality E3 Free-runs TMC2209 32-bit** instalada en una **Creality Ender-5 Pro**. Documentada paso a paso después de resolver todos los problemas de configuración desde cero.

---

## 🔧 Hardware

| Componente | Detalle |
|---|---|
| Impresora | Creality Ender-5 Pro |
| Placa | Creality E3 Free-runs TMC2209 32-bit |
| Código de placa | CR4NT220801C10 |
| MCU | **STM32F401RET6** (84MHz, 512KB Flash, 64KB RAM) |
| Drivers | TMC2209 integrados (UART interno) |
| Pantalla | CR-10 Stock Display (LCD azul 128x64 con encoder) |
| Fecha PCB | 2022-08-01 |

> ⚠️ **IMPORTANTE:** Esta placa usa **STM32F401** (familia F4), NO STM32F103 (familia F1).  
> La mayoría de guías para Ender-5 Pro usan el environment `STM32F103RE_creality` — eso es incorrecto para esta placa y el firmware no arrancará.

---

## 📋 Prerrequisitos

### Software necesario

1. **Visual Studio Code**  
   Descarga desde: https://code.visualstudio.com/

2. **Extensión PlatformIO** (dentro de VS Code)  
   - Abrir VS Code → Extensions (Ctrl+Shift+X)  
   - Buscar `PlatformIO IDE` → Instalar  
   - Esperar que instale todas sus dependencias (puede tardar varios minutos)

3. **Git** (para clonar repositorios)  
   Descarga desde: https://git-scm.com/

---

## 📥 Paso 1 — Descargar Marlin 2.1.2.7

```bash
# Opción A: Descargar ZIP desde GitHub
# https://github.com/MarlinFirmware/Marlin/releases/tag/2.1.2.7
# Descomprimir en una carpeta de tu elección

# Opción B: Clonar con git
git clone https://github.com/MarlinFirmware/Marlin.git --branch 2.1.2.7
```

---

## 📥 Paso 2 — Descargar repositorio oficial E3 Free-runs

El fabricante Creality publica el código fuente oficial con los archivos de configuración y definición de pines correctos para esta placa.

```bash
# Clonar repositorio oficial de Creality para esta placa
git clone https://github.com/CrealityOfficial/E3-Free-runs-Silent-Motherboard.git
```

Dentro encontrarás:
- `example/Ender-3/Configuration.h` — config de ejemplo para Ender-3
- `example/Marlin/Marlin/src/pins/stm32f4/pins_CREALITY_CR4NTxxC10.h` — **archivo de pines oficial**
- `hardware/` — esquemáticos en PDF del PCB
- `bootloader/` — bootloader STM32F401 por si necesitas restaurarlo

---

## 🛠️ Paso 3 — Modificar el código fuente de Marlin

Abre la carpeta de Marlin en VS Code: `File → Open Folder`

### 3.1 Copiar el archivo de pines oficial

Copia el archivo de pines oficial al directorio correcto de Marlin:

```
ORIGEN:  E3-Free-runs-Silent-Motherboard/example/Marlin/Marlin/src/pins/stm32f4/pins_CREALITY_CR4NTxxC10.h
DESTINO: Marlin-2.1.2.7/Marlin/src/pins/stm32f4/pins_CREALITY_CR4NTxxC10.h
```

### 3.2 Corregir nombres obsoletos en el archivo de pines

El archivo oficial fue escrito para una versión anterior de Marlin. Marlin 2.1.2.7 renombró varios identificadores. Aplica estos cambios en `pins_CREALITY_CR4NTxxC10.h`:

| Nombre antiguo (archivo oficial) | Nombre correcto (Marlin 2.1.2.7) |
|---|---|
| `FAN_PIN` | `FAN0_PIN` |
| `PIN_EXISTS(FAN)` | `PIN_EXISTS(FAN0)` |
| `LCD_PINS_ENABLE` | `LCD_PINS_EN` |
| `SDIO_SUPPORT` | `ONBOARD_SDIO` |

Busca y reemplaza cada uno en el archivo. Ejemplo del bloque del ventilador:

```cpp
// ANTES (incorrecto en Marlin 2.1.2.7):
#ifndef FAN_PIN
  #define FAN_PIN  PA0
#endif
#if PIN_EXISTS(FAN)
  #define FAN_SOFT_PWM
#endif

// DESPUÉS (correcto):
#ifndef FAN0_PIN
  #define FAN0_PIN  PA0
#endif
#if PIN_EXISTS(FAN0)
  #define FAN_SOFT_PWM
#endif
```

### 3.3 Agregar definición de placa en boards.h

Archivo: `Marlin/src/core/boards.h`

Busca la línea con `BOARD_TRONXY_CXY_446_V10` y añade después:

```cpp
#define BOARD_TRONXY_CXY_446_V10      5244  // TRONXY ...
#define BOARD_CREALITY_CR4NTxxC10     4241  // Creality E3 Free-runs CR4NT (STM32F401RC/RE)
```

### 3.4 Registrar el archivo de pines en pins.h

Archivo: `Marlin/src/pins/pins.h`

Busca la sección de boards STM32F4 y añade la referencia:

```cpp
#elif MB(CREALITY_CR4NTxxC10)
  #include "stm32f4/pins_CREALITY_CR4NTxxC10.h"  // STM32F4  env:STM32F401RE_creality env:STM32F401RC_creality
```

Puedes añadirlo justo antes del comentario `// ARM Cortex-M7` al final de la sección F4.

---

## ⚙️ Paso 4 — Configurar Marlin para Ender-5 Pro

### 4.1 Copiar configuraciones base del Ender-5 Pro

Marlin incluye configuraciones de ejemplo. Copia los 4 archivos de la carpeta del Ender-5 Pro V427 al directorio `Marlin/`:

```
ORIGEN:  Marlin/config/examples/Creality/Ender-5 Pro/CrealityV427/
DESTINO: Marlin/Marlin/

Archivos a copiar:
  - Configuration.h
  - Configuration_adv.h
  - _Bootscreen.h
  - _Statusscreen.h
```

> ℹ️ Usamos la config del V427 como base porque el Ender-5 Pro con E3 Free-runs tiene la misma mecánica (dimensiones, endstops, steps/mm). Solo cambia la placa electrónica.

### 4.2 Cambios en Configuration.h

Abre `Marlin/Marlin/Configuration.h` y aplica estos cambios:

#### Cambiar la placa (CRÍTICO)
```cpp
// CAMBIAR de:
#define MOTHERBOARD BOARD_CREALITY_V427
// A:
#define MOTHERBOARD BOARD_CREALITY_CR4NTxxC10
```

#### Cambiar drivers a TMC2209_STANDALONE
```cpp
#define X_DRIVER_TYPE  TMC2209_STANDALONE
#define Y_DRIVER_TYPE  TMC2209_STANDALONE
#define Z_DRIVER_TYPE  TMC2209_STANDALONE
#define E0_DRIVER_TYPE TMC2209_STANDALONE
```

> **Nota:** La placa soporta modo UART completo (TMC2209) con direcciones 0-3 en Serial6 (PC6/PC7).  
> Usar `TMC2209_STANDALONE` funciona en modo silencioso sin necesidad de configurar UART.  
> Si quieres control avanzado de corriente y microstepping, cambia a `TMC2209` y configura el UART.

#### Idioma (opcional)
```cpp
#define LCD_LANGUAGE es   // Español
// Otras opciones: en (inglés), de (alemán), fr (francés), etc.
```

#### Activar EEPROM auto-init (evita el error al primer arranque)
```cpp
#define EEPROM_SETTINGS
#if ENABLED(EEPROM_SETTINGS)
  #define EEPROM_AUTO_INIT  // <- Descomentar esta línea
#endif
```

#### Activar PID para la cama (evita el overshoot de temperatura)
```cpp
// Descomentar esta línea:
#define PIDTEMPBED
```

#### Tipo de pantalla (ya incluido en la config base, verificar que esté así)
```cpp
#define CR10_STOCKDISPLAY
#define RET6_12864_LCD   // Específico para el conector RET6 de esta placa
```

### 4.3 Configurar platformio.ini

Abre `platformio.ini` en la raíz del proyecto y cambia:

```ini
[platformio]
default_envs = STM32F401RE_creality   ; ← ESTE es el correcto para STM32F401RET6
```

> ❌ NO usar: `mega2560`, `STM32F103RE_creality`, ni ningún entorno F1.

---

## 🔨 Paso 5 — Compilar

En VS Code con PlatformIO instalado:

**Opción A — Interfaz gráfica:**
- Click en el ícono de PlatformIO (hormiga) en la barra lateral
- `PROJECT TASKS → STM32F401RE_creality → Build`

**Opción B — Terminal:**
```bash
cd /ruta/a/Marlin-2.1.2.7
pio run
```

**Resultado esperado:**
```
RAM:   [=         ]  ~12% (used ~8000 bytes from 65536 bytes)
Flash: [===       ]  ~30% (used ~153000 bytes from 524288 bytes)
[SUCCESS]
```

El firmware compilado queda en:
```
.pio/build/STM32F401RE_creality/firmware-YYYY-MM-DD-HH-MM-SS.bin
```

---

## 💾 Paso 6 — Flashear el firmware

> ⚠️ El proceso de flasheo para STM32F4 es **diferente** al de F1. NO pongas el .bin en la raíz de la SD.

### Proceso correcto:

1. Formatea una tarjeta microSD como **FAT32** (tamaño de unidad: 4096 bytes)
2. Crea una carpeta llamada exactamente **`STM32F4_UPDATE`** en la raíz de la SD
3. Copia el archivo `firmware-*.bin` dentro de esa carpeta  
   (puedes renombrarlo a `firmware.bin` para simplificar)
4. Inserta la SD en la impresora con ella **apagada**
5. **Enciende** la impresora — la pantalla mostrará progreso y se reiniciará
6. Retira la SD después del flasheo

### Si el flasheo falla:
- Verifica que la carpeta se llame exactamente `STM32F4_UPDATE` (mayúsculas, sin espacios)
- Prueba formatear la SD con unidad de asignación de **2048 bytes**
- El archivo no debe tener caracteres chinos en el nombre

---

## 🔬 Paso 7 — Calibración post-instalación

Una vez flasheado y arrancando correctamente:

### 7.1 Resetear EEPROM
```gcode
M502   ; Carga defaults del firmware
M500   ; Guarda en EEPROM
```

### 7.2 Verificar endstops
Con la impresora en reposo (sin homing):
```gcode
M119
```
Todos deben decir `open`. Si alguno dice `TRIGGERED` sin estar presionado, hay un problema de cableado.

### 7.3 Auto Home
```gcode
G28
```
Observar:
- **X**: cabeza se mueve a la **derecha** hasta el endstop (MAX)
- **Y**: cabeza se mueve hacia el **fondo** hasta el endstop (MAX)
- **Z**: la **cama baja** hasta el endstop Z (MIN)

Si algún eje va al revés, cambiar `INVERT_X_DIR`, `INVERT_Y_DIR`, o `INVERT_Z_DIR` de `false` a `true` en Configuration.h.

### 7.4 Calibrar PID de la cama
```gcode
M303 E-1 S60 C8   ; Autotune PID a 60°C, 8 ciclos (~5 minutos)
M500              ; Guarda resultado
```

### 7.5 Calibrar PID del hotend
```gcode
M303 E0 S200 C8   ; Autotune PID a 200°C, 8 ciclos
M500
```

### 7.6 Calibrar pasos del extrusor (E-steps)
1. Marca 120mm de filamento desde la entrada del extrusor
2. Calienta hotend a 200°C
3. Extruye 100mm: `G1 E100 F100`
4. Mide cuánto filamento se consumió realmente
5. Calcula: `nuevo_Estep = (pasos_actuales × 100) / mm_reales`
6. Aplica: `M92 E<nuevo_valor>` y guarda con `M500`

Los pasos actuales configurados son **93 steps/mm** (estándar Creality).

---

## 🐛 Problemas conocidos y soluciones

### ❌ Pantalla no hace nada al encender
**Causa:** Firmware compilado con environment incorrecto (F1 en vez de F4).  
**Solución:** Verificar que `default_envs = STM32F401RE_creality` en platformio.ini.

### ❌ Error al flashear — pantalla no muestra progreso
**Causa:** Proceso de flasheo incorrecto.  
**Solución:** Crear carpeta `STM32F4_UPDATE` en la SD (no poner el .bin en la raíz).

### ❌ Encoder (rueda) errático o no funciona
**Causa raíz descubierta:** La placa usa **PB10 para el calefactor de la cama**, NO para el encoder. Las guías basadas en V427 asignan PB10 al encoder (BTN_EN1), lo que hace que el firmware lea el PWM de la cama como señal del encoder.  
**Solución:** Usar el archivo de pines oficial `pins_CREALITY_CR4NTxxC10.h` donde `BTN_EN1 = PA2`.

### ❌ Error "EEPROM Version" al primer arranque
**Causa:** EEPROM con datos de firmware anterior incompatible.  
**Solución:** Está corregido con `EEPROM_AUTO_INIT`. Si persiste, enviar `M502` + `M500` por USB.

### ❌ Temperatura de cama con mucho overshoot
**Causa:** Control bang-bang (por defecto) no tiene PID, overshoota en camas con mucha inercia térmica.  
**Solución:** Activar `#define PIDTEMPBED` en Configuration.h y ejecutar autotune `M303 E-1 S60 C8`.

### ❌ Error de compilación: "FAN_PIN is now FAN0_PIN"
**Causa:** El archivo de pines oficial usa nombres obsoletos para Marlin 2.1.2.7.  
**Solución:** Ver sección 3.2 — aplicar los 4 renombrados.

---

## 📌 Pines importantes del conector LCD (RET6_12864_LCD)

| Señal | Pin MCU | Función |
|---|---|---|
| BTN_ENC | PB2 | Botón del encoder (click) |
| BTN_EN1 | **PA2** | Encoder canal A (rotación) |
| BTN_EN2 | PB14 | Encoder canal B (rotación) |
| LCD_RS | PB12 | LCD chip select |
| LCD_EN | PB15 | LCD data |
| LCD_D4 | PB13 | LCD clock |
| BEEPER | PC0 | Buzzer |
| HEATER_BED | **PB10** | ⚠️ Calefactor cama (NO encoder) |

---

## 📐 Parámetros mecánicos Ender-5 Pro

```cpp
#define X_BED_SIZE 220
#define Y_BED_SIZE 220
#define Z_MAX_POS  300

#define X_HOME_DIR  1   // MAX (homing a la derecha)
#define Y_HOME_DIR  1   // MAX (homing al fondo)
#define Z_HOME_DIR -1   // MIN (cama baja al endstop)

#define DEFAULT_AXIS_STEPS_PER_UNIT { 80, 80, 800, 93 }
// X: 80 steps/mm  (correa GT2 + polea 20T)
// Y: 80 steps/mm  (correa GT2 + polea 20T)
// Z: 800 steps/mm (tornillo M5 + 1/16 microstepping)
// E: 93 steps/mm  (extrusor Creality estándar)
```

---

## 🔗 Referencias

- [Marlin Firmware](https://github.com/MarlinFirmware/Marlin) — Código fuente oficial
- [Repositorio E3 Free-runs Creality](https://github.com/CrealityOfficial/E3-Free-runs-Silent-Motherboard) — Fuente oficial de pines y esquemáticos
- [Marlin Configurations](https://github.com/MarlinFirmware/Configurations) — Configuraciones de ejemplo oficiales
- [PlatformIO](https://platformio.org/) — Entorno de compilación
- [Datasheet STM32F401RE](https://www.st.com/resource/en/datasheet/stm32f401re.pdf) — MCU de la placa

---

## 📝 Historial de cambios

| Versión | Fecha | Cambio |
|---|---|---|
| 1.0 | 2026-05-29 | Configuración inicial funcional para Ender-5 Pro |

---

*Documentado a partir de la experiencia de instalación real con todos los errores y soluciones encontrados.*
