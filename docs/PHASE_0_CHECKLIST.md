# Phase 0 — Foundations Checklist

## Track C: System Architecture & GitHub ✓

### Module C1: Project Structure
- [x] Created complete directory structure
- [x] Set up firmware, AI, docs, tools, hardware folders
- [x] Created .gitignore with proper exclusions
- [x] Written main README.md

### Module C2: Architecture Documentation
- [x] System overview document (`docs/architecture/system_overview.md`)
- [x] Firmware architecture document (`docs/firmware/architecture.md`)
- [x] Signal chain diagrams
- [x] Data flow specifications
- [x] Memory layout planning

### Module C3: GitHub Standards
- [x] Documentation workflow defined
- [x] Commit message standards
- [x] Branch strategy documented
- [x] Code review checklist
- [ ] **PENDING:** Initialize Git repository
- [ ] **PENDING:** Push to GitHub

---

## Track A: Electrical Fundamentals

### Module A1: Voltage, Current & Power
- [ ] Understand AC vs DC
- [ ] Learn RMS values and why they matter
- [ ] Real power vs apparent power
- [ ] Power factor concept
- [ ] Practice calculations

### Module A2: Electrical Signals
- [ ] Sine wave fundamentals
- [ ] Harmonics and distortion
- [ ] Transients and inrush current
- [ ] Why these matter for device detection

### Module A3: Current Sensing
- [ ] How CT sensors work
- [ ] Burden resistors
- [ ] Signal conditioning basics
- [ ] ADC input requirements

### Module A4: Safety & Isolation
- [ ] Mains voltage dangers
- [ ] Isolation requirements
- [ ] Safe design practices
- [ ] What can kill vs what can damage

---

## Track B: Embedded C Programming

### Module B1: C Fundamentals for Embedded
- [ ] Memory: stack vs heap
- [ ] Pointers and arrays mastery
- [ ] Structs for data organization
- [ ] Fixed-width types (`uint16_t`, etc.)

### Module B2: Real-Time Concepts
- [ ] Interrupts and ISR design
- [ ] Timing and delays
- [ ] Circular buffers
- [ ] Non-blocking code patterns

### Module B3: Bit Manipulation
- [ ] Bitwise operators (AND, OR, XOR, shifts)
- [ ] Register manipulation
- [ ] Flags and status bits
- [ ] Practical embedded examples

---

## Phase 0 Exit Criteria

Before moving to Phase 1 (hardware purchase), must complete:

✅ **Track C** — COMPLETE
- [x] GitHub repository initialized
- [x] Architecture documented
- [x] Coding standards defined

⬜ **Track A** — IN PROGRESS
- [ ] Can explain AC power measurement
- [ ] Understand CT sensor operation
- [ ] Know safety requirements

⬜ **Track B** — IN PROGRESS
- [ ] Can write clean embedded C
- [ ] Understand interrupt-driven design
- [ ] Comfortable with bit manipulation

---

## Deliverables Checklist

### Documentation
- [x] `README.md` — Project overview
- [x] `docs/architecture/system_overview.md` — System design
- [x] `docs/firmware/architecture.md` — Firmware design
- [x] `docs/GITHUB_STANDARDS.md` — Development workflow
- [x] This checklist

### Repository Structure
- [x] firmware/Core/{Inc,Src}/
- [x] ai/{models,training,data}/
- [x] tools/{calibration,testing}/
- [x] hardware/{schematics,pcb}/
- [x] docs/{architecture,firmware,hardware,ai}/

### Git Setup
- [ ] Repository initialized
- [ ] Initial commit done
- [ ] Remote added (GitHub)
- [ ] Pushed to GitHub
- [ ] `develop` branch created

---

## Next Steps After Phase 0

1. **Complete Track A** (Electrical fundamentals)
2. **Complete Track B** (Embedded C programming)
3. **Review architecture documents** — ensure understanding
4. **Plan Phase 1** — STM32 selection and purchase
5. **Set up development environment** — toolchain installation

---

## Time Estimate

- **Track C (Architecture):** ✓ Complete (~2-3 hours)
- **Track A (Electrical):** 6-10 hours
- **Track B (Embedded C):** 8-12 hours

**Total Phase 0:** 16-25 hours of focused learning

---

## Notes & Decisions

**Architecture Decisions Made:**
- Sampling rate: 4kHz (80 samples per 50Hz cycle)
- Communication: UART at 115200 baud
- Buffer strategy: Double buffering with DMA
- Memory: Static allocation only (no malloc)

**Hardware Decisions Pending:**
- Specific STM32 model (Phase 1)
- CT sensor selection (Phase 2)
- Edge AI processor choice (Phase 3)

**Questions for Track A:**
- What is RMS and why not just peak voltage?
- How does power factor affect device detection?
- Why do we need anti-aliasing filters?

**Questions for Track B:**
- How do interrupts actually work?
- What's the difference between volatile and const?
- How to debug embedded code without printf?

---

**Last Updated:** 2026-04-21  
**Status:** Track C complete, Tracks A & B in progress
