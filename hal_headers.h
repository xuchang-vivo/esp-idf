#include <hal/uart_hal.h>
#include <esp_clk_tree.h>

uint32_t rust_helper_uart_hal_get_rxfifo_len(uart_hal_context_t *hal);

uint32_t rust_helper_uart_hal_get_txfifo_len(uart_hal_context_t *hal);

void rust_helper_uart_clr_intsts_mask(uart_hal_context_t *hal, uint32_t mask);

uint32_t rust_helper_uart_hal_get_intsts_mask(uart_hal_context_t *hal);

void rust_helper_uart_hal_disable_intr_mask(uart_hal_context_t *hal, uint32_t mask);

void rust_helper_uart_hal_ena_intr_mask(uart_hal_context_t *hal, uint32_t mask);

int rust_helper_uart_hal_set_baudrate(uart_hal_context_t *hal, uint32_t baud_rate, uint32_t sclk_freq);

int rust_helper_uart_hal_set_sclk(uart_hal_context_t *hal, soc_module_clk_t sclk);
