//
// Created by root on 10.7.18.
//

#include <stdlib.h>

#include "cache.h"

static ArpEntry *arp_entry_new(const IPv4Address *ip_address, const MacAddress *mac_address)
{
    ArpEntry *entry = malloc(sizeof(ArpEntry));
    entry->ip_address = ipv4_address_clone(ip_address);
    entry->mac_address = mac_address_clone(mac_address);
    return entry;
}

static void arp_entry_free(ArpEntry *entry)
{
    ipv4_address_free(entry->ip_address);
    mac_address_free(entry->mac_address);
    free(entry);
}

ArpCache *arp_cache_new(void)
{
    ArpCache *cache = malloc(sizeof(ArpCache));
    cache->entries = calloc(1, sizeof(ArpEntry *));
    cache->entries[0] = NULL;
    return cache;
}

static unsigned arp_cache_entry_count(const ArpCache *cache)
{
    unsigned count = 0;
    ArpEntry **entries = cache->entries;
    while (*entries)
    {
        count++;
        entries++;
    }
    return count;
}

static ArpEntry *arp_cache_find(const ArpCache *cache, const IPv4Address *ip_address)
{
    ArpEntry **entries = cache->entries;
    while (*entries)
    {
        ArpEntry *entry = *entries;

        if (ipv4_address_equals(ip_address, entry->ip_address))
            return entry;

        entries++;
    }
    return NULL;
}

void arp_cache_insert(ArpCache *cache, const IPv4Address *ip_address, const MacAddress *mac_address)
{
    ArpEntry *entry = arp_cache_find(cache, ip_address);

    if (entry)
        entry->mac_address = mac_address_clone(mac_address);
    else
    {
        unsigned count = arp_cache_entry_count(cache);
        cache->entries = realloc(cache->entries, (count + 2) * sizeof(ArpEntry *));
        cache->entries[count] = arp_entry_new(ip_address, mac_address);
        cache->entries[count + 1] = NULL;
    }
}

const MacAddress *arp_cache_get_mac(const ArpCache *cache, const IPv4Address *address)
{
    ArpEntry *entry = arp_cache_find(cache, address);
    return entry ? entry->mac_address : NULL;
}

void arp_cache_free(ArpCache *cache)
{
    ArpEntry **entries = cache->entries;
    while (*entries)
    {
        ArpEntry *entry = *entries;
        arp_entry_free(entry);
        entries++;
    }
    free(cache->entries);
    free(cache);
}