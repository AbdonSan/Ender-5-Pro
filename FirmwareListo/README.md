# Firmware Listo para Instalar — Sin necesidad de compilar

Si llegaste aquí y solo quieres instalar el firmware en tu impresora **sin saber programación**, este es tu lugar. Solo necesitas una tarjeta microSD y seguir estos pasos.

---

## ¿Para qué impresora y placa es este firmware?

| | |
|---|---|
| **Impresora** | Creality Ender-5 Pro |
| **Placa** | Creality E3 Free-runs TMC2209 32-bit |
| **Código de placa** | CR4NT220801C10 |
| **Firmware** | Marlin 2.1.2.7 |
| **Idioma** | Español |

> ⚠️ **Verifica tu placa antes de instalar.** El código `CR4NT220801C10` está impreso en la placa madre. Si tu placa tiene un código diferente, este firmware podría no funcionar.

---

## ¿Cuál archivo usar?

Hay dos versiones compiladas en esta carpeta:

| Archivo | Fecha | Qué incluye |
|---|---|---|
| `firmware-20260529-151826.bin` | 29 May 2026 - 15:18 | Versión inicial de referencia |
| `firmware-20260529-153702.bin` | 29 May 2026 - 15:37 | Versión anterior (PIDTEMPBED activado — NO usar, bug de cama) |
| `firmware-20260529-165457.bin` | 29 May 2026 - 16:54 | ✅ **Versión recomendada** (bug de cama caliente corregido) |

**Usa `firmware-20260529-165457.bin`** — es la versión corregida que soluciona el problema de la cama calentándose sola al encender.

> ⚠️ **Si tu cama se calienta sola al encender sin objetivo de temperatura:** flashea esta versión. Era un bug del timer STM32F4 activado por PIDTEMPBED.

---

## Lo que necesitas

- ✅ Una tarjeta **microSD de 4GB o 8GB** (máximo 8GB — las de 16GB o más pueden no funcionar)
- ✅ Un adaptador de microSD a USB o SD para conectarla al computador
- ✅ Los archivos de esta carpeta

> ❌ **NO uses tarjetas de más de 8GB** — el bootloader STM32F4 de esta placa no las reconoce correctamente.

---

## Paso 1 — Formatear la tarjeta microSD

La tarjeta debe estar formateada de una manera específica. Sigue estos pasos según tu sistema operativo:

### En Windows:
1. Inserta la microSD en el computador
2. Abre el Explorador de Windows, click derecho en la unidad → **Formatear**
3. Configura:
   - **Sistema de archivos:** FAT32
   - **Tamaño de unidad de asignación:** 4096 bytes
   - **Etiqueta de volumen:** (cualquier nombre)
4. Click en **Iniciar** y confirma

### En Mac:
1. Inserta la microSD
2. Abre **Utilidad de Discos** (búscala en Spotlight con Cmd+Space)
3. Selecciona la tarjeta → click en **Borrar**
4. Configura:
   - **Formato:** MS-DOS (FAT)
   - **Esquema:** Master Boot Record
5. Click en **Borrar**

### En Linux:
```bash
# Reemplaza sdX con tu dispositivo (verificar con lsblk)
sudo mkfs.vfat -F 32 -s 8 /dev/sdX1
```

---

## Paso 2 — Crear la carpeta correcta en la SD

> ⚠️ **Este es el paso más importante y donde más gente falla.**
>
> Esta placa usa un chip STM32F4 que busca el firmware en una carpeta específica.  
> Si pones el archivo en la raíz de la SD (sin carpeta), **no pasará nada**.

1. Abre la tarjeta microSD recién formateada en el computador
2. Crea una carpeta nueva llamada exactamente:

```
STM32F4_UPDATE
```

> ⚠️ El nombre debe ser **exactamente así**: mayúsculas, guión bajo, sin espacios.  
> `STM32F4_UPDATE` ✅  
> `stm32f4_update` ❌  
> `STM32F4 UPDATE` ❌  
> `firmware` ❌

---

## Paso 3 — Copiar el firmware

