//
// Created by dingjing on 2/19/25.
//

#include "tag.h"

#include <ctype.h>
#include <assert.h>
#include <string.h>


const char * kGumboTagNames[] = {
    "html",
    "head",
    "title",
    "base",
    "link",
    "meta",
    "style",
    "script",
    "noscript",
    "template",
    "body",
    "article",
    "section",
    "nav",
    "aside",
    "h1",
    "h2",
    "h3",
    "h4",
    "h5",
    "h6",
    "hgroup",
    "header",
    "footer",
    "address",
    "p",
    "hr",
    "pre",
    "blockquote",
    "ol",
    "ul",
    "li",
    "dl",
    "dt",
    "dd",
    "figure",
    "figcaption",
    "main",
    "div",
    "a",
    "em",
    "strong",
    "small",
    "s",
    "cite",
    "q",
    "dfn",
    "abbr",
    "data",
    "time",
    "code",
    "var",
    "samp",
    "kbd",
    "sub",
    "sup",
    "i",
    "b",
    "u",
    "mark",
    "ruby",
    "rt",
    "rp",
    "bdi",
    "bdo",
    "span",
    "br",
    "wbr",
    "ins",
    "del",
    "image",
    "img",
    "iframe",
    "embed",
    "object",
    "param",
    "video",
    "audio",
    "source",
    "track",
    "canvas",
    "map",
    "area",
    "math",
    "mi",
    "mo",
    "mn",
    "ms",
    "mtext",
    "mglyph",
    "malignmark",
    "annotation-xml",
    "svg",
    "foreignobject",
    "desc",
    "table",
    "caption",
    "colgroup",
    "col",
    "tbody",
    "thead",
    "tfoot",
    "tr",
    "td",
    "th",
    "form",
    "fieldset",
    "legend",
    "label",
    "input",
    "button",
    "select",
    "datalist",
    "optgroup",
    "option",
    "textarea",
    "keygen",
    "output",
    "progress",
    "meter",
    "details",
    "summary",
    "menu",
    "menuitem",
    "applet",
    "acronym",
    "bgsound",
    "dir",
    "frame",
    "frameset",
    "noframes",
    "isindex",
    "listing",
    "xmp",
    "nextid",
    "noembed",
    "plaintext",
    "rb",
    "strike",
    "basefont",
    "big",
    "blink",
    "center",
    "font",
    "marquee",
    "multicol",
    "nobr",
    "spacer",
    "tt",
    "rtc",
    "", // TAG_UNKNOWN
    "", // TAG_LAST
};

static const unsigned char kGumboTagSizes[] = {
    4, 4, 5, 4, 4, 4, 5, 6, 8, 8, 4, 7, 7, 3, 5, 2, 2, 2, 2, 2, 2, 6, 6, 6, 7, 1, 2, 3, 10, 2, 2, 2, 2, 2, 2, 6, 10, 4, 3, 1, 2, 6, 5, 1, 4, 1, 3, 4, 4, 4, 4, 3, 4, 3, 3, 3, 1, 1, 1, 4, 4, 2, 2, 3, 3, 4, 2, 3, 3, 3, 5, 3, 6, 5, 6, 5, 5, 5,
    6, 5, 6, 3, 4, 4, 2, 2, 2, 2, 5, 6, 10, 14, 3, 13, 4, 5, 7, 8, 3, 5, 5, 5, 2, 2, 2, 4, 8, 6, 5, 5, 6, 6, 8, 8, 6, 8, 6, 6, 8, 5, 7, 7, 4, 8, 6, 7, 7, 3, 5, 8, 8, 7, 7, 3, 6, 7, 9, 2, 6, 8, 3, 5, 6, 4, 7, 8, 4, 6, 2, 3,
    0, // TAG_UNKNOWN
    0, // TAG_LAST
};

