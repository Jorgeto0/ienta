# Ienta — Autonomous Energy Intelligence System

**Privacy-first edge AI for luxury residential energy management**

## Overview

Ienta is an autonomous energy intelligence system that operates entirely locally within a home's electrical panel. It provides real-time energy measurement, AI-powered device detection, and predictive failure analysis without cloud dependency.

## Project Status

**Current Phase:** Phase 0 — Foundations  
**Hardware:** Not yet acquired  
**Development Environment:** In setup

## System Architecture

```
AC Mains → CT Sensors → Signal Conditioning → STM32 MCU → Edge AI Processor → Application
```

### Key Components
- **Firmware (C):** Real-time data acquisition and processing on STM32
- **AI Engine (Python):** Device classification and anomaly detection
- **Application:** User interface and control

## Repository Structure

```
ienta/
├── docs/           # System documentation
├── firmware/       # STM32 embedded code (C)
├── ai/            # Machine learning components (Python)
├── tools/         # Utilities and scripts
└── hardware/      # Circuit designs and schematics
```

## Technical Specifications

### Performance Requirements
- Sampling rate: ≥5kHz
- Voltage accuracy: ±1%
- Current accuracy: ±2%
- Power accuracy: ±3%
- AI classification: ≥90%
- Latency: <1 second

### Hardware Platform
- MCU: STM32 (≥12-bit ADC)
- Sensors: CT current transformers (±1%)
- Edge AI: TBD (likely Raspberry Pi)

## Development Phases

- **Phase 0:** Foundations 
- **Phase 1:** Embedded system development
- **Phase 2:** Energy measurement engine
- **Phase 3:** AI device detection
- **Phase 4:** Edge AI integration
- **Phase 5:** Application layer
- **Phase 6:** Product prototype

## Documentation

See `docs/` directory for:
- System architecture
- Firmware design
- AI model documentation
- Hardware specifications

## Building

*Build instructions will be added in Phase 1*

## License

*To be determined*

## Contact

*Project maintainer information*
