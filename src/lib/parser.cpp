#include "parser.hpp"

std::string ExtractTextFromNode(xmlNode* node, int indentLevel) {
    std::string text;

    if (node->type == XML_TEXT_NODE) {
        text += std::string(indentLevel, '\t') + (char*)node->content + "\n";
    }

    text.erase(text.begin(), std::find_if(text.begin(), text.end(), [](int ch) {
            return !std::isspace(ch);
        }));

    size_t found = text.find("$$$");
    while (found != std::string::npos) {
        text.replace(found, 3, "$");
        found = text.find("$$$", found + 1); // Search for the next occurrence
    }

    for (xmlNode* child = node->children; child; child = child->next) {
        text += ExtractTextFromNode(child, indentLevel + 1);
    }

    return text;
}

xmlNode* FindNodeWithClass(xmlNode* node, const char* className) {
    if (node == nullptr) {
        return nullptr;
    }

    if (node->type == XML_ELEMENT_NODE && xmlStrcmp(node->name, (const xmlChar*)"div") == 0) {
        xmlChar* nodeClassName = xmlGetProp(node, (const xmlChar*)"class");
        if (nodeClassName && xmlStrcmp(nodeClassName, (const xmlChar*)className) == 0) {
            return node;
        }
    }

    for (xmlNode* child = node->children; child; child = child->next) {
        xmlNode* foundNode = FindNodeWithClass(child, className);
        if (foundNode != nullptr) {
            return foundNode;
        }
    }

    return nullptr;
}
