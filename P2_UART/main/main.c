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
    char msgpacks[4][MSG_TAM_STR];
    char feedBackPack[MSG_TAM_STR];

    uartInit1();
    uartInit(0, 115200, 8, eStop, eParityEven, UART_TX_PIN0, UART_RX_PIN0); // uart_num, baudrate,  size,  parity, stop,  txPin,  rxPin)
    uart_flush(1);
    uart_flush(0);

    UART_Package *pkgs;
    pkgs = (UART_Package *)malloc(4 * sizeof(UART_Package));

    createPackage(&pkgs[0], 0x5A, 0x10, 0, 0, 0, 0, 0, 0xB2);
    createPackage(&pkgs[1], 0x5A, 0x11, 0, 0, 0, 0, 0, 0xB2);
    createPackage(&pkgs[2], 0x5A, 0x12, 0, 0, 0, 0, 0, 0xB2);
    createPackage(&pkgs[3], 0x5A, 0x13, 0, 0, 0, 0, 0, 0xB2);

    while (1)
    {
        PackageToString(pkgs[i], msgpacks[i]);
        uart_write_bytes(UART_NUM1, msgpacks[i], strlen(msgpacks[i]));
        uart_read_bytes(UART_NUM1, feedBackPack, MSG_TAM_STR, pdMS_TO_TICKS(100));
        uartPuts(0, feedBackPack);
        uartPuts(0, "\n");


        vTaskDelay(pdMS_TO_TICKS(INTERVAL));

        i++;
        i = i % 4;
        // uart_write_bytes(UART_NUM_1, "10", 2);
        // uart_read_bytes(1, secs, (READ_BUF_SIZE), 20 / portTICK_RATE_MS);
        // uartClrScr(0);
        // uartGotoxy(0, 5, 5);
        // uartPuts(0, secs);
        // vTaskDelay(pdMS_TO_TICKS(INTERVAL));

        // uartClrScr(0);
        // uart_write_bytes(UART_NUM_1, "11", 2);
        // uart_read_bytes(1, led_state, (READ_BUF_SIZE), 20 / portTICK_RATE_MS);
        // uartGotoxy(0, 6, 5);
        // uartPuts(0, led_state);
        // // uartPuts(0, "Sended: 0x11");

        // vTaskDelay(pdMS_TO_TICKS(INTERVAL));

        // uartClrScr(0);
        // uart_write_bytes(UART_NUM_1, "12", 2);
        // uart_read_bytes(1, temperature, (READ_BUF_SIZE), 20 / portTICK_RATE_MS);
        // uartGotoxy(0, 7, 5);
        // uartPuts(0, temperature);
        // // uartPuts(0, "Sended: 0x12");
        // vTaskDelay(pdMS_TO_TICKS(INTERVAL));

        // uart_write_bytes(UART_NUM_1, "13", 2);
        // uartClrScr(0);
        // uartGotoxy(0, 8, 5);
        // uartPuts(0, "0x13: Invertimos LED");
        // vTaskDelay(pdMS_TO_TICKS(INTERVAL));
    }
}