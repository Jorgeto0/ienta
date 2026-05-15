# Phase 3 — Device Detection AI

## Purpose

Train and validate a machine learning model that classifies active
electrical devices from their load signatures.

---

## Scope

### Problem

Identify which device is active from V/I waveform data alone.
No camera, microphone, or network traffic. Electrical signature only.

### Features extracted per device

| Feature | What it captures |
|---------|-----------------|
| Steady-state RMS | Baseline power draw |
| Power factor | Load type — resistive, inductive, capacitive |
| THD | Harmonic fingerprint: 3rd, 5th, 7th harmonic content |
| Inrush magnitude | Startup current surge (1x to 40x normal) |
| Inrush duration | How long the surge lasts in milliseconds |
| Transient shape | Turn-on waveform envelope |

### Model requirements

| Requirement | Target |
|-------------|--------|
| Classification accuracy | >=90% on held-out test set |
| Inference latency | <1 second |
| Deployment | On-device, no cloud dependency |
| Framework | Python + PyTorch or TFLite |

### Data collection

One device at a time connected to the monitored circuit.
Each session captures: cold startup, steady-state, shutdown.
Minimum 50 labeled sessions per device class.
Training and test data collected on different days.

### Exit criterion

>=90% classification accuracy on held-out test set collected from
real devices on a calibrated Phase 2 setup.
