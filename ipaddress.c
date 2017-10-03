//
// Created by rnetuka on 4.4.18.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ipaddress.h"

IPv4Address *ipv4_address_new(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4)
{
    IPv4Address *address = malloc(sizeof(IPv4Address));
    address->bytes[0] = b1;
    address->bytes[1] = b2;
    address->bytes[2] = b3;
    address->bytes[3] = b4;
    sprintf(address->string, "%i.%i.%i.%i", b1, b2, b3, b4);
    return address;
}

IPv4Address *ipv4_address_from_value(uint32_t value)
{
    return ipv4_address_new(
            (value >> 24) & 255,
            (value >> 16) & 255,
            (value >> 8) & 255,
            value & 255
    );
}

IPv4Address *ipv4_address_from_bytes(const uint8_t bytes[IPv4_ADDR_SIZE])
{
    return ipv4_address_new(bytes[0], bytes[1], bytes[2], bytes[3]);
}

IPv4Address *ipv4_address_clone(const IPv4Address *address)
{
    if (!address)
        return NULL;

    return ipv4_address_from_bytes(address->bytes);
}

IPv4Address *ipv4_address_parse(const char *string)
{
    char working_copy[strlen(string) + 1];
    strcpy(working_copy, string);

    char *err;
    char *p = strtok(working_copy, ".");
    uint8_t a = (uint8_t) strtol(p, &err, 10);

    p = strtok(NULL, ".");
    uint8_t b = (uint8_t) strtol(p, &err, 10);

    p = strtok(NULL, ".");
    uint8_t c = (uint8_t) strtol(p, &err, 10);

    p = strtok(NULL, ".");
    uint8_t d = (uint8_t) strtol(p, &err, 10);

    IPv4Address *address = malloc(sizeof(IPv4Address));
    address->bytes[0] = a;
    address->bytes[1] = b;
    address->bytes[2] = c;
    address->bytes[3] = d;
    strcpy(address->string, string);
    return address;
}

void ipv4_address_free(IPv4Address *address)
{
    free(address);
}

uint32_t ipv4_address_value(const IPv4Address *address)
{
    uint32_t value = address->bytes[0];
    value <<= 8;
    value += address->bytes[1];
    value <<= 8;
    value += address->bytes[2];
    value <<= 8;
    value += address->bytes[3];
    return value;
}

bool ipv4_address_equals(const IPv4Address *a, const IPv4Address *b)
{
    if (! a)
        return false;

    if (! b)
        return false;

    return ipv4_address_value(a) == ipv4_address_value(b);
}

IPv6Address *ipv6_address_clone(const IPv6Address *address)
{

}

IPv6Address *ipv6_address_parse(const char *string)
{

}

bool ipv6_address_equals(const IPv6Address *first, const IPv6Address *second)
{

}

void ipv6_address_free(IPv6Address *address)
{

}