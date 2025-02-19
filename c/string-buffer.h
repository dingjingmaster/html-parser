//
// Created by dingjing on 2/19/25.
//

#ifndef html_parser_STRING_BUFFER_H
#define html_parser_STRING_BUFFER_H
#include "gumbo.h"

C_BEGIN_EXTERN_C

struct GumboInternalParser;

// A struct representing a mutable, growable string.  This consists of a
// heap-allocated buffer that may grow (by doubling) as necessary.  When
// converting to a string, this allocates a new buffer that is only as long as
// it needs to be.  Note that the internal buffer here is *not* nul-terminated,
// so be sure not to use ordinary string manipulation functions on it.
typedef struct
{
    // A pointer to the beginning of the string.  NULL iff length == 0.
    char * data;

    // The length of the string fragment, in bytes.  May be zero.
    size_t length;

    // The capacity of the buffer, in bytes.
    size_t capacity;
} GumboStringBuffer;

char*   gumbo_string_buffer_to_string       (struct GumboInternalParser * parser, GumboStringBuffer * input);
void    gumbo_string_buffer_clear           (struct GumboInternalParser * parser, GumboStringBuffer * input);
void    gumbo_string_buffer_destroy         (struct GumboInternalParser * parser, GumboStringBuffer * buffer);
void    gumbo_string_buffer_init            (struct GumboInternalParser * parser, GumboStringBuffer * output);
void    gumbo_string_buffer_append_codepoint(struct GumboInternalParser * parser, int c, GumboStringBuffer * output);
void    gumbo_string_buffer_reserve         (struct GumboInternalParser * parser,size_t min_capacity, GumboStringBuffer * output);
void    gumbo_string_buffer_append_string   (struct GumboInternalParser * parser, GumboStringPiece * str, GumboStringBuffer * output);

C_END_EXTERN_C

#endif // html_parser_STRING_BUFFER_H
