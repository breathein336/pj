#include "stm32f4xx_hal.h"
#include "main.h"
#include "gpio.h"
#include "main.h"
#include "key.h"


void KEY_scan(void){
    static uint8_t key_state[5] = {0};
    if(HAL_GPIO_ReadPin(GPIOA, KEY3_Pin) == GPIO_PIN_RESET){
			HAL_Delay(100);
        if(key_state[0] == 0){
            if(page==1){
                num++;
            }
            key_state[0] = 1;
        }
    }else{key_state[0] = 0;
    }
    if(HAL_GPIO_ReadPin(GPIOA, KEY2_Pin) == GPIO_PIN_RESET){
			HAL_Delay(100);
        if(key_state[1] == 0){
            if(page==1&&num>0){
                num--;
            }
            key_state[1] = 1;
        }
    }else{key_state[1] = 0;
    }

if(HAL_GPIO_ReadPin(GPIOA, KEY1_Pin) == GPIO_PIN_RESET){ 
    HAL_Delay(20); // 缩短消抖时间
    if(key_state[2] == 0){
        page++; // 切换到下一页
        if(page > 3) page = 1; // 循环回到第一页（假设只有3个页面）
        key_state[2] = 1;
    }
} else {
    key_state[2] = 0;
}

    if(HAL_GPIO_ReadPin(GPIOA, KEY3_Pin) == GPIO_PIN_RESET) {
			HAL_Delay(120);
        if(key_state[3] == 0) {
            if(page == 2) {
                auto_count_enabled = !auto_count_enabled;
            }
            key_state[3] = 1;
        }
    } else {
        key_state[3] = 0;
    }
		
    if(HAL_GPIO_ReadPin(GPIOA, KEY2_Pin) == GPIO_PIN_RESET) {
			HAL_Delay(120);
        if(key_state[4] == 0) {
            if(page == 2&&auto_num&&auto_seconds>1) {
							auto_count_enabled = 0;
              auto_num=0;
            }
            key_state[4] = 1;
        }
    } else {
        key_state[4] = 0;
    }
}
