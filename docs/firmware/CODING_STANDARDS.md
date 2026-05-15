# Coding Standards

All Ienta firmware follows these rules without exception.

---

## 1. `1u` in shift expressions

```c
flags |= (1u << VALID_BIT);    // correct
flags |= (1  << VALID_BIT);    // wrong — signed shift, undefined behavior
```

---

## 2. `f` suffix on float literals

```c
float scale = 0.1611f;   // correct — single-precision FPU
float scale = 0.1611;    // wrong — double, software emulation
```

---

## 3. `sqrtf()` not `sqrt()`

```c
float rms = sqrtf(acc / N_SAMPLES);  // correct — VSQRT.F32 instruction
float rms = sqrt(acc / N_SAMPLES);   // wrong — software double
```

---

## 4. `|=` on peripheral registers

```c
GPIOA->ODR |= GPIO_ODR_OD5;    // correct — sets bit only
GPIOA->ODR  = GPIO_ODR_OD5;    // wrong — clears all other bits
```

---

## 5. `volatile` on ISR-shared variables

```c
volatile uint8_t buf_ready = 0;   // correct
uint8_t buf_ready = 0;            // wrong — compiler may cache it
```

---

## 6. No `malloc`

```c
static uint8_t buf[256];
cbuf_init(&cb, buf, sizeof(buf));  // correct

uint8_t *buf = malloc(256);        // wrong — forbidden
```

---

## 7. No blocking delays in the main loop

```c
if (systick_elapsed_ms() >= 100) { }  // correct
HAL_Delay(100);                        // wrong — blocks the loop
```

---

## 8. Short ISR bodies

```c
// correct
void DMA2_Stream0_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_adc1);
    active_idx ^= 1u;
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_v[active_idx], N_SAMPLES);
    buf_ready = 1u;
}

// wrong — HAL_UART_Transmit can block for 1 second
void DMA2_Stream0_IRQHandler(void) {
    HAL_UART_Transmit(&huart2, data, len, 1000);
}
```

---

## Quick reference

| Rule | Wrong | Correct |
|------|-------|---------|
| Shift | `1 << n` | `1u << n` |
| Float | `0.1611` | `0.1611f` |
| Sqrt | `sqrt(x)` | `sqrtf(x)` |
| Register | `REG = val` | `REG \|= val` |
| ISR var | `uint8_t f` | `volatile uint8_t f` |
| Memory | `malloc(n)` | static array |
| Delay | `HAL_Delay(n)` | timer flag |
| ISR | long/blocking | flag + exit |
