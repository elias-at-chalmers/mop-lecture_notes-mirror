# CH32V307 Interrupt Vector Table (Complete)

## 
| No. | Priority | Name              | Description             | Address      |
|----:|---------:|-------------------|-------------------------|--------------|
| 0   | -        | -                 | Reserved | 0x00000000   |
| 1   | -        | -                 | Reserved | 0x00000004   |
| 2   | -5 (fixed)      | NMI               | Non-maskable interrupt  | 0x00000008   |
| 3   | -4 (fixed)      | HardFault         | Hard fault exception    | 0x0000000C   |
| 4   | -        | -                 | Reserved                       | 0x00000010   |
| 5   | -3 (fixed)      | ECALL-M           | Machine-mode ECALL      | 0x00000014   |
| 6–7 | -        | -                 | Reserved                       | 0x00000018–1C|
| 8   | -2 (fixed)      | ECALL-U           | User-mode ECALL | 0x00000020   |
| 9   | -1 (fixed)      | BreakPoint        | Breakpoint exception | 0x00000024   |
| 10–11 | -      | -                 | Reserved                       
| 12  | 0       | SysTick | System timer interrupt  | 0x00000030   |
| 13  | -        | -       | Reserved                | 0x00000034   |
| 14  | 1        | SW      | Software interrupt      | 0x00000038   |
| 15  | -        | -       | Reserved                | 0x0000003C   |

