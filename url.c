//
// Created by rnetuka on 28.3.18.
//

#include <stdlib.h>
#include <string.h>

#include "url.h"

Url *url_new(const char *string)
{
    Url *url = malloc(sizeof(Url));
    url->string = malloc(strlen(string + 1));
    strcpy(url->string, string);
    return url;
}

void url_free(Url *url)
{
    free(url->string);
    free(url);
}