//
// Created by dingjing on 2/20/25.
//
#include "../c/charset.h"

#include <stdlib.h>

int main(void)
{
    const char *charsets[] = {
        "/tmp/data-analysis-BkCyfP/index.html",
        "/data/code/demo/c/stat.c",
        "/data/code/demo/c/stat.run",
        "/data/code/demo/c/cgroup1.c",
        "/home/dingjing/aa.html",
        "/home/dingjing/aa.docx",
        "/home/dingjing/aa.doc",
        NULL,
    };

    for (int i = 0; charsets[i]; i++) {
        char* cs = charset_get_charset_by_file(charsets[i]);
        if (cs) {
            printf("'%s' --> charset: %s\n", charsets[i], cs);
            free(cs);
        }
    }

    return 0;
}
