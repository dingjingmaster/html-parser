//
// Created by dingjing on 2/19/25.
//

#include "string-buffer.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "util.h"
#include "string-piece.h"


struct GumboInternalParser;

static const size_t kDefaultStringBufferSize = 5;

static void maybe_resize_string_buffer(struct GumboInternalParser * parser, size_t additional_chars, GumboStringBuffer * buffer)
{
    size_t new_length = buffer->length + additional_chars;
    size_t new_capacity = buffer->capacity;
    while (new_capacity < new_length) {
        new_capacity *= 2;
    }
    if (new_capacity != buffer->capacity) {
        char * new_data = gumbo_parser_allocate(parser, new_capacity);
        memcpy(new_data, buffer->data, buffer->length);
        gumbo_parser_deallocate(parser, buffer->data);
        buffer->data = new_data;
        buffer->capacity = new_capacity;
    }
}

void gumbo_string_buffer_init(struct GumboInternalParser * parser, GumboStringBuffer * output)
{
    output->data = gumbo_parser_allocate(parser, kDefaultStringBufferSize);
    output->length = 0;
    output->capacity = kDefaultStringBufferSize;
}

void gumbo_string_buffer_reserve(struct GumboInternalParser * parser, size_t min_capacity, GumboStringBuffer * output)
{
    maybe_resize_string_buffer(parser, min_capacity - output->length, output);
}

void gumbo_string_buffer_append_codepoint(struct GumboInternalParser * parser, int c, GumboStringBuffer * output)
{
    // num_bytes is actually the number of continuation bytes, 1 less than the
    // total number of bytes.  This is done to keep the loop below simple and
    // should probably change if we unroll it.
    int num_bytes, prefix;
    if (c <= 0x7f) {
        num_bytes = 0;
        prefix = 0;
    }
    else if (c <= 0x7ff) {
        num_bytes = 1;
        prefix = 0xc0;
    }
    else if (c <= 0xffff) {
        num_bytes = 2;
        prefix = 0xe0;
    }
    else {
        num_bytes = 3;
        prefix = 0xf0;
    }
    maybe_resize_string_buffer(parser, num_bytes + 1, output);
    output->data[output->length++] = prefix | (c >> (num_bytes * 6));
    for (int i = num_bytes - 1; i >= 0; --i) {
        output->data[output->length++] = 0x80 | (0x3f & (c >> (i * 6)));
    }
}

void gumbo_string_buffer_append_string(struct GumboInternalParser * parser, GumboStringPiece * str, GumboStringBuffer * output)
{
    maybe_resize_string_buffer(parser, str->length, output);
    memcpy(output->data + output->length, str->data, str->length);
    output->length += str->length;
}

char* gumbo_string_buffer_to_string(struct GumboInternalParser * parser, GumboStringBuffer * input)
{
    char * buffer = gumbo_parser_allocate(parser, input->length + 1);
    memcpy(buffer, input->data, input->length);
    buffer[input->length] = '\0';
    return buffer;
}

void gumbo_string_buffer_clear(struct GumboInternalParser * parser, GumboStringBuffer * input)
{
    input->length = 0;
}

void gumbo_string_buffer_destroy(struct GumboInternalParser * parser, GumboStringBuffer * buffer)
{
    gumbo_parser_deallocate(parser, buffer->data);
}
