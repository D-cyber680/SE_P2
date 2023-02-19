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
