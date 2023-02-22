#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "myUart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "p2_packagedata.h"

#define MASTER 0
#define SLAVE 1
#define MODE SLAVE

#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)
#define BAUD_RATE 115200

#define INTERVAL 3000

void app_main()
{
    uint8_t i = 0;
    char secs[20];
    char led_state[20];
    char temperature[20];
    char msgpacks[MSG_TAM_STR];
    char feedBackPack[24];
    volatile uint8_t offsetX = 0;
    volatile uint8_t offsetY = 0;
    
    uartInit1();
    uartInit(0, 115200, 8, eStop, eParityEven, UART_TX_PIN0, UART_RX_PIN0); // uart_num, baudrate,  size,  parity, stop,  txPin,  rxPin)
    uart_flush(1);
    uart_flush(0);
    uartClrScr(0);

    UART_Package *pkgs;
    pkgs = (UART_Package *)malloc(4 * sizeof(UART_Package));

    createPackage(&pkgs[0], 0x5A, 0x10, 0, 0, 0, 0, 0, 0xB2);
    createPackage(&pkgs[1], 0x5A, 0x11, 0, 0, 0, 0, 0, 0xB2);
    createPackage(&pkgs[2], 0x5A, 0x12, 0, 0, 0, 0, 0, 0xB2);
    createPackage(&pkgs[3], 0x5A, 0x13, 0, 0, 0, 0, 0, 0xB2);

    while (1)
    {
        PackageToString(pkgs[i], msgpacks);
        uart_write_bytes(UART_NUM1, msgpacks, strlen(msgpacks));
        uart_read_bytes(UART_NUM1, feedBackPack, 24, pdMS_TO_TICKS(100));
        uartPuts(0, feedBackPack);
        uartGotoxy(0, offsetX, offsetY);

        vTaskDelay(pdMS_TO_TICKS(INTERVAL));

        offsetY += 10;
        i++;
        i = i % 4;

    }
}