# Peripheral Interrupts (Programmable: 16–104)
| No. | Name              | Description                                      | Address      |
|----:|-------------------|--------------------------------------------------|--------------|
| 16  | WWDG              | Window watchdog                                  | 0x00000040   |
| 17  | PVD               | PVD via EXTI                                     | 0x00000044   |
| 18  | TAMPER            | Tamper interrupt                                 | 0x00000048   |
| 19  | RTC               | RTC global interrupt                             | 0x0000004C   |
| 20  | FLASH             | Flash global interrupt                           | 0x00000050   |
| 21  | RCC               | RCC global interrupt                             | 0x00000054   |
| 22  | EXTI0             | EXTI line0 interrupt                             | 0x00000058   |
| 23  | EXTI1             | EXTI line1 interrupt                             | 0x0000005C   |
| 24  | EXTI2             | EXTI line2 interrupt                             | 0x00000060   |
| 25  | EXTI3             | EXTI line3 interrupt                             | 0x00000064   |
| 26  | EXTI4             | EXTI line4 interrupt                             | 0x00000068   |
| 27  | DMA1_CH1          | DMA1 Channel 1                                   | 0x0000006C   |
| 28  | DMA1_CH2          | DMA1 Channel 2                                   | 0x00000070   |
| 29  | DMA1_CH3          | DMA1 Channel 3                                   | 0x00000074   |
| 30  | DMA1_CH4          | DMA1 Channel 4                                   | 0x00000078   |
| 31  | DMA1_CH5          | DMA1 Channel 5                                   | 0x0000007C   |
| 32  | DMA1_CH6          | DMA1 Channel 6                                   | 0x00000080   |
| 33  | DMA1_CH7          | DMA1 Channel 7                                   | 0x00000084   |
| 34  | ADC1_2            | ADC1/ADC2 global                                 | 0x00000088   |
| 35  | USB_HP/CAN1_TX    | USB FS HP or CAN1 TX                             | 0x0000008C   |
| 36  | USB_LP/CAN1_RX0   | USB FS LP or CAN1 RX0                            | 0x00000090   |
| 37  | CAN1_RX1          | CAN1 RX1                                         | 0x00000094   |
| 38  | CAN1_SCE          | CAN1 status/error                                | 0x00000098   |
| 39  | EXTI9_5           | EXTI lines 5–9                                   | 0x0000009C   |
| 40  | TIM1_BRK          | TIM1 break                                       | 0x000000A0   |
| 41  | TIM1_UP           | TIM1 update                                      | 0x000000A4   |
| 42  | TIM1_TRG_COM      | TIM1 trigger/comm                                | 0x000000A8   |
| 43  | TIM1_CC           | TIM1 capture/compare                             | 0x000000AC   |
| 44  | TIM2              | TIM2 global                                      | 0x000000B0   |
| 45  | TIM3              | TIM3 global                                      | 0x000000B4   |
| 46  | TIM4              | TIM4 global                                      | 0x000000B8   |
| 47  | I2C1_EV           | I2C1 event                                       | 0x000000BC   |
| 48  | I2C1_ER           | I2C1 error                                       | 0x000000C0   |
| 49  | I2C2_EV           | I2C2 event                                       | 0x000000C4   |
| 50  | I2C2_ER           | I2C2 error                                       | 0x000000C8   |
| 51  | SPI1              | SPI1 global                                      | 0x000000CC   |
| 52  | SPI2              | SPI2 global                                      | 0x000000D0   |
| 53  | USART1            | USART1 global                                    | 0x000000D4   |
| 54  | USART2            | USART2 global                                    | 0x000000D8   |
| 55  | USART3            | USART3 global                                    | 0x000000DC   |
| 56  | EXTI15_10         | EXTI lines 10–15                                 | 0x000000E0   |
| 57  | RTCAlarm          | RTC alarm via EXTI                               | 0x000000E4   |
| 58  | USBWakeUp         | USB wakeup via EXTI                              | 0x000000E8   |
| 59  | TIM8_BRK          | TIM8 break                                       | 0x000000EC   |
| 60  | TIM8_UP           | TIM8 update                                      | 0x000000F0   |
| 61  | TIM8_TRG_COM      | TIM8 trigger/comm                                | 0x000000F4   |
| 62  | TIM8_CC           | TIM8 capture/compare                             | 0x000000F8   |
| 63  | RNG               | Random number generator                          | 0x000000FC   |
| 64  | Reserved          | —                                                | 0x00000100   |
| 65  | SDIO              | SDIO global                                      | 0x00000104   |
| 66  | TIM5              | TIM5 global                                      | 0x00000108   |
| 67  | SPI3              | SPI3 global                                      | 0x0000010C   |
| 68  | USART4            | USART4 global                                    | 0x00000110   |
| 69  | USART5            | USART5 global                                    | 0x00000114   |
| 70  | TIM6              | TIM6 global                                      | 0x00000118   |
| 71  | TIM7              | TIM7 global                                      | 0x0000011C   |
| 72  | DMA2_CH1          | DMA2 Channel 1                                   | 0x00000120   |
| 73  | DMA2_CH2          | DMA2 Channel 2                                   | 0x00000124   |
| 74  | DMA2_CH3          | DMA2 Channel 3                                   | 0x00000128   |
| 75  | DMA2_CH4          | DMA2 Channel 4                                   | 0x0000012C   |
| 76  | DMA2_CH5          | DMA2 Channel 5                                   | 0x00000130   |
| 77  | ETH               | Ethernet global interrupt                        | 0x00000134   |
| 78  | ETH_WKUP          | Ethernet wakeup interrupt                        | 0x00000138   |
| 79  | CAN2_TX           | CAN2 TX                                          | 0x0000013C   |
| 80  | CAN2_RX0          | CAN2 RX FIFO 0                                   | 0x00000140   |
| 81  | CAN2_RX1          | CAN2 RX FIFO 1                                   | 0x00000144   |
| 82  | CAN2_SCE          | CAN2 status/error                                | 0x00000148   |
| 83  | OTG_FS            | USB FS OTG interrupt                             | 0x0000014C   |
| 84  | USBHSWakeUp       | USBHS wakeup                                     | 0x00000150   |
| 85  | USBHS             | USBHS global                                     | 0x00000154   |
| 86  | DVP               | Digital video port                               | 0x00000158   |
| 87  | USART6            | USART6 global                                    | 0x0000015C   |
| 88  | USART7            | USART7 global                                    | 0x00000160   |
| 89  | USART8            | USART8 global                                    | 0x00000164   |
| 90  | TIM9_BRK          | TIM9 break                                       | 0x00000168   |
| 91  | TIM9_UP           | TIM9 update                                      | 0x0000016C   |
| 92  | TIM9_TRG_COM      | TIM9 trigger/comm                                | 0x00000170   |
| 93  | TIM9_CC           | TIM9 capture/compare                             | 0x00000174   |
| 94  | TIM10_BRK         | TIM10 break                                      | 0x00000178   |
| 95  | TIM10_UP          | TIM10 update                                     | 0x0000017C   |
| 96  | TIM10_TRG_COM     | TIM10 trigger/comm                               | 0x00000180   |
| 97  | TIM10_CC          | TIM10 capture/compare                            | 0x00000184   |
| 98  | DMA2_CH6          | DMA2 Channel 6                                   | 0x00000188   |
| 99  | DMA2_CH7          | DMA2 Channel 7                                   | 0x0000018C   |
| 100 | DMA2_CH8          | DMA2 Channel 8                                   | 0x00000190   |
| 101 | DMA2_CH9          | DMA2 Channel 9                                   | 0x00000194   |
| 102 | DMA2_CH10         | DMA2 Channel 10                                  | 0x00000198   |
| 103 | DMA2_CH11         | DMA2 Channel 11                                  | 0x0000019C   |
