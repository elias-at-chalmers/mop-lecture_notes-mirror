# CH32V307 Interrupt Vector Table (Complete)

## 
| No. | Priority | Type        | Name              | Description                                      | Address      |
|----:|---------:|------------|-------------------|--------------------------------------------------|--------------|
| 0   | -        | -          | -                 | Reserved                                         | 0x00000000   |
| 1   | -        | -          | -                 | Reserved                                         | 0x00000004   |
| 2   | -5       | fixed      | NMI               | Non-maskable interrupt                           | 0x00000008   |
| 3   | -4       | fixed      | HardFault         | Hard fault exception                             | 0x0000000C   |
| 4   | -        | -          | -                 | Reserved                                         | 0x00000010   |
| 5   | -3       | fixed      | ECALL-M           | Machine-mode ECALL                               | 0x00000014   |
| 6–7 | -        | -          | -                 | Reserved                                         | 0x00000018–1C|
| 8   | -2       | fixed      | ECALL-U           | User-mode ECALL                                  | 0x00000020   |
| 9   | -1       | fixed      | BreakPoint        | Breakpoint exception                             | 0x00000024   |
| 10–11 | -      | -          | -                 | Reserved                       
| 12  | 0        | programmable| SysTick | System timer interrupt  | 0x00000030   |
| 13  | -        | -           | -       | Reserved                | 0x00000034   |
| 14  | 1        | programmable| SW      | Software interrupt      | 0x00000038   |
| 15  | -        | -           | -       | Reserved                | 0x0000003C   |

