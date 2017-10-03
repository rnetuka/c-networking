//
// Created by rnetuka on 4.4.18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mac.h"

MacAddress *mac_address_new(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6)
{
    MacAddress *address = malloc(sizeof(MacAddress));
    address->bytes[0] = b1;
    address->bytes[1] = b2;
    address->bytes[2] = b3;
    address->bytes[3] = b4;
    address->bytes[4] = b5;
    address->bytes[5] = b6;
    sprintf(address->string, "%x:%x:%x:%x:%x:%x", b1, b2, b3, b4, b5, b6);
    return address;
}

MacAddress *mac_address_from_bytes(const uint8_t bytes[MAC_ADDR_SIZE])
{
    return mac_address_new(bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5]);
}

MacAddress *mac_address_clone(const MacAddress *address)
{
    if (! address)
        return NULL;

    return mac_address_from_bytes(address->bytes);
}

MacAddress *mac_address_parse(const char *string)
{
    if (strlen(string) != MAC_ADDR_STR_LENGTH)
        return NULL;

    MacAddress *address = malloc(sizeof(MacAddress));
    strcpy(address->string, string);

    char working_copy[MAC_ADDR_STR_LENGTH + 1];
    strcpy(working_copy, string);

    char *end;
    char *p = strtok(working_copy, MAC_ADDR_DELIM);

    for (int i = 0; i < 6; i++)
    {
        address->bytes[i] = (uint8_t) strtol(p, &end, 16);

        if (!end)
        {
            mac_address_free(address);
            return NULL;
        }
        p = strtok(NULL, MAC_ADDR_DELIM);
    }
    return address;
}

void mac_address_free(MacAddress *address)
{
    free(address);
}