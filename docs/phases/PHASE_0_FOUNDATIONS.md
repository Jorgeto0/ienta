# Phase 0 - Foundations
**Status:** Complete
**Completed:** 2026-04-25

---

## Goal

Build the theoretical and toolchain foundation required to write correct
embedded firmware for Ienta. No hardware. No code.

---

## What was completed

### Electrical fundamentals
- AC mains: voltage, current, frequency, sine wave behaviour
- RMS: mathematical definition and significance for energy measurement
- Real power vs apparent power, power factor, reactive loads
- Harmonics: THD and why devices have electrical fingerprints
- Transients: inrush current, startup signatures
- CT sensor: turns ratio, burden resistor, galvanic isolation
- Isolation transformer: why a resistive divider is unsafe at mains voltage

### System architecture
- Layered firmware pattern: APP → LOGIC → DRIVERS → IO → HW
- Two-zone split: platform-independent (green) vs hardware-dependent (gray)
- Decoupling rule: no hardware includes in signal processing modules
- Double-buffer pattern for zero-loss DMA acquisition
- Data contracts: measurement_t and uart_packet_t
- Module responsibilities and public interfaces defined

### Toolchain
- arm-none-eabi-gcc + Makefile, no CubeIDE
- STM32 HAL: selective use at init only
- GitHub repository initialized with folder structure

---

## Decisions locked

| Decision | Value | Reason |
|----------|-------|--------|
| Sample rate | 4 kHz | 80 samples per 50 Hz cycle |
| Samples per cycle | 80 | 4000 / 50 Hz |
| ADC | 12-bit dual-channel | Simultaneous V+I, enough dynamic range |
| Acquisition | DMA + double buffer | Zero CPU overhead, zero data loss |
| CT sensor | SCT-013-000 2000:1 120 ohm burden | 73% ADC utilization at 20A |
| Voltage sensing | Isolation transformer 230V:9V | Safety |
| Toolchain | arm-none-eabi-gcc + Makefile | Full control |
| HAL usage | Init only | Reliability at boot, bare-metal on hot paths |

---

## Problems hit

None. Phase 0 was design and learning only.