# Peripheral Interrupts (Programmable: 0–88)
| No. | Priority | Type         | Name              | Description                                      | Address      |
|----:|---------:|-------------|-------------------|--------------------------------------------------|--------------|
| 0   | 7        | programmable | WWDG              | Window watchdog                                  | 0x00000040   |
| 1   | 8        | programmable | PVD               | PVD via EXTI                                     | 0x00000044   |
| 2   | 9        | programmable | TAMPER            | Tamper interrupt                                 | 0x00000048   |
| 3   | 10       | programmable | RTC               | RTC global interrupt                             | 0x0000004C   |
| 4   | 11       | programmable | FLASH             | Flash global interrupt                           | 0x00000050   |
| 5   | 12       | programmable | RCC               | RCC global interrupt                             | 0x00000054   |
| 6   | 13       | programmable | EXTI0             | EXTI line0 interrupt                             | 0x00000058   |
| 7   | 14       | programmable | EXTI1             | EXTI line1 interrupt                             | 0x0000005C   |
| 8   | 15       | programmable | EXTI2             | EXTI line2 interrupt                             | 0x00000060   |
| 9   | 16       | programmable | EXTI3             | EXTI line3 interrupt                             | 0x00000064   |
| 10  | 17       | programmable | EXTI4             | EXTI line4 interrupt                             | 0x00000068   |
| 11  | 18       | programmable | DMA1_CH1          | DMA1 Channel 1                                   | 0x0000006C   |
| 12  | 19       | programmable | DMA1_CH2          | DMA1 Channel 2                                   | 0x00000070   |
| 13  | 20       | programmable | DMA1_CH3          | DMA1 Channel 3                                   | 0x00000074   |
| 14  | 21       | programmable | DMA1_CH4          | DMA1 Channel 4                                   | 0x00000078   |
| 15  | 22       | programmable | DMA1_CH5          | DMA1 Channel 5                                   | 0x0000007C   |
| 16  | 23       | programmable | DMA1_CH6          | DMA1 Channel 6                                   | 0x00000080   |
| 17  | 24       | programmable | DMA1_CH7          | DMA1 Channel 7                                   | 0x00000084   |
| 18  | 25       | programmable | ADC1_2            | ADC1/ADC2 global                                 | 0x00000088   |
| 19  | 26       | programmable | USB_HP/CAN1_TX    | USB FS HP or CAN1 TX                             | 0x0000008C   |
| 20  | 27       | programmable | USB_LP/CAN1_RX0   | USB FS LP or CAN1 RX0                            | 0x00000090   |
| 21  | 28       | programmable | CAN1_RX1          | CAN1 RX1                                         | 0x00000094   |
| 22  | 29       | programmable | CAN1_SCE          | CAN1 status/error                                | 0x00000098   |
| 23  | 30       | programmable | EXTI9_5           | EXTI lines 5–9                                   | 0x0000009C   |
| 24  | 31       | programmable | TIM1_BRK          | TIM1 break                                       | 0x000000A0   |
| 25  | 32       | programmable | TIM1_UP           | TIM1 update                                      | 0x000000A4   |
| 26  | 33       | programmable | TIM1_TRG_COM      | TIM1 trigger/comm                                | 0x000000A8   |
| 27  | 34       | programmable | TIM1_CC           | TIM1 capture/compare                             | 0x000000AC   |
| 28  | 35       | programmable | TIM2              | TIM2 global                                      | 0x000000B0   |
| 29  | 36       | programmable | TIM3              | TIM3 global                                      | 0x000000B4   |
| 30  | 37       | programmable | TIM4              | TIM4 global                                      | 0x000000B8   |
| 31  | 38       | programmable | I2C1_EV           | I2C1 event                                       | 0x000000BC   |
| 32  | 39       | programmable | I2C1_ER           | I2C1 error                                       | 0x000000C0   |
| 33  | 40       | programmable | I2C2_EV           | I2C2 event                                       | 0x000000C4   |
| 34  | 41       | programmable | I2C2_ER           | I2C2 error                                       | 0x000000C8   |
| 35  | 42       | programmable | SPI1              | SPI1 global                                      | 0x000000CC   |
| 36  | 43       | programmable | SPI2              | SPI2 global                                      | 0x000000D0   |
| 37  | 44       | programmable | USART1            | USART1 global                                    | 0x000000D4   |
| 38  | 45       | programmable | USART2            | USART2 global                                    | 0x000000D8   |
| 39  | 46       | programmable | USART3            | USART3 global                                    | 0x000000DC   |
| 40  | 47       | programmable | EXTI15_10         | EXTI lines 10–15                                 | 0x000000E0   |
| 41  | 48       | programmable | RTCAlarm          | RTC alarm via EXTI                               | 0x000000E4   |
| 42  | 49       | programmable | USBWakeUp         | USB wakeup via EXTI                              | 0x000000E8   |
| 43  | 50       | programmable | TIM8_BRK          | TIM8 break                                       | 0x000000EC   |
| 44  | 51       | programmable | TIM8_UP           | TIM8 update                                      | 0x000000F0   |
| 45  | 52       | programmable | TIM8_TRG_COM      | TIM8 trigger/comm                                | 0x000000F4   |
| 46  | 53       | programmable | TIM8_CC           | TIM8 capture/compare                             | 0x000000F8   |
| 47  | 54       | programmable | RNG               | Random number generator                          | 0x000000FC   |
| 48  | 55       | -           | Reserved           | —                                                | 0x00000100   |
| 49  | 56       | programmable | SDIO              | SDIO global                                      | 0x00000104   |
| 50  | 57       | programmable | TIM5              | TIM5 global                                      | 0x00000108   |
| 51  | 58       | programmable | SPI3              | SPI3 global                                      | 0x0000010C   |
| 52  | 59       | programmable | USART4            | USART4 global                                    | 0x00000110   |
| 53  | 60       | programmable | USART5            | USART5 global                                    | 0x00000114   |
| 54  | 61       | programmable | TIM6              | TIM6 global                                      | 0x00000118   |
| 55  | 62       | programmable | TIM7              | TIM7 global                                      | 0x0000011C   |
| 56  | 63       | programmable | DMA2_CH1          | DMA2 Channel 1                                   | 0x00000120   |
| 57  | 64       | programmable | DMA2_CH2          | DMA2 Channel 2                                   | 0x00000124   |
| 58  | 65       | programmable | DMA2_CH3          | DMA2 Channel 3                                   | 0x00000128   |
| 59  | 66       | programmable | DMA2_CH4          | DMA2 Channel 4                                   | 0x0000012C   |
| 60  | 67       | programmable | DMA2_CH5          | DMA2 Channel 5                                   | 0x00000130   |
| 61  | 68       | programmable | ETH               | Ethernet global interrupt                        | 0x00000134   |
| 62  | 69       | programmable | ETH_WKUP          | Ethernet wakeup interrupt                        | 0x00000138   |
| 63  | 70       | programmable | CAN2_TX           | CAN2 TX                                          | 0x0000013C   |
| 64  | 71       | programmable | CAN2_RX0          | CAN2 RX FIFO 0                                   | 0x00000140   |
| 65  | 72       | programmable | CAN2_RX1          | CAN2 RX FIFO 1                                   | 0x00000144   |
| 66  | 73       | programmable | CAN2_SCE          | CAN2 status/error                                | 0x00000148   |
| 67  | 74       | programmable | OTG_FS            | USB FS OTG interrupt                             | 0x0000014C   |
| 68  | 75       | programmable | USBHSWakeUp       | USBHS wakeup                                     | 0x00000150   |
| 69  | 76       | programmable | USBHS             | USBHS global                                     | 0x00000154   |
| 70  | 77       | programmable | DVP               | Digital video port                               | 0x00000158   |
| 71  | 78       | programmable | USART6            | USART6 global                                    | 0x0000015C   |
| 72  | 79       | programmable | USART7            | USART7 global                                    | 0x00000160   |
| 73  | 80       | programmable | USART8            | USART8 global                                    | 0x00000164   |
| 74  | 81       | programmable | TIM9_BRK          | TIM9 break                                       | 0x00000168   |
| 75  | 82       | programmable | TIM9_UP           | TIM9 update                                      | 0x0000016C   |
| 76  | 83       | programmable | TIM9_TRG_COM      | TIM9 trigger/comm                                | 0x00000170   |
| 77  | 84       | programmable | TIM9_CC           | TIM9 capture/compare                             | 0x00000174   |
| 78  | 85       | programmable | TIM10_BRK         | TIM10 break                                      | 0x00000178   |
| 79  | 86       | programmable | TIM10_UP          | TIM10 update                                     | 0x0000017C   |
| 80  | 87       | programmable | TIM10_TRG_COM     | TIM10 trigger/comm                               | 0x00000180   |
| 81  | 88       | programmable | TIM10_CC          | TIM10 capture/compare                            | 0x00000184   |
| 82  | 89       | programmable | DMA2_CH6          | DMA2 Channel 6                                   | 0x00000188   |
| 83  | 90       | programmable | DMA2_CH7          | DMA2 Channel 7                                   | 0x0000018C   |
| 84  | 91       | programmable | DMA2_CH8          | DMA2 Channel 8                                   | 0x00000190   |
| 85  | 92       | programmable | DMA2_CH9          | DMA2 Channel 9                                   | 0x00000194   |
| 86  | 93       | programmable | DMA2_CH10         | DMA2 Channel 10                                  | 0x00000198   |
| 87  | 94       | programmable | DMA2_CH11         | DMA2 Channel 11                                  | 0x0000019C   |
| **88 total** | | | | | |
