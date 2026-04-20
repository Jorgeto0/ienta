# Ienta System Architecture

## 1. High-Level Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                         AC Mains (240V)                         │
└────────────────────────┬────────────────────────────────────────┘
                         │
                         ▼
            ┌────────────────────────┐
            │   Current Transformer  │  (Non-invasive clamp)
            │      (CT Sensor)       │
            └───────────┬────────────┘
                        │
                        ▼
            ┌────────────────────────┐
            │  Signal Conditioning   │  (Burden resistor, bias, filter)
            │   & Voltage Divider    │
            └───────────┬────────────┘
                        │
                        ▼
            ┌────────────────────────┐
            │    STM32 MCU           │  ← Firmware (C)
            │  - 12-bit ADC          │
            │  - 5kHz+ sampling      │
            │  - Feature extraction  │
            └───────────┬────────────┘
                        │
                        ▼
            ┌────────────────────────┐
            │   Edge AI Processor    │  ← AI Engine (Python)
            │  - Device detection    │
            │  - Anomaly detection   │
            │  - Optimization        │
            └───────────┬────────────┘
                        │
                        ▼
            ┌────────────────────────┐
            │   Application Layer    │
            │  - Dashboard           │
            │  - Alerts              │
            │  - Control             │
            └────────────────────────┘
```

## 2. Signal Chain Detail

### 2.1 Current Measurement Path
```
AC Current → CT Sensor → Burden Resistor → Anti-alias Filter → ADC Channel 0
                         (I = V/R)          (Low-pass)
```

### 2.2 Voltage Measurement Path
```
AC Voltage → Isolation Transformer → Resistor Divider → Anti-alias Filter → ADC Channel 1
             (SAFETY CRITICAL)       (Scale down)        (Low-pass)
```

### 2.3 ADC Sampling
- **Dual-channel simultaneous sampling** (voltage + current)
- **DMA-based continuous acquisition** (no CPU overhead)
- **Double buffering** (process one buffer while filling the other)

## 3. Data Flow

```
Raw ADC samples (uint16_t)
    ↓
Circular buffer (80 samples = 1 AC cycle @ 50Hz, 4kHz sampling)
    ↓
Feature extraction (firmware):
  - RMS voltage
  - RMS current
  - Real power
  - Power factor
  - Harmonics (optional)
    ↓
UART transmission to Edge AI
    ↓
AI processing:
  - Device classification
  - Anomaly detection
  - Pattern recognition
    ↓
Application output:
  - Device identification
  - Energy consumption
  - Predictions
  - Alerts
```

## 4. Firmware Architecture

### 4.1 Module Breakdown

```
firmware/
├── Core/
│   ├── Inc/
│   │   ├── main.h                 # Main application header
│   │   ├── adc_driver.h           # ADC configuration & control
│   │   ├── signal_processing.h   # RMS, power calculations
│   │   ├── circular_buffer.h     # Data buffering
│   │   └── uart_comm.h           # Communication with AI
│   └── Src/
│       ├── main.c                 # Main loop & initialization
│       ├── adc_driver.c           # ADC + DMA setup
│       ├── signal_processing.c   # Feature extraction
│       ├── circular_buffer.c     # Buffer management
│       └── uart_comm.c           # Data transmission
└── Drivers/
    └── STM32xxx_HAL/             # Hardware abstraction layer
```

### 4.2 Execution Model

**Interrupt-driven architecture:**
- ADC conversion complete → DMA interrupt → swap buffers
- Main loop → process filled buffer → extract features → send via UART
- No blocking operations
- Deterministic timing

## 5. Memory Layout

### 5.1 RAM Usage (Estimated)
```
ADC buffers (double buffer):     320 bytes  (2 × 80 samples × 2 bytes)
Circular buffer metadata:         16 bytes
Processing workspace:            100 bytes
UART transmit buffer:             64 bytes
Stack:                          1024 bytes
                               ─────────
Total:                         ~1.5 KB
```

STM32 typically has 20KB+ RAM → plenty of headroom

### 5.2 Flash Usage (Estimated)
```
Application code:               ~10 KB
HAL drivers:                    ~20 KB
                               ───────
