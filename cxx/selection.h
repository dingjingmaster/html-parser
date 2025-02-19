//
// Created by dingjing on 2/19/25.
//

#ifndef html_parser_SELECTION_H
#define html_parser_SELECTION_H
#include "object.h"

#include <vector>
#include <string>

#include "node.h"
#include "../c/gumbo.h"


namespace html
{
    class Selection : public Object
    {
    public:
        ~Selection() override;
        Selection(GumboNode* apNode);
        Selection(std::vector<GumboNode*> aNodes);

        size_t nodeNum() const;
        Node nodeAt(size_t i) const;
        Selection find(std::string aSelector);

    private:
        std::vector<GumboNode*>             mNodes;
    };
}


#endif // html_parser_SELECTION_H
