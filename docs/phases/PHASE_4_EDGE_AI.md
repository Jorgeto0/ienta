# Phase 4 — Edge AI Integration

## Purpose

Run STM32 firmware and AI classifier together as a complete end-to-end
pipeline on real hardware. Benchmark latency and resource usage.

---

## Scope

- Integrate trained Phase 3 model with UART data stream from STM32
- Measure end-to-end inference latency on target edge processor
- Evaluate edge vs cloud deployment based on real benchmark data
- Optimize pipeline for continuous operation

### Key decision this phase

Edge vs cloud inference. This decision is deferred until real model
size, memory footprint, and latency numbers are available from this phase.
Neither option is assumed.
