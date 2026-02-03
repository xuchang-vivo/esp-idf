// Copyright (c) 2026 vivo Mobile Communication Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "hal/uart_hal.h"

uint32_t rust_helper_uart_hal_get_rxfifo_len(uart_hal_context_t *hal)
{
    return uart_ll_get_rxfifo_len(hal->dev);
}

uint32_t rust_helper_uart_hal_get_txfifo_len(uart_hal_context_t *hal)
{
    return uart_ll_get_txfifo_len(hal->dev);
}

void rust_helper_uart_clr_intsts_mask(uart_hal_context_t *hal, uint32_t mask)
{
    uart_ll_clr_intsts_mask(hal->dev, mask);
}

uint32_t rust_helper_uart_hal_get_intsts_mask(uart_hal_context_t *hal)
{
    return uart_ll_get_intr_ena_status(hal->dev);
}

void rust_helper_uart_hal_disable_intr_mask(uart_hal_context_t *hal, uint32_t mask)
{
    uart_ll_disable_intr_mask(hal->dev, mask);
}

void rust_helper_uart_hal_ena_intr_mask(uart_hal_context_t *hal, uint32_t mask)
{
    uart_ll_ena_intr_mask(hal->dev, mask);
}

int rust_helper_uart_hal_set_baudrate(uart_hal_context_t *hal, uint32_t baud_rate, uint32_t sclk_freq)
{
    return uart_ll_set_baudrate(hal->dev, baud_rate, sclk_freq);
}

void rust_helper_uart_hal_set_sclk(uart_hal_context_t *hal, soc_module_clk_t sclk)
{
    return uart_ll_set_sclk(hal->dev, sclk);
}
