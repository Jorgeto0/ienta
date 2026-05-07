# IENTA — Product Priorities

**Status**: Authoritative as of 2026-05-07. Update only with explicit
decision noted in commit message.

## The vision

Ienta is the foundation of a company that aims to be **what Apple is
to consumer electronics — for the smart-home and industrial-electrical
domain**. That positioning shapes every engineering decision below. We
are not building "a good energy monitor." We are building the
infrastructure category that did not exist before, executed with the
craft and longevity that justifies premium pricing for a decade.

## The order

1. **Accuracy**
2. **Longevity** (10+ years in the field, no attention required)
3. **Customer ease-of-use**
4. **Competitive superiority** (measurably better than every alternative)

Learning value is *not* on this list. Learning is a happy byproduct,
not an objective. When learning conflicts with any priority above, the
product wins. Capture lessons in `docs/` rather than injecting pedagogy
into production code.

## What each priority means in practice

### 1. Accuracy
The product measures and detects electrical events. A measurement that
is wrong by 2% is a worse product than one wrong by 0.5%. A
device-detection system at 92% accuracy is a worse product than one at
97%. Engineering decisions favor the option that produces more correct
numbers, even at modest cost in code complexity, binary size, or
development time.

Implications:
- Calibration is non-negotiable
- Floating-point on the M4F's hardware FPU, not fixed-point hacks
  (unless we measure that fixed-point is meaningfully faster *and*
  equally accurate for our specific dynamic range)
- Sample timing must be deterministic to single-microsecond precision
  (drives our use of timer-triggered ADC, not polling)
- Validate against known loads at every phase boundary

### 2. Longevity
The device sits in a luxury home's electrical panel for 10+ years. It
must not fail. It must not need attention. Design for thermal stress,
component aging, firmware corruption, communication dropouts.

Implications:
- No dynamic memory allocation (`malloc`) in firmware
- Watchdog timer enabled (planned for Phase 6)
- Field-upgradeable firmware (planned for Phase 5/6)
- ECC where the chip supports it
- Conservative voltage/current ratings on every external component
- Code reviewed for memory safety, race conditions, integer overflow

### 3. Customer ease-of-use
Luxury customers do not read manuals. They expect things to work the
moment they are plugged in. Setup must be trivial; operation must be
invisible until something needs attention. The Apple analogy is
operative here: the user never sees the engineering, only the result.

Implications:
- Auto-discovery and self-calibration in firmware where possible
- Clear failure modes (the device tells you what is wrong, not just
  "it is broken")
- Mobile/web interface designed by the rules of consumer apps, not
  industrial dashboards
- Sensible defaults; configuration is opt-in, not required
- The installed device should feel inevitable, not configurable

### 4. Competitive superiority
Measurably, demonstrably better than every alternative — Sense,
Smappee, Emporia Vue, Curb, and any newer entrants. Not "as good";
better. Better accuracy, better device-detection rates, better UI,
better installation, better longevity, or all of the above.

Implications:
- Track competitor capabilities; benchmark against them quarterly
- Edge AI inference for sub-second classification (most cloud-based
  competitors have 5–30s lag)
- Pricing reflects positioning, not parts cost
- Industrial-application path: the same firmware foundation should
  scale up to commercial buildings without rewrite

## Open questions (deferred)

- **Edge vs. cloud architecture**: Originally framed as "privacy-first
  local processing." Now an open question. Decision factors: AI model
  size, inference latency, customer connectivity assumptions,
  regulatory considerations (GDPR-equivalent regimes). Decide at
  Phase 4 with real data, not assumption.

- **Subscription model boundaries**: Original brief had free vs paid
  tiers. Revisit when we know what AI features cost to run.

- **Industrial vs residential split**: Same firmware foundation, but
  product packaging may diverge. Defer until Phase 5+.

## How to use this document

Future-you, future-AI-assistant, future-collaborator: when an
engineering choice has multiple reasonable answers, walk this priority
list top-to-bottom. The first priority that distinguishes the options
makes the decision. If none do, fall through to whatever is simpler,
faster to ship, or easier to maintain.

When in doubt, ask: **would Apple ship this?** Not in look — in
*restraint*. Apple does not ship features that feel half-finished.
Neither do we.