Total:                         ~30 KB
```

STM32 typically has 64KB+ Flash → plenty of headroom

## 6. Timing Requirements

| Parameter | Requirement | Rationale |
|-----------|-------------|-----------|
| Sampling rate | ≥5kHz | Nyquist: 2× highest harmonic of interest (50Hz × ~50 = 2.5kHz) |
| Sample buffer | 80 samples | Exactly 1 AC cycle @ 50Hz (50ms @ 5kHz = 250 samples, but 4kHz = 80) |
| Feature extraction | <10ms | Must complete before next cycle |
| UART transmission | <5ms | Non-blocking, DMA-based |
| End-to-end latency | <1s | User experience requirement |

**Note:** We'll use 4kHz sampling (80 samples per 50Hz cycle) for clean cycle boundaries.

## 7. Communication Protocol

### 7.1 Firmware → Edge AI (UART)

**Packet format:**
```c
typedef struct {
    uint32_t timestamp;      // Milliseconds since boot
    float voltage_rms;       // RMS voltage (V)
    float current_rms;       // RMS current (A)
    float power_real;        // Real power (W)
    float power_factor;      // Dimensionless [-1, 1]
    uint8_t flags;           // Status flags
    uint16_t checksum;       // CRC-16
} __attribute__((packed)) measurement_packet_t;
```

Total: 23 bytes per packet  
Rate: 50 packets/second (one per AC cycle)  
Bandwidth: 1150 bytes/sec = 9200 baud → use 115200 baud UART

### 7.2 Edge AI → Application (TBD)

Will likely use:
- REST API over local network
- MQTT for real-time updates
- WebSocket for live dashboard

## 8. Safety & Isolation

### 8.1 Critical Requirements

⚠️ **NEVER directly connect MCU to mains voltage**

1. **Current sensing:** CT sensor provides galvanic isolation
2. **Voltage sensing:** MUST use isolation transformer or optoisolated circuit
3. **Power supply:** Use isolated AC-DC converter (commercial module)

### 8.2 Signal Conditioning

Both voltage and current signals must be:
- **Centered at ADC midpoint** (e.g., 1.65V for 3.3V ADC)
- **Scaled to ADC range** (0-3.3V for 3.3V ADC)
- **Filtered** (anti-aliasing low-pass filter before ADC)
- **Protected** (clamping diodes + series resistor)

## 9. Failure Modes & Handling

| Failure Mode | Detection | Response |
|--------------|-----------|----------|
| ADC saturation | Value = 0 or 4095 | Set error flag, skip cycle |
| Sensor disconnection | Abnormally low signal | Set error flag, alert |
| Timing jitter | Missed DMA interrupt | Increment error counter |
| UART overflow | Transmission buffer full | Drop packet, log error |
| Power supply noise | Excessive signal variance | Apply filtering |

## 10. Calibration Strategy

### 10.1 Factory Calibration
1. Apply known resistive load (e.g., 100W incandescent bulb)
2. Measure voltage with calibrated multimeter
3. Compare MCU reading vs actual
4. Calculate calibration factor: `cal_factor = actual / measured`
5. Store in flash memory

### 10.2 Runtime Validation
- Periodically measure known baseline (when no devices active)
- Compare against calibration baseline
- Alert if drift detected

## 11. Development Phases Mapping

| Phase | Focus Area | Architecture Component |
|-------|-----------|------------------------|
| Phase 0 | Foundations | This document |
| Phase 1 | Embedded system | Firmware skeleton, ADC driver |
| Phase 2 | Measurement | Signal processing, calibration |
| Phase 3 | AI detection | Edge AI processor, model training |
| Phase 4 | Integration | Full data pipeline |
| Phase 5 | Application | Dashboard, user interface |
| Phase 6 | Product | Complete system, deployment |

## 12. Open Questions (To be resolved)

- [ ] Specific STM32 model selection (Phase 1)
- [ ] CT sensor model and specifications (Phase 2)
- [ ] Edge AI processor choice (Phase 3)
- [ ] Communication protocol details (Phase 4)
- [ ] Enclosure and mounting (Phase 6)

---

**Document Version:** 1.0  
**Last Updated:** Phase 0  
**Status:** Living document (will evolve with project)
