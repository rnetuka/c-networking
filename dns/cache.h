//
// Created by root on 10.7.18.
//

#pragma once

#include "result.h"

typedef struct
{
    char *domain_name;
    HostAddress *address;
}
DnsEntry;

typedef struct
{
    DnsEntry **entries;
}
DnsCache;

DnsCache *dns_cache_new(void);
void dns_cache_add_ipv4_address(const char *domain, IPv4Address address);
void dns_cache_add_ipv6_address(const char *domain, IPv6Address address);
const HostAddress *dns_cache_get_address(const DnsCache *cache, const char *domain);
const IPv4Address *dns_cache_get_ipv4_address(const char *domain);
const IPv6Address *dns_cache_get_ipv6_address(const char *domain);
void dns_cache_free(DnsCache *cache);