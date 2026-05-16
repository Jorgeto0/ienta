/**
 * ============================================================
 * Ienta — main.c
 * ------------------------------------------------------------
 * Phase 1, Step 2: blink LD2 (green LED, PA5) on Nucleo-F401RE.
 *
 * Purpose
 *   Prove the toolchain works end-to-end: source → compile →
 *   link → flash → execute on real silicon. The blinking LED
 *   is the simplest possible visible proof.
 *
 * Hardware
 *   STM32F401RE on Nucleo-F401RE board.
 *   Green LED LD2 is hardwired to PA5 (per UM1724, the Nucleo
 *   user manual). High = LED on, low = LED off.
 *
 * Approach
 *   1. Enable the AHB1 clock for GPIOA (RCC->AHB1ENR bit 0).
 *      Until this happens, GPIOA does not respond to anything.
 *   2. Configure PA5 as a digital push-pull output by writing
 *      0b01 into bits [11:10] of GPIOA->MODER.
 *   3. In an infinite loop, toggle GPIOA->ODR bit 5 with a
 *      busy-loop delay between toggles.
 *
 * Known debt
 *   The delay is a CPU-spin loop. Imprecise, optimization-
 *   dependent, and wastes power. Acceptable for first-program
 *   sanity check; will be replaced with SysTick in a later step.
 * ============================================================
 */

#include "stm32f4xx.h"

/* Crude busy-wait. Iteration count tuned by feel; exact period
 * depends on -O level and CPU clock. We just need "blink slow
 * enough to see." */
static void delay_busy(volatile uint32_t cycles) {
  while (cycles--) {
    __asm__ volatile("nop");
  }
}

int main(void) {
  /* --- Step 1: enable the GPIOA peripheral clock. --- */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  /* --- Step 2: configure PA5 as a general-purpose output. ---
   * MODER has 2 bits per pin. PA5 occupies bits [11:10].
   * We must clear both bits first, then set bit 10 (mode = 01).
   * The named macros from stm32f401xe.h make this explicit. */
  GPIOA->MODER &= ~GPIO_MODER_MODER5;  /* clear bits [11:10] */
  GPIOA->MODER |= GPIO_MODER_MODER5_0; /* set bit 10 (mode = 01, output) */

  /* --- Step 3: blink forever. ---
   * ODR bit 5 controls PA5's output level.
   * XOR with the bit toggles it on each pass. */
  while (1) {
    GPIOA->ODR ^= GPIO_ODR_OD5;
    delay_busy(1);
  }

  /* Unreachable; bare-metal main never returns. */
}
