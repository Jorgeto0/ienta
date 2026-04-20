# GitHub Workflow & Standards

## Repository Management

### Branch Strategy

**Main Branches:**
- `main` — Production-ready code only
- `develop` — Integration branch for features

**Feature Branches:**
- `phase-1-adc-driver` — Phase-specific development
- `feature-rms-calculation` — Specific feature work
- `bugfix-uart-timeout` — Bug fixes

### Workflow
```
main (always stable)
  ↑
develop (integration)
  ↑
feature/phase-1-adc-driver (active work)
```

---

## Commit Standards

### Commit Message Format
```
<type>: <subject>

<body>

<footer>
```

### Types
- `feat:` New feature
- `fix:` Bug fix
- `docs:` Documentation only
- `refactor:` Code restructuring (no behavior change)
- `test:` Adding tests
- `chore:` Maintenance (build, dependencies)

### Examples

**Good:**
```
feat: implement ADC dual-channel sampling

- Configure ADC1 for simultaneous voltage/current
- Setup DMA double buffering
- Add interrupt handler for buffer swap

Tested with simulated signals, stable at 4kHz
```

**Bad:**
```
fixed stuff
```

---

## Code Review Checklist

Before committing firmware code, verify:

- [ ] No compilation warnings
- [ ] Fixed-width types used (`uint16_t` not `int`)
- [ ] No magic numbers (use `#define`)
- [ ] Functions documented with comments
- [ ] Error handling implemented
- [ ] No blocking delays in main loop
- [ ] Memory usage within budget
- [ ] Tested on actual hardware (Phase 2+)

---

## Documentation Standards

### README Requirements

Every major directory should have a README:

**firmware/README.md:**
```markdown
# Firmware

STM32 embedded software for Ienta energy monitoring.

## Building
[instructions]

## Flashing
[instructions]

## Architecture
See docs/firmware/architecture.md
```

### Code Documentation

**Header files:**
```c
/**
 * @file adc_driver.h
 * @brief ADC peripheral driver with DMA support
 * @author [Your Name]
 * @date 2026-04-21
 */
```

**Functions:**
```c
/**
 * @brief Initialize ADC peripheral for dual-channel sampling
 * 
 * Configures ADC1 channels 0 (voltage) and 1 (current) for
 * simultaneous sampling at 4kHz with DMA transfer.
 * 
 * @return 0 on success, -1 on error
 */
int adc_init(void);
```

---

## File Organization

### Firmware Module Pattern
```
adc_driver.h        # Public interface
adc_driver.c        # Implementation
```

**Header file structure:**
```c
#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H

#include <stdint.h>

// Public constants
#define SAMPLES_PER_CYCLE  80

// Public types
typedef struct {
    // ...
} adc_config_t;

// Public functions
int adc_init(void);
void adc_start(void);

#endif // ADC_DRIVER_H
```

---

## Version Control Best Practices

### What to Commit
✅ Source code (`.c`, `.h`)  
✅ Makefiles and build scripts  
✅ Documentation (`.md`)  
✅ Schematics (`.kicad_*`, PDFs)  
✅ README files  

### What NOT to Commit
❌ Compiled binaries (`.o`, `.elf`, `.bin`)  
❌ IDE project files (`.vscode/`, `.idea/`)  
❌ Build artifacts (`build/` directory)  
❌ Large datasets (use Git LFS or external storage)  
❌ API keys or credentials  

### .gitignore (Already Created)
The `.gitignore` file handles this automatically.

---

## Release Management

### Version Numbering
Follow Semantic Versioning: `MAJOR.MINOR.PATCH`

- **MAJOR:** Breaking changes
- **MINOR:** New features (backward compatible)
- **PATCH:** Bug fixes

**Examples:**
- `v0.1.0` — Phase 1 complete (ADC working)
- `v0.2.0` — Phase 2 complete (measurement engine)
- `v1.0.0` — Phase 6 complete (first production release)

### Tagging Releases
```bash
git tag -a v0.1.0 -m "Phase 1: ADC driver complete"
git push origin v0.1.0
```

---

## GitHub Project Setup

### Initial Repository Creation

**On GitHub:**
1. Create new repository: `ienta`
2. Set visibility: Private (initially)
3. Don't initialize with README (we have our own)

**Local setup:**
```bash
cd ienta
git init
git add .
git commit -m "chore: initial project structure"
git branch -M main
git remote add origin https://github.com/[username]/ienta.git
git push -u origin main
```

### Create Development Branch
```bash
git checkout -b develop
git push -u origin develop
```

---

## Collaboration Guidelines

### Pull Request Process

1. Create feature branch from `develop`
2. Make changes and commit
3. Push branch to GitHub
4. Open pull request to `develop`
5. Review and test
6. Merge if approved

### Code Review Focus Areas

**Firmware:**
- Memory safety (no buffer overflows)
- Timing correctness (no race conditions)
- Error handling (all paths covered)
- Documentation (clear comments)

**AI/Python:**
- Algorithm correctness
- Performance (inference time)
- Data validation
- Model reproducibility

---

## Issue Tracking

### Issue Labels

- `bug` — Something broken
- `feature` — New functionality
- `docs` — Documentation improvement
- `phase-1` through `phase-6` — Phase tracking
- `hardware` — Hardware-related
- `firmware` — Firmware-related
- `ai` — AI/ML-related

### Issue Template
```markdown
**Phase:** [Phase number]
**Component:** [firmware/ai/hardware/docs]

**Description:**
[Clear description of issue or feature]

**Acceptance Criteria:**
- [ ] Criterion 1
- [ ] Criterion 2

**Technical Notes:**
[Any relevant technical details]
```

---

## Continuous Integration (Future)

When project matures, consider:

- **GitHub Actions** for automated builds
- **Unit test runners** on every commit
- **Code quality checks** (linting, static analysis)
- **Documentation generation** (Doxygen for C code)

Example `.github/workflows/build.yml`:
```yaml
name: Build Firmware

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Install ARM toolchain
        run: sudo apt-get install gcc-arm-none-eabi
      - name: Build firmware
        run: cd firmware && make
```

---

## Backup Strategy

### GitHub as Primary Backup
- Push regularly (at least daily during active work)
- Tag important milestones
- Keep `main` branch always deployable

### Local Backups
- Clone to external drive weekly
- Export critical documentation to PDF

---

## Portfolio Presentation

When showing this project to employers:

### What to Highlight

1. **Architecture Quality**
   - Point to `docs/architecture/system_overview.md`
   - Show clean separation of concerns

2. **Code Quality**
   - Clean, well-documented C code
   - No warnings, no magic numbers
   - Professional firmware patterns

3. **Testing Rigor**
   - Real hardware validation
   - Calibration documentation
   - Performance benchmarks

4. **Documentation**
   - Architecture diagrams
   - Clear README files
   - Design decision logs

### GitHub README Sections

Make sure the main README.md includes:
- Clear project description
- Architecture diagram
- Technical specifications
- Build instructions
- Current status/roadmap
- Your contact info

---

## Monthly Maintenance Tasks

- [ ] Review open issues
- [ ] Update documentation if architecture changed
- [ ] Tag major milestones
- [ ] Back up to external storage
- [ ] Review commit history for clarity

---

**Document Version:** 1.0  
**Last Updated:** Phase 0  
**Next Review:** After Phase 1 completion
