//
// Created by dingjing on 2/19/25.
//

#include "query-util.h"

using namespace html;


std::string QueryUtil::tolower(std::string s)
{
    for (unsigned int i = 0; i < s.size(); i++) {
        char c = s[i];
        if (c >= 'A' && c <= 'Z') {
            c = 'a' + c - 'A';
            s[i] = c;
        }
    }

    return s;
}

std::vector<GumboNode*> QueryUtil::unionNodes(std::vector<GumboNode*> aNodes1, std::vector<GumboNode*> aNodes2)
{
    for (std::vector<GumboNode*>::iterator it = aNodes2.begin(); it != aNodes2.end(); it++) {
        GumboNode* pNode = *it;
        if (nodeExists(aNodes1, pNode)) {
            continue;
        }

        aNodes1.push_back(pNode);
    }

    return aNodes1;
}

bool QueryUtil::nodeExists(std::vector<GumboNode*> aNodes, GumboNode* apNode)
{
    for (std::vector<GumboNode*>::iterator it = aNodes.begin(); it != aNodes.end(); it++)
    {
        GumboNode* pNode = *it;
        if (pNode == apNode)
        {
            return true;
        }
    }
    return false;
}

std::string QueryUtil::nodeText(GumboNode* apNode)
{
    std::string text;
    writeNodeText(apNode, text);
    return text;
}

std::string QueryUtil::nodeOwnText(GumboNode* apNode)
{
    std::string text;
    if (apNode->type != GUMBO_NODE_ELEMENT) {
        return text;
    }

    GumboVector children = apNode->v.element.children;
    for (unsigned int i = 0; i < children.length; i++) {
        GumboNode* child = (GumboNode*) children.data[i];
        if (child->type == GUMBO_NODE_TEXT) {
            text.append(child->v.text.text);
        }
    }

    return text;
}

void QueryUtil::writeNodeText(GumboNode* apNode, std::string& aText)
{
    switch (apNode->type)
    {
    case GUMBO_NODE_TEXT:
        aText.append(apNode->v.text.text);
        break;
    case GUMBO_NODE_ELEMENT:
    {
        GumboVector children = apNode->v.element.children;
        for (unsigned int i = 0; i < children.length; i++)
        {
            GumboNode* child = (GumboNode*) children.data[i];
            writeNodeText(child, aText);
        }
        break;
    }
    default:
        break;
    }
}
