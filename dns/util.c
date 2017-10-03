//
// Created by root on 2.5.18.
//

#include "util.h"

const char *dns_question_type_str(int type)
{
    switch (type) {
        case A:     return "A";
        case NS:    return "NS";
        case CNAME: return "CNAME";
        case SOA:   return "SOA";
        case WKS:   return "WKS";
        case PTR:   return "PTR";
        case MX:    return "MX";
        case SRV:   return "SRV";
        case AAAA:  return "AAAA";
        case ANY:   return "ANY";
        default:    return "?";
    }
}

const char *dns_question_class_str(int class)
{
    return (class == IN) ? "IN" : "?";
}