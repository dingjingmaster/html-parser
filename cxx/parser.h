//
// Created by dingjing on 2/19/25.
//

#ifndef html_parser_PARSER_H
#define html_parser_PARSER_H
#include <string>

namespace html
{
    class Selector;
    class Parser
    {
    private:
        Parser(std::string aInput);
    public:
        virtual ~Parser();

        static Selector* create(std::string aInput);

    private:
        int parseInteger();
        bool skipWhitespace();
        bool nameChar(char c);
        static bool hexDigit(char c);
        bool nameStart(char c);
        std::string parseName();
        std::string parseEscape();
        std::string parseString();
        bool consumeParenthesis();
        Selector* parseSelector();
        Selector* parseIDSelector();
        Selector* parseTypeSelector();
        std::string parseIdentifier();
        Selector* parseSelectorGroup();
        Selector* parseClassSelector();
        void parseNth(int& aA, int& aB);
        bool consumeClosingParenthesis();
        Selector* parseAttributeSelector();
        Selector* parsePseudoclassSelector();
        std::string error(std::string message);
        Selector* parseSimpleSelectorSequence();

    private:
        std::string                 mInput;
        size_t                      mOffset;
    };
}


#endif // html_parser_PARSER_H
