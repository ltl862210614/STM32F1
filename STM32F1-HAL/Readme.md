# STM32F103 - 基于HAL库

***

## 更新记录：

|    Date    | Version | Author | Note                                                          |
| :--------: | :-----: | :----: | :------------------------------------------------------------ |
| 2021/12/02 |  v1.0.0 |  naihe | 基于HAL库建立工程                                                    |
| 2022/04/20 |  v1.0.0 |  naihe | 移植FreeRTOS                                                    |
| 2022/04/26 |  v1.0.0 |  naihe | 添加应用button; 移植flexible\_button库;                              |
| 2022/04/27 |  v1.0.0 |  naihe | 添加led应用; 添加usart初始化接口;                                        |
| 2022/04/28 |  v1.0.0 |  naihe | 修改RCC配置时序;                                                    |
| 2022/05/01 |  v1.0.0 |  naihe | 添加usart1串口打印;                                                 |
| 2022/05/04 |  v1.0.0 |  naihe | 修改FreeRTOSConfig,配置system service,RTOS运行ok; 实现串口1空闲中断接收不定长数据; |

***

## Toolchain supported：

*   Keil MDK  5.22

\==========================================
Hardware requirements：
杨桃1号开发板

\==========================================
Board settings：
No special settings are required.

\==========================================
Burn Flash:

1.  Connect a serial board between the host PC and the open USB port(JXX) on the target board.

2.  Open the serial terminal(STM32 ST-LINK Utility.exe) with the following settings:

    *   115200 baud rate

3.  Download the program to the target board.

\==========================================
Debug：

1.  Connect a serial board between the host PC and the open USB port(JXX) on the target board.

2.  Open the serial terminal with the following settings:

    *   115200 baud rate

    *   8 data bits

    *   No parity

    *   One stop bit

    *   No flow control

\==========================================
Customization options:
