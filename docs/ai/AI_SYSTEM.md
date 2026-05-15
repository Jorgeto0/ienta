# AI System

Device detection and anomaly detection pipeline. Phase 3 onward.
Do not begin until Phase 2 measurements are verified accurate.

---

## Problem

Classify which device is active on the monitored circuit and detect
anomalies — using only the electrical signature at the panel.

---

## Input

`uart_packet_t` frames at 50 Hz: Vrms, Irms, real power, power factor.
Raw V/I samples (80 per channel) available for higher-resolution features.

---

## Output

```json
{
  "device": "washing_machine",
  "confidence": 0.94,
  "anomaly": false,
  "power_usage": 1850.0
}
```

---

## Device signatures

| Feature | What it captures |
|---------|-----------------|
| Steady-state RMS | Baseline power draw |
| Power factor | Load type (resistive / inductive / capacitive) |
| THD | Harmonic fingerprint (3rd, 5th, 7th) |
| Inrush magnitude | Startup surge (1×–40× normal) |
| Inrush duration | How long the surge lasts |
| Transient shape | Turn-on waveform envelope |

No single feature is sufficient. The combination is the fingerprint.

---

## Model requirements

| Requirement | Target |
|-------------|--------|
| Classification accuracy | ≥ 90% |
| Inference latency | < 1 second |
| Deployment | On-device (no cloud required) |
| Framework | Python + PyTorch / TFLite |

---

## Data collection (Phase 3)

1. Run calibrated Phase 2 firmware
2. Connect one device at a time
3. Label each recording with device type
4. Capture: cold start, steady-state, shutdown
5. Minimum 50 labeled sessions per device class

---

## Training rules

- Never train on uncalibrated data
- 20% holdout test set — never trained on
- Validate on data from a different day
- Fix data quality before tuning the model

---

## Edge vs cloud

Not decided. Evaluate at Phase 4 with real model size and latency data.
