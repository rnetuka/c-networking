//
// Created by root on 11.7.18.
//

#pragma once

typedef struct
{
    char *name;
    char *value;
}
HttpHeader;

HttpHeader* http_header_new(const char *name, const char *value);
void http_header_free(HttpHeader *header);