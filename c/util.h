//
// Created by dingjing on 2/19/25.
//

#ifndef html_parser_UTIL_H
#define html_parser_UTIL_H
#include "macros/macros.h"

C_BEGIN_EXTERN_C

struct GumboInternalParser;

void    gumbo_debug             (const char* format, ...);
void    gumbo_parser_deallocate (struct GumboInternalParser* parser, void* ptr);
char*   gumbo_copy_stringz      (struct GumboInternalParser* parser, const char* str);
void*   gumbo_parser_allocate   (struct GumboInternalParser* parser, size_t num_bytes);

C_END_EXTERN_C

#endif // html_parser_UTIL_H
