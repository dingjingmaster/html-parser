//
// Created by dingjing on 2/20/25.
//

#include "charset.h"

#include <math.h>
#include <enca.h>
#include <errno.h>
#include <stdio.h>
#include <iconv.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>


#define ELEMENTS(array) (sizeof(array)/sizeof((array)[0]))
#define NALIASES (ELEMENTS(ALIAS_LIST))

#define enca_tolower(c) (enca_isupper(c) ? (c) + ('a' - 'A') : (c))
#define enca_toupper(c) (enca_islower(c) ? (c) - ('a' - 'A') : (c))

#define enca_ctype_test(c, t) ((enca_ctype_data[(unsigned char)c] & t) != 0)

#define enca_isalnum(c)  enca_ctype_test((c), ENCA_CTYPE_ALNUM)
#define enca_isalpha(c)  enca_ctype_test((c), ENCA_CTYPE_ALPHA)
#define enca_iscntrl(c)  enca_ctype_test((c), ENCA_CTYPE_CNTRL)
#define enca_isdigit(c)  enca_ctype_test((c), ENCA_CTYPE_DIGIT)
#define enca_isgraph(c)  enca_ctype_test((c), ENCA_CTYPE_GRAPH)
#define enca_islower(c)  enca_ctype_test((c), ENCA_CTYPE_LOWER)
#define enca_isprint(c)  enca_ctype_test((c), ENCA_CTYPE_PRINT)
#define enca_ispunct(c)  enca_ctype_test((c), ENCA_CTYPE_PUNCT)
#define enca_isspace(c)  enca_ctype_test((c), ENCA_CTYPE_SPACE)
#define enca_isupper(c)  enca_ctype_test((c), ENCA_CTYPE_UPPER)
#define enca_isxdigit(c) enca_ctype_test((c), ENCA_CTYPE_XDIGIT)
#define enca_isname(c)   enca_ctype_test((c), ENCA_CTYPE_NAME)
#define enca_isbinary(c) enca_ctype_test((c), ENCA_CTYPE_BINARY)
#define enca_istext(c)   enca_ctype_test((c), ENCA_CTYPE_TEXT)

enum {
    ENCA_CTYPE_ALNUM  = 1 << 0,
    ENCA_CTYPE_ALPHA  = 1 << 1,
    ENCA_CTYPE_CNTRL  = 1 << 2,
    ENCA_CTYPE_DIGIT  = 1 << 3,
    ENCA_CTYPE_GRAPH  = 1 << 4,
    ENCA_CTYPE_LOWER  = 1 << 5,
    ENCA_CTYPE_PRINT  = 1 << 6,
    ENCA_CTYPE_PUNCT  = 1 << 7,
    ENCA_CTYPE_SPACE  = 1 << 8,
    ENCA_CTYPE_UPPER  = 1 << 9,
    ENCA_CTYPE_XDIGIT = 1 << 10,
    ENCA_CTYPE_NAME   = 1 << 11,
    ENCA_CTYPE_BINARY = 1 << 12,
    ENCA_CTYPE_TEXT   = 1 << 13
};

struct _EncaCharsetInfo
{
    int enca;
    int rfc1345;
    int cstocs;
    int iconv;
    int mime;
    const char *human;
    unsigned int flags;
    unsigned int nsurface;
};

typedef struct _EncaCharsetInfo EncaCharsetInfo;

static int check_encoding_name(const char *name);
static int enca_name_to_charset(const char *csName);
static int squeeze_compare(const char *x, const char *y);
static bool read_file (const char* filePath, char* buf, ssize_t size);
static int alias_search (const char *const *aList, int n, const char *s);

