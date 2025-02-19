//
// Created by dingjing on 2/19/25.
//

#ifndef html_parser_NODE_H
#define html_parser_NODE_H
#include "../c/gumbo.h"

#include <string>

namespace html
{
    class Selection;
    class Node final
    {
    public:
        explicit Node(GumboNode* apNode = nullptr);
        virtual ~Node();

        bool valid() const;
        Node parent() const;
        size_t endPos() const;
        std::string tag() const;
        size_t startPos() const;
        std::string text() const;
        Node nextSibling() const;
        Node prevSibling() const;
        size_t endPosOuter() const;
        std::string ownText() const;
        Node childAt(size_t i) const;
        size_t startPosOuter() const;
        unsigned int childNum() const;
        Selection find(std::string aSelector) const;
        std::string attribute(std::string key) const;

    private:
        GumboNode*              mpNode;
    };
}


#endif // html_parser_NODE_H
