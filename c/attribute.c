//
// Created by dingjing on 2/19/25.
//

#include "attribute.h"

#include <assert.h>
#include <string.h>
#include <strings.h>

#include "util.h"

struct GumboInternalParser;

GumboAttribute* gumbo_get_attribute(const GumboVector * attributes, const char * name)
{
    for (unsigned int i = 0; i < attributes->length; ++i) {
        GumboAttribute * attr = attributes->data[i];
        if (!strcasecmp(attr->name, name)) {
            return attr;
        }
    }
    return NULL;
}

void gumbo_destroy_attribute(struct GumboInternalParser * parser, GumboAttribute * attribute)
{
    gumbo_parser_deallocate(parser, (void*)attribute->name);
    gumbo_parser_deallocate(parser, (void*)attribute->value);
    gumbo_parser_deallocate(parser, (void*)attribute);
}
