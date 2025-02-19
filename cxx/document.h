//
// Created by dingjing on 2/19/25.
//

#ifndef html_parser_DOCUMENT_H
#define html_parser_DOCUMENT_H
#include "../c/gumbo.h"

#include <string>

#include "selection.h"

namespace html
{
    class Document final : public Object
    {
    public:
        Document();
        ~Document() override;
        void parse(const std::string& aInput);
        virtual Selection find(std::string aSelector);

    private:
        void reset();

    private:
        GumboOutput*                mpOutput;
    };
}

#endif // html-parser_DOCUMENT_H
