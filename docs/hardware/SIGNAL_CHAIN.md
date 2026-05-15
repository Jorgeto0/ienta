# Signal Chain

Sensors, signal conditioning, isolation, component values, and safety.

---

## Overview

Two signals are measured: mains voltage and load current. Both are
conditioned to 0–3.3V, centered at 1.65V, before reaching the ADC.

```
AC mains (220–240V RMS)
  │
  ├──► Isolation transformer (230V:9V)
  │       → voltage divider + 1.65V bias
  │       → clamp + anti-alias filter
  │       → ADC Channel 1 (voltage)
  │
  └──► CT sensor (SCT-013-000, 2000:1)
          → 120Ω burden resistor
          → 1.65V bias
          → clamp + anti-alias filter
          → ADC Channel 0 (current)
```

---

## Isolation

**Voltage — isolation transformer:**
Magnetic coupling only. No electrical path between mains and MCU.
A resistive voltage divider is never acceptable for mains voltage sensing.

**Current — CT sensor:**
Clamps around the live conductor. No electrical contact with mains.

---

## CT sensor design

| Parameter | Value |
|-----------|-------|
| Sensor | SCT-013-000 |
| Turns ratio | 2000:1 |
| Burden resistor | 120Ω |
| Output at 20A | 1.2V RMS |
| ADC utilization at 20A | ~73% |
| DC bias | 1.65V |

Output: `V = (I_primary ÷ 2000) × 120Ω`
At 20A: `(20 ÷ 2000) × 120 = 1.2V RMS`

73% utilization leaves headroom for inrush transients (up to 10× normal).

---

## Voltage sensing design

| Parameter | Value |
|-----------|-------|
| Transformer | 230V:9V isolation |
| Secondary RMS | 9V → 12.7V peak |
| After divider | ~3.0V peak |
| DC bias | 1.65V |

---

## Anti-aliasing filter

Required on both inputs before ADC.
First-order RC lowpass: R = 1kΩ, C = 100nF → fc ≈ 1.6 kHz
Attenuates noise above signal band, below 2 kHz Nyquist.

---

## Input protection

Both ADC inputs require:
- 1kΩ series resistor (limits fault current)
- Schottky diodes clamping to 0V and 3.3V

---

## Accuracy targets

| Measurement | Target |
|-------------|--------|
| Voltage RMS | ±1% |
| Current RMS | ±2% |
| Real power | ±3% |

Achieved through calibration against known resistive loads in Phase 2.

---

## Safety

- All mains conductors fully enclosed
- No exposed mains connections on PCB
- CT sensor rated for installation voltage
- Isolation transformer UL/CE rated
- Never connect mains through a resistive divider
