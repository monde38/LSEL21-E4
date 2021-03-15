#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"

#include "fsm.h"
#include "fsm_led_alarm.h"
#include "hardware_esp8266.h"
#include "fsm_door_checking.h"

uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;
    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void task_blink(void* ignore)
{
    fsm_led_alarm_t  led1_fsm;
    fsm_led_alarm_init(&led1_fsm, D0, 100);

    fsm_door_checking_t  l_door;
    fsm_door_checking_init(&l_door, D1);
    
    hard_gpio_pinSetUp(D2, HARD_GPIO_INPUT);
    hard_gpio_pinSetUp(D3, HARD_GPIO_OUTPUT);

    while (1) {

        fsm_led_alarm_turn_on_off_active(&led1_fsm, hard_gpio_digitalRead(D2));
        fsm_door_checking_turn_on_off_active(&l_door, hard_gpio_digitalRead(D2));

        fsm_fire((fsm_t*)&led1_fsm);
        fsm_fire((fsm_t*)&l_door);

        if (fsm_door_checking_is_in_alarm(&l_door)) hard_gpio_digitalWrite(D3,1);
        else hard_gpio_digitalWrite(D3,0);
  
    }

    vTaskDelete(NULL);
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
    xTaskCreate(&task_blink, "startup", 2048, NULL, 1, NULL);
}