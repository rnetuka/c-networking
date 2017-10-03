#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "arp/cache.h"
#include "arp/resolver.h"
#include "dns/cache.h"
#include "dns/resolver.h"
#include "os.h"
#include "http/request.h"

ArpCache *arp_cache;
DnsCache *dns_cache;

int main()
{
    arp_cache = arp_cache_new();
    dns_cache = dns_cache_new();

    printf("My MAC address: %s\n", active_net_interface()->mac_address->string);
    printf("My IPv4 address: %s\n", active_net_interface()->ipv4_address->string);

    get_host_address("www.google.cz");
    get_host_address("www.google.cz");

    IPv4Address *address = ipv4_address_new(0, 0, 0, 0);
    HttpRequest *request = http_get_request_new("/");
    http_request_set_header(request, "Host", "www.google.cz");
    //http_request_send("http://www.google.cz", request);
    http_ipv4_request_send(address, request);
    http_request_free(request);

    arp_cache_free(arp_cache);
    dns_cache_free(dns_cache);
}