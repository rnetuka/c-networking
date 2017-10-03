//
// Created by root on 2.5.18.
//

#pragma once

#define dns_answer_type_str(type) dns_question_type_str(type)
#define dns_answer_class_str(class) dns_question_class_str(class)

typedef enum
{
    A = 1,
    NS = 2,
    CNAME = 5,
    SOA = 6,
    WKS = 11,
    PTR = 12,
    MX = 15,
    SRV = 33,
    AAAA = 28,
    ANY = 255
}
DnsQuestionType;

typedef enum
{
    IN = 1
}
DnsQuestionClass;

typedef DnsQuestionType DnsAnswerType;
typedef DnsQuestionClass DnsAnswerClass;

const char *dns_question_type_str(int type);
const char *dns_question_class_str(int class);