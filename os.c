//
// Created by rnetuka on 4.4.18.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "os.h"

const char * const base_path = "/sys/class/net/";

NetInterface *active_interface = NULL;
Gateway *gateway_ = NULL;

static MacAddress *mac_address(const char *interface_name)
{
    FILE *pipe = popen("cat /sys/class/net/wlp4s0/address", "r");
    char mac_address[18];
    fgets(mac_address, 18, pipe);
    pclose(pipe);
    return mac_address_parse(mac_address);
}

static IPv4Address *ipv4_address(const char *interface_name)
{
    FILE *pipe = popen("ip -f inet addr show wlp4s0 | grep -Eo 'inet [0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}' -", "r");
    char buffer[100];
    fgets(buffer, 100, pipe);
    pclose(pipe);
    return ipv4_address_parse(buffer + strlen("inet "));
}

static IPv6Address *ipv6_address(const char *interface_name)
{
    FILE *pipe = popen("ip -f inet6 addr show wlp4s0 | grep -Eo 'inet6 [0-9a-f]{0,4}:[0-9a-f]{0,4}:[0-9a-f]{0,4}:[0-9a-f]{0,4}:[0-9a-f]{0,4}:[0-9a-f]{0,4}' -", "r");
    char buffer[100];
    fgets(buffer, 100, pipe);
    pclose(pipe);
    return ipv6_address_parse(buffer + strlen("inet6 "));
}

NetInterface *active_net_interface(void)
{
    if (active_interface)
        return active_interface;
    else
    {
        const char *name = "wlp4s0";

        active_interface = malloc(sizeof(NetInterface));
        active_interface->name = name;
        active_interface->mac_address = mac_address(name);
        active_interface->ipv4_address = ipv4_address(name);
        active_interface->ipv6_address = ipv6_address(name);
        return active_interface;
    }
}

MacAddress *active_net_interface_mac_address()
{
    NetInterface *interface = active_net_interface();
    return mac_address_clone(interface->mac_address);
}

IPv4Address *active_net_interface_ipv4_address()
{
    NetInterface *interface = active_net_interface();
    return ipv4_address_clone(interface->ipv4_address);
}

IPv6Address *active_net_interface_ipv6_address()
{
    NetInterface *interface = active_net_interface();
    return ipv6_address_clone(interface->ipv6_address);
}

Gateway *gateway(void)
{
    if (! gateway_)
    {
        gateway_ = malloc(sizeof(Gateway));
        gateway_->ipv4_address = ipv4_address_parse("192.168.0.1");
        gateway_->ipv6_address = NULL;
    }
    return gateway_;
}

IPv4Address *dns_server_ipv4_address(void)
{
    return ipv4_address_parse("213.46.172.36");
}

unsigned short int generate_port(void)
{

}