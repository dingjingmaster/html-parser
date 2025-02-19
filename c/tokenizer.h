//
// Created by dingjing on 2/19/25.
//

#ifndef html_parser_TOKENIZER_H
#define html_parser_TOKENIZER_H
#include "gumbo.h"

C_BEGIN_EXTERN_C

typedef enum
{
    GUMBO_TOKEN_DOCTYPE,
    GUMBO_TOKEN_START_TAG,
    GUMBO_TOKEN_END_TAG,
    GUMBO_TOKEN_COMMENT,
    GUMBO_TOKEN_WHITESPACE,
    GUMBO_TOKEN_CHARACTER,
    GUMBO_TOKEN_CDATA,
    GUMBO_TOKEN_NULL,
    GUMBO_TOKEN_EOF
} GumboTokenType;

typedef enum
{
    GUMBO_LEX_DATA,
    GUMBO_LEX_CHAR_REF_IN_DATA,
    GUMBO_LEX_RCDATA,
    GUMBO_LEX_CHAR_REF_IN_RCDATA,
    GUMBO_LEX_RAWTEXT,
    GUMBO_LEX_SCRIPT,
    GUMBO_LEX_PLAINTEXT,
    GUMBO_LEX_TAG_OPEN,
    GUMBO_LEX_END_TAG_OPEN,
    GUMBO_LEX_TAG_NAME,
    GUMBO_LEX_RCDATA_LT,
    GUMBO_LEX_RCDATA_END_TAG_OPEN,
    GUMBO_LEX_RCDATA_END_TAG_NAME,
    GUMBO_LEX_RAWTEXT_LT,
    GUMBO_LEX_RAWTEXT_END_TAG_OPEN,
    GUMBO_LEX_RAWTEXT_END_TAG_NAME,
    GUMBO_LEX_SCRIPT_LT,
    GUMBO_LEX_SCRIPT_END_TAG_OPEN,
    GUMBO_LEX_SCRIPT_END_TAG_NAME,
    GUMBO_LEX_SCRIPT_ESCAPED_START,
    GUMBO_LEX_SCRIPT_ESCAPED_START_DASH,
    GUMBO_LEX_SCRIPT_ESCAPED,
    GUMBO_LEX_SCRIPT_ESCAPED_DASH,
    GUMBO_LEX_SCRIPT_ESCAPED_DASH_DASH,
    GUMBO_LEX_SCRIPT_ESCAPED_LT,
    GUMBO_LEX_SCRIPT_ESCAPED_END_TAG_OPEN,
    GUMBO_LEX_SCRIPT_ESCAPED_END_TAG_NAME,
    GUMBO_LEX_SCRIPT_DOUBLE_ESCAPED_START,
    GUMBO_LEX_SCRIPT_DOUBLE_ESCAPED,
    GUMBO_LEX_SCRIPT_DOUBLE_ESCAPED_DASH,
    GUMBO_LEX_SCRIPT_DOUBLE_ESCAPED_DASH_DASH,
    GUMBO_LEX_SCRIPT_DOUBLE_ESCAPED_LT,
    GUMBO_LEX_SCRIPT_DOUBLE_ESCAPED_END,
    GUMBO_LEX_BEFORE_ATTR_NAME,
    GUMBO_LEX_ATTR_NAME,
    GUMBO_LEX_AFTER_ATTR_NAME,
    GUMBO_LEX_BEFORE_ATTR_VALUE,
    GUMBO_LEX_ATTR_VALUE_DOUBLE_QUOTED,
    GUMBO_LEX_ATTR_VALUE_SINGLE_QUOTED,
    GUMBO_LEX_ATTR_VALUE_UNQUOTED,
    GUMBO_LEX_CHAR_REF_IN_ATTR_VALUE,
    GUMBO_LEX_AFTER_ATTR_VALUE_QUOTED,
    GUMBO_LEX_SELF_CLOSING_START_TAG,
    GUMBO_LEX_BOGUS_COMMENT,
    GUMBO_LEX_MARKUP_DECLARATION,
    GUMBO_LEX_COMMENT_START,
    GUMBO_LEX_COMMENT_START_DASH,
    GUMBO_LEX_COMMENT,
    GUMBO_LEX_COMMENT_END_DASH,
    GUMBO_LEX_COMMENT_END,
    GUMBO_LEX_COMMENT_END_BANG,
    GUMBO_LEX_DOCTYPE,
    GUMBO_LEX_BEFORE_DOCTYPE_NAME,
    GUMBO_LEX_DOCTYPE_NAME,
    GUMBO_LEX_AFTER_DOCTYPE_NAME,
    GUMBO_LEX_AFTER_DOCTYPE_PUBLIC_KEYWORD,
    GUMBO_LEX_BEFORE_DOCTYPE_PUBLIC_ID,
    GUMBO_LEX_DOCTYPE_PUBLIC_ID_DOUBLE_QUOTED,
    GUMBO_LEX_DOCTYPE_PUBLIC_ID_SINGLE_QUOTED,
    GUMBO_LEX_AFTER_DOCTYPE_PUBLIC_ID,
    GUMBO_LEX_BETWEEN_DOCTYPE_PUBLIC_SYSTEM_ID,
    GUMBO_LEX_AFTER_DOCTYPE_SYSTEM_KEYWORD,
    GUMBO_LEX_BEFORE_DOCTYPE_SYSTEM_ID,
    GUMBO_LEX_DOCTYPE_SYSTEM_ID_DOUBLE_QUOTED,
    GUMBO_LEX_DOCTYPE_SYSTEM_ID_SINGLE_QUOTED,
    GUMBO_LEX_AFTER_DOCTYPE_SYSTEM_ID,
    GUMBO_LEX_BOGUS_DOCTYPE,
    GUMBO_LEX_CDATA
} GumboTokenizerEnum;


struct GumboInternalParser;

typedef struct GumboInternalTokenDocType
{
    const char * name;
    const char * public_identifier;
    const char * system_identifier;
    bool force_quirks;
    bool has_public_identifier;
    bool has_system_identifier;
} GumboTokenDocType;

typedef struct GumboInternalTokenStartTag
{
    GumboTag tag;
    GumboVector /* GumboAttribute */ attributes;
    bool is_self_closing;
} GumboTokenStartTag;


typedef struct GumboInternalToken
{
    GumboTokenType type;
    GumboSourcePosition position;
    GumboStringPiece original_text;

    union
    {
        GumboTokenDocType doc_type;
        GumboTokenStartTag start_tag;
        GumboTag end_tag;
        const char * text; // For comments.
        int character; // For character, whitespace, null, and EOF tokens.
    } v;
} GumboToken;


void gumbo_tokenizer_state_destroy                  (struct GumboInternalParser * parser);
void gumbo_tokenizer_set_is_current_node_foreign    (struct GumboInternalParser * parser, bool is_foreign);
void gumbo_token_destroy                            (struct GumboInternalParser * parser, GumboToken * token);
bool gumbo_lex                                      (struct GumboInternalParser * parser, GumboToken * output);
void gumbo_tokenizer_set_state                      (struct GumboInternalParser * parser, GumboTokenizerEnum state);
void gumbo_tokenizer_state_init                     (struct GumboInternalParser * parser, const char * text, size_t text_length);


C_END_EXTERN_C

#endif // html_parser_TOKENIZER_H
