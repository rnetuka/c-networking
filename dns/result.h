//
// Created by root on 10.7.18.
//

#pragma once

#include "../ipaddress.h"

typedef struct
{
    IPv4Address *ipv4_address;
    IPv6Address *ipv6_address;
}
HostAddress;