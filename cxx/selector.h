//
// Created by dingjing on 2/19/25.
//

#ifndef html_parser_SELECTOR_H
#define html_parser_SELECTOR_H
#include "../c/gumbo.h"

#include "object.h"

#include <string>
#include <vector>

namespace html
{
    class Selector : public Object
    {
    public:
        typedef enum
        {
            EDummy,
            EEmpty,
            EOnlyChild,
            ENthChild,
            ETag,
        } TOperator;
    public:
        Selector(TOperator aOp = EDummy)
        {
            init();
            mOp = aOp;
        }

        Selector(bool aOfType)
        {
            init();
            mOp = EOnlyChild;
            mOfType = aOfType;
        }

        Selector(unsigned int aA, unsigned int aB, bool aLast, bool aOfType)
        {
            init();
            mOp = ENthChild;
            mA = aA;
            mB = aB;
            mLast = aLast;
            mOfType = aOfType;
        }

        Selector(GumboTag aTag)
        {
            init();
            mOp = ETag;
            mTag = aTag;
        }

        virtual ~Selector()
        {
        }

        virtual bool match(GumboNode* apNode);
        std::vector<GumboNode*> matchAll(GumboNode* apNode);
        std::vector<GumboNode*> filter(std::vector<GumboNode*> nodes);

    private:
        void init()
        {
            mOfType = false;
            mA = 0;
            mB = 0;
            mLast = false;
            mTag = GumboTag(0);
        }
        void matchAllInto(GumboNode* apNode, std::vector<GumboNode*>& nodes);

    private:
        unsigned int                mA;
        unsigned int                mB;
        TOperator                   mOp;
        GumboTag                    mTag;
        bool                        mLast;
        bool                        mOfType;
    };

    class UnarySelector: public Selector
    {
    public:
        typedef enum
        {
            ENot,
            EHasDescendant,
            EHasChild,
        } TOperator;

    public:
        UnarySelector(TOperator aOp, Selector* apS);
        virtual ~UnarySelector();
        virtual bool match(GumboNode* apNode);

    private:
        bool hasChildMatch(GumboNode* apNode, Selector* apS);
        bool hasDescendantMatch(GumboNode* apNode, Selector* apS);

    private:
        Selector*               mpS;
        TOperator               mOp;
    };

    class BinarySelector: public Selector
    {
    public:
        typedef enum
        {
            // || 操作符
            EUnion,
            // && 操作符
            EIntersection,
            //
            EChild,
            //
            EDescendant,
            //
            EAdjacent,
        } TOperator;

    public:
        BinarySelector(TOperator aOp, Selector* apS1, Selector* apS2);
        BinarySelector(Selector* apS1, Selector* apS2, bool aAdjacent);
        ~BinarySelector();
        virtual bool match(GumboNode* apNode);

    private:
        TOperator                   mOp;
        Selector*                   mpS1;
        Selector*                   mpS2;
        bool                        mAdjacent;
    };

    class AttributeSelector : public Selector
    {
    public:
        typedef enum
        {
            /**
             * 是否存在
             */
            EExists,
            /**
             * 是否相等
             */
            EEquals,
            /**
             * 是否包含
             */
            EIncludes,
            /**
             * 是否-开始
             */
            EDashMatch,
            /**
             * 是否前缀
             */
            EPrefix,
            /**
             * 是否后缀
             */
            ESuffix,
            /**
             * 是否子串
             */
            ESubString,
        } TOperator;

    public:
        AttributeSelector(TOperator aOp, std::string aKey, std::string aValue = "");
        virtual bool match(GumboNode* apNode);

    private:
        TOperator                       mOp;
        std::string                     mKey;
        std::string                     mValue;
    };

    class TextSelector: public Selector
    {
    public:
        typedef enum
        {
            //
            EOwnContains,
            //
            EContains,
        } TOperator;

    public:
        TextSelector(TOperator aOp, std::string aValue)
        {
            mValue = aValue;
            mOp = aOp;
        }

        ~TextSelector()
        {
        }

        virtual bool match(GumboNode* apNode);

    private:
        TOperator                       mOp;
        std::string                     mValue;
    };
}


#endif // html_parser_SELECTOR_H
