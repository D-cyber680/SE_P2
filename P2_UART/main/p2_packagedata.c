#include "p2_packagedata.h"
#include "myUart.h"
#include "crc32.h"
// pkg = (UART_Package *)malloc(N_PACKAGES * sizeof(UART_Package));

// pkg[0].header = 0x5A;
// pkg[0].command = 0x10;
// pkg[0].length = 0;
// pkg[0].data = (uint8_t *)malloc(PAYLOAD_LEN * sizeof(uint8_t));
// pkg[0].end = 0xB2;
// pkg[0].crc32 = 0;

// data = (uint8_t *)malloc(BYTES_PER_PACKAGE * sizeof(uint8_t));

// pkg[1].header = 0x5A;
// pkg[1].command = 0x11;
// pkg[1].length = 0;
// pkg[1].data = (uint8_t *)malloc(PAYLOAD_LEN * sizeof(uint8_t));

// pkg[1].end = 0xB2;
// pkg[1].crc32 = 0;

// pkg[2].header = 0x5A;
// pkg[2].command = 0x12;
// pkg[2].length = 0;
// pkg[2].data = (uint8_t *)malloc(PAYLOAD_LEN * sizeof(uint8_t));

// pkg[2].end = 0xB2;
// pkg[2].crc32 = 0;

// pkg[3].header = 0x5A;
// pkg[3].command = 0x13;
// pkg[3].length = 0;
// pkg[3].data = (uint8_t *)malloc(PAYLOAD_LEN * sizeof(uint8_t));

// pkg[3].end = 0xB2;
// pkg[3].crc32 = 0;

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
