# Phase 1 — Embedded System

## Purpose

Implement the STM32 data acquisition pipeline using timer-triggered ADC,
DMA double-buffering, and UART transmission. No real sensors. No signal
processing. The goal is a correct, loss-free pipeline running continuously.

---

## Firmware modules

| Module | Responsibility |
|--------|---------------|
| adc_driver.c | Timer-triggered dual-channel ADC via DMA double-buffer |
| uart_driver.c | 14-byte packet transmission over USART2 |
| timer_driver.c | TIM2 configured to trigger ADC at exactly 4 kHz |
| gpio_driver.c | LED indication and relay control |
| cbuf.c | Generic circular byte buffer shared across drivers |
| ienta_types.h | Shared data structures: measurement_t, uart_packet_t |
| ienta_flags.h | Flag bit constants: VALID, ANOMALY, OVERRANGE, DEVICE |
| main.c | State machine: IDLE, SAMPLING, PROCESSING, TX_SEND, ERROR |

---

## ADC pipeline

TIM2 triggers ADC conversion every 250 us at 4 kHz. DMA transfers each
result directly to SRAM with no CPU involvement. On transfer-complete
interrupt the active buffer index swaps, DMA restarts on the fresh buffer,
and the main loop processes the idle buffer in parallel. Zero samples lost.

---

## UART packet format

14 bytes per packet transmitted at 50 Hz:

| Field | Type | Encoding |
|-------|------|----------|
| Start byte | uint8 | Always 0xAA |
| Voltage | uint16 | Vrms x 10 |
| Current | uint16 | Irms x 100 |
| Power | uint16 | Watts x 1 |
| Power factor | uint16 | PF x 100 |
| Flags | uint8 | Bit field |
| Reserved | uint16 | — |
| Checksum | uint8 | — |

---

## Validation

Two ADC channels sampled at 4 kHz, 80 samples per cycle, transmitted
at 50 Hz over UART. Verified with logic analyser or serial monitor.
No data loss under continuous operation.
