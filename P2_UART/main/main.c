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

#define BAUD_RATE 115200
// F comando 0x10
uint32_t get_time_in_seconds(UART_Package *pack)
{
    // enviar devuelta
    char secs[20];
    char msg_pack[MSG_TAM_STR];
    uint8_t t= xTaskGetTickCount() / configTICK_RATE_HZ;
    sprintf(secs, "timestamp=%d", xTaskGetTickCount() / configTICK_RATE_HZ);

    createPackage(pack, 0x5B, 0x10, 1, 0, 0, 0, t, 0xB2);
    PackageToString(pack, msg_pack);
    uart_write_bytes(UART2_PORT, msg_pack, strlen(msg_pack));
    uartPuts(0, secs);
    return xTaskGetTickCount() / configTICK_RATE_HZ;
}

// F. cmd. 0x11
uint8_t send_led_state(UART_Package *pack, uint8_t led_state)
{
    char led_cad[20];
    char msg_pack[MSG_TAM_STR];
    sprintf(led_cad, "led=%d", led_state);

    createPackage(pack, 0x5B, 0x11, 1, 0, 0, 0, led_state, 0xB2);
    PackageToString(pack, msg_pack);
    uart_write_bytes(UART2_PORT, msg_pack, strlen(msg_pack));
    uartPuts(0, led_cad);
    return led_state;
}
// F. comando 0x12
void send_temp(UART_Package *pack)
{
    // uint8_t num = rand() % 100;
    char cad[20];
    char msg_pack[MSG_TAM_STR];
    sprintf(cad, "temp=45");

    createPackage(pack, 0x5B, 0x12, 1, 0, 0, 0, 45, 0xB2);
    PackageToString(pack, msg_pack);
    uart_write_bytes(UART2_PORT, msg_pack, strlen(msg_pack));
    uartPuts(0, cad);
}

// F comando 0x13
void toggle_led_state(UART_Package *pack, uint8_t *led_state)
{
    char msg_pack[MSG_TAM_STR];
    *led_state == 1 ? (*led_state = 0) : (*led_state = 1);
    gpio_set_level(LED, *led_state);
    createPackage(pack, 0x5B, 0x13, 0, 0, 0, 0, 0, 0xB2);
    PackageToString(pack, msg_pack);
    uart_write_bytes(UART2_PORT, msg_pack, strlen(msg_pack));
}

void app_main()
{
    int len;
    uint8_t led_state = 1;
    UART_Package *pkgs = malloc(5 * sizeof(UART_Package));
    char msgpacks[N_PACKAGES][MSG_TAM_STR];

    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    gpio_set_level(LED, led_state);

    uartInit(PC_UART_PORT,115200,8,0,1,PC_UART_TX_PIN,PC_UART_RX_PIN);
    uartInit(UART2_PORT,115200,8,0,1,UART2_TX_PIN,UART2_RX_PIN);
    uart_flush(0);
    uart_flush(2);

    while (1)
    {
        // 1) Leer el paquete en forma de cadena
        len = uart_read_bytes(UART2_PORT, msgpacks[0], MSG_TAM_STR, pdMS_TO_TICKS(100));
        // 2) Convertir cadena a un paquete y guardar
        StringToPackage(&pkgs[0], msgpacks[0]);
        // 3) Comparar crc32s:
        //  iguales -> fue recibido correctamente
        //       ejecutar accion dado el comando
        //       actualizar paquete
        //       devolver cadena
        if(checkCrc32(pkgs[0].crc32, msgpacks[0])==1){
            //showPackage(pkgs[0]);
            switch (pkgs[0].command)
            {
            case 0x10:
                uartClrScr(0);
                uartPuts(0, "Comando: 0x10");
                uartGotoxy(0, 5, 5);
                get_time_in_seconds(&pkgs[1]);
                break;
            case 0x11:
                uartClrScr(0);
                uartPuts(0, "Comando: 0x11");
                uartGotoxy(0, 5, 5);
                send_led_state(&pkgs[2],led_state);
                break;
            case 0x12:
                uartClrScr(0);
                uartPuts(0, "Comando: 0x12");
                uartGotoxy(0, 5, 5);
                send_temp(&pkgs[3]);
                break;
            case 0x13:
                uartClrScr(0);
                toggle_led_state(&pkgs[4],&led_state);
                uartGotoxy(0, 5, 5);
                uartPuts(0, "Comando: 0x13");
                break;
            default:
                break;
            }

        }
        //  distintos -> imprimir que no se recibio correctamente
        else uartPuts(0, "\nError");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}