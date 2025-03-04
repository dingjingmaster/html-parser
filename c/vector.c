//
// Created by dingjing on 2/19/25.
//

#include "vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"


struct GumboInternalParser;

const GumboVector kGumboEmptyVector = {NULL, 0, 0};

void gumbo_vector_init(struct GumboInternalParser * parser, size_t initial_capacity, GumboVector * vector)
{
    vector->length = 0;
    vector->capacity = initial_capacity;
    if (initial_capacity > 0) {
        vector->data =
            gumbo_parser_allocate(parser, sizeof(void*) * initial_capacity);
    }
    else {
        vector->data = NULL;
    }
}

void gumbo_vector_destroy(struct GumboInternalParser * parser, GumboVector * vector)
{
    if (vector->capacity > 0) {
        gumbo_parser_deallocate(parser, vector->data);
    }
}

static void enlarge_vector_if_full(struct GumboInternalParser * parser, GumboVector * vector)
{
    if (vector->length >= vector->capacity) {
        if (vector->capacity) {
            size_t old_num_bytes = sizeof(void*) * vector->capacity;
            vector->capacity *= 2;
            size_t num_bytes = sizeof(void*) * vector->capacity;
            void ** temp = gumbo_parser_allocate(parser, num_bytes);
            memcpy(temp, vector->data, old_num_bytes);
            gumbo_parser_deallocate(parser, vector->data);
            vector->data = temp;
        }
        else {
            // 0-capacity vector; no previous array to deallocate.
            vector->capacity = 2;
            vector->data =
                gumbo_parser_allocate(parser, sizeof(void*) * vector->capacity);
        }
    }
}

void gumbo_vector_add(struct GumboInternalParser * parser, void * element, GumboVector * vector)
{
    enlarge_vector_if_full(parser, vector);
    assert(vector->data);
    assert(vector->length < vector->capacity);
    vector->data[vector->length++] = element;
}

void* gumbo_vector_pop(struct GumboInternalParser * parser, GumboVector * vector)
{
    if (vector->length == 0) {
        return NULL;
    }
    return vector->data[--vector->length];
}

int gumbo_vector_index_of(GumboVector * vector, const void * element)
{
    for (unsigned int i = 0; i < vector->length; ++i) {
        if (vector->data[i] == element) {
            return i;
        }
    }
    return -1;
}

void gumbo_vector_insert_at(struct GumboInternalParser * parser, void * element, unsigned int index, GumboVector * vector)
{
    assert(index >= 0);
    assert(index <= vector->length);
    enlarge_vector_if_full(parser, vector);
    ++vector->length;
    memmove(&vector->data[index + 1], &vector->data[index],
            sizeof(void*) * (vector->length - index - 1));
    vector->data[index] = element;
}

void gumbo_vector_remove(struct GumboInternalParser * parser, void * node, GumboVector * vector)
{
    int index = gumbo_vector_index_of(vector, node);
    if (index == -1) {
        return;
    }
    gumbo_vector_remove_at(parser, index, vector);
}

void* gumbo_vector_remove_at(struct GumboInternalParser * parser, unsigned int index, GumboVector * vector)
{
    assert(index >= 0);
    assert(index < vector->length);
    void * result = vector->data[index];
    memmove(&vector->data[index], &vector->data[index + 1], sizeof(void*) * (vector->length - index - 1));
    --vector->length;
    return result;
}
