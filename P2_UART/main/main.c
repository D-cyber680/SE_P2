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
    // uartPuts(1, secs);
    return xTaskGetTickCount() / configTICK_RATE_HZ;
}

// F. cmd. 0x11
uint8_t send_led_state(uint8_t led_state)
{
    char led_cad[8];
    sprintf(led_cad, "led=%d", led_state);
    uartPuts(0, led_cad);
    // uartPuts(1, led_cad);
    // uart_write_bytes(UART_NUM1, led_cad, strlen(led_cad));
    return led_state;
}
// F. comando 0x12
uint8_t send_temp(void)
{
    // uint8_t num = rand() % 100;
    char cad[20];
    sprintf(cad, "temp=45");
    uartPuts(0, cad);
    // uartPuts(1, cad);
    return 45;
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
    UART_Package pkg;
    char msgpacks[MSG_TAM_STR];
    char feedBackMsg[MSG_TAM_STR];

    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    uartInit1();
    uartInit(0, BAUD_RATE, 8, eParityDis, eStop, UART_TX_PIN0, UART_RX_PIN0); // uart_num, baudrate,  size,  parity, stop,  txPin,  rxPin)
    uart_flush(1);
    uart_flush(0);

    while (1)
    {
        // 1) Leer el paquete en forma de cadena
        int len = uart_read_bytes(UART_NUM_1, msgpacks, MSG_TAM_STR, pdMS_TO_TICKS(100));
        if (len)
        {
            // 2) Convertir cadena a un paquete y guardar
            StringToPackage(&pkg, msgpacks);
            // 3) Comparar crc32s:
            //  iguales -> fue recibido correctamente
            //       ejecutar accion dado el comando
            //       actualizar paquete
            //       devolver cadena
            //  distintos -> imprimir que no se recibio correctamente
            if (checkCrc32(pkg.crc32, msgpacks))
            {
                showPackage(pkg); // mostrar pack
                switch (pkg.command)
                {
                case 0x10:
                    // uartPuts(0, "comando 0x10 recibido");
                    // uartPuts(0, "\n");
                    // ESP_LOGI("timestamp", "\ntimestamp = %d\n", get_time_in_seconds());
                    pkg.length = 4;
                    pkg.data[0] = 0;
                    pkg.data[1] = 0;
                    pkg.data[2] = 0;
                    pkg.data[3] = get_time_in_seconds() % 255;
                    break;
                case 0x11:
                    // uartPuts(0, "comando 0x11 recibido");
                    send_led_state(led_state);
                    pkg.length = 2;
                    pkg.data[0] = 0;
                    pkg.data[1] = 0;
                    pkg.data[2] = 0;
                    pkg.data[3] = led_state;
                    break;
                case 0x12:
                    // uartPuts(0, "comando 0x12 recibido");
                    pkg.length = 8;
                    pkg.data[0] = 0;
                    pkg.data[1] = 0;
                    pkg.data[2] = 0;
                    pkg.data[3] = send_temp();
                    break;
                case 0x13:
                    uartPuts(0, "comando 0x13 recibido");
                    toggle_led_state(&led_state);
                    pkg.length = 0;
                    pkg.data[0] = 0;
                    pkg.data[1] = 0;
                    pkg.data[2] = 0;
                    pkg.data[3] = 0;
                    break;
                default:
                    ESP_LOGE("Error command", "Comando no enontrado \n");
                    break;
                }
            }

            PackageToString(pkg, feedBackMsg);
            showPackage(pkg);
            uart_write_bytes(UART_NUM_1, feedBackMsg, strlen(feedBackMsg));
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
        // showPackage(pkgs[0]);
        // uartPuts(0, msgpacks[0]);
        // uartPuts(0, "\n");
    }
}
