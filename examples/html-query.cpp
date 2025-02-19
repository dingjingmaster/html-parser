//
// Created by dingjing on 2/19/25.
//
#include <fstream>
#include <sstream>
#include <iostream>

#include "../cxx/node.h"
#include "../cxx/document.h"
#include "../cxx/selection.h"

void test_parser()
{
    std::string page("<h1><a>wrong link</a><a class=\"special\"\\>some link</a></h1>");

    std::ifstream file("/tmp/data-analysis-uQdrWt/index.html");
    if (!file) {
        std::cerr << "无法打开开文件!" << std::endl;
        return;
    }
    std::cerr << "1" << std::endl;
    std::stringstream buffer;
    std::cerr << "2" << std::endl;
    buffer << file.rdbuf();
    std::cerr << "3" << std::endl;

    page = buffer.str();

    std::cerr << "4" << std::endl;
    html::Document doc;
    doc.parse(page.c_str());
    std::cerr << "5" << std::endl;

    html::Selection c = doc.find("#jjjz > div > table > tbody > tr:nth-child(4)");
    html::Node node = c.nodeAt(0);
    printf("Node: %s\n", node.text().c_str());
    //      std::string content = page.substr(node.startPos(), node.endPos()-node.startPos());
    //      printf("Node: %s\n", content.c_str());
}

void test_html()
{
    std::string page = "<html><div><span>1\n</span>2\n</div></html>";
    html::Document doc;
    doc.parse(page.c_str());
    html::Node pNode = doc.find("div").nodeAt(0);
    std::string content = page.substr(pNode.startPos(), pNode.endPos() - pNode.startPos());
    printf("Node: #%s#\n", content.c_str());
}

void test_escape()
{
    std::string page = "<html><div><span id=\"that's\">1\n</span>2\n</div></html>";
    html::Document doc;
    doc.parse(page.c_str());
    html::Node pNode = doc.find("span[id=\"that's\"]").nodeAt(0);
    std::string content = page.substr(pNode.startPos(), pNode.endPos() - pNode.startPos());
    printf("Node: #%s#\n", content.c_str());
}

int main()
{
    test_parser();
    test_html();
    test_escape();
}
