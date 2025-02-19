//
// Created by dingjing on 2/19/25.
//

#ifndef html_parser_OBJECT_H
#define html_parser_OBJECT_H

namespace html
{
    class Object
    {
    public:
        Object();
        virtual ~Object();

        virtual void retain();
        virtual void release();
        unsigned int references() const;

    private:
        int mReferences;
    };
}

#endif // html_parser_OBJECT_H
