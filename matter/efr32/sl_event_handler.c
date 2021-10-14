#include "sl_event_handler.h"

#include "em_chip.h"
#include "gpiointerrupt.h"
#include "pa_conversions_efr32.h"
#include "sl_board_init.h"
#include "sl_device_init_clocks.h"
#if defined(EFR32MG21)
#include "sl_device_init_hfrco.h"
#include "sl_rail_util_rf_path.h"
#else
#include "sl_device_init_dcdc.h"
#endif
#include "cmsis_os2.h"
#include "sl_device_init_dcdc.h"
#include "sl_device_init_emu.h"
#include "sl_device_init_hfxo.h"
#include "sl_device_init_lfxo.h"
#include "sl_device_init_nvic.h"
#include "sl_rail_util_pti.h"
#include "sl_simple_button_instances.h"
#include "sl_simple_led_instances.h"
#include "sl_sleeptimer.h"
#include "sl_uartdrv_instances.h"
#include "sl_hfxo_manager.h"


void sl_platform_init(void)
{
    CHIP_Init();
    sl_device_init_nvic();
    sl_board_preinit();
#if defined(EFR32MG21)
    sl_device_init_hfrco();
#else
    sl_device_init_dcdc();
#endif

#if defined(_SILICON_LABS_32B_SERIES_2)
    sl_hfxo_manager_init_hardware();
#endif
    // Turn on the HF peripheral bus
    CMU_ClockEnable(cmuClock_HFPER, true);

    sl_device_init_hfxo();
    //sl_device_init_lfxo();
    sl_device_init_clocks();
    sl_device_init_emu();
    sl_board_init();
    osKernelInitialize();
}

void sl_kernel_start(void)
{
    osKernelStart();
}

void sl_driver_init(void)
{
    GPIOINT_Init();
    // sl_simple_button_init_instances();
    // sl_simple_led_init_instances();
    sl_uartdrv_init_instances();
}

void sl_service_init(void)
{
    sl_sleeptimer_init();
#if defined(_SILICON_LABS_32B_SERIES_2)
    sl_hfxo_manager_init();
#endif
}

void sl_stack_init(void)
{
    sl_rail_util_pa_init();
    sl_rail_util_pti_init();
#if defined(EFR32MG21)
    sl_rail_util_rf_path_init();
#endif
}

void sl_internal_app_init(void) {}

void sl_platform_process_action(void) {}

void sl_service_process_action(void) {}

void sl_stack_process_action(void) {}

void sl_internal_app_process_action(void) {}
