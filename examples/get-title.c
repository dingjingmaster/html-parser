//
// Created by dingjing on 2/19/25.
//
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "../c/gumbo.h"

static void read_file(const char* filePath, char** output, off_t* length)
{
    struct stat fileStats;

    if (0 != stat(filePath, &fileStats)) {
        printf("fstat failed\n");
        return;
    }

    const int32_t len = fileStats.st_size;
    const int fd = open(filePath, O_RDONLY);
    if (fd < 0) {
        printf("File %s not found!\n", filePath);
        exit(EXIT_FAILURE);
    }

    char* outputT = malloc(len + 1);
    memset(outputT, 0, len + 1);

    const ssize_t size = read(fd, outputT, len);
    if (size < 0) {
        printf("Read error[%d]: %s\n", size, strerror(errno));
    }
    close(fd);

    *output = outputT;
    *length = len;
}

static const char* find_title(const GumboNode * root)
{
    assert(root->type == GUMBO_NODE_ELEMENT);
    assert(root->v.element.children.length >= 2);

    const GumboVector * root_children = &root->v.element.children;
    GumboNode * head = NULL;
    for (int i = 0; i < root_children->length; ++i) {
        GumboNode * child = root_children->data[i];
        if (child->type == GUMBO_NODE_ELEMENT &&
            child->v.element.tag == GUMBO_TAG_HEAD) {
            head = child;
            break;
        }
    }
    assert(head != NULL);

    GumboVector * head_children = &head->v.element.children;
    for (int i = 0; i < head_children->length; ++i) {
        GumboNode * child = head_children->data[i];
        if (child->type == GUMBO_NODE_ELEMENT &&
            child->v.element.tag == GUMBO_TAG_TITLE) {
            if (child->v.element.children.length != 1) {
                return "<empty title>";
            }
            GumboNode * title_text = child->v.element.children.data[0];
            assert(title_text->type == GUMBO_NODE_TEXT || title_text->type == GUMBO_NODE_WHITESPACE);
            return title_text->v.text.text;
        }
    }
    return "<no title found>";
}

int main(int argc, const char ** argv)
{
    if (argc != 2) {
        printf("Usage: get_title <html filename>.\n");
        exit(EXIT_FAILURE);
    }
    const char * filename = argv[1];

    char* input = NULL;
    off_t input_length = 0;
    read_file(filename, &input, &input_length);

    GumboOutput * output = gumbo_parse_with_options(&kGumboDefaultOptions, input, input_length);
    const char * title = find_title(output->root);
    printf("%s\n", title);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    free(input);
}
