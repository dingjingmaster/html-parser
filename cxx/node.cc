//
// Created by dingjing on 2/19/25.
//

#include "node.h"

#include "selection.h"
#include "query-util.h"


html::Node::Node(GumboNode * apNode)
{
    mpNode = apNode;
}

html::Node::~Node()
{
}

html::Node html::Node::parent() const
{
    return html::Node(mpNode->parent);
}

html::Node html::Node::nextSibling() const
{
    return parent().childAt(mpNode->index_within_parent + 1);
}

html::Node html::Node::prevSibling() const
{
    return parent().childAt(mpNode->index_within_parent - 1);
}

unsigned int html::Node::childNum() const
{
    if (mpNode->type != GUMBO_NODE_ELEMENT) {
        return 0;
    }

    return mpNode->v.element.children.length;
}

bool html::Node::valid() const
{
    return mpNode != nullptr;
}

html::Node html::Node::childAt(size_t i) const
{
    if (mpNode->type != GUMBO_NODE_ELEMENT || i >= mpNode->v.element.children.length) {
        return html::Node();
    }

    return html::Node((GumboNode*)mpNode->v.element.children.data[i]);
}

std::string html::Node::attribute(std::string key) const
{
    if (mpNode->type != GUMBO_NODE_ELEMENT) {
        return "";
    }

    GumboVector attributes = mpNode->v.element.attributes;
    for (unsigned int i = 0; i < attributes.length; i++) {
        GumboAttribute * attr = (GumboAttribute*)attributes.data[i];
        if (key == attr->name) {
            return attr->value;
        }
    }

    return "";
}

std::string html::Node::text() const
{
    return html::QueryUtil::nodeText(mpNode);
}

std::string html::Node::ownText() const
{
    return html::QueryUtil::nodeOwnText(mpNode);
}

size_t html::Node::startPos() const
{
    switch (mpNode->type) {
    case GUMBO_NODE_ELEMENT:
        return mpNode->v.element.start_pos.offset + mpNode->v.element.original_tag.length;
    case GUMBO_NODE_TEXT:
        return mpNode->v.text.start_pos.offset;
    default:
        return 0;
    }
}

size_t html::Node::endPos() const
{
    switch (mpNode->type) {
    case GUMBO_NODE_ELEMENT:
        return mpNode->v.element.end_pos.offset;
    case GUMBO_NODE_TEXT:
        return mpNode->v.text.original_text.length + startPos();
    default:
        return 0;
    }
}

size_t html::Node::startPosOuter() const
{
    switch (mpNode->type) {
    case GUMBO_NODE_ELEMENT:
        return mpNode->v.element.start_pos.offset;
    case GUMBO_NODE_TEXT:
        return mpNode->v.text.start_pos.offset;
    default:
        return 0;
    }
}

size_t html::Node::endPosOuter() const
{
    switch (mpNode->type) {
    case GUMBO_NODE_ELEMENT:
        return mpNode->v.element.end_pos.offset + mpNode->v.element.original_end_tag.length;
    case GUMBO_NODE_TEXT:
        return mpNode->v.text.original_text.length + startPos();
    default:
        return 0;
    }
}

std::string html::Node::tag() const
{
    if (mpNode->type != GUMBO_NODE_ELEMENT) {
        return "";
    }

    return gumbo_normalized_tagname(mpNode->v.element.tag);
}

html::Selection html::Node::find(std::string aSelector) const
{
    html::Selection c(mpNode);
    return c.find(aSelector);
}
