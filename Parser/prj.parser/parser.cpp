
#include <iostream>
#include <fstream>
#include <string>
#include <cpr/cpr.h>
#include <libxml/HTMLparser.h>

// Функция для извлечения текста из HTML-узла по уровням
std::string ExtractTextFromNode(xmlNode* node, int indentLevel = 0) {
    std::string text;

    if (node->type == XML_TEXT_NODE) {
        text += std::string(indentLevel, '\t') + (char*)node->content + "\n";
    }

    for (xmlNode* child = node->children; child; child = child->next) {
        text += ExtractTextFromNode(child, indentLevel + 1);
    }

    return text;
}
// Рекурсивная функция для поиска узла с нужным классом
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


int main() {
    std::string contestId = "16";
    std::string problemId = "A";

    // Формирование URL-адреса задачи
    std::string url = "https://codeforces.com/problemset/problem/" + contestId + "/" + problemId;

    // Отправка GET-запроса и получение HTML-страницы
    cpr::Response response = cpr::Get(cpr::Url{ url });

    // Проверка статуса запроса
    if (response.status_code != 200) {
        std::cerr << "Failed to retrieve webpage" << std::endl;
        return 1;
    }

    // Инициализация парсера HTML
    htmlDocPtr doc = htmlReadMemory(response.text.c_str(), response.text.size(), nullptr, nullptr, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
    if (doc == nullptr) {
        std::cerr << "Failed to parse HTML" << std::endl;
        return 1;
    }

    // Поиск условия задачи
    xmlNode* rootNode = xmlDocGetRootElement(doc);
    xmlNode* statementNode = nullptr;

   

    statementNode = FindNodeWithClass(rootNode, "problem-statement");

    // Извлечение текста из узла условия задачи
    std::string statementText;
    if (statementNode != nullptr) {
        statementText = ExtractTextFromNode(statementNode);
    }
    else {
        std::cerr << "Failed to find statement node" << std::endl;
        return 1;
    }

    // Запись текста в файл
    std::ofstream outputFile("problem.md");
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file" << std::endl;
        return 1;
    }

    outputFile << statementText;

    outputFile.close();

    std::cout << "Problem statement has been saved to problem.md" << std::endl;

    // Освобождение ресурсов парсера HTML
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 0;
}