static const EncaCharsetInfo CHARSET_INFO[] = {
    {
        18, 16, 19, 18, 191,
        "7bit ASCII characters",
        ENCA_CHARSET_7BIT | ENCA_CHARSET_FIXED,
        ENCA_SURFACE_EOL_LF
    },
    {
     104, 105, 89, 104, 104,
     "ISO 8859-2 standard; ISO Latin 2",
     ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
     ENCA_SURFACE_EOL_LF
    },
    {
        106, 107, -1, 106, 106,
        "ISO 8859-4 standard; Latin 4",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_LF
    },
    {
        108, 109, -1, 108, 108,
        "ISO 8859-5 standard; ISO Cyrillic",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_LF
    },
    {
        97, 100, -1, 97, 97,
        "ISO 8859-13 standard; ISO Baltic; Latin 7",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_LF
    },
    {
        101, 103, -1, 101, 101,
        "ISO 8859-16 standard",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_LF
    },
    {
        31, 31, -1, 31, 203,
        "MS-Windows code page 1125",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_CRLF
    },
    {
        32, 32, 2, 32, 205,
        "MS-Windows code page 1250",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_CRLF
    },
    {
        33, 33, -1, 33, 207,
        "MS-Windows code page 1251",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_CRLF
    },
    {
        34, 34, -1, 34, 209,
        "MS-Windows code page 1257; WinBaltRim",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_CRLF
    },
    {
        85, 85, 165, 85, 85,
        "IBM/MS code page 852; PC (DOS) Latin 2",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_CRLF
    },
    {
        86, 86, -1, 86, 86,
        "IBM/MS code page 855",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_CRLF
    },
    {
        84, 84, -1, 84, 84,
        "IBM/MS code page 775",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_CRLF
    },
    {
        87, 87, -1, 87, 87,
        "IBM/MS code page 866",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_CRLF
    },
    {
        21, 116, -1, 20, -1,
        "ISO-IR-179; Baltic",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_LF
    },
    {
        126, 126, 124, -1, -1,
        "Kamenicky encoding; KEYBCS2",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_CRLF
    },
    {
        147, 153, 147, -1, -1,
        "Macintosh Central European",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_CR
    },
    {
        148, 150, -1, 149, -1,
        "Macintosh Cyrillic",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_CR
    },
    {
        71, 72, -1, 73, 73,
        "ECMA Cyrillic; ECMA-113",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_LF
    },
    {
        130, 130, 127, -1, 131,
        "KOI8-CS2 code (`T602')",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_CRLF
    },
    {
        133, 133, -1, 133, 133,
        "KOI8-R Cyrillic",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_CRLF
    },
    {
        134, 134, -1, 134, 134,
        "KOI8-U Cyrillic",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_CRLF
    },
    {
        135, 135, -1, -1, -1,
        "KOI8-Unified Cyrillic",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        ENCA_SURFACE_EOL_CRLF
    },
    {
        177, 140, 178, -1, -1,
        "(La)TeX control sequences",
        ENCA_CHARSET_7BIT | ENCA_CHARSET_VARIABLE | ENCA_CHARSET_MULTIBYTE,
        ENCA_SURFACE_EOL_LF
    },
    {
        186, 91, -1, 186, 91,
        "Universal character set 2 bytes; UCS-2; BMP",
        ENCA_CHARSET_16BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_BINARY | ENCA_CHARSET_MULTIBYTE,
        0
    },
    {
        187, 92, -1, 187, 92,
        "Universal character set 4 bytes; UCS-4; ISO-10646",
        ENCA_CHARSET_32BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_BINARY | ENCA_CHARSET_MULTIBYTE,
        0
    },
    {
        193, 189, -1, 193, 189,
        "Universal transformation format 7 bits; UTF-7",
        ENCA_CHARSET_7BIT | ENCA_CHARSET_VARIABLE | ENCA_CHARSET_MULTIBYTE,
        ENCA_SURFACE_EOL_CRLF
    },
    {
        194, 194, 195, 194, 194,
        "Universal transformation format 8 bits; UTF-8",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_VARIABLE | ENCA_CHARSET_MULTIBYTE,
        ENCA_SURFACE_EOL_LF
    },
    {
        29, 29, -1, -1, -1,
        "Cork encoding; T1",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_FIXED | ENCA_CHARSET_REGULAR,
        0
    },
    {
        80, 79, -1, 80, 77,
        "Simplified Chinese National Standard; GB2312",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_VARIABLE | ENCA_CHARSET_MULTIBYTE,
        ENCA_SURFACE_EOL_LF
    },
    {
        23, 24, -1, 23, 23,
        "Traditional Chinese Industrial Standard; Big5",
        ENCA_CHARSET_8BIT | ENCA_CHARSET_VARIABLE | ENCA_CHARSET_MULTIBYTE,
        ENCA_SURFACE_EOL_LF
    },
    {
        81, 81, -1, 76, 82,
        "HZ encoded GB2312",
        ENCA_CHARSET_7BIT | ENCA_CHARSET_VARIABLE | ENCA_CHARSET_MULTIBYTE,
        ENCA_SURFACE_EOL_LF
    },
};

