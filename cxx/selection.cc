//
// Created by dingjing on 2/19/25.
//

#include "selection.h"

#include "parser.h"
#include "selector.h"
#include "query-util.h"

using namespace html;


Selection::Selection(GumboNode* apNode)
{
    mNodes.push_back(apNode);
}

Selection::Selection(std::vector<GumboNode*> aNodes)
{
    mNodes = aNodes;
}

Selection::~Selection()
{
}

Selection Selection::find(std::string aSelector)
{
    Selector* sel = Parser::create(aSelector);
    std::vector<GumboNode*> ret;
    for (std::vector<GumboNode*>::iterator it = mNodes.begin(); it != mNodes.end(); it++) {
        GumboNode* pNode = *it;
        std::vector<GumboNode*> matched = sel->matchAll(pNode);
        ret = QueryUtil::unionNodes(ret, matched);
    }
    sel->release();
    return Selection(ret);
}

Node Selection::nodeAt(size_t i) const
{
    if (i >= mNodes.size()) {
        return Node();
    }

    return Node(mNodes[i]);
}

size_t Selection::nodeNum() const
{
    return mNodes.size();
}
