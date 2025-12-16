#include "stm32f4xx_hal.h"
#include "OLED_Font.h"
#include "i2c.h"
#include "OLED.h"
// 外部变量声明
extern I2C_HandleTypeDef hi2c1;
extern uint8_t num;
extern uint8_t page;
extern uint8_t auto_num;
extern uint8_t auto_count_enabled;
extern uint8_t auto_seconds;
extern uint8_t current_system_time;
// OLED从机地址
#define OLED_ADDRESS 0x78

/*引脚初始化*/
void OLED_I2C_Init(void)
{
    // 使用硬件I2C，不需要软件初始化GPIO
    // 硬件I2C已在MX_I2C1_Init中初始化
}

/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WriteCommand(uint8_t Command)
{
    uint8_t data[2] = {0x00, Command};
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, data, 2, 100);
    if (status != HAL_OK) {
        // 错误处理（可选）
    }
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
    uint8_t data[2] = {0x40, Data};
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, data, 2, 100);
    if (status != HAL_OK) {
        // 错误处理（可选）
    }
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    OLED_WriteCommand(0xB0 | Y);                    //设置Y位置
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));    //设置X位置高4位
    OLED_WriteCommand(0x00 | (X & 0x0F));           //设置X位置低4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void)
{  
    uint8_t i, j;
    for (j = 0; j < 8; j++)
    {
        OLED_SetCursor(j, 0);
        for(i = 0; i < 128; i++)
        {
            OLED_WriteData(0x00);
        }
    }
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{       
    uint8_t i;
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);        //设置光标位置在上半部分
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i]);            //显示上半部分内容
    }
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);    //设置光标位置在下半部分
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);       //显示下半部分内容
    }
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        OLED_ShowChar(Line, Column + i, String[i]);
    }
}

uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    return Result;
}

/**
  * @brief  OLED显示数字
  * @param  Line 行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字
  * @param  Length 要显示的位数
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)							
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

void OLED_Display(void) {
    static uint8_t current_page = 0;
    
    // 页面切换时，重置初始化标志
    if(current_page != page) {
        display_initialized[current_page-1] = 0; // 重置旧页面初始化状态
        display_initialized[page-1] = 0;         // 重置新页面初始化状态
        current_page = page;
    }
    
    switch(page) {
        case 1:
            OLED_Display_Page1();
            break;
        case 2:
            OLED_Display_Page2();
            break;
        case 3:
            OLED_Display_Page3();
            break;
        default:
            page = 1;
            OLED_Display_Page1();
    }
}

// 优化：只在数值变化时更新显示
void OLED_Display_Page1(void) {
    // 首次进入页面，全屏初始化
    if(!display_initialized[0]) {
        OLED_Clear();
        OLED_ShowString(1, 1, "MANUAL COUNT");
        OLED_ShowString(2, 3, "VALUE:");
        OLED_ShowString(4, 1, "< +  - > PAGE  AUTO");
        display_initialized[0] = 1;
        last_num = num - 1; // 确保首次显示
    }
    
    // 仅在数值变化时更新
    if(num != last_num) {
        // 局部擦除方法1：用空格覆盖
        OLED_ShowString(2, 10, "   "); // 清除3位数字区域
        
        // 局部擦除方法2（更优）：直接覆盖而不先擦除
        OLED_ShowNum(2, 10, num, 3); // 显示3位数字
        
        last_num = num; // 更新上次显示值
    }
}

void OLED_Display_Page2(void) {
    // 首次进入页面，全屏初始化
    if(!display_initialized[1]) {
        OLED_Clear();
        OLED_ShowString(1, 1, "AUTO COUNT");
        OLED_ShowString(2, 3, "STATUS:");
        OLED_ShowString(3, 3, "COUNT:");
        OLED_ShowString(4, 2, "TIME(s):");
        display_initialized[1] = 1;
        last_auto_enabled = !auto_count_enabled; // 确保首次显示
        last_auto_num = auto_num - 1;            // 确保首次显示
        last_auto_seconds = auto_seconds - 1;   // 确保首次显示
    }
    
    // 1. 仅在自动计数状态变化时更新状态显示
    if(auto_count_enabled != last_auto_enabled) {
        OLED_ShowString(2, 12, "    "); // 清除4字符区域
        if(auto_count_enabled) {
            OLED_ShowString(2, 12, "ON ");
        } else {
            OLED_ShowString(2, 12, "OFF");
        }
        last_auto_enabled = auto_count_enabled;
    }
    
    // 2. 仅在自动计数值变化时更新
    if(auto_num != last_auto_num) {
        OLED_ShowString(3, 10, "     "); // 清除5字符区域
        OLED_ShowNum(3, 10, auto_num, 5); // 显示5位数字
        last_auto_num = auto_num;
    }
    
    // 3. 仅在运行时间变化时更新
    if(auto_seconds != last_auto_seconds) {
        OLED_ShowString(4, 11, "     "); // 清除5字符区域
        OLED_ShowNum(4, 11, auto_seconds, 5);
        last_auto_seconds = auto_seconds;
    }
}

void OLED_Display_Page3(void) {
    static uint32_t last_system_time = 0;
    uint32_t current_system_time = HAL_GetTick() / 1000;
    
    // 首次进入页面，全屏初始化
    if(!display_initialized[2]) {
        OLED_Clear();
        OLED_ShowString(1, 1, "SYSTEM INFO");
        OLED_ShowString(2, 3, "PAGE:");
        OLED_ShowString(3, 3, "TIME(s):");
        display_initialized[2] = 1;
        last_system_time = current_system_time - 1; // 确保首次显示
    }
    
    // 1. 仅在页面变化时更新
    if(page != last_num) { // 复用last_num变量存储上次页面
        OLED_ShowNum(2, 9, page, 1);
        last_num = page;
    }
    
    // 2. 仅在系统时间变化时更新
    if(current_system_time != last_system_time) {
        OLED_ShowString(3, 11, "     "); // 清除5字符区域
        OLED_ShowNum(3, 11, current_system_time, 5);
        last_system_time = current_system_time;
    }
};
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t *BMP) {
    uint8_t x, y;
    uint8_t page_start = y0 / 8;
    uint8_t page_end = y1 / 8;
    uint16_t index = 0;

    if (y1 % 8 != 0) page_end++;

    for (y = page_start; y < page_end; y++) {
        OLED_SetCursor(y, x0);
        for (x = x0; x <= x1; x++) {
            OLED_WriteData(BMP[index++]);
        }
    }
}
void OLED_Init(void)
{
    HAL_Delay(100);  // 硬件复位延时
    
    OLED_I2C_Init();
    
    // 标准SSD1306初始化序列
    OLED_WriteCommand(0xAE); // 关闭显示
    OLED_WriteCommand(0xD5); // 设置时钟分频因子
    OLED_WriteCommand(0x80); 
    OLED_WriteCommand(0xA8); // 设置驱动路数
    OLED_WriteCommand(0x3F); 
    OLED_WriteCommand(0xD3); // 设置显示偏移
    OLED_WriteCommand(0x00); 
    OLED_WriteCommand(0x40); // 设置显示开始行
    OLED_WriteCommand(0xA1); // 设置段重定义
    OLED_WriteCommand(0xC8); // 设置COM扫描方向
    OLED_WriteCommand(0xDA); // 设置COM硬件引脚配置
    OLED_WriteCommand(0x12); 
    OLED_WriteCommand(0x81); // 对比度设置
    OLED_WriteCommand(0xCF); 
    OLED_WriteCommand(0xD9); // 设置预充电周期
    OLED_WriteCommand(0xF1); 
    OLED_WriteCommand(0xDB); // 设置VCOMH电压倍率
    OLED_WriteCommand(0x30); 
    OLED_WriteCommand(0xA4); // 全局显示开启
    OLED_WriteCommand(0xA6); // 正常显示
    OLED_WriteCommand(0x8D); // 电荷泵设置
    OLED_WriteCommand(0x14); 
    OLED_WriteCommand(0xAF); // 开启显示
        
    OLED_Clear();
}
