//
// Created by dingjing on 2/19/25.
//

#ifndef html_parser_QUERY_UTIL_H
#define html_parser_QUERY_UTIL_H
#include "../c/gumbo.h"

#include <string>
#include <vector>


namespace html
{
    class QueryUtil
    {
    public:
        static std::string tolower(std::string s);
        static std::vector<GumboNode*> unionNodes(std::vector<GumboNode*> aNodes1, std::vector<GumboNode*> aNode2);
        static bool nodeExists(std::vector<GumboNode*> aNodes, GumboNode* apNode);
        static std::string nodeText(GumboNode* apNode);
        static std::string nodeOwnText(GumboNode* apNode);

    private:
        static void writeNodeText(GumboNode* apNode, std::string& aText);
    };
}



#endif // html_parser_QUERY_UTIL_H
