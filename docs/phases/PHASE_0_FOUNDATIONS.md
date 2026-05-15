# Phase 0 — Foundations

## Purpose

Establish the theoretical and toolchain foundation for Ienta firmware.
Covers electrical fundamentals, system architecture design, and build
system setup. No hardware. No code written.

---

## Scope

### Electrical fundamentals
- AC mains behaviour: voltage, current, frequency, sine wave
- RMS: mathematical definition and application to energy measurement
- Real power vs apparent power, power factor, reactive loads
- Harmonics: total harmonic distortion, device fingerprints
- Transients: inrush current, startup signatures
- CT sensor design: turns ratio, burden resistor, galvanic isolation
- Isolation transformer: why a resistive divider is unsafe at mains voltage

### Architecture decisions

| Decision | Value | Reason |
|----------|-------|--------|
| Sample rate | 4 kHz | 80 samples per 50 Hz cycle — sufficient for harmonic analysis |
| Samples per cycle | 80 | 4000 / 50 Hz |
| ADC | 12-bit dual-channel | Simultaneous V+I, sufficient dynamic range |
| Acquisition | DMA + double buffer | Zero CPU overhead, zero data loss |
| CT sensor | SCT-013-000, 2000:1, 120 ohm burden | 73% ADC utilization at 20A |
| Voltage sensing | Isolation transformer 230V:9V | No electrical path to MCU |
| Toolchain | arm-none-eabi-gcc + Makefile | Reproducible builds, no IDE dependency |
| HAL usage | Initialization only | Deterministic hot paths via direct registers |
