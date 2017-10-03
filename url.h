//
// Created by rnetuka on 28.3.18.
//

#pragma once

typedef struct
{
    char *string;
}
Url;

Url *url_new(const char *string);
void url_free(Url *url);