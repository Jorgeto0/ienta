# Phase 1 — Embedded System

## Purpose

Implement and validate the STM32 data acquisition pipeline.
Proves that the firmware can sample two ADC channels at 4 kHz using
DMA double-buffering and transmit structured packets over UART —
continuously, with zero data loss.

No real sensors. No signal processing. Pipeline correctness only.

---

## Scope

### Firmware modules introduced

| Module | Responsibility |
|--------|---------------|
|  | Timer-triggered dual-channel ADC via DMA double-buffer |
|  | 14-byte packet transmission over USART2 |
|  | TIM2 configured to trigger ADC at exactly 4 kHz |
|  | LED indication and relay control |
|  | Generic circular byte buffer shared across drivers |
|  | Shared data structures: measurement_t, uart_packet_t |
|  | Flag bit constants: VALID, ANOMALY, OVERRANGE, DEVICE |
|  | Top-level state machine: IDLE → SAMPLING → PROCESSING → TX |

### ADC pipeline

Timer TIM2 triggers ADC conversion every 250 µs (4 kHz).
DMA transfers each result directly to SRAM — no CPU involvement.
On transfer-complete interrupt: swap active buffer, restart DMA,
signal main loop. Main loop processes the idle buffer in ~1.5 ms
while DMA fills the fresh one in parallel.

### UART packet format

14 bytes per packet, 50 packets per second:

| Field | Type | Scale |
|-------|------|-------|
| Start byte | uint8 | 0xAA |
| Voltage | uint16 | Vrms x 10 |
| Current | uint16 | Irms x 100 |
| Power | uint16 | Watts x 1 |
| Power factor | uint16 | PF x 100 |
| Flags | uint8 | Bit field |
| Reserved | uint16 | — |
| Checksum | uint8 | — |

### Exit criterion

80 samples per channel, 50 cycles per second, zero data loss, 1+ hour continuous.