static const char *ALIAS_LIST[] = {
    "10646",
    "1125",
    "1250",
    "1251",
    "1257",
    "367",
    "775",
    "7bit",
    "852",
    "855",
    "866",
    "8859-2",
    "8859-4",
    "8859-5",
    "895",
    "912",
    "ANSI_X3.4-1968",
    "ANSI_X3.4-1986",
    "ASCII",
    "ascii",
    "BALTIC",
    "baltic",
    "BIG-5",
    "BIG5",
    "Big5",
    "BIG-FIVE",
    "BIGFIVE",
    "BMP",
    "CN-BIG5",
    "CORK",
    "Cork",
    "CP1125",
    "CP1250",
    "CP1251",
    "CP1257",
    "CP367",
    "CP775",
    "CP852",
    "CP855",
    "CP866",
    "CP895",
    "CP912",
    "CP936",
    "CP950",
    "csASCII",
    "csIBM775",
    "csIBM852",
    "csIBM855",
    "csIBM866",
    "csISOLatin2",
    "csISOLatinCyrillic",
    "csKOI8R",
    "csLatin10",
    "csLatin2",
    "csLatin4",
    "csLatin5",
    "csLatin7",
    "CSPC367",
    "csPC775",
    "csPC852",
    "csPC855",
    "csPC866",
    "csPC895",
    "csUCS2",
    "csUCS4",
    "csUnicode",
    "csUnicode11",
    "csUnicode11UTF7",
    "csUTF7",
    "csUTF8",
    "Cyrillic",
    "ECMA-113",
    "ECMA-113:1986",
    "ECMA-cyrillic",
    "ecma-cyrillic",
    "FSS_UTF",
    "GB18030",
    "GB2312",
    "GB2312.1980-0",
    "GB_2312-80",
    "GBK",
    "HZ",
    "HZ-GB-2312",
    "IBM367",
    "IBM775",
    "IBM852",
    "IBM855",
    "IBM866",
    "IBM912",
    "il2",
    "ISO-10646",
    "ISO-10646-UCS-2",
    "ISO-10646-UCS-4",
    "ISO-10646-UTF7",
    "ISO-10646-UTF8",
    "ISO_646.irv:1991",
    "ISO-646-US",
    "ISO-8859-13",
    "ISO8859-13",
    "ISO_8859-13",
    "ISO_8859-13:1998",
    "ISO-8859-16",
    "ISO_8859-16",
    "ISO_8859-16:2000",
    "ISO-8859-2",
    "ISO_8859-2:1987",
    "ISO-8859-4",
    "ISO_8859-4:1988",
    "ISO-8859-5",
    "ISO_8859-5:1988",
    "iso-baltic",
    "iso-ir-101",
    "iso-ir-110",
    "iso-ir-111",
    "iso-ir-144",
    "ISO-IR-179",
    "iso-ir-179",
    "iso-ir-179a",
    "ISO-IR-226",
    "ISO-IR-58",
    "iso-ir-6",
    "ISOLatin2",
    "ISOLatin4",
    "ISOLatin5",
    "kam",
    "Kamenicky",
    "KEYBCS2",
    "koi8",
    "koi8cs",
    "KOI-8_CS2",
    "KOI-8_CS_2",
    "KOI8-CS2",
    "koi8cz",
    "KOI8-R",
    "KOI8-U",
    "KOI8-UNI",
    "l10",
    "l2",
    "l4",
    "l7",
    "LaTeX",
    "latex",
    "latin10",
    "Latin2",
    "Latin4",
    "Latin7",
    "ltex",
    "macce",
    "maccyr",
    "MACCYRILLIC",
    "mac_cyrillic",
    "maccyrillic",
    "macee",
    "macintosh_ce",
    "macintoshce",
    "macintoshcyr",
    "macintoshcyrillic",
    "macintoshee",
    "MS-1125",
    "MS-1250",
    "MS-1251",
    "MS-1257",
    "ms-balt",
    "ms-cyrl",
    "ms-ee",
    "pc2",
    "PC775",
    "PC852",
    "PC855",
    "PC866",
    "PC895",
    "pcl2",
    "pclatin2",
    "pl2",
    "rune",
    "T1",
    "T602",
    "TeX",
    "tex",
    "TF-7",
    "TF-8",
    "u2",
    "u4",
    "u7",
    "u8",
    "UCS",
    "UCS-2",
    "UCS-4",
    "UNICODE-1-1",
    "UNICODE-1-1-UTF-7",
    "us",
    "US-ASCII",
    "UTF-2",
    "UTF-7",
    "UTF-8",
    "utf8",
    "UTF-FSS",
    "Win-1125",
    "Win-1250",
    "Win-1251",
    "Win-1257",
    "WinBaltRim",
    "Windows-1125",
    "windows-1125",
    "Windows-1250",
    "windows-1250",
    "Windows-1251",
    "windows-1251",
    "Windows-1257",
    "windows-1257",
};

