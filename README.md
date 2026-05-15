# Ienta — Autonomous Energy Intelligence System

An energy intelligence system installed inside a home's electrical panel.
Measures electricity in real time, detects individual devices from their
electrical signatures, predicts failures, and optimizes usage.

---

## What it does

- **Measures** — voltage, current, real power, power factor at 4 kHz
- **Detects** — identifies individual devices from electrical signatures (edge AI)
- **Predicts** — flags anomalies and failure patterns before they happen
- **Optimizes** — automates energy usage based on learned behavior

---

## Current status

| Phase | Name | Status |
|-------|------|--------|
| 0 | Foundations | ✅ Complete |
| 1 | Embedded system (STM32 firmware) | 🔄 In progress |
| 2 | Energy measurement engine | ⏳ Pending |
| 3 | Device detection AI | ⏳ Pending |
| 4 | Edge AI integration | ⏳ Pending |
| 5 | Application layer | ⏳ Pending |
| 6 | Product prototype | ⏳ Pending |

---

## Repository structure

```
ienta/
├── firmware/           # STM32 embedded firmware (C, bare-metal)
│   ├── Core/
│   │   ├── Inc/        # Headers — drivers, types, flags
│   │   └── Src/        # Source — drivers, signal processing, main
│   ├── Drivers/        # STM32 HAL (generated, never manually edited)
│   ├── Linker/         # Linker script
│   └── Makefile
├── ai/                 # Device detection and anomaly AI (Python)
│   ├── data/
│   ├── models/
│   └── training/
├── hardware/           # Schematics and PCB design
│   ├── schematics/
│   └── pcb/
├── tools/              # Calibration and testing utilities
│   ├── calibration/
│   └── testing/
└── docs/               # Engineering documentation
```

---

## Technical specifications

| Parameter | Value |
|-----------|-------|
| Sample rate | 4 kHz (80 samples per 50 Hz AC cycle) |
| ADC resolution | 12-bit, dual-channel |
| Voltage accuracy | ±1% |
| Current accuracy | ±2% |
| Power accuracy | ±3% |
| AI classification | ≥90% |
| Firmware latency | <20 ms per cycle |
| AI inference | <1 second |

---

## Hardware platform

| Component | Choice |
|-----------|--------|
| MCU | STM32F4xx (Nucleo-F446RE or F401RE) |
| Current sensor | SCT-013-000 CT sensor |
| Voltage sensing | 230V:9V isolation transformer |
| Edge AI processor | TBD at Phase 4 |

---

## Documentation

Full engineering documentation in [docs/](docs/README.md).

| Topic | File |
|-------|------|
| System overview | [docs/architecture/SYSTEM_OVERVIEW.md](docs/architecture/SYSTEM_OVERVIEW.md) |
| Firmware architecture | [docs/firmware/FIRMWARE_ARCHITECTURE.md](docs/firmware/FIRMWARE_ARCHITECTURE.md) |
| ADC and DMA | [docs/firmware/ADC_AND_DMA.md](docs/firmware/ADC_AND_DMA.md) |
| Coding standards | [docs/firmware/CODING_STANDARDS.md](docs/firmware/CODING_STANDARDS.md) |
| Signal chain | [docs/hardware/SIGNAL_CHAIN.md](docs/hardware/SIGNAL_CHAIN.md) |
| AI system | [docs/ai/AI_SYSTEM.md](docs/ai/AI_SYSTEM.md) |
| Phase log | [docs/PHASE_LOG.md](docs/PHASE_LOG.md) |

---

## Building the firmware

```bash
cd firmware
make        # compile
make flash  # flash to board via ST-LINK
make clean  # clean build artifacts
```

*Full build instructions added in Phase 1.*

---

## License

To be determined.
