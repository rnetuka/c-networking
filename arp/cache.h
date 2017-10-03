//
// Created by root on 10.7.18.
//

#pragma once

#include "../ipaddress.h"
#include "../mac.h"

typedef struct
{
    IPv4Address *ip_address;
    MacAddress *mac_address;
}
ArpEntry;

typedef struct
{
    ArpEntry **entries;
}
ArpCache;

ArpCache *arp_cache_new(void);
void arp_cache_insert(ArpCache *cache, const IPv4Address *ip_address, const MacAddress *mac_address);
const MacAddress *arp_cache_get_mac(const ArpCache *cache, const IPv4Address *address);
void arp_cache_free(ArpCache *cache);