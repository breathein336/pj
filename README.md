# pj
小玩具系列与学习历程
一、T800_F：

1. 本项目采用 STM32F407ZGT6 作为主控制器，外设包括：4 个 LED 指示灯与微动开关、0.96 英寸 OLED 屏幕、有源蜂鸣器以及声音检测器（低电平输出）。

2. 主要功能是通过按键 KEY（1~3）切换不同的 OLED 页面（共三页），每页显示不同功能：
Page1 为手动计数器，
Page2 为半自动定时器，
Page3 为自动计数器。
其中，LED1 和 LED2 用于指示当前所处页面（Page1 或 Page2），LED3 用于指示自动计数器的加数频率（1 秒/次闪烁，表示“1s/shake”状态）。

3. 项目基于 STM32CubeMX 配置，使用 Keil5 进行开发，程序采用模块化设计。

4. 原计划搭载 FreeRTOS，但由于 MCU 外部晶振无法起振（经代码调试确认），最终放弃 RTOS，转为裸机（bare-metal）开发。

5. 本项目为本人在校期间《ARM 嵌入式系统》课程的作业，功能较为简单，代码质量不高，属于早期练手项目。开源仅为了将项目存档于网络平台，如有不足之处，敬请谅解（不喜勿喷）。

6. 本项目以 MIT 许可证开源。

I. T800_F:

1. This project uses the STM32F407ZGT6 as the main controller, with the following peripherals: four LEDs and tactile switches, a 0.96-inch OLED display, an active buzzer, and a sound sensor (outputs low-level signal).

2. The primary functionality involves switching among three different OLED pages using keys KEY1 to KEY3, each page displaying a distinct feature:
Page 1: Manual counter
Page 2: Semi-automatic timer
Page 3: Automatic counter
LEDs 1 and 2 indicate the current page (Page 1 or Page 2), while LED 3 indicates the counting frequency of the automatic counter (blinking once per second, representing a "1s/shake" state).

3. The project is configured using STM32CubeMX and developed in Keil MDK-ARM (Keil5), with a modular software architecture.

4. FreeRTOS was originally intended for use; however, due to the MCU’s external crystal oscillator failing to start up (confirmed through code debugging), the project was ultimately shifted to bare-metal development without an RTOS.

5. This project was created as a coursework assignment for my university course “ARM Embedded Systems.” It features limited functionality and modest code quality, as it represents one of my early learning projects. It is open-sourced solely for archival purposes on online platforms—please be understanding of its shortcomings (constructive feedback welcome, but please refrain from harsh criticism).

6. This project is open-sourced under the MIT License.
