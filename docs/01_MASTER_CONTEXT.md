# IENTA — MASTER CONTEXT

## Identity
Ienta is an autonomous energy intelligence system installed inside the
electrical panel of luxury residential and industrial properties. It
measures, detects, optimizes, and predicts — at the source, in real
time, with the accuracy and longevity required to live in a customer's
infrastructure for 10+ years.

The vision: Ienta is the foundation of a company aiming to be **what
Apple is to consumer electronics — for the smart-home and industrial-
electrical domain**. Premium positioning, vertical integration, restraint
over feature count.

## Core Functions
- Real-time energy measurement (voltage, current, power, power factor)
- Device detection from electrical signatures (edge AI)
- Energy optimization (autonomous, learned per-home)
- Failure prediction (thermal, electrical, behavioral anomalies)

## Value
- Accurate insight into household or facility energy at the appliance
  level, not just the meter
- Prevent failures before they cost the customer money or comfort
- Reduce energy waste through automation, not nagging
- A device that disappears into the panel and works for a decade

## User
**Primary**: high-end residential owners — luxury homes, smart estates,
properties where reliability and ease of use are non-negotiable.

**Secondary (later phases)**: industrial and commercial facilities where
the same firmware foundation scales up.

In both segments, the user expects things to work without manuals,
without configuration, and without attention. Setup is trivial;
operation is invisible until something needs them.

## Business Model
Free tier:
- Real-time monitoring and historical dashboards

Paid tier:
- AI device detection and classification
- Energy optimization and automation
- Failure prediction and proactive alerts

Tier boundaries are not final. Revisit when AI-feature operating costs
are known. See `PRIORITIES.md` ("Open questions").

## Product Philosophy
- Not an IoT gadget
- Not a dashboard
- It is **the electrical brain of the building**

Engineering decisions are governed by a strict priority order:
**accuracy, longevity, customer ease-of-use, competitive superiority**
(in that order). When choices conflict, the higher priority wins.
See `PRIORITIES.md` for the authoritative product priority list and
`IMPLEMENTATION_PHILOSOPHY.md` for per-decision HAL-vs-bare-metal rules.

Edge-vs-cloud architecture is currently an **open question**, deferred
to Phase 4 when AI model size, inference latency, and customer
connectivity assumptions are concrete. The original "privacy-first
local-only" framing is no longer a binding constraint.