static const int INDEX_LIST[] = {
    25,  6,  7,  8,  9,  0, 12,  0, 10, 11, 13,  1,  2,  3, 15,  1,
     0,  0,  0,  0, 14, 14, 30, 30, 30, 30, 30, 24, 30, 28, 28,  6,
     7,  8,  9,  0, 12, 10, 11, 13, 15,  1, 29, 30,  0, 12, 10, 11,
    13,  1,  3, 20,  5,  1,  2,  3,  4,  0, 12, 10, 11, 13, 15, 24,
    25, 24, 24, 26, 26, 27,  3, 18, 18, 18, 18, 27, 31, 29, 29, 29,
    29, 31, 31,  0, 12, 10, 11, 13,  1,  1, 25, 24, 25, 26, 27,  0,
     0,  4,  4,  4,  4,  5,  5,  5,  1,  1,  2,  2,  3,  3,  4,  1,
     2, 18,  3, 14, 14,  4,  5, 29,  0,  1,  2,  3, 15, 15, 15, 19,
    19, 19, 19, 19, 19, 20, 21, 22,  5,  1,  2,  4, 23, 23,  5,  1,
     2,  4, 23, 16, 17, 17, 17, 17, 16, 16, 16, 17, 17, 16,  6,  7,
     8,  9,  9,  8,  7, 10, 12, 10, 11, 13, 15, 10, 10, 10, 24, 28,
    19, 23, 23, 26, 27, 24, 25, 26, 27, 25, 24, 25, 24, 26,  0,  0,
    27, 26, 27, 27, 27,  6,  7,  8,  9,  9,  6,  6,  7,  7,  8,  8,
     9,  9,
};


const short int enca_ctype_data[0x100] = {
    0x1004, 0x1004, 0x1004, 0x1004, 0x1004, 0x1004, 0x1004, 0x1004, /* 0x00 */
    0x1004, 0x0104, 0x0104, 0x1004, 0x0104, 0x0104, 0x1004, 0x1004, /* 0x08 */
    0x1004, 0x1004, 0x1004, 0x1004, 0x1004, 0x1004, 0x1004, 0x1004, /* 0x10 */
    0x1004, 0x1004, 0x1004, 0x1004, 0x1004, 0x1004, 0x1004, 0x1004, /* 0x18 */
    0x0140, 0x00d0, 0x00d0, 0x00d0, 0x00d0, 0x00d0, 0x00d0, 0x00d0, /* 0x20 */
    0x00d0, 0x00d0, 0x00d0, 0x00d0, 0x00d0, 0x08d0, 0x08d0, 0x08d0, /* 0x28 */
    0x0c59, 0x0c59, 0x0c59, 0x0c59, 0x0c59, 0x0c59, 0x0c59, 0x0c59, /* 0x30 */
    0x0c59, 0x0c59, 0x08d0, 0x00d0, 0x00d0, 0x00d0, 0x00d0, 0x00d0, /* 0x38 */
    0x00d0, 0x2e53, 0x2e53, 0x2e53, 0x2e53, 0x2e53, 0x2e53, 0x2a53, /* 0x40 */
    0x2a53, 0x2a53, 0x2a53, 0x2a53, 0x2a53, 0x2a53, 0x2a53, 0x2a53, /* 0x48 */
    0x2a53, 0x2a53, 0x2a53, 0x2a53, 0x2a53, 0x2a53, 0x2a53, 0x2a53, /* 0x50 */
    0x2a53, 0x2a53, 0x2a53, 0x00d0, 0x00d0, 0x00d0, 0x00d0, 0x08d0, /* 0x58 */
    0x00d0, 0x2c73, 0x2c73, 0x2c73, 0x2c73, 0x2c73, 0x2c73, 0x2873, /* 0x60 */
    0x2873, 0x2873, 0x2873, 0x2873, 0x2873, 0x2873, 0x2873, 0x2873, /* 0x68 */
    0x2873, 0x2873, 0x2873, 0x2873, 0x2873, 0x2873, 0x2873, 0x2873, /* 0x70 */
    0x2873, 0x2873, 0x2873, 0x00d0, 0x00d0, 0x00d0, 0x00d0, 0x1004, /* 0x78 */
    0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, /* 0x80 */
    0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, /* 0x88 */
    0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, /* 0x90 */
    0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, /* 0x98 */
    0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, /* 0xa0 */
    0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, /* 0xa8 */
    0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, /* 0xb0 */
    0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, /* 0xb8 */
    0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, /* 0xc0 */
    0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, /* 0xc8 */
    0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, /* 0xd0 */
    0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, /* 0xd8 */
    0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, /* 0xe0 */
    0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, /* 0xe8 */
    0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, /* 0xf0 */
    0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, /* 0xf8 */
};


