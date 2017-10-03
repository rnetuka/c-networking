//
// Created by rnetuka on 4.4.18.
//

#pragma once

#include <stdbool.h>
#include <stdint.h>

#define IPv4_ADDR_SIZE 4
#define IPv4_ADDR_STR_MAXLEN 15

typedef struct
{
    uint8_t bytes[IPv4_ADDR_SIZE];
    char string[IPv4_ADDR_STR_MAXLEN + 1];
}
IPv4Address;

IPv4Address *ipv4_address_new(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
IPv4Address *ipv4_address_from_value(uint32_t value);
IPv4Address *ipv4_address_from_bytes(const uint8_t bytes[IPv4_ADDR_SIZE]);
IPv4Address *ipv4_address_clone(const IPv4Address *address);
IPv4Address *ipv4_address_parse(const char *string);
void ipv4_address_free(IPv4Address *address);
uint32_t ipv4_address_value(const IPv4Address *address);
bool ipv4_address_equals(const IPv4Address *a, const IPv4Address *b);

#define IPv6_ADDR_SIZE 12

typedef struct
{
    const char *string;
}
IPv6Address;

IPv6Address *ipv6_address_clone(const IPv6Address *address);
IPv6Address *ipv6_address_parse(const char *string);
bool ipv6_address_equals(const IPv6Address *first, const IPv6Address *second);
void ipv6_address_free(IPv6Address *address);