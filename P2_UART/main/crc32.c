#include "crc32.h"
#include <string.h>

uint32_t calc_crc32(uint8_t *data, uint32_t size)
{
    uint32_t crc = CRC32_INITIAL_VALUE;
    uint32_t i;

    for (i = 0; i < size; i++)
    {
        crc = crc32_table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    }

    return ~crc;
}

int checkCrc32(uint32_t dato, char *cad)
{
    char str[8];
    uint32_t dato1 = 0;
    // 5A120000000000B2 3613A097
    strncpy(str, cad + 15, 8);
    dato1 = strtoul(str, NULL, 16);
    if (dato == dato1)
        return 1;
    else
        return 0;
}

return ~crc;
}