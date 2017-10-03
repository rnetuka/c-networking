//
// Created by rnetuka on 4.4.18.
//

#pragma once

#include <stdint.h>

#include "bbuffer.h"

#define MAC_ADDR_SIZE 6
#define MAC_ADDR_DELIM ":"
#define MAC_ADDR_STR_LENGTH 17      // 6 2-digit octets + 5 delimiters

typedef struct
{
    uint8_t bytes[MAC_ADDR_SIZE];
    char string[MAC_ADDR_STR_LENGTH + 1];
}
MacAddress;

MacAddress *mac_address_new(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6);
MacAddress *mac_address_from_bytes(const uint8_t bytes[MAC_ADDR_SIZE]);
MacAddress *mac_address_clone(const MacAddress *address);
MacAddress *mac_address_parse(const char *string);
void mac_address_free(MacAddress *address);