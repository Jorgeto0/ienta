# Ienta — Documentation

Engineering documentation for the Ienta autonomous energy intelligence system.
Each file covers exactly one topic. Start with the index below.

---

## Architecture

| File | Contents |
|------|----------|
| [architecture/SYSTEM_OVERVIEW.md](architecture/SYSTEM_OVERVIEW.md) | Full system stack, signal chain, data flow, performance requirements |

## Firmware

| File | Contents |
|------|----------|
| [firmware/FIRMWARE_ARCHITECTURE.md](firmware/FIRMWARE_ARCHITECTURE.md) | Layered pattern, module responsibilities, decoupling rule, public interfaces |
| [firmware/ADC_AND_DMA.md](firmware/ADC_AND_DMA.md) | ADC state machine, double-buffer pattern, ISR design, timing budget |
| [firmware/CODING_STANDARDS.md](firmware/CODING_STANDARDS.md) | Mandatory coding rules with correct and incorrect examples |
| [firmware/IMPLEMENTATION_PHILOSOPHY.md](firmware/IMPLEMENTATION_PHILOSOPHY.md) | HAL vs bare-metal decision rules and per-function decision table |

## Hardware

| File | Contents |
|------|----------|
| [hardware/SIGNAL_CHAIN.md](hardware/SIGNAL_CHAIN.md) | Sensors, signal conditioning, isolation, component values, safety |

## AI

| File | Contents |
|------|----------|
| [ai/AI_SYSTEM.md](ai/AI_SYSTEM.md) | Problem definition, feature extraction, model requirements, data strategy |

## Project Log

| File | Contents |
|------|----------|
| [PHASE_LOG.md](PHASE_LOG.md) | Running log of every phase — decisions made, problems hit, outcomes |

---

## Navigation guide

| I need to... | Read |
|---|---|
| Understand the full system | `architecture/SYSTEM_OVERVIEW.md` |
| Work on firmware modules | `firmware/FIRMWARE_ARCHITECTURE.md` |
| Understand ADC / DMA | `firmware/ADC_AND_DMA.md` |
| Decide HAL vs bare-metal | `firmware/IMPLEMENTATION_PHILOSOPHY.md` |
| Write or review C code | `firmware/CODING_STANDARDS.md` |
| Work on sensors / analog | `hardware/SIGNAL_CHAIN.md` |
| Work on AI pipeline | `ai/AI_SYSTEM.md` |
| See project history | `PHASE_LOG.md` |
