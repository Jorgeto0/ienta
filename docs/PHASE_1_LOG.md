# IENTA — Phase 1 Engineering Log

**Purpose**: Running log of decisions, conventions, and lessons learned
during Phase 1 (Embedded System Development). Read this at the start of
every future AI-assisted session on this project before giving advice
or commands.

Append at the end of each step. Don't rewrite.

---

## Project State Snapshot (as of 2026-05-07)

**Current phase**: Phase 1 — Embedded System Development
**Current step**: Step 2 complete (LED blinks; toolchain verified end-to-end)

### Hardware
- Target board: **STM32 Nucleo-F401RE** (single mini-USB on CN1, ST-LINK end)
- MCU: STM32F401RET6 — Cortex-M4F, 84 MHz max, 512 KB flash, 96 KB SRAM
- Programmer: on-board ST-LINK/V2-1 (chipid 0x433, dev-type STM32F401xD_xE)

### Toolchain (macOS, Apple Silicon)
- ARM GNU Toolchain **15.2.Rel1** (15.2.1, Dec 2025) — installed via official
  `.pkg` from developer.arm.com, **NOT** Homebrew cask
- stlink **v1.8.0** — `brew install stlink`
- GNU Make **3.81** — Apple-shipped (has quirks; see Lessons)
- Path to ARM toolchain: `/Applications/ArmGNUToolchain/15.2.rel1/...`

### Filesystem layout
- Project repo: `~/Workspace/ienta/`
- Vendor SDKs (read-only reference): `~/Workspace/vendor/`
- STM32CubeF4 (cloned --recursive, tag v1.28.3): `~/Workspace/vendor/STM32CubeF4/`

---

## Architectural Decisions (with rationale)

### HAL philosophy (REVISED from Phase 0's "minimum HAL only")
Use **HAL** where it earns its place: peripheral *plumbing* (ADC+DMA init,
UART+DMA init, system-clock setup, vendor-supplied startup, vendor-supplied
linker script). These are well-tested, register-sequence-heavy, and
pedagogically low-value to write by hand.

Use **bare-metal** where it *teaches*: GPIO, application logic, timer
configuration for the 4 kHz ADC trigger, ISR bodies (not the table itself),
all Ienta-specific data flow.

Principle: **HAL handles "how to talk to the peripheral"; bare-metal handles
"what the peripheral does for Ienta."**

### Build profiles (Makefile)
- `make` (default `BUILD=dev`) → `-Og` (debug-friendly optimization)
- `make BUILD=debug` → `-O0` (escape hatch for stubborn debugging)
- `make BUILD=release` → `-O2` (production, future Phase 6+)

Default is `-Og`, NOT `-O0`. Reasoning: `-Og` enables only optimizations
that don't interfere with debugging. Developing at `-O0` hides latent bugs
(e.g. missing `volatile`) that surface only when you eventually flip to
higher optimization. Better to write code that survives optimization.

### Build directory
`firmware/build/` — gitignored, mirrors source tree.

### Repo structure
````
firmware/
├── Core/
│   ├── Inc/    (our headers)
│   └── Src/    (our C files — main.c)
├── Drivers/
│   └── CMSIS/
│       ├── Device/ST/STM32F4xx/{Include,Source}/  (ST silicon)
│       └── Include/                                 (ARM Cortex core)
├── Linker/
│   └── STM32F401RETX_FLASH.ld
└── Makefile
````

### Vendor file management
- ST CubeF4 cloned to `~/Workspace/vendor/STM32CubeF4` (NOT inside the repo)
- 36 specific files copied into `firmware/`. Provenance recorded in commit
  `1a2ca44` ("vendor: import STM32CubeF4 v1.28.3 ...")
- Imported version: **CubeF4 v1.28.3**. Do not silently upgrade — bump the
  tag deliberately and document in a new vendor-drop commit.
- Treat copied files as read-only. Modifications go in separate commits with
  explicit rationale.

### Critical compiler flags (don't omit any)
````
-mcpu=cortex-m4
-mthumb              # CRITICAL: M4 only runs Thumb. Forget this → garbage.
-mfpu=fpv4-sp-d16    # M4F single-precision FPU.
-mfloat-abi=hard     # Pass floats in FPU registers.
-DSTM32F401xE        # Required by CubeF4's umbrella header.
````

---

## Code Conventions (firmware)

Established in Phase 0, non-negotiable:

- `1u` (unsigned) not `1` in shifts: `(1u << 5)` not `(1 << 5)`. Avoids
  signed-int undefined behavior.
- `f` suffix on float literals: `0.1611f` not `0.1611` (which is `double`).
- `sqrtf()` for floats, not `sqrt()`.
- `|=` not `=` on peripheral registers — preserves other bits.
- `volatile` on any variable accessed by both ISR and main code. Also on
  busy-loop counters the compiler might elide.
- `static` for any function not exposed in a header — limits linkage,
  helps `--gc-sections`.

---

## Lessons Learned

### macOS quirks
- **`cat -A` does not exist on macOS**. BSD `cat` only has `[-belnstuv]`.
  Use `cat -et` (or `cat -e`) for showing tabs and line endings.
- **Default `make` is GNU Make 3.81 (2006)**. Has quirks vs modern 4.x.
  Specifically: `$<` automatic variable can expand to empty in some
  `.PHONY`-related rule contexts. Workaround: use explicit file paths
  instead of automatic variables in such targets. If modern Make is ever
  required: `brew install make` provides `gmake` (GNU 4.x).
