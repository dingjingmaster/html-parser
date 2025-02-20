//
// Created by dingjing on 2/19/25.
//

#include "util.h"

#include <stdio.h>
#include <string.h>

#include "gumbo.h"
#include "parser.h"

// TODO(jdtang): This should be elsewhere, but there's no .c file for
// SourcePositions and yet the constant needs some linkage, so this is as good
// as any.
const GumboSourcePosition kGumboEmptySourcePosition = {0, 0, 0};

void* gumbo_parser_allocate(GumboParser * parser, size_t num_bytes)
{
    return parser->_options->allocator(parser->_options->userdata, num_bytes);
}

void gumbo_parser_deallocate(GumboParser * parser, void * ptr)
{
    parser->_options->deallocator(parser->_options->userdata, ptr);
}

char* gumbo_copy_stringz(GumboParser * parser, const char * str)
{
    char * buffer = gumbo_parser_allocate(parser, strlen(str) + 1);
    strcpy(buffer, str);
    return buffer;
}

// Debug function to trace operation of the parser.  Pass --copts=-DGUMBO_DEBUG
// to use.
void gumbo_debug(const char * format, ...)
{
#if 0
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);
#endif
}
