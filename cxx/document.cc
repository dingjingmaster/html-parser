//
// Created by dingjing on 2/19/25.
//

#include "document.h"

html::Document::Document()
{
    mpOutput = nullptr;
}

void html::Document::parse(const std::string& aInput)
{
    reset();
    mpOutput = gumbo_parse(aInput.c_str());
}

html::Document::~Document()
{
    reset();
}

html::Selection html::Document::find(std::string aSelector)
{
    if (mpOutput == nullptr) {
        throw "document not initialized";
    }

    html::Selection sel(mpOutput->root);
    return sel.find(aSelector);
}

void html::Document::reset()
{
    if (mpOutput != nullptr) {
        gumbo_destroy_output(&kGumboDefaultOptions, mpOutput);
        mpOutput = nullptr;
    }
}
