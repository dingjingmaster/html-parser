//
// Created by dingjing on 2/19/25.
//

#ifndef html_parser_CHAR_REF_H
#define html_parser_CHAR_REF_H
#include "macros/macros.h"

C_BEGIN_EXTERN_C

struct GumboInternalParser;
struct GumboInternalUtf8Iterator;

typedef struct
{
    int first;
    int second;
} OneOrTwoCodepoints;

extern const int kGumboNoChar;

bool consume_char_ref(struct GumboInternalParser* parser, struct GumboInternalUtf8Iterator* input, int additional_allowed_char, bool is_in_attribute, OneOrTwoCodepoints* output);

C_END_EXTERN_C

#endif // html_parser_CHAR_REF_H
