//
// Created by dingjing on 2/20/25.
//

#ifndef html_parser_CHARSET_H
#define html_parser_CHARSET_H
#include <stdbool.h>
#include "macros/macros.h"


char*   charset_get_charset_by_file     (const char* filePath);
char*   charset_get_charset_by_buffer   (const char* buffer, ssize_t bufLen);
bool    charset_trans_charset_to_utf8   (const char* buffer, ssize_t bufLen, char** outBuffer, size_t* outLen);



#endif // html_parser_CHARSET_H
