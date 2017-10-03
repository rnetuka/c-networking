//
// Created by root on 2.5.18.
//

#pragma once

#include <stdint.h>

uint8_t *dns_domain_name_encode(const char *domain_name);
char *dns_domain_name_decode(const uint8_t *encoded);