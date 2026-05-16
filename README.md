# Ienta

[![CI](https://github.com/Jorgeto0/ienta/actions/workflows/ci.yml/badge.svg?branch=develop)](https://github.com/Jorgeto0/ienta/actions/workflows/ci.yml)

Autonomous energy intelligence system installed inside a residential or
industrial electrical panel. Measures, classifies, and predicts electrical
behavior entirely on-device using bare-metal C firmware on STM32 and an
edge AI pipeline.

---

## What it does

- **Measures** — voltage, current, real power, and power factor at 4 kHz
  with ±1–3% accuracy using a CT sensor and isolation transformer
- **Detects** — identifies individual appliances from their electrical
  load signatures using on-device machine learning (≥90% target accuracy)
- **Predicts** — flags degraded motors, thermal drift, and abnormal inrush
  current before failures occur
- **Optimizes** — learns per-home usage patterns and automates decisions
  at the panel level

---

## Architecture

```
AC Mains (220–240V)
       │
       ├── Isolation transformer ──► Voltage conditioning ──► ADC CH1
       └── CT sensor (SCT-013)  ──► Current conditioning ──► ADC CH0
                                               │
                                        STM32F401RE
                                   4 kHz · DMA · UART · bare-metal C
                                               │
                                       Edge AI Processor
                                  feature extraction · classifier
                                               │
                                          Application
                                   dashboard · alerts · relay
```

The STM32 firmware samples both channels at 4 kHz using timer-triggered
ADC and DMA double-buffering — the CPU never touches the ADC data path.
Every 20 ms (one AC cycle, 80 samples) a 14-byte packet is transmitted
over UART to the edge AI processor at 50 Hz. All intelligence runs
locally on-device.

---

## Firmware design

**Layered architecture**

```
┌─────────────────────────────────────┐
│  Application — main() state machine │
├─────────────────────────────────────┤
│  Platform-independent modules       │  ← no hardware includes
│  signal_processing · packet_builder │    testable on host with gcc
│  cbuf                               │
├─────────────────────────────────────┤
│  Hardware-dependent drivers         │  ← STM32-specific
│  adc_driver · uart_driver           │    changes only if chip changes
│  timer_driver · gpio_driver         │
├─────────────────────────────────────┤
│  IO / HAL — init only               │
├─────────────────────────────────────┤
│  STM32F401RE                        │
└─────────────────────────────────────┘
```

**Key decisions**
- Bare-metal C — no RTOS, no Arduino abstractions
- HAL used only for peripheral initialization — all hot paths and ISRs
  use direct register access
- No `malloc` — all memory statically allocated, auditable at compile time
- Double-buffer DMA — zero data loss at 4 kHz continuous acquisition
- Platform-independent math modules — unit-testable on host without hardware

See [`docs/firmware/`](docs/firmware/) for full architecture documentation.

---

## Repository structure

```
ienta/
├── firmware/                   # STM32 bare-metal firmware (C)
│   ├── Core/
│   │   ├── Inc/                # Headers: drivers, types, flags
│   │   └── Src/                # Source: drivers, signal processing, main
│   ├── Drivers/                # STM32 HAL — generated, never hand-edited
│   ├── Linker/                 # Linker script
│   └── Makefile
├── ai/                         # Device detection AI (Python)
│   ├── data/                   # Labeled training recordings
│   ├── models/                 # Trained model artifacts
│   └── training/               # Training scripts
├── hardware/                   # Circuit design
│   ├── schematics/
│   └── pcb/
├── tools/                      # Calibration and testing utilities
├── external/                   # Third-party submodules (none yet)
└── docs/                       # Engineering documentation
    ├── architecture/
    ├── firmware/
    ├── hardware/
    ├── ai/
    └── phases/                 # Per-phase technical decisions
```

---

## Technical specifications

| Parameter | Value |
|-----------|-------|
| MCU | STM32F401RE — Cortex-M4F, hardware FPU |
| Sample rate | 4 kHz (80 samples per 50 Hz AC cycle) |
| ADC | 12-bit, dual-channel simultaneous |
| Acquisition | DMA double-buffer — zero CPU overhead |
| Voltage accuracy | ±1% |
| Current accuracy | ±2% |
| Power accuracy | ±3% |
| AI classification | ≥90% target |
| Firmware cycle time | 20 ms |
| AI inference latency | <1 second |
| Current sensor | SCT-013-000 CT, 2000:1, 120Ω burden |
| Voltage sensing | 230V:9V isolation transformer |
| UART packet | 14 bytes · 115200 baud · 50 Hz |

---

## Building

```bash
cd firmware
make              # compile (dev profile, -Og)
make BUILD=debug  # compile with -O0
make BUILD=release # compile with -O2
make flash        # flash to board via ST-LINK
make check        # run cppcheck static analysis
make format       # apply clang-format to Core/
make format-check # verify formatting (used by CI)
make size         # print ELF section sizes
make clean        # wipe build artifacts
```

Install dependencies on macOS:
```bash
brew install armmbed/formulae/arm-none-eabi-gcc stlink cppcheck clang-format
```

---

## CI pipeline

Every push to `develop`, `feat/**`, `fix/**`, `chore/**` and every pull
request to `main` runs:

1. **Format check** — `clang-format --dry-run --Werror` on `Core/`
2. **Build** — `make BUILD=dev` with `arm-none-eabi-gcc`
3. **Static analysis** — `cppcheck` on `Core/Src` and `Core/Inc`
4. **Size report** — ELF section sizes printed to CI log

`main` is branch-protected. Merging requires CI to pass.

---

## Branch conventions

| Branch | Purpose |
|--------|---------|
| `main` | Stable — CI required, no direct push |
| `develop` | Integration branch |
| `feat/scope-description` | New features |
| `fix/scope-description` | Bug fixes |
| `chore/description` | Tooling, docs, config |

---

## Commit conventions

```
type(scope): short description

Body: explain why this change was made.
The code shows what — the commit explains why.

Types:  feat · fix · test · docs · refactor · chore
Scopes: adc · uart · dma · gpio · timer · signal · packet · makefile · ci · docs
```

---

## Development phases

| Phase | Name | Deliverable |
|-------|------|-------------|
| 0 | Foundations | Electrical theory, architecture, toolchain |
| 1 | Embedded system | STM32 firmware — ADC, DMA, UART, double-buffering |
| 2 | Energy measurement | Real sensors, RMS computation, calibration |
| 3 | Device detection | Data collection, ML model |
| 4 | Edge AI integration | End-to-end pipeline, latency benchmarks |
| 5 | Application layer | Dashboard, alerts, relay control |
| 6 | Product prototype | Panel-ready, watchdog, field-upgradeable firmware |

Phase decisions and outcomes: [`docs/phases/`](docs/phases/)

---

## Documentation

| Topic | File |
|-------|------|
| System overview | [`docs/architecture/SYSTEM_OVERVIEW.md`](docs/architecture/SYSTEM_OVERVIEW.md) |
| Firmware architecture | [`docs/firmware/FIRMWARE_ARCHITECTURE.md`](docs/firmware/FIRMWARE_ARCHITECTURE.md) |
| ADC and DMA | [`docs/firmware/ADC_AND_DMA.md`](docs/firmware/ADC_AND_DMA.md) |
| Coding standards | [`docs/firmware/CODING_STANDARDS.md`](docs/firmware/CODING_STANDARDS.md) |
| HAL vs bare-metal | [`docs/firmware/IMPLEMENTATION_PHILOSOPHY.md`](docs/firmware/IMPLEMENTATION_PHILOSOPHY.md) |
| Signal chain | [`docs/hardware/SIGNAL_CHAIN.md`](docs/hardware/SIGNAL_CHAIN.md) |
| AI system | [`docs/ai/AI_SYSTEM.md`](docs/ai/AI_SYSTEM.md) |

---

## License

To be determined.
