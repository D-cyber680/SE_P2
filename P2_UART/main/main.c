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

#define MASTER 2
#define SLAVE 1
#define MODE SLAVE

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
    // uint8_t num = rand() % 100;
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
    uint8_t led_state = 0;
    UART_Package *pkgs = malloc(4 * sizeof(UART_Package));
    char msgpacks[N_PACKAGES][MSG_TAM_STR];

    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    uartInit1();
    uartInit(0, BAUD_RATE, 8, eParityDis, eStop, UART_TX_PIN0, UART_RX_PIN0); // uart_num, baudrate,  size,  parity, stop,  txPin,  rxPin)
    uart_flush(1);
    uart_flush(0);

    while (1)
    {
        // 1) Leer el paquete en forma de cadena
        int len = uart_read_bytes(UART_NUM_1, msgpacks[0], MSG_TAM_STR, pdMS_TO_TICKS(100));
        // 2) Convertir cadena a un paquete y guardar
        StringToPackage(&pkgs[0], msgpacks[0]);
        // 3) Comparar crc32s:
        //  iguales -> fue recibido correctamente
        //       ejecutar accion dado el comando
        //       actualizar paquete
        //       devolver cadena
        //  distintos -> imprimir que no se recibio correctamente
        if(checkCrc32(pkgs[0].crc32, msgpacks[0]))
        {
            showPackage(pkgs[0]); // mostrar pack
            switch (pkgs[0].command)
            {
            case 0x10:
                uartPuts(0, "comando 0x10 recibido");
                get_time_in_seconds();
                break;
            case 0x11:
                uartPuts(0, "comando 0x11 recibido");
                send_led_state(led_state);

                break;
            case 0x12:
                uartPuts(0, "comando 0x12 recibido");
                send_temp();
                break;
            case 0x13:
                uartPuts(0, "comando 0x13 recibido");
                toogle_led_state(&led_state);
                break;
            default:
                ESP_LOGE("Error command", "Comando no enontrado \n");
                break;
            }                     // 
        }else
        {
            uartPuts(0, "Error");
        }
        // showPackage(pkgs[0]);
        // uartPuts(0, msgpacks[0]);
        // uartPuts(0, "\n");
        vTaskDelay(pdMS_TO_TICKS(3000));

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
    }
}
