# Firmware Architecture

## Overview

The Ienta firmware is responsible for:
1. Real-time data acquisition from ADC
2. Signal processing and feature extraction
3. Communication with edge AI processor

**Language:** C (bare-metal or lightweight RTOS)  
**Target:** STM32 microcontroller  
**Paradigm:** Interrupt-driven, non-blocking

---

## Module Structure

### 1. ADC Driver (`adc_driver.c/h`)

**Responsibilities:**
- Configure ADC peripheral
- Setup DMA for continuous sampling
- Manage double buffering
- Trigger buffer swap on DMA complete

**Key Functions:**
```c
void adc_init(void);
void adc_start_sampling(void);
void adc_stop_sampling(void);
uint16_t* adc_get_ready_buffer(void);
```

**Interrupt Handler:**
```c
void DMA1_Channel1_IRQHandler(void) {
    // Swap buffers
    // Set buffer ready flag
    // Clear interrupt
}
```

---

### 2. Signal Processing (`signal_processing.c/h`)

**Responsibilities:**
- Calculate RMS values
- Compute real power
- Calculate power factor
- Detect anomalies (saturation, noise)

**Key Functions:**
```c
float calculate_rms(uint16_t* samples, uint16_t count);
float calculate_real_power(uint16_t* v_samples, uint16_t* i_samples, uint16_t count);
float calculate_power_factor(float power_real, float power_apparent);
```

**Algorithm Example (RMS):**
```c
float calculate_rms(uint16_t* samples, uint16_t count) {
    float sum_squares = 0.0f;
    
    for (uint16_t i = 0; i < count; i++) {
        // Convert ADC value to voltage
        float voltage = (samples[i] - ADC_MIDPOINT) * ADC_TO_VOLTAGE;
        sum_squares += voltage * voltage;
    }
    
    return sqrtf(sum_squares / count);
}
```

---

### 3. Circular Buffer (`circular_buffer.c/h`)

**Responsibilities:**
- Manage sample storage
- Provide thread-safe read/write
- Handle buffer overflow

**Data Structure:**
```c
typedef struct {
    uint16_t* buffer;
    uint16_t size;
    uint16_t head;
    uint16_t tail;
    uint8_t full;
} circular_buffer_t;
```

**Key Functions:**
```c
void cbuf_init(circular_buffer_t* cb, uint16_t* buffer, uint16_t size);
uint8_t cbuf_put(circular_buffer_t* cb, uint16_t data);
uint8_t cbuf_get(circular_buffer_t* cb, uint16_t* data);
uint8_t cbuf_is_full(circular_buffer_t* cb);
uint8_t cbuf_is_empty(circular_buffer_t* cb);
```

---

### 4. UART Communication (`uart_comm.c/h`)

**Responsibilities:**
- Transmit measurement packets to edge AI
- Handle transmission errors
- Maintain non-blocking operation (DMA-based)

**Packet Structure:**
```c
typedef struct {
    uint32_t timestamp;
    float voltage_rms;
    float current_rms;
    float power_real;
    float power_factor;
    uint8_t flags;
    uint16_t checksum;
} __attribute__((packed)) measurement_packet_t;
```

**Key Functions:**
```c
void uart_init(uint32_t baudrate);
void uart_send_packet(measurement_packet_t* packet);
uint16_t calculate_checksum(uint8_t* data, uint16_t length);
```

---

## Execution Flow

### Initialization Sequence
```
1. System clock configuration
2. GPIO initialization
3. ADC + DMA initialization
4. UART initialization
5. Enable interrupts
6. Start ADC sampling
7. Enter main loop
```

