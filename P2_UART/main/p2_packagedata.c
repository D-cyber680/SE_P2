#include "p2_packagedata.h"
#include "myUart.h"
#include "crc32.h"
// pkg = (UART_Package *)malloc(N_PACKAGES * sizeof(UART_Package));

void createPackage(UART_Package *mi_pack, uint8_t header, uint8_t command, uint8_t length, uint8_t dat0, uint8_t dat1, uint8_t dat2, uint8_t dat3, uint8_t fin)
{
    uint8_t input_arr[] = {header, command, length, dat0, dat1, dat2, dat3, fin};
    mi_pack->header = header;
    mi_pack->command = command;
    mi_pack->length = length;
    mi_pack->data[0] = dat0;
    mi_pack->data[1] = dat1;
    mi_pack->data[2] = dat2;
    mi_pack->data[3] = dat3;
    mi_pack->end = fin;
    mi_pack->crc32 = calc_crc32(input_arr, sizeof(input_arr));
}

void PackageToString(UART_Package pack, char *msg_pack)
{
    char buffer[MSG_TAM_STR];
    int offset = 0;

    // Convertir cada campo a una cadena y almacenarlo en el buffer temporal
    snprintf(buffer, MSG_TAM_STR, "%02X%02X%02X", pack.header, pack.command, pack.length);
    strcat(msg_pack, buffer);
    offset += strlen(buffer);
    for (int i = 0; i < PAYLOAD_LEN; i++)
    {
        snprintf(buffer, MSG_TAM_STR, "%02X", pack.data[i]);
        strcat(msg_pack + offset, buffer);
        offset += strlen(buffer);
    }
    snprintf(buffer, MSG_TAM_STR, "%02X%08X", pack.end, pack.crc32);
    strcat(msg_pack + offset, buffer);
    printf("%s", msg_pack);
}