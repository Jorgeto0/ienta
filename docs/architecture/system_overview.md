# System Overview

Ienta is an autonomous energy intelligence system installed inside a
residential or industrial electrical panel. It measures, detects,
optimizes, and predicts — at the source, in real time, with the accuracy
and longevity required to operate for 10+ years without attention.

---

## System stack

```
┌─────────────────────────────────────┐
│  Application Layer                  │
│  Dashboard · alerts · relay control │
├─────────────────────────────────────┤
│  Edge AI Processor                  │
│  Classification · anomaly detection │
├─────────────────────────────────────┤
│  STM32 Firmware                     │
│  ADC · DMA · signal processing      │
│  UART · state machine               │
├─────────────────────────────────────┤
│  Analog Front-End                   │
│  CT sensor · isolation transformer  │
│  Signal conditioning · AAF          │
├─────────────────────────────────────┤
│  AC Mains — 220–240V                │
└─────────────────────────────────────┘
```

---

## Signal chain

```
AC mains (220–240V RMS)
  │
  ├──► Isolation transformer (230V:9V)
  │       + voltage divider + 1.65V bias
  │       → 0–3.3V centered at 1.65V
  │       → STM32 ADC Channel 1 (voltage)
  │
  └──► CT sensor (SCT-013-000, 2000:1)
          + 120Ω burden + 1.65V bias
          → 0–3.3V centered at 1.65V
          → STM32 ADC Channel 0 (current)
```

Both paths are galvanically isolated from mains. No electrical path
between AC conductors and the MCU.

---

## Data flow

```
ADC (4 kHz, dual-channel)
  │ DMA — zero CPU overhead
  ▼
SRAM double-buffer (80 samples × 2 channels)
  │ ISR swap on transfer-complete
  ▼
Signal processing — Vrms, Irms, P, PF (~1.5 ms)
  │
  ▼
uart_packet_t (14 bytes @ 50 Hz over USART2)
  │
  ▼
Edge AI processor — feature extraction → classifier
  │
  ▼
Application — JSON predictions
```

---

## Performance requirements

| Metric | Requirement |
|--------|-------------|
| Sample rate | 4 kHz |
| Samples per AC cycle | 80 |
| Data loss | 0% |
| CPU utilization (firmware) | < 20% |
| AI inference latency | < 1 second |
| Device operating life | 10+ years |

---

## Interfaces

| Link | Protocol | Rate |
|------|----------|------|
| STM32 → Edge AI | UART 115200 baud | 50 packets/sec |
| Edge AI → App | Local JSON API | On event |

---

## Development phases

| Phase | Deliverable |
|-------|-------------|
| 0 | Foundations — theory, architecture, toolchain |
| 1 | STM32 firmware — ADC, DMA, UART, double buffering |
| 2 | Energy measurement — real sensors, RMS, calibration |
| 3 | Device detection — data collection, ML model |
| 4 | Edge AI integration — end-to-end pipeline |
| 5 | Application layer — dashboard, alerts |
| 6 | Product prototype — panel-ready, field-upgradeable |
