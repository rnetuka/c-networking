//
// Created by rnetuka on 4.4.18.
//

#pragma once

#include "ipaddress.h"
#include "mac.h"

typedef struct
{
    const char *name;
    MacAddress *mac_address;
    IPv4Address *ipv4_address;
    IPv6Address *ipv6_address;
}
NetInterface;

typedef struct
{
    IPv4Address *ipv4_address;
    IPv6Address *ipv6_address;
}
Gateway;

NetInterface *active_net_interface(void);

MacAddress *active_net_interface_mac_address(void);
IPv4Address *active_net_interface_ipv4_address(void);
IPv6Address *active_net_interface_ipv6_address(void);

Gateway *gateway(void);

IPv4Address *dns_server_ipv4_address(void);
IPv6Address *dns_server_ipv6_address(void);

unsigned short int generate_port(void);
