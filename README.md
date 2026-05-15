# Ienta

Ienta is an autonomous energy intelligence system that installs inside a
residential or industrial electrical panel. It measures, classifies, and
predicts electrical behavior entirely on-device — no cloud required for
core operation.

---

## What it does

Ienta turns a standard electrical panel into an intelligent system that
knows what devices are running, how much energy they consume, and when
something is about to fail.

- **Real-time measurement** — voltage, current, real power, and power
  factor sampled at 4 kHz with ±1–3% accuracy
- **Device detection** — identifies individual appliances from their
  electrical load signature using edge AI (≥90% accuracy target)
- **Anomaly detection** — flags degraded motors, thermal drift, and
  abnormal inrush before they become failures
- **Energy optimization** — learns per-home usage patterns and automates
  decisions at the panel level

---

## Architecture

```
AC Mains (220–240V)
       │
       ├── Isolation transformer ──► Voltage conditioning
       └── CT sensor (SCT-013) ───► Current conditioning
                                           │
                                    STM32F4xx MCU
                              (4 kHz ADC · DMA · UART)
                                           │
                                   Edge AI Processor
                            (feature extraction · classifier)
                                           │
                                     Application
                               (dashboard · alerts · relay)
```

The STM32 firmware acquires 80 samples per AC cycle at 4 kHz using
timer-triggered ADC and DMA double-buffering. It transmits a 14-byte
packet 50 times per second to the edge AI processor over UART.
All intelligence runs locally on-device.

---

## Repository structure

```
ienta/
├── firmware/                   # STM32 embedded firmware (C, bare-metal)
│   ├── Core/
│   │   ├── Inc/                # Headers: drivers, types, flags
│   │   └── Src/                # Source: drivers, signal processing, main
│   ├── Drivers/                # STM32 HAL — generated, never hand-edited
│   ├── Linker/                 # Linker script
│   └── Makefile
│
├── ai/                         # Device detection AI (Python)
│   ├── data/                   # Labeled training recordings
│   ├── models/                 # Trained model artifacts
│   └── training/               # Training scripts and notebooks
│
├── hardware/                   # Circuit design
│   ├── schematics/
│   └── pcb/
│
├── tools/                      # Development utilities
│   ├── calibration/            # Sensor calibration scripts
│   └── testing/                # Automated test scripts
│
└── docs/                       # Engineering documentation
    ├── architecture/
    ├── firmware/
    ├── hardware/
    ├── ai/
    └── phases/                 # Per-phase design and decisions
```

---

## Technical specifications

| Parameter | Value |
|-----------|-------|
| MCU | STM32F4xx — Cortex-M4F with hardware FPU |
| Sample rate | 4 kHz (80 samples per 50 Hz AC cycle) |
| ADC | 12-bit, dual-channel simultaneous |
| Voltage accuracy | ±1% |
| Current accuracy | ±2% |
| Power accuracy | ±3% |
| AI classification accuracy | ≥90% |
| Firmware cycle time | 20 ms |
| AI inference latency | <1 second |
| Current sensor | SCT-013-000 CT, 2000:1, 120Ω burden |
| Voltage sensing | 230V:9V isolation transformer |
| UART packet | 14 bytes at 115200 baud, 50 Hz |

---

## Firmware design principles

- **Bare-metal C** — no RTOS, no Arduino abstractions
- **HAL at init only** — ADC+DMA and UART initialization use STM32 HAL;
  all hot paths and ISRs are direct register access
- **Static allocation** — no `malloc` anywhere; all memory is known at
  compile time
- **Layered architecture** — platform-independent signal processing sits
  above hardware-dependent drivers; math modules are testable on host

See [`docs/firmware/`](docs/firmware/) for full architecture documentation.

---

## Development phases

| Phase | Name | Deliverable |
|-------|------|-------------|
| 0 | Foundations | Electrical theory, C fundamentals, architecture design |
| 1 | Embedded system | STM32 firmware — ADC, DMA, UART, double buffering |
| 2 | Energy measurement | Real sensors, RMS computation, calibration |
| 3 | Device detection | Data collection, ML model training |
| 4 | Edge AI integration | End-to-end pipeline, latency benchmarks |
| 5 | Application layer | Dashboard, alerts, relay control |
| 6 | Product prototype | Panel-ready hardware, watchdog, field-upgradeable firmware |

Phase logs with decisions, problems, and outcomes:
[`docs/phases/`](docs/phases/)

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

## Contributing

This repository follows conventional commits:

```
type(scope): short description

Types: feat · fix · test · docs · refactor · chore
Scopes: adc · uart · dma · gpio · timer · signal · packet · ai · docs

Examples:
  feat(adc): timer-triggered dual-channel sampling at 4kHz
  fix(dma): correct buffer index swap in transfer-complete ISR
  test(uart): verify 14-byte packet framing at 115200 baud
  docs(firmware): add ADC state machine diagram
```

One logical change per commit. Commit messages describe the solution,
not the problem.

---

## License

To be determined.
