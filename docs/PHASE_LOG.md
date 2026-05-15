# Ienta — Phase Log

Running record of every phase. Add an entry when a phase or significant
step is completed. Include: what was done, decisions made, problems hit,
and how they were resolved.

---

## Phase 0 — Foundations
**Status:** Complete
**Completed:** 2026-04-25

### What was done
- Learned AC/DC fundamentals, RMS, power factor, harmonics, transients
- Learned CT sensor design (turns ratio, burden resistor, isolation)
- Learned why isolation transformer is mandatory for voltage sensing
- Designed system architecture and data contracts
- Set up GitHub repository and folder structure
- Defined firmware module structure and layered architecture pattern

### Decisions made
| Decision | Value | Reason |
|----------|-------|--------|
| Sample rate | 4 kHz | 80 samples per 50 Hz cycle — sufficient for harmonic analysis |
| ADC resolution | 12-bit dual-channel | Enough dynamic range; simultaneous V+I sampling |
| Acquisition method | DMA + double buffer | Zero CPU overhead, zero data loss |
| CT sensor | SCT-013-000, 120Ω burden | 73% ADC utilization at 20A — good headroom |
| Voltage sensing | Isolation transformer 230V:9V | Safety — no electrical path to MCU |
| Toolchain | arm-none-eabi-gcc + Makefile | Full control, no CubeIDE dependency |
| HAL usage | Init only (ADC+DMA, UART) | Reliability at boot; bare-metal on hot paths |

### Problems hit
None — Phase 0 was foundations and design only.

---

## Phase 1 — Embedded System
**Status:** In progress
**Started:** TBD

### Steps completed
<!-- Add entries as steps are finished -->
<!-- Format:
### Step N — Name
**Date:** YYYY-MM-DD
**Commit:** `type(scope): description`
**What worked:**
**Problems hit and how resolved:**
-->

---
<!-- Add Phase 2+ entries here as they are completed -->
