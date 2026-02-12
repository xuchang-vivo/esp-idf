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

void __attribute__((noreturn)) __assert_func(const char *file, int line, const char *func, const char *expr)
{
    while (1)
    {
    }
}

#include "esp_log.h"
#include "esp_attr.h"
#include "hal/cache_hal.h"
#include "hal/mmu_hal.h"
#include "hal/wdt_hal.h"
#include "esp_clk_internal.h"
#include "bootloader_mem.h"
#include "esp_private/esp_clk_tree_common.h"
#include "soc/interrupts.h" 
#include "esp_cpu.h"

ESP_LOG_ATTR_TAG(TAG, "helpers");

extern int __bss_start;
extern int __bss_end;

void sys_rtc_init(const soc_reset_reason_t *rst_reas)
{
#if CONFIG_IDF_TARGET_ESP32P4
#define RWDT_RESET RESET_REASON_CORE_RWDT
#define MWDT_RESET RESET_REASON_CORE_MWDT
#else
#define RWDT_RESET RESET_REASON_CORE_RTC_WDT
#define MWDT_RESET RESET_REASON_CORE_MWDT0
#endif
    if (rst_reas[0] == RWDT_RESET || rst_reas[0] == MWDT_RESET)
    {
        wdt_hal_context_t rtc_wdt_ctx = RWDT_HAL_CONTEXT_DEFAULT();
        wdt_hal_write_protect_disable(&rtc_wdt_ctx);
        wdt_hal_disable(&rtc_wdt_ctx);
        wdt_hal_write_protect_enable(&rtc_wdt_ctx);
    }

    esp_rtc_init();
}

static void core_intr_matrix_clear(void)
{
    __attribute__((unused)) uint32_t core_id = esp_cpu_get_core_id();

    /* NOTE: With ESP-TEE enabled, each iteration in this loop results in a service call.
    * To accelerate the boot-up process, the interrupt configuration is pre-cleared in the TEE,
    * allowing this step to be safely skipped here.
    */
#if !CONFIG_SECURE_ENABLE_TEE
    for (int i = 0; i < ETS_MAX_INTR_SOURCE; i++) {
#if SOC_INT_CLIC_SUPPORTED
        interrupt_clic_ll_route(core_id, i, ETS_INVALID_INUM);
#else
        esp_rom_route_intr_matrix(core_id, i, ETS_INVALID_INUM);
#endif  // SOC_INT_CLIC_SUPPORTED
    }
#endif  // !CONFIG_SECURE_ENABLE_TEE

#if SOC_INT_CLIC_SUPPORTED
    for (int i = 0; i < 32; i++) {
        /* Set all the CPU interrupt lines to vectored by default, as it is on other RISC-V targets */
        esprv_int_set_vectored(i, true);
    }
#endif // SOC_INT_CLIC_SUPPORTED
}

void IRAM_ATTR soc_init(void)
{
    if (esp_cpu_dbgr_is_attached()) {
        for (uint32_t ms_num = 0; ms_num < 2; ms_num++) {
            esp_rom_delay_us(100000);
        }
    }
    
    // get reset reason
    soc_reset_reason_t rst_reas[1] = {RESET_REASON_CHIP_POWER_ON};
    rst_reas[0] = esp_rom_get_reset_reason(0);

    // init bss
    memset(&__bss_start, 0, (uintptr_t)&__bss_end - (uintptr_t)&__bss_start);

    // init ext memory
    cache_hal_config_t config = {
        .core_nums = 1,
    };

    cache_hal_init(&config);

    mmu_hal_config_t mmu_config = {
        .core_nums = 1,
    };
    mmu_hal_ctx_init(&mmu_config);

    // init rtc
    sys_rtc_init(rst_reas);

    esp_clk_tree_initialize();
    esp_clk_init();
    esp_perip_clk_init();

    // Clear interrupt matrix for PRO CPU core
    core_intr_matrix_clear();
}

void rust_helper_esp_cpu_inter_enable(uint32_t intr_mask)
{
    esp_cpu_intr_enable(intr_mask);
}

void rust_helper_esp_cpu_intr_set_priority(int rv_int_num, int priority)
{
    esp_cpu_intr_set_priority(rv_int_num, priority);
}
