/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Creality E3 Free-runs (CR4NT220801C10) (STM32F401RET6) board pin assignments.
 * Drop-in replacement for Ender-5 Pro / Ender-3 series with V4.2.x boards.
 * Uses same MCU pin assignments as the V4.2.7 (STM32F103RE) board.
 */

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Creality E3 Free-runs"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "Ender-5 Pro"
#endif

#define DISABLE_DEBUG false  // DISABLE_(DEBUG|JTAG) not supported for STM32F4
#define ALLOW_STM32F4

// V4.2.7-compatible STEP/DIR pin assignments
#define X_STEP_PIN                          PB9
#define X_DIR_PIN                           PC2

#define Y_STEP_PIN                          PB7
#define Y_DIR_PIN                           PB8

#define Z_STEP_PIN                          PB5
#define Z_DIR_PIN                           PB6

#define E0_STEP_PIN                         PB3
#define E0_DIR_PIN                          PB4

#include "../stm32f1/pins_CREALITY_V4.h"