char * charset_get_charset_by_file(const char * filePath)
{
    C_RETURN_VAL_IF_FAIL(filePath && (0 == access(filePath, R_OK)), NULL);
    char* buffer = NULL;
    ssize_t size = 0;
    struct stat st;

    if (0 != stat(filePath, &st)) {
        return NULL;
    }

    size = st.st_size;

    buffer = (char*) malloc (size + 1);
    C_RETURN_VAL_IF_FAIL(buffer, NULL);

    memset(buffer, 0, size + 1);
    if (!read_file(filePath, buffer, size)) {
        return NULL;
    }

    char* ret = charset_get_charset_by_buffer(buffer, size);

    free(buffer);

    return ret;
}

char* charset_get_charset_by_buffer(const char * buffer, ssize_t bufLen)
{
    C_RETURN_VAL_IF_FAIL(buffer && bufLen > 0, NULL);

    const EncaAnalyser an = enca_analyser_alloc("zh");
    C_RETURN_VAL_IF_FAIL(an, NULL);

    char* res = NULL;

    enca_set_multibyte(an, 1);
    enca_set_ambiguity(an, 1);
    enca_set_garbage_test(an, 1);
    enca_set_threshold(an, 1.38);

    static int utf8 = ENCA_CS_UNKNOWN;

    C_STMT_START {
        if (!enca_charset_is_known(utf8)) {
            utf8 = enca_name_to_charset("utf8");
        }
        size_t sgnF = 0;
        const double mu = 0.005;  /* derivation in 0 */
        const double m = 15.0;  /* value in infinity */

        sgnF = ceil((double) bufLen / ((double) bufLen / m + 1.0 / mu));

        enca_set_significant(an, sgnF);
        enca_set_termination_strictness(an, 1);

        enca_set_filtering(an, sgnF > 2);

        const EncaEncoding result = enca_analyse(an, (unsigned char*) buffer, bufLen);
        const char* s1 = enca_charset_name(result.charset, ENCA_NAME_STYLE_MIME);
        if (s1) {
            res = strdup(s1);
            break;
        }
        const char* s2 = enca_charset_name(result.charset, ENCA_NAME_STYLE_ICONV);
        if (s2) {
            res = strdup(s2);
            break;
        }

    } C_STMT_END;

    enca_analyser_free(an);

    return res;
}