### Runtime Loop
```c
int main(void) {
    // Initialization
    system_init();
    adc_init();
    uart_init(115200);
    
    adc_start_sampling();
    
    while (1) {
        // Check if buffer ready
        if (buffer_ready_flag) {
            uint16_t* voltage_samples = adc_get_buffer(CHANNEL_VOLTAGE);
            uint16_t* current_samples = adc_get_buffer(CHANNEL_CURRENT);
            
            // Process samples
            measurement_packet_t packet;
            packet.timestamp = get_milliseconds();
            packet.voltage_rms = calculate_rms(voltage_samples, SAMPLES_PER_CYCLE);
            packet.current_rms = calculate_rms(current_samples, SAMPLES_PER_CYCLE);
            packet.power_real = calculate_real_power(voltage_samples, 
                                                     current_samples, 
                                                     SAMPLES_PER_CYCLE);
            packet.power_factor = calculate_power_factor(packet.power_real, 
                                                         packet.voltage_rms * packet.current_rms);
            packet.flags = get_status_flags();
            packet.checksum = calculate_checksum((uint8_t*)&packet, 
                                                 sizeof(packet) - 2);
            
            // Send via UART
            uart_send_packet(&packet);
            
            // Clear flag
            buffer_ready_flag = 0;
        }
        
        // Other housekeeping tasks
        check_system_health();
    }
}
```

---

## Memory Management

### Static Allocation Only
- **No dynamic memory allocation** (no `malloc`/`free`)
- All buffers allocated at compile time
- Predictable memory usage

### Buffer Sizing
```c
#define SAMPLES_PER_CYCLE   80      // 4kHz sampling / 50Hz = 80
#define NUM_BUFFERS         2       // Double buffering
#define NUM_CHANNELS        2       // Voltage + current

uint16_t adc_buffer_voltage[NUM_BUFFERS][SAMPLES_PER_CYCLE];
uint16_t adc_buffer_current[NUM_BUFFERS][SAMPLES_PER_CYCLE];
```

---

## Error Handling

### Status Flags
```c
typedef enum {
    FLAG_NORMAL          = 0x00,
    FLAG_ADC_OVERFLOW    = 0x01,
    FLAG_ADC_UNDERFLOW   = 0x02,
    FLAG_SENSOR_ERROR    = 0x04,
    FLAG_UART_ERROR      = 0x08,
    FLAG_TIMING_ERROR    = 0x10
} status_flags_t;
```

### Error Recovery
```c
void handle_error(status_flags_t error) {
    switch (error) {
        case FLAG_ADC_OVERFLOW:
            // Skip current measurement
            // Log error
            break;
        
        case FLAG_UART_ERROR:
            // Reinitialize UART
            // Increment error counter
            break;
        
        // ... other cases
    }
}
```

---

## Coding Standards

### Naming Conventions
- **Functions:** `lowercase_with_underscores()`
- **Variables:** `lowercase_with_underscores`
- **Constants:** `UPPERCASE_WITH_UNDERSCORES`
- **Types:** `lowercase_with_underscores_t`

### Type Safety
```c
// Use fixed-width types
uint8_t  flags;           // NOT: unsigned char
uint16_t adc_value;       // NOT: unsigned int
uint32_t timestamp;       // NOT: unsigned long
float    voltage;         // OK for floating point
```

### Comments
```c
// Single-line comments for brief explanations

/**
 * @brief Calculate RMS value from ADC samples
 * @param samples Pointer to sample buffer
 * @param count Number of samples
 * @return RMS value in volts
 */
float calculate_rms(uint16_t* samples, uint16_t count);
```

---

## Performance Targets

| Metric | Target | Critical? |
|--------|--------|-----------|
| ADC sampling | 4kHz continuous | Yes |
| RMS calculation | <5ms for 80 samples | Yes |
| UART transmission | <3ms per packet | No |
| CPU utilization | <50% | No |
| Interrupt latency | <10μs | Yes |

---

## Testing Strategy

### Unit Tests (Development Phase)
- Test each module independently
- Use known input → verify output
- Example: RMS of pure sine wave = peak / √2

### Integration Tests
- Full signal chain with simulated ADC data
- Verify timing and synchronization
- Check error handling

### Hardware Tests (Phase 2)
- Real CT sensor + known load
- Compare measurements vs multimeter
- Long-duration stability test

---

## Future Enhancements (Post-Phase 2)

- [ ] Harmonic analysis (FFT)
- [ ] Waveform recording for anomaly detection
- [ ] Over-the-air firmware updates
- [ ] Power-saving modes
- [ ] Self-calibration routines

---

**Document Version:** 1.0  
**Last Updated:** Phase 0  
**Next Review:** Phase 1 completion
