# Implementation Philosophy

Every peripheral function is an independent HAL vs bare-metal decision.
No project-wide policy. Each function evaluated on its own merits.

Goal: firmware that is fast, lean, deterministic, and debuggable in a
customer's panel for 10+ years.

---

## When HAL wins

- Init sequence is long, multi-register, and error-prone
- HAL provides timeout/error handling we would otherwise rewrite
- Known configuration footguns (DMA stream/channel, NVIC, clock tree)
- Boot-time code where reliability beats brevity

**HAL is used for:** ADC+DMA init, UART init, system clock (PLL+HSE),
GPIO alternate-function pin setup.

---

## When bare-metal wins

- ISR or hot path where HAL overhead is measurable
- 1–3 register writes — HAL adds more code than it saves
- Deterministic timing required
- Simpler to debug in the field

**Bare-metal is used for:** GPIO toggle, ADC result read in ISR, UART
TX byte loop, all ISR bodies, all application logic.

---

## Decision checklist

1. Hot path or ISR? → bare-metal
2. Init that runs once at boot? → HAL
3. ≤3 register writes? → bare-metal
4. HAL provides error handling we need? → HAL
5. Default → HAL

When close: choose what is easier to debug at 3 AM in a customer's panel.

---

## Decision table — Phase 1

| Function | Decision | Reason |
|----------|----------|--------|
| `SystemClock_Config` | HAL | Wrong clock = 16 MHz not 180 MHz |
| ADC + DMA init | HAL | DMA channel config has many footguns |
| UART init | HAL | Baud prescaler + GPIO AF sequencing |
| TIM2 base init | HAL | Clock sequencing |
| GPIO AF pin init | HAL | CubeMX generated — do not hand-edit |
| DMA ISR body | Bare-metal | Must exit < 1 µs |
| ADC result read (ISR) | Bare-metal | `ADC1->DR` = one instruction |
| UART TX byte loop | Bare-metal | 700 writes/sec — HAL overhead measurable |
| LED / relay toggle | Bare-metal | One instruction |
| `signal_processing.c` | Bare-metal | Pure math, no peripheral |
| `packet_builder.c` | Bare-metal | Pure struct work, no peripheral |

---

## Documenting the decision

Every peripheral function states its reason in the header comment:

```c
// HAL — init-only, DMA setup has known footguns
void adc_driver_init(void) { ... }

// Bare-metal — hot path, ≤3 register writes
static void uart_send_byte(uint8_t b) { ... }
```
