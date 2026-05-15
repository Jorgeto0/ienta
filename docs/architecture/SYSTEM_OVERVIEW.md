# System Overview

Ienta is an autonomous energy intelligence system installed inside a
residential or industrial electrical panel. It measures, detects,
optimizes, and predicts electrical behavior entirely on-device.

---

## System stack

```
+-------------------------------------+
|  Application Layer                  |
|  Dashboard · alerts · relay control |
+-------------------------------------+
|  Edge AI Processor                  |
|  Classification · anomaly detection |
+-------------------------------------+
|  STM32 Firmware                     |
|  ADC · DMA · signal processing      |
|  UART · state machine               |
+-------------------------------------+
|  Analog Front-End                   |
|  CT sensor · isolation transformer  |
|  Signal conditioning · AAF          |
+-------------------------------------+
|  AC Mains 220-240V                  |
+-------------------------------------+
```

---

## Signal chain

```
AC mains (220-240V RMS)
  |
  +--► Isolation transformer (230V:9V)
  |       + voltage divider + 1.65V bias
  |       → 0-3.3V centered at 1.65V
  |       → STM32 ADC Channel 1 (voltage)
  |
  +--► CT sensor (SCT-013-000, 2000:1)
          + 120 ohm burden + 1.65V bias
          → 0-3.3V centered at 1.65V
          → STM32 ADC Channel 0 (current)
```

Both paths are galvanically isolated from mains.

---

## Data flow

```
ADC (4 kHz, dual-channel)
  | DMA - zero CPU overhead
  v
SRAM double-buffer (80 samples x 2 channels)
  | ISR swap on transfer-complete
  v
Signal processing - Vrms, Irms, P, PF (~1.5 ms)
  |
  v
uart_packet_t (14 bytes at 50 Hz over USART2)
  |
  v
Edge AI processor - feature extraction → classifier
  |
  v
Application - JSON predictions
```

---

## Performance requirements

| Metric | Requirement |
|--------|-------------|
| Sample rate | 4 kHz |
| Samples per AC cycle | 80 |
| Data loss | 0% |
| CPU utilization | < 20% |
| AI inference latency | < 1 second |
| Device operating life | 10+ years |

---

## Interfaces

| Link | Protocol | Rate |
|------|----------|------|
| STM32 to Edge AI | UART 115200 baud | 50 packets/sec |
| Edge AI to App | Local JSON API | On event |
