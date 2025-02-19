//
// Created by dingjing on 2/19/25.
//

#include "object.h"

html::Object::Object()
{
    mReferences = 1;
}

html::Object::~Object()
{
    if (mReferences != 1) {
        throw "something wrong, reference count not zero";
    }
}

void html::Object::retain()
{
    mReferences++;
}

void html::Object::release()
{
    if (mReferences < 0) {
        throw "something wrong, reference count is negative";
    }
    if (mReferences == 1) {
        delete this;
    }
    else {
        mReferences--;
    }
}

unsigned int html::Object::references() const
{
    return mReferences;
}
