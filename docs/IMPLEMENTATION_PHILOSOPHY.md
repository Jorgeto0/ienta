# IENTA — Implementation Philosophy

**Companion to `PRIORITIES.md`. Defines per-decision rules for choosing
between HAL and bare-metal implementations.**

## The rule

**Every peripheral interaction is a separate decision.** There is no
project-wide policy of "all HAL" or "all bare-metal." We pick what
serves the product best — function by function, sometimes line by line.

The goal is a firmware that is **fast, lean, deterministic, and
debuggable in the field for 10+ years**. That goal makes some choices
HAL and some bare-metal; getting the mix right is the engineering.

## When HAL wins

Use HAL (`HAL_Xxx_*` functions, `stm32f4xx_hal_xxx.c` drivers) when
**any** of the following apply:

- The init sequence is long and error-prone (ADC+DMA, USART+DMA, PLL
  configuration, USB, Ethernet)
- HAL provides timeout/error handling we would otherwise reinvent
- The peripheral has well-known integration footguns where HAL has been
  stress-tested by ST and the community (DMA setup, NVIC priorities,
  clock sequencing)
- We may need to port to a different STM32 chip later and want source
  compatibility
- The performance/size cost of HAL is negligible relative to its
  reliability win

Examples we expect to use HAL for:
- ADC + DMA initialization
- UART + DMA initialization
- System clock (PLL) configuration
- HSE/HSI clock startup with timeout

## When bare-metal wins

Use bare-metal (direct register writes via `PERIPHERAL->REGISTER`) when
**any** of the following apply:

- The operation runs on a hot path where HAL's overhead is measurable
  (ISRs, sample-rate-bound loops, transmission of 80-sample buffers at
  50 packets/sec)
- The function is short enough that bare-metal is genuinely simpler
  than configuring a HAL handle (single GPIO toggle, single register
  read in an ISR)
- We need deterministic timing that HAL's state machines cannot
  guarantee
- Code size matters enough that pulling in a HAL module just to use
  one function is wasteful
- Debugging in the field will be easier when you can see the exact
  register write that caused the behavior

Examples we expect to use bare-metal for:
- LED toggle (`GPIOA->ODR ^= GPIO_ODR_OD5`)
- ADC result read inside ISR
- Timer ISR bodies (just the body — `HAL_TIM_IRQHandler` is fine for
  routing)
- Circular buffer push/pop
- All Ienta application logic (RMS, packet framing, device detection)

## How to choose: the decision checklist

When implementing a new function, ask in order:

1. **Is this in a hot path or ISR?**
   → Yes: bare-metal almost always wins. Measure if uncertain.
   → No: continue.

2. **Is this initialization that runs once at boot?**
   → Yes: HAL almost always wins. Reliability beats elegance for code
   that runs only once.
   → No: continue.

3. **Is the operation 1–3 register writes?**
   → Yes: bare-metal. Wrapping that in HAL adds more code than it saves.
   → No: continue.

4. **Does HAL provide error handling we would otherwise have to write?**
   → Yes: HAL.
   → No: continue.

5. **Will this code be read by humans more than written?**
   → Yes (production logic): pick the one that reads more clearly.
   → No (one-off init or perf-critical inner loop): pick what runs better.

6. **Default**: HAL. The principle "fewer custom implementations =
   fewer bugs unique to us" is itself a tiebreaker.

## What this means for Step 3 (UART)

Walking the checklist for each UART function:

| Function | Hot path? | One-time? | ≤3 reg writes? | Decision |
|----------|-----------|-----------|----------------|----------|
| USART2 init (clock, baud, GPIO AF) | No | Yes | No | **HAL** |
| Send a single byte | Maybe (Phase 4+) | No | Yes (write DR, poll TC) | **Bare-metal** |
| Send a string (loop) | Yes (Phase 4+) | No | N/A — loops bare-metal byte send | **Bare-metal** |
| Receive a byte (Phase 1 only) | No | No | Yes | **Bare-metal** |
| ADC+DMA init (later) | No | Yes | No | **HAL** |
| ADC sample read in ISR (later) | Yes | No | Yes | **Bare-metal** |
| GPIO LED toggle | No | No | Yes | **Bare-metal** |

The transmit-byte loop runs every time we send a packet. At 50
packets/sec of telemetry in Phase 4+, with packets ~20 bytes each,
that is ~1000 byte-sends per second. `HAL_UART_Transmit`'s per-call
overhead would be measurable on that hot path. Bare-metal byte send
(write `USART2->DR`, poll `USART2->SR` for TC) is ~5 lines, no state
machine, deterministic timing.

## How to use this document

When writing any new peripheral function, walk the checklist out loud
in the commit message or the function's header comment. "HAL because
init reliability" or "bare-metal because ≤3 register writes" — the
reasoning is part of the deliverable. Future-you should never have to
guess why a function was implemented one way or the other.

When the answer is genuinely close, pick **whichever is easier to
debug at 3 AM in a customer's electrical panel** five years from now.
