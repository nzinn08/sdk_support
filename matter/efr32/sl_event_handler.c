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

    // Set up the system clock to run off the HFXO.
    // Set the system clock to a known other source while configuring the HFXO.
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);
    // The init struct was copied from the CMU_HFXOINIT_DEFAULT definition and is
    // duplicated here for visibility and conistency (in case the library's
    // default changes).
    CMU_HFXOInit_TypeDef hfxoInit = {
        false, /* Low-noise mode for EFR32 */
        false, /* @deprecated no longer in use */
        false, /* @deprecated no longer in use */
        false, /* @deprecated no longer in use */
        _CMU_HFXOSTARTUPCTRL_CTUNE_DEFAULT,
        267,
        0xA,  /* Default Shunt steady-state current */
        0x20, /* Matching errata fix in @ref CHIP_Init() */
        0x7,  /* Recommended steady-state XO core bias current */
        0x6,  /* Recommended peak detection threshold */
        0x2,  /* Recommended shunt optimization timeout */
        0xA,  /* Recommended peak detection timeout  */
        0x4,  /* Recommended steady timeout */
        _CMU_HFXOTIMEOUTCTRL_STARTUPTIMEOUT_DEFAULT,
        cmuOscMode_Crystal,
    };
    CMU_HFXOInit(&hfxoInit);
    SystemHFXOClockSet(38400000);
    CMU_OscillatorEnable(cmuOsc_HFXO, true, true);
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    // Turn off the HFRCO now that we aren't using it anymore
    CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);

    // Initialize the global system clock value
    SystemCoreClockUpdate();
    // sl_device_init_hfxo();
    // sl_device_init_lfxo();
    // sl_device_init_clocks();
    // sl_device_init_emu();
    // sl_board_init();
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