bool charset_trans_charset_to_utf8(const char * buffer, ssize_t bufLen, char ** outBuffer, size_t * outLen)
{
    bool res = false;
    iconv_t cd = NULL;
    char* outT = NULL;
    size_t outLenT = 0;
    char srcCode[64] = {0};

    {
        char* srcCodeT = charset_get_charset_by_buffer(buffer, bufLen);
        if (!srcCodeT) {
            return false;
        }
        strncpy(srcCode, srcCodeT, sizeof(srcCode) - 1);
        C_FREE_FUNC(srcCodeT, free);
    }

    if (0 == strcasecmp(srcCode, "utf8") || 0 == strcasecmp(srcCode, "utf-8")) {
        *outBuffer = malloc(bufLen + 1);
        if (!(*outBuffer)) {
            return false;
        }
        memset(*outBuffer, 0, bufLen + 1);
        memccpy(*outBuffer, buffer, 0, bufLen);
        *outLen = bufLen;
        return true;
    }

    C_STMT_START {
        strcat(srcCode, "//IGNORE");
        cd = iconv_open("UTF-8", srcCode);
        if ((iconv_t) -1 == cd) {
            break;
        }

        char* in1 = (char*) buffer;
        size_t in1Len = bufLen;

        outLenT = in1Len * 8;
        outT = malloc(outLenT + 1);
        if (!outT) {
            break;
        }
        memset(outT, 0, outLenT + 1);

        char* out2 = outT; // no need free
        size_t out2Len = outLenT;
        const size_t ret = iconv(cd, &in1, &in1Len, &out2, &out2Len);
        if ((size_t) -1 == ret) {
            break;
        }

        const size_t transLen = outLenT - out2Len + 1;
        *outBuffer = (char*) malloc (transLen + 1);
        if (*outBuffer) {
            memset(*outBuffer, 0, transLen + 1);
            *outLen = transLen;
            memcpy(*outBuffer, outT, transLen);
        }
        C_FREE_FUNC(outT, free);
        res = true;
    } C_STMT_END;

    C_FREE_FUNC(cd, iconv_close);
    C_FREE_FUNC(outT, free);

    return res;
}

static bool read_file (const char* filePath, char* buf, ssize_t size)
{
    C_RETURN_VAL_IF_FAIL(filePath && buf && size > 0, false);

    const int fd = open(filePath, O_RDONLY);
    if (fd < 0) {
        return false;
    }

    const ssize_t ret = read (fd, buf, size);
    if (ret < 0) {
        close(fd);
        return false;
    }

    close(fd);

    return true;
}

static int enca_name_to_charset(const char *csName)
{
    C_RETURN_VAL_IF_FAIL(csName && check_encoding_name(csName) > 0, ENCA_CS_UNKNOWN);

    const int i = alias_search(ALIAS_LIST, NALIASES, csName);

    return i < 0 ? ENCA_CS_UNKNOWN : INDEX_LIST[i];
}

static int check_encoding_name(const char *name)
{
    C_RETURN_VAL_IF_FAIL(name, -1);

    size_t i = 0, n = 0;

    for (i = n = 0; name[i] != '\0'; i++) {
        if (!enca_isname(name[i])) {
            return -1;
        }

        if (enca_isalnum(name[i])) {
            n++;
        }
    }

    return (int) n;
}

static int alias_search (const char *const *aList, int n, const char *s)
{
    int i = 0;
    int i1 = 0;
    int i2 = n-1;

    i = squeeze_compare(s, aList[i1]);
    if (i < 0) {
        return -1;
    }
    if (i == 0) {
        return i1;
    }

    i = squeeze_compare(s, aList[i2]);
    if (i > 0) {
        return -1;
    }
    if (i == 0) {
        return i2;
    }

    while (i1+1 < i2) {
        int im = (i1 + i2)/2;
        i = squeeze_compare(s, aList[im]);
        if (i == 0) {
            return im;
        }
        if (i > 0) {
            i1 = im;
        }
        else {
            i2 = im;
        }
    }
    if (squeeze_compare(s, aList[i1+1]) == 0) {
        return i1+1;
    }

    return -1;
}

static int squeeze_compare(const char *x, const char *y)
{
    if (x == NULL || y == NULL) {
        if (x == NULL && y == NULL) {
            return 0;
        }

        if (x == NULL) {
            return -1;
        }
        else {
            return 1;
        }
    }

    while (*x != '\0' || *y != '\0') {
        while (*x != '\0' && !enca_isalnum(*x)) {
            x++;
        }
        while (*y != '\0' && !enca_isalnum(*y)) {
            y++;
        }

        if (enca_tolower(*x) != enca_tolower(*y)) {
            return (int)enca_tolower(*x) - (int)enca_tolower(*y);
        }

        if (*x != '\0') {
            x++;
        }
        if (*y != '\0') {
            y++;
        }
    }

    return 0;
}
