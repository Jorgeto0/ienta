# Ienta

[![CI](https://github.com/Jorgeto0/ienta/actions/workflows/ci.yml/badge.svg?branch=develop)](https://github.com/Jorgeto0/ienta/actions/workflows/ci.yml)

Autonomous energy intelligence system installed inside a residential or
industrial electrical panel. Measures, classifies, and predicts electrical
behavior entirely on-device using bare-metal C firmware on STM32 and an
edge AI pipeline.

---

## What it does

- **Measures** — voltage, current, real power, and power factor at 4 kHz
- **Detects** — identifies individual appliances from electrical load signatures using on-device ML
- **Predicts** — flags degraded motors, thermal drift, and abnormal inrush before failures occur
- **Optimizes** — learns per-home usage patterns and automates panel-level decisions

---

## Architecture

The STM32 firmware samples voltage and current at 4 kHz using timer-triggered
ADC and DMA double-buffering. The CPU never touches the ADC data path. Every
20 ms a 14-byte UART packet is transmitted to the edge AI processor at 50 Hz.
All intelligence runs locally on-device.

---

## Firmware design

Layered architecture — platform-independent modules sit above hardware-dependent
drivers. Math modules have zero hardware includes and are unit-testable on host.

- Bare-metal C. No RTOS. No Arduino abstractions.
- HAL used only for peripheral initialization. All hot paths use direct registers.
- No malloc. All memory statically allocated, auditable at compile time.
- Double-buffer DMA. Zero data loss at 4 kHz continuous acquisition.

See docs/firmware/ for full architecture documentation.

---

## Technical specifications

| Parameter | Value |
|-----------|-------|
| MCU | STM32F401RE Cortex-M4F with hardware FPU |
| Sample rate | 4 kHz (80 samples per 50 Hz AC cycle) |
| ADC | 12-bit dual-channel simultaneous |
| Acquisition | DMA double-buffer, zero CPU overhead |
| Voltage accuracy | ±1% |
| Current accuracy | ±2% |
| Power accuracy | ±3% |
| AI classification | ≥90% target |
| Firmware cycle time | 20 ms |
| AI inference latency | <1 second |
| Current sensor | SCT-013-000 CT 2000:1 120Ω burden |
| Voltage sensing | 230V:9V isolation transformer |
| UART packet | 14 bytes at 115200 baud at 50 Hz |

---

## Building

```bash
cd firmware
make               # compile dev profile -Og
make BUILD=debug   # compile -O0
make BUILD=release # compile -O2
make flash         # flash via ST-LINK
make check         # cppcheck static analysis
make format        # apply clang-format to Core/
make format-check  # verify formatting used by CI
make size          # print ELF section sizes
make clean         # wipe build artifacts
```

Install on macOS:

```bash
brew install armmbed/formulae/arm-none-eabi-gcc stlink cppcheck clang-format
```

---

## CI pipeline

Every push to develop, feat/**, fix/**, chore/** and every PR to main runs:

1. Format check — clang-format --dry-run --Werror on Core/
2. Build — make BUILD=dev with arm-none-eabi-gcc
3. Static analysis — cppcheck on Core/Src and Core/Inc
4. Size report — ELF section sizes printed to CI log

main is branch-protected. Merging requires CI to pass.

---

## Branch conventions

| Branch | Purpose |
|--------|---------|
| main | Stable, CI required, no direct push |
| develop | Integration branch |
| feat/scope-description | New features |
| fix/scope-description | Bug fixes |
| chore/description | Tooling, docs, config |

---

## Commit conventions

Every commit has a title and a body. Title says what. Body says why.

Types: feat, fix, test, docs, refactor, chore
Scopes: adc, uart, dma, gpio, timer, signal, packet, makefile, ci, docs

---

## Development phases

| Phase | Name | Deliverable |
|-------|------|-------------|
| 0 | Foundations | Electrical theory, architecture, toolchain |
| 1 | Embedded system | STM32 firmware: ADC, DMA, UART, double-buffering |
| 2 | Energy measurement | Real sensors, RMS computation, calibration |
| 3 | Device detection | Data collection, ML model |
| 4 | Edge AI integration | End-to-end pipeline, latency benchmarks |
| 5 | Application layer | Dashboard, alerts, relay control |
| 6 | Product prototype | Panel-ready, watchdog, field-upgradeable firmware |

Phase decisions and outcomes: docs/phases/

---

## Documentation

| Topic | File |
|-------|------|
| System overview | docs/architecture/SYSTEM_OVERVIEW.md |
| Firmware architecture | docs/firmware/FIRMWARE_ARCHITECTURE.md |
| ADC and DMA | docs/firmware/ADC_AND_DMA.md |
| Coding standards | docs/firmware/CODING_STANDARDS.md |
| HAL vs bare-metal | docs/firmware/IMPLEMENTATION_PHILOSOPHY.md |
| Signal chain | docs/hardware/SIGNAL_CHAIN.md |
| AI system | docs/ai/AI_SYSTEM.md |

---

## License

To be determined.
