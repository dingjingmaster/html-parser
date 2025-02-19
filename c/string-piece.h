//
// Created by dingjing on 2/19/25.
//

#ifndef html_parser_STRING_PIECE_H
#define html_parser_STRING_PIECE_H
#include "gumbo.h"

C_BEGIN_EXTERN_C

struct GumboInternalParser;

void    gumbo_string_copy   (struct GumboInternalParser* parser, GumboStringPiece* dest, const GumboStringPiece* source);

C_END_EXTERN_C

#endif // html_parser_STRING_PIECE_H