static unsigned int tag_hash(register const char * str, register unsigned int len)
{
    static unsigned short asso_values[] = {
        296, 296, 296, 296, 296, 296, 296, 296,
        296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296,
        296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296,
        296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 6, 4, 3, 1, 1, 0,
        1, 0, 0, 296, 296, 296, 296, 296, 296, 296, 22, 73, 151, 4, 13, 59, 65, 2,
        69, 0, 134, 9, 16, 52, 55, 28, 101, 0, 1, 6, 63, 126, 104, 93, 124, 296,
        296, 296, 296, 296, 296, 296, 22, 73, 151, 4, 13, 59, 65, 2, 69, 0, 134,
        9, 16, 52, 55, 28, 101, 0, 1, 6, 63, 126, 104, 93, 124, 296, 296, 296,
        296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296,
        296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296,
        296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296,
        296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296,
        296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296,
        296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296,
        296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296,
        296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296,
        296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296
    };
    register unsigned int hval = len;

    switch (hval) {
    default:
        hval += asso_values[(unsigned char)str[1] + 3];
    /*FALLTHROUGH*/
    case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
    return hval + asso_values[(unsigned char)str[len - 1]];
}

static const unsigned char kGumboTagMap[] = {
    GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_S, GUMBO_TAG_H6, GUMBO_TAG_H5, GUMBO_TAG_H4,
    GUMBO_TAG_H3, GUMBO_TAG_SPACER, GUMBO_TAG_H2, GUMBO_TAG_HEADER,
    GUMBO_TAG_H1, GUMBO_TAG_HEAD, GUMBO_TAG_LAST, GUMBO_TAG_DETAILS,
    GUMBO_TAG_SELECT, GUMBO_TAG_DIR, GUMBO_TAG_LAST, GUMBO_TAG_DEL,
    GUMBO_TAG_LAST, GUMBO_TAG_SOURCE, GUMBO_TAG_LEGEND, GUMBO_TAG_DATALIST,
    GUMBO_TAG_METER, GUMBO_TAG_MGLYPH, GUMBO_TAG_LAST, GUMBO_TAG_MATH,
    GUMBO_TAG_LABEL, GUMBO_TAG_TABLE, GUMBO_TAG_TEMPLATE, GUMBO_TAG_LAST,
    GUMBO_TAG_RP, GUMBO_TAG_TIME, GUMBO_TAG_TITLE, GUMBO_TAG_DATA,
    GUMBO_TAG_APPLET, GUMBO_TAG_HGROUP, GUMBO_TAG_SAMP, GUMBO_TAG_TEXTAREA,
    GUMBO_TAG_ABBR, GUMBO_TAG_MARQUEE, GUMBO_TAG_LAST, GUMBO_TAG_MENUITEM,
    GUMBO_TAG_SMALL, GUMBO_TAG_META, GUMBO_TAG_A, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_EMBED,
    GUMBO_TAG_MAP, GUMBO_TAG_LAST, GUMBO_TAG_PARAM, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_NOBR, GUMBO_TAG_P, GUMBO_TAG_SPAN, GUMBO_TAG_EM,
    GUMBO_TAG_LAST, GUMBO_TAG_NOFRAMES, GUMBO_TAG_SECTION, GUMBO_TAG_NOEMBED,
    GUMBO_TAG_NEXTID, GUMBO_TAG_FOOTER, GUMBO_TAG_NOSCRIPT, GUMBO_TAG_HR,
    GUMBO_TAG_LAST, GUMBO_TAG_FONT, GUMBO_TAG_DL, GUMBO_TAG_TR,
    GUMBO_TAG_SCRIPT, GUMBO_TAG_MO, GUMBO_TAG_LAST, GUMBO_TAG_DD,
    GUMBO_TAG_MAIN, GUMBO_TAG_TD, GUMBO_TAG_FOREIGNOBJECT, GUMBO_TAG_FORM,
    GUMBO_TAG_OBJECT, GUMBO_TAG_LAST, GUMBO_TAG_FIELDSET, GUMBO_TAG_LAST,
    GUMBO_TAG_BGSOUND, GUMBO_TAG_MENU, GUMBO_TAG_TFOOT, GUMBO_TAG_FIGURE,
    GUMBO_TAG_RB, GUMBO_TAG_LI, GUMBO_TAG_LISTING, GUMBO_TAG_BASEFONT,
    GUMBO_TAG_OPTGROUP, GUMBO_TAG_LAST, GUMBO_TAG_BASE, GUMBO_TAG_ADDRESS,
    GUMBO_TAG_MI, GUMBO_TAG_LAST, GUMBO_TAG_PLAINTEXT, GUMBO_TAG_LAST,
    GUMBO_TAG_PROGRESS, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_ACRONYM, GUMBO_TAG_ARTICLE, GUMBO_TAG_LAST, GUMBO_TAG_PRE,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_AREA,
    GUMBO_TAG_RT, GUMBO_TAG_LAST, GUMBO_TAG_OPTION, GUMBO_TAG_IMAGE,
    GUMBO_TAG_DT, GUMBO_TAG_LAST, GUMBO_TAG_TT, GUMBO_TAG_HTML, GUMBO_TAG_WBR,
    GUMBO_TAG_OL, GUMBO_TAG_LAST, GUMBO_TAG_STYLE, GUMBO_TAG_STRIKE,
    GUMBO_TAG_SUP, GUMBO_TAG_MULTICOL, GUMBO_TAG_U, GUMBO_TAG_DFN, GUMBO_TAG_UL,
    GUMBO_TAG_FIGCAPTION, GUMBO_TAG_MTEXT, GUMBO_TAG_LAST, GUMBO_TAG_VAR,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_FRAMESET, GUMBO_TAG_LAST,
    GUMBO_TAG_BR, GUMBO_TAG_I, GUMBO_TAG_FRAME, GUMBO_TAG_LAST, GUMBO_TAG_DIV,
    GUMBO_TAG_LAST, GUMBO_TAG_TH, GUMBO_TAG_MS, GUMBO_TAG_ANNOTATION_XML,
    GUMBO_TAG_B, GUMBO_TAG_TBODY, GUMBO_TAG_THEAD, GUMBO_TAG_BIG,
    GUMBO_TAG_BLOCKQUOTE, GUMBO_TAG_XMP, GUMBO_TAG_LAST, GUMBO_TAG_KBD,
    GUMBO_TAG_LAST, GUMBO_TAG_LINK, GUMBO_TAG_IFRAME, GUMBO_TAG_MARK,
    GUMBO_TAG_CENTER, GUMBO_TAG_OUTPUT, GUMBO_TAG_DESC, GUMBO_TAG_CANVAS,
    GUMBO_TAG_COL, GUMBO_TAG_MALIGNMARK, GUMBO_TAG_IMG, GUMBO_TAG_ASIDE,
    GUMBO_TAG_LAST, GUMBO_TAG_CODE, GUMBO_TAG_LAST, GUMBO_TAG_SUB, GUMBO_TAG_MN,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_INS, GUMBO_TAG_AUDIO,
    GUMBO_TAG_STRONG, GUMBO_TAG_CITE, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_INPUT, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_NAV, GUMBO_TAG_LAST, GUMBO_TAG_COLGROUP,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_SVG, GUMBO_TAG_KEYGEN, GUMBO_TAG_VIDEO,
    GUMBO_TAG_BDO, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_BODY, GUMBO_TAG_LAST, GUMBO_TAG_Q, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_TRACK,
    GUMBO_TAG_LAST, GUMBO_TAG_BDI, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_CAPTION, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_RUBY, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_BUTTON,
    GUMBO_TAG_SUMMARY, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_RTC, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_BLINK, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_LAST,
    GUMBO_TAG_LAST, GUMBO_TAG_LAST, GUMBO_TAG_ISINDEX
};


const char* gumbo_normalized_tagname(GumboTag tag)
{
    assert(tag <= GUMBO_TAG_LAST);
    return kGumboTagNames[tag];
}

void gumbo_tag_from_original_text(GumboStringPiece * text)
{
    if (text->data == NULL) {
        return;
    }

    assert(text->length >= 2);
    assert(text->data[0] == '<');
    assert(text->data[text->length - 1] == '>');
    if (text->data[1] == '/') {
        // End tag.
        assert(text->length >= 3);
        text->data += 2; // Move past </
        text->length -= 3;
    }
    else {
        // Start tag.
        text->data += 1; // Move past <
        text->length -= 2;
        // strnchr is apparently not a standard C library function, so I loop
        // explicitly looking for whitespace or other illegal tag characters.
        for (const char * c = text->data; c != text->data + text->length; ++c) {
            if (isspace(*c) || *c == '/') {
                text->length = c - text->data;
                break;
            }
        }
    }
}

static int case_memcmp(const char * s1, const char * s2, unsigned int n)
{
    while (n--) {
        unsigned char c1 = tolower(*s1++);
        unsigned char c2 = tolower(*s2++);
        if (c1 != c2) return (int)c1 - (int)c2;
    }
    return 0;
}

#define TAG_MAP_SIZE (sizeof(kGumboTagMap) / sizeof(kGumboTagMap[0]))

GumboTag gumbo_tagn_enum(const char * tagname, unsigned int length)
{
    if (length) {
        unsigned int key = tag_hash(tagname, length);
        if (key < TAG_MAP_SIZE) {
            GumboTag tag = kGumboTagMap[key];
            if (length == kGumboTagSizes[(int)tag] &&
                !case_memcmp(tagname, kGumboTagNames[(int)tag], length))
                return tag;
        }
    }
    return GUMBO_TAG_UNKNOWN;
}

GumboTag gumbo_tag_enum(const char * tagname)
{
    return gumbo_tagn_enum(tagname, strlen(tagname));
}
