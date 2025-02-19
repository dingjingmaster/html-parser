//
// Created by dingjing on 2/19/25.
//

#ifndef html_parser_ATTRIBUTE_H
#define html_parser_ATTRIBUTE_H
#include "gumbo.h"

C_BEGIN_EXTERN_C

struct GumboInternalParser;

void gumbo_destroy_attribute(struct GumboInternalParser* parser, GumboAttribute* attribute);

C_END_EXTERN_C

#endif // html_parser_ATTRIBUTE_H
