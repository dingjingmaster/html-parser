//
// Created by dingjing on 2/19/25.
//

#ifndef html_parser_PARSER_H
#define html_parser_PARSER_H
#include "macros/macros.h"

C_BEGIN_EXTERN_C

struct GumboInternalOutput;
struct GumboInternalOptions;
struct GumboInternalParserState;
struct GumboInternalTokenizerState;

typedef struct GumboInternalParser
{
    const struct GumboInternalOptions * _options;
    struct GumboInternalOutput * _output;
    struct GumboInternalTokenizerState * _tokenizer_state;
    struct GumboInternalParserState * _parser_state;
} GumboParser;

C_END_EXTERN_C

#endif // html_parser_PARSER_H