1. Copia el archivo `firmware-20260529-153702.bin` dentro de la carpeta `STM32F4_UPDATE`
2. Puedes renombrarlo a `firmware.bin` si quieres (es opcional, funciona con cualquier nombre)

La estructura de la SD debe quedar así:
```
📁 microSD/
  └── 📁 STM32F4_UPDATE/
        └── 📄 firmware-20260529-153702.bin
```

---

## Paso 4 — Instalar el firmware en la impresora

1. **Apaga completamente** la impresora (interruptor trasero en OFF)
2. Inserta la microSD en la ranura de la impresora (parte inferior derecha de la placa)
3. **Enciende** la impresora
4. La pantalla mostrará una barra de progreso o parpadeará durante 10-30 segundos mientras instala
5. La impresora se reiniciará sola cuando termine
6. Verás la pantalla de inicio con **"Ender-5 Pro"** en español

> ✅ Si ves la pantalla de inicio normal → instalación exitosa  
> ❌ Si no pasa nada → revisa que la carpeta se llame exactamente `STM32F4_UPDATE`

---

## Paso 5 — Después de instalar (obligatorio)

Una vez que la impresora arranca con el nuevo firmware, **debes resetear la memoria interna** para evitar errores de configuración:

1. Conecta el computador a la impresora con un cable **Micro USB**
2. Abre cualquier programa de terminal serial (por ejemplo **Pronterface**, gratis en pronterface.com)
3. Conéctate al puerto COM/USB a **115200 baudios**
4. Envía estos dos comandos:

```
M502
M500
```

Esto borra la configuración vieja y guarda los valores correctos del firmware.

Si no tienes cable USB o programa serial, también puedes ir al menú:
**Configuración → Restablecer EEPROM** (si aparece esa opción).

---

## Paso 6 — Calibrar temperatura de la cama (recomendado)

Para que la cama caliente con precisión, ejecuta el autotune de PID. Por terminal serial:

```
M303 E-1 S60 C8
```

Espera ~5 minutos mientras hace el proceso automático. Al terminar guarda con:

```
M500
```

---

## ¿Algo salió mal?

### La pantalla no muestra nada / se congela
→ Verifica que la carpeta se llame `STM32F4_UPDATE` exactamente

### "Error EEPROM Version" al arrancar
→ Normal en la primera instalación. Envía `M502` + `M500` por USB

### La rueda (encoder) no funciona
→ Este firmware ya tiene corregido el problema de los pines. Si persiste, prueba con la versión anterior `firmware-20260529-151826.bin`

### La cama se calienta demasiado (mucho del valor pedido)
→ Ejecuta el autotune: `M303 E-1 S60 C8` seguido de `M500`

### La impresora hace homing en la dirección equivocada
→ El firmware está configurado para Ender-5 Pro (homing X e Y hacia MAX — el extremo). Si tu impresora es diferente, necesitas recompilar.

---

## Especificaciones del firmware

```
Marlin versión:    2.1.2.7
Placa:             BOARD_CREALITY_CR4NTxxC10
MCU:               STM32F401RET6 (STM32F4)
Environment:       STM32F401RE_creality
Idioma:            Español
Pantalla:          CR10_STOCKDISPLAY (RET6)
Drivers:           TMC2209_STANDALONE
Dimensiones X/Y/Z: 220 x 220 x 300 mm
Homing X:          MAX (derecha)
Homing Y:          MAX (fondo)
Homing Z:          MIN (cama baja)
Steps/mm:          X=80 Y=80 Z=800 E=93
PID Cama:          Activado
EEPROM Auto-init:  Activado
```

---

## ¿Quieres compilar el firmware tú mismo o modificarlo?

Lee el archivo `README_E3_FREERUNS_ENDER5PRO.md` en la carpeta raíz del repositorio — tiene la guía técnica completa de cómo se llegó a esta configuración y cómo recompilar desde cero.

---

*Firmware compilado y probado en Creality Ender-5 Pro con placa E3 Free-runs CR4NT220801C10.*
