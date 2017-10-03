//
// Created by root on 11.7.18.
//

#pragma once

#include "header.h"

typedef struct
{
    int code;
    HttpHeader **headers;
    char *body;

}
HttpResponse;

const char *http_response_get_header(const HttpResponse *response, const char *name);
