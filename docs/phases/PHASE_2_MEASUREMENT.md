# Phase 2 — Energy Measurement Engine

## Purpose

Connect real sensors and validate measurement accuracy against known
reference loads. Introduces signal processing and calibration.

---

## Scope

### Hardware introduced

| Component | Specification |
|-----------|--------------|
| CT sensor | SCT-013-000, 2000:1 turns ratio, 120 ohm burden resistor |
| Isolation transformer | 230V:9V, galvanically isolated |
| Anti-aliasing filter | RC lowpass, fc ~1.6 kHz |
| Input protection | Schottky clamp diodes on both ADC inputs |

### Firmware modules introduced

| Module | Responsibility |
|--------|---------------|
|  | compute_rms(), compute_power(), compute_pf() |
|  | Converts measurement_t to uart_packet_t with calibrated scale factors |

### Calibration

Scale factors V_SCALE and I_SCALE are adjusted against a known
resistive load measured simultaneously with a calibrated multimeter.

### Accuracy targets

| Measurement | Target |
|-------------|--------|
| Voltage RMS | +/-1% |
| Current RMS | +/-2% |
| Real power | +/-3% |

### Exit criterion

All three accuracy targets met on a known load, running continuously
for 1+ hour without drift.
