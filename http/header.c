//
// Created by root on 11.7.18.
//

#include <stdlib.h>
#include <string.h>

#include "header.h"

HttpHeader *http_header_new(const char *name, const char *value)
{
    HttpHeader *header = malloc(sizeof(HttpHeader));
    header->name = malloc(strlen(name) + 1);
    header->value = malloc(strlen(value) + 1);

    strcpy(header->name, name);
    strcpy(header->value, value);
    return header;
}

void http_header_free(HttpHeader *header)
{
    free(header->name);
    free(header->value);
    free(header);
}