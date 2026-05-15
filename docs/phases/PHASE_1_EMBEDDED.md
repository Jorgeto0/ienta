# Phase 1 - Embedded System
**Status:** In progress
**Started:** TBD

---

## Goal

Prove the full data acquisition pipeline on real hardware.
No real sensors yet. The pipeline itself is the deliverable.

---

## Exit criterion

STM32 samples 80 values per channel (voltage + current), 50 times per
second, continuously, with zero data loss, for 1+ hour.

---

## What this phase does NOT include

- No CT sensor or isolation transformer (Phase 2)
- No RMS or power calculations (Phase 2)
- No real AC signal - test signal only
- No AI (Phase 3+)

---

## Hardware

STM32 Nucleo-F446RE or F401RE. ST-LINK via USB. Nothing else.

---

## Steps

| Step | Name | Status | Commit |
|------|------|--------|--------|
| 1 | Hardware arrival + inspection | Pending | |
| 2 | Environment setup + blink LED | Pending | |
| 3 | UART communication to Mac | Pending | |
| 4 | ADC single-channel polling | Pending | |
| 5 | Timer-triggered ADC at 4 kHz | Pending | |
| 6 | DMA configuration | Pending | |
| 7 | Double buffering | Pending | |
| 8 | Dual-channel ADC V + I simultaneous | Pending | |
| 9 | UART packet transmission | Pending | |
| 10 | Stress test 1 hour continuous | Pending | |
| 11 | Code cleanup + documentation | Pending | |

---

## Files added this phase

| File | Purpose |
|------|---------|
| firmware/Core/Inc/ienta_types.h | measurement_t and uart_packet_t structs |
| firmware/Core/Inc/ienta_flags.h | Flag bit constants |
| firmware/Core/Inc/cbuf.h | Circular buffer interface |
| firmware/Core/Src/cbuf.c | Circular buffer implementation |
| firmware/Core/Inc/adc_driver.h | ADC driver interface |
| firmware/Core/Src/adc_driver.c | ADC + DMA acquisition |
| firmware/Core/Inc/uart_driver.h | UART driver interface |
| firmware/Core/Src/uart_driver.c | UART packet transmission |
| firmware/Core/Inc/timer_driver.h | Timer driver interface |
| firmware/Core/Src/timer_driver.c | TIM2 4 kHz trigger |
| firmware/Core/Inc/gpio_driver.h | GPIO driver interface |
| firmware/Core/Src/gpio_driver.c | LED and relay control |
| firmware/Core/Src/main.c | State machine |

---

## Decisions made this phase

| Decision | Value | Reason |
|----------|-------|--------|

---

## Problems hit and resolved

