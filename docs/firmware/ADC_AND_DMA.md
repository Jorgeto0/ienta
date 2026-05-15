# ADC and DMA

ADC acquisition state machine, double-buffer pattern, ISR body, and
timing budget.

---

## State machine

```
boot → IDLE
         │ HAL_ADC_Start_DMA()
         ▼
     SAMPLING ◄──────────────────────────────────┐
         │ DMA transfer-complete ISR fires         │
         │  1. active_idx ^= 1                     │
         │  2. restart DMA on fresh buffer         │
         │  3. buf_ready = 1                       │
         ▼                                         │
     BUFFER_READY                                  │
         │ main loop sees buf_ready == 1           │
         ▼                                         │
     PROCESSING (~1.5 ms)                          │
         │ compute_rms · compute_power · fill m    │
         ▼                                         │
     TX_SEND (~120 µs)                             │
         │ build_packet · uart_send_packet         │
         │ buf_ready = 0                           │
         └─────────────────────────────────────────┘

     ERROR (any state)
         │ overrange / DMA fault / UART timeout
         │ set flag bits · attempt recovery
         │ 3 failures → HALT + LED SOS
```

---

## Double-buffer layout

```c
volatile uint16_t adc_v[2][N_SAMPLES];
volatile uint16_t adc_i[2][N_SAMPLES];
volatile uint8_t  active_idx;   // DMA writes to this index
volatile uint8_t  buf_ready;    // set by ISR, cleared by main loop
```

DMA fills `adc_v[active_idx]`.
Main loop reads `adc_v[active_idx ^ 1]`.
They never touch the same buffer.

`volatile` is mandatory — without it the compiler caches stale values.

---

## ISR body

```c
void DMA2_Stream0_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_adc1);
    active_idx ^= 1u;
    HAL_ADC_Start_DMA(&hadc1,
        (uint32_t *)adc_v[active_idx], N_SAMPLES);
    buf_ready = 1u;
}
```

Must exit in < 1 µs. Never call printf, HAL_UART_Transmit,
malloc, or anything that blocks or takes a lock.

---

## Timing budget per 20 ms cycle

| State | Duration | CPU % |
|-------|----------|-------|
| PROCESSING | ~1.5 ms | 7.5% |
| TX_SEND | ~0.12 ms | 0.6% |
| SAMPLING (DMA, CPU free) | ~18.4 ms | 91.9% |

Target: < 20% CPU utilization.

---

## Sampling parameters

| Parameter | Value |
|-----------|-------|
| Sample rate | 4000 Hz |
| Samples per cycle | 80 |
| Channels | 2 (voltage + current) |
| ADC resolution | 12-bit (0–4095) |
| Signal midpoint | 2048 |
| Overrange threshold | 4000 |
| Packet rate | 50 Hz |