- macOS Gatekeeper may quarantine downloaded binaries. Use
  `xattr -d com.apple.quarantine <file>` if needed.

### STM32CubeF4 specifics
- **Must clone with `--recursive`** — submodules required. GitHub `.zip`
  downloads silently miss them.
- **Pin to a specific tag**: `--branch v1.28.3 --depth 1` for
  reproducibility. Don't track main.
- **The `Templates/STM32CubeIDE/` folder for STM32F401RE-Nucleo contains a
  misnamed linker script** (`STM32F401CEUX_FLASH.ld`). Memory layout is
  actually correct for F401RE because F401CE and F401RE share the same
  silicon die (same flash + RAM, differ only in package: C=UFQFPN48,
  R=LQFP64; and temperature grade). For clarity, prefer the correctly-named
  `Templates_LL/STM32CubeIDE/STM32F401RETX_FLASH.ld`.
- **Always verify a linker script's chip identity** by grepping `LENGTH = `
  and checking values match the target. Don't assume from filename alone.

### Nucleo-F401RE board specifics
- **Single mini-USB connector** at CN1 (ST-LINK side). The two-USB-port
  arrangement is on Nucleo-144 boards, not Nucleo-64.
- **Green LED LD2 is hardwired to PA5** (per UM1724).
- **PA13 and PA14 are SWD debug pins** (alternate function 10 by default).
  Don't reconfigure them or the debugger is lost.
- Default reset clock is **HSI at 16 MHz** until PLL is configured.

### Toolchain installation
- **`brew install --cask gcc-arm-embedded` is unreliable** — has had
  repeated signing/symlink issues over the years. `ln -s` workarounds
  leave fragile setups.
- **Reliable path: download official `.pkg` from developer.arm.com**,
  install, add `bin/` to PATH. ARM signs and ships this.
- `brew install stlink` is fine and stable.

### Linker / newlib
- **Newlib-nano emits warnings about unimplemented `_close`/`_lseek`/
  `_read`/`_write`** when linked with `--specs=nosys.specs`. The warning's
  second line — *"the message above does not take linker garbage collection
  into account"* — means `--gc-sections` removes them anyway. Final binary
  contains zero of these stubs. Warnings are noise.
- Future polish: minimal `Core/Src/syscalls.c` to silence properly.

---

## Self-Rules for AI Assistant

These come from concrete mistakes made during this conversation. Following
them prevents wasted user time:

1. **Search the web before giving install commands or version-specific
   advice.** Toolchains, brew formulas, SDK release tags change. Don't
   recall from memory and present as fact.

2. **When claiming a vendor file is for the wrong chip, verify by
   inspecting it.** Grep for `LENGTH = `, `MEMORY {`, or chip-ID macros
   and quote the actual content. Never assert "this is the wrong chip"
   from filename pattern alone. (I did exactly this with
   STM32F401CEUX_FLASH.ld and was wrong — F401CE and F401RE share the
   same die.)

3. **macOS ≠ Linux for command-line tools.** Don't assume GNU coreutils
   or GNU Make. Common gotchas: `cat -A`, GNU sed extensions,
   `find -printf`, modern Make features. Verify or use BSD-portable
   equivalents.

4. **Don't conflate STM32 board families.** Nucleo-32 / Nucleo-64 /
   Nucleo-144 differ in pinouts, USB layouts, debugging arrangements.
   The board user manual (UMxxxx) is authoritative.

5. **Pin SDK versions explicitly** when guiding installs. "Latest" rots;
   tag-and-cite is reproducible.

6. **Read the entire build log before troubleshooting.** Linker warnings
   about garbage-collected symbols look alarming but are noise. Build
   success is "objcopy completed and binary exists" — a failing post-step
   doesn't undo prior work.

7. **Treat the user's calibration as authoritative**. When the user
   updates a preference (e.g. "use HAL where convenient" replacing an
   earlier "minimum HAL"), don't drift back to the prior position.

8. **For GNU Make recipes that may run on macOS**, prefer explicit paths
   over `$<`/`$@` automatic variables in `.PHONY` or single-prerequisite
   contexts. Robustness > terseness.

---

## Active Tech Debt

To resolve before Phase 1 exit:

- [ ] Replace `delay_busy()` (CPU spin-loop) with SysTick-based delay.
      Imprecise, optimization-dependent, wastes power.
- [ ] Add `Core/Src/syscalls.c` with minimal stubs to silence newlib-nano
      `_close`/`_lseek`/`_read`/`_write` warnings.
- [ ] Configure system clock to 84 MHz via PLL — currently HSI 16 MHz.
- [ ] Step 3+ work: UART, ADC, DMA, double buffering.

---

## Phase 1 Progress Tracker

- [x] **Step 1**: Hardware arrival & verification (board detected by st-info)
- [x] **Step 2**: Dev environment + blink LED on PA5
- [ ] **Step 3**: UART communication (115200 baud, "Hello" → Mac)
- [ ] **Step 4**: ADC single-channel polling
- [ ] **Step 5**: Timer-triggered ADC at 4 kHz
- [ ] **Step 6**: DMA configuration ⚠️ hardest step
- [ ] **Step 7**: Double buffering
- [ ] **Step 8**: Dual-channel ADC
- [ ] **Step 9**: UART packet transmission
- [ ] **Step 10**: Continuous-operation stress test
- [ ] **Step 11**: Final cleanup & documentation

---

*Last updated: 2026-05-07, end of Step 2.*
