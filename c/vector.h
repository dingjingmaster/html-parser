//
// Created by dingjing on 2/19/25.
//

#ifndef html_parser_VECTOR_H
#define html_parser_VECTOR_H
#include "gumbo.h"

C_BEGIN_EXTERN_C

struct GumboInternalParser;

void    gumbo_vector_destroy    (struct GumboInternalParser* parser, GumboVector* vector);
void*   gumbo_vector_pop        (struct GumboInternalParser* parser, GumboVector* vector);
void    gumbo_vector_remove     (struct GumboInternalParser* parser, void* element, GumboVector* vector);
void    gumbo_vector_add        (struct GumboInternalParser* parser, void* element, GumboVector* vector);
void*   gumbo_vector_remove_at  (struct GumboInternalParser* parser, unsigned int index, GumboVector* vector);
void    gumbo_vector_init       (struct GumboInternalParser* parser, size_t initial_capacity, GumboVector* vector);
void    gumbo_vector_insert_at  (struct GumboInternalParser* parser, void* element, unsigned int index, GumboVector* vector);

C_END_EXTERN_C

#endif // html_parser_VECTOR_H
