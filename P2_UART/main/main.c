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
#define MODE MASTER

#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)
#define LED (GPIO_NUM_2)

#define BAUD_RATE 115200
// F comando 0x10
uint32_t get_time_in_seconds()
{
    // enviar devuelta
    char secs[20];
    sprintf(secs, "timestamp=%d", xTaskGetTickCount() / configTICK_RATE_HZ);
    uartPuts(0, secs);
    uartPuts(1, secs);
    return xTaskGetTickCount() / configTICK_RATE_HZ;
}

// F. cmd. 0x11
uint8_t send_led_state(uint8_t led_state)
{
    char led_cad[20];
    sprintf(led_cad, "led=%d", led_state);
    uartPuts(0, led_cad);
    uartPuts(1, led_cad);
     return led_state;
}
// F. comando 0x12
void send_temp(void)
{
    //uint8_t num = rand() % 100;
    char cad[20];
    sprintf(cad, "temp=45");
    uartPuts(0, cad);
    uartPuts(1, cad);
}

// F comando 0x13
void toggle_led_state(uint8_t *led_state)
{
    *led_state == 1 ? (*led_state = 0) : (*led_state = 1);
    gpio_set_level(LED, *led_state);
}

void app_main()
{
    // char secs[20];
    // char led_state[20];
    // char temperature[20];
    // //char str[24];
    // //char str2[24];
    
    //     //Crear un apuntador a la estructura Package[0]
    // UART_Package *pkgs;
    //     //Crea 8 package
    // pkgs = (UART_Package *)malloc(N_PACKAGES * sizeof(UART_Package));
    // uint8_t led_state = 0;
    // gpio_reset_pin(LED);
    // gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    // uartInit1();
    // uartInit(0, BAUD_RATE, 8, eParityDis, eStop, UART_TX_PIN0, UART_RX_PIN0); // uart_num, baudrate,  size,  parity, stop,  txPin,  rxPin)
    // uart_flush(1);
    // uart_flush(0);
    // uint8_t command[3];
    // while (1)
    // {
    //     int len = uart_read_bytes(UART_NUM_1, command, 3, pdMS_TO_TICKS(100));
    //     if (len == 2 && command[0] == '1' && command[1] == '0')
    //     {
    //         uartClrScr(0);
    //         uartPuts(0, "Comando: 0x10");
    //         uartGotoxy(0, 5, 5);
    //         get_time_in_seconds();
    //     }
    //     else if (len == 2 && command[0] == '1' && command[1] == '1')
    //     {
    //         uartClrScr(0);
    //         uartPuts(0, "Comando: 0x11");
    //         uartGotoxy(0, 5, 5);
    //         send_led_state(led_state);
    //     }
    //     else if (len == 2 && command[0] == '1' && command[1] == '2')
    //     {
    //         uartClrScr(0);
    //         uartPuts(0, "Comando: 0x12");
    //         uartGotoxy(0, 5, 5);
    //         send_temp();
    //     }
    //     else if (len == 2 && command[0] == '1' && command[1] == '3')
    //     {
    //         uartClrScr(0);
    //         toggle_led_state(&led_state);
    //         uartPuts(0, "Comando: 0x13");
    //     }
    // }
    UART_Package *pkgs;
    pkgs = (UART_Package *)malloc(N_PACKAGES * sizeof(UART_Package));
    createPackage(&pkgs[0], 0x5A, 0x10, 0, 0, 0, 0, 0, 0xB2);
    createPackage(&pkgs[1], 0x5A, 0x11, 0, 0, 0, 0, 0, 0xB2);
    createPackage(&pkgs[2], 0x5A, 0x12, 0, 0, 0, 0, 0, 0xB2);
    createPackage(&pkgs[3], 0x5A, 0x13, 0, 0, 0, 0, 0, 0xB2);

    //uartInit1();
    // uartInit(0, 115200, 8, eStop, eParityEven, UART_TX_PIN0, UART_RX_PIN0); // uart_num, baudrate,  size,  parity, stop,  txPin,  rxPin)
    // uart_flush(1);
    // uart_flush(0);
    uartInit(0,115200,8,0,1,UART_TX_PIN0,UART_RX_PIN0);
    while (1)
    {
        /*MASTER*/
        sendPackage(2,pkgs[2]);
        /*SLAVE*/
        if(receivePackage(2,&pkgs[4])) uartPuts(0,"TRUE");
        else uartPuts(0,"FALSE");
        /*Enviamos la cadena del paquete comprimido*/
        delayMs(1000);
        
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
