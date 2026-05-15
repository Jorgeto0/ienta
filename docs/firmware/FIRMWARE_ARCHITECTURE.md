# Firmware Architecture

Ienta firmware uses the layered architecture pattern. Each layer
communicates only with the layer directly below it.

---

## Layer diagram

```
┌──────────────────────────────────────────────┐
│  APPLICATION — main() state machine          │
├──────────────────────────────────────────────┤
│  PLATFORM-INDEPENDENT MODULES                │  ← no hardware includes
│  signal_processing · packet_builder · cbuf   │
├──────────────────────────────────────────────┤
│  HARDWARE-DEPENDENT DRIVERS                  │  ← STM32-specific
│  adc_driver · uart_driver · timer_driver     │
│  gpio_driver · system_init                   │
├──────────────────────────────────────────────┤
│  IO / HAL — registers · HAL_Xxx_Init()       │
├──────────────────────────────────────────────┤
│  STM32F4xx microcontroller                   │
└──────────────────────────────────────────────┘
```

---

## Two zones

### Platform-independent (green zone)
`signal_processing.c` · `packet_builder.c` · `cbuf.c`
`ienta_types.h` · `ienta_flags.h`

- Zero hardware includes
- Compile and run on STM32 or Mac with plain gcc
- Unit-testable without hardware
- Unchanged if the MCU changes

### Hardware-dependent (gray zone)
`adc_driver.c` · `uart_driver.c` · `timer_driver.c`
`gpio_driver.c` · `system_init.c`

- Knows registers, HAL, DMA, NVIC
- Changes if the MCU changes

---

## Decoupling rule

The contract between layers is a function signature — nothing else.

```c
// WRONG — signal_processing reaches into hardware
float compute_rms(void) {
    return sqrtf(ADC1->DR / 4096.0f);
}

// CORRECT — receives a plain buffer, pure math
float compute_rms(const uint16_t *buf, uint16_t n);
```

---

## Module responsibilities

| Module | One job |
|--------|---------|
| `adc_driver.c` | Acquire ADC samples into SRAM via DMA |
| `uart_driver.c` | Transmit `uart_packet_t` over USART2 |
| `timer_driver.c` | Trigger ADC at 4 kHz via TIM2 |
| `gpio_driver.c` | Control LED and relay |
| `signal_processing.c` | Compute Vrms, Irms, P, PF from buffer |
| `packet_builder.c` | Convert `measurement_t` → `uart_packet_t` |
| `cbuf.c` | Generic circular byte buffer |

---

## Public interfaces

```c
// adc_driver.h
void            adc_driver_init(void);
const uint16_t *adc_get_ready_buffer_v(void);
const uint16_t *adc_get_ready_buffer_i(void);

// uart_driver.h
void uart_driver_init(void);
void uart_send_packet(const uart_packet_t *pkt);

// timer_driver.h
void timer_driver_init(void);

// gpio_driver.h
void gpio_led_toggle(void);
void gpio_relay_set(uint8_t state);

// signal_processing.h
float compute_rms(const uint16_t *buf, uint16_t n);
float compute_power(const uint16_t *v, const uint16_t *i, uint16_t n);
float compute_pf(float P, float vrms, float irms);

// packet_builder.h
void build_packet(const measurement_t *m, uart_packet_t *pkt);

// cbuf.h
void    cbuf_init(cbuf_t *cb, uint8_t *buf, uint16_t size);
uint8_t cbuf_push(cbuf_t *cb, uint8_t byte);
uint8_t cbuf_pop(cbuf_t *cb, uint8_t *byte);
```

---

## Shared definitions

`ienta_types.h` — `measurement_t` and `uart_packet_t` defined once.
`ienta_flags.h` — `VALID_BIT`, `ANOMALY_BIT`, `OVERRANGE_BIT`, `DEVICE_BIT` defined once.
`cbuf.c` — one ring buffer, shared by all drivers. Never copy-pasted.
