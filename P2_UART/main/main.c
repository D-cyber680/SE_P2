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
#include "crc32.h"

#define MASTER 0
#define SLAVE 1
#define MODE SLAVE

/*#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)
#define BAUD_RATE 115200*/

#define INTERVAL 3000

void app_main()
{
    char secs[20];
    char led_state[20];
    char temperature[20];
    char msgpack0[MSG_TAM_STR];
    int len;

    //uartInit1();
    //uartInit(0, 115200, 8, eStop, eParityEven, UART_TX_PIN0, UART_RX_PIN0); // uart_num, baudrate,  size,  parity, stop,  txPin,  rxPin)
    //uart_flush(1);
    uartInit(PC_UART_PORT,115200,8,0,1,PC_UART_TX_PIN,PC_UART_RX_PIN);
    uartInit(UART2_PORT,115200,8,0,1,UART2_TX_PIN,UART2_RX_PIN);
    uart_flush(0);
    uart_flush(2);

    UART_Package *pkgs;
    pkgs = (UART_Package *)malloc(N_PACKAGES * sizeof(UART_Package));
    char msgpacks[N_PACKAGES][MSG_TAM_STR];
    
    createPackage(&pkgs[0], 0x5A, 0x10, 0, 0, 0, 0, 0, 0xB2);
    createPackage(&pkgs[1], 0x5A, 0x11, 0, 0, 0, 0, 0, 0xB2);
    createPackage(&pkgs[2], 0x5A, 0x12, 0, 0, 0, 0, 0, 0xB2);
    createPackage(&pkgs[3], 0x5A, 0x13, 0, 0, 0, 0, 0, 0xB2);

    while (1)
    {
        PackageToString(pkgs[0], msgpack0);
        uart_write_bytes(UART2_PORT, msgpack0, strlen(msgpack0));
        //uartPuts(0, msgpack0);
        //uartPuts(0, "\n");
        // 1) Leer el paquete en forma de cadena
        len = uart_read_bytes(UART2_PORT, msgpacks[0], MSG_TAM_STR, pdMS_TO_TICKS(100));
        // 2) Convertir cadena a un paquete y guardar
        StringToPackage(&pkgs[4], msgpacks[0]);
        if(checkCrc32(pkgs[4].crc32, msgpacks[0])==1){
            uartClrScr(0);
            uartGotoxy(0, 5, 5);
            sprintf(secs, "timestamp=%d", pkgs[4].data[3]);
            uartPuts(0, secs);
        }
        else uartPuts(0,"ERRORT");
        
        vTaskDelay(pdMS_TO_TICKS(INTERVAL));
/*********/
        PackageToString(pkgs[1], msgpack0);
        uart_write_bytes(UART2_PORT, msgpack0, strlen(msgpack0));
        // 1) Leer el paquete en forma de cadena
        len = uart_read_bytes(UART2_PORT, msgpacks[1], MSG_TAM_STR, pdMS_TO_TICKS(100));
        // 2) Convertir cadena a un paquete y guardar
        StringToPackage(&pkgs[5], msgpacks[1]);
        if(checkCrc32(pkgs[5].crc32, msgpacks[1])==1){
            uartClrScr(0);
            uartGotoxy(0, 5, 5);
            sprintf(led_state, "led=%d", pkgs[5].data[3]);
            uartPuts(0, led_state);
        }
        else uartPuts(0,"ERRORLed\n");
        
        vTaskDelay(pdMS_TO_TICKS(INTERVAL));

/*********/
        PackageToString(pkgs[2], msgpack0);
        uart_write_bytes(UART2_PORT, msgpack0, strlen(msgpack0));
        // 1) Leer el paquete en forma de cadena
        len = uart_read_bytes(UART2_PORT, msgpacks[2], MSG_TAM_STR, pdMS_TO_TICKS(100));
        // 2) Convertir cadena a un paquete y guardar
        StringToPackage(&pkgs[6], msgpacks[2]);
        if(checkCrc32(pkgs[6].crc32, msgpacks[2])==1){
            uartClrScr(0);
            uartGotoxy(0, 5, 5);
            sprintf(temperature, "temp=%d", pkgs[6].data[3]);
            uartPuts(0, temperature);
        }
        else uartPuts(0,"ERRORC");
        
        vTaskDelay(pdMS_TO_TICKS(INTERVAL));
/******/
        PackageToString(pkgs[3], msgpack0);
        uart_write_bytes(UART2_PORT, msgpack0, strlen(msgpack0));
        // 1) Leer el paquete en forma de cadena
        len = uart_read_bytes(UART2_PORT, msgpacks[3], MSG_TAM_STR, pdMS_TO_TICKS(100));
        // 2) Convertir cadena a un paquete y guardar
        StringToPackage(&pkgs[7], msgpacks[3]);
        if(checkCrc32(pkgs[7].crc32, msgpacks[3])==1){
            uartClrScr(0);
            uartGotoxy(0, 5, 5);
            uartPuts(0, "Invertimos LED");
        }
        else uartPuts(0,"ERRORLED");
        
        vTaskDelay(pdMS_TO_TICKS(INTERVAL));

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