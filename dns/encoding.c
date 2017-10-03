//
// Created by root on 2.5.18.
//

#include <stdlib.h>
#include <string.h>

#include "encoding.h"

uint8_t *dns_domain_name_encode(const char *domain_name)
{
    char *result = malloc(1 + strlen(domain_name) + 1);
    strcpy(result + 1, domain_name);

    char *p = strtok(result + 1, ".");
    while (p)
    {
        *(p - 1) = strlen(p);
        p = strtok(NULL, ".");
    }
    return result;
}

char *dns_domain_name_decode(const uint8_t *encoded)
{
    char *result = malloc(strlen(encoded) - 1);
    strcpy(result, encoded + 1);
    for (int i = 0; i < strlen(result); i++)
    {
        if (result[i] < 10)
            result[i] = '.';
    }
    return result;
}