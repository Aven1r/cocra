#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <libxml/HTMLparser.h>
#include <filesystem>




using json = nlohmann::json;
namespace fs = std::filesystem;


// Функция для извлечения HTML
std::string ExtractHtmlFromNode(xmlNode* node) {
    std::string html;

    xmlBufferPtr buffer = xmlBufferCreate();
    if (buffer == nullptr) {
        std::cerr << "Failed to create buffer" << std::endl;
        return html;
    }

    xmlNodeDump(buffer, node->doc, node, 0, 0);

    html = reinterpret_cast<char*>(buffer->content);
    html.erase(html.begin(), std::find_if(html.begin(), html.end(), [](int ch) {
        return !std::isspace(ch);
        }));

    size_t found = html.find("$$$");
    while (found != std::string::npos) {
        html.replace(found, 3, "$");
        found = html.find("$$$", found + 1); // Search for the next occurrence
    }

    xmlBufferFree(buffer);

    return html;
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
    // Отправка GET-запроса и получение списка задач
    cpr::Response response = cpr::Get(cpr::Url{ "https://codeforces.com/api/problemset.problems" });
    if (response.status_code != 200) {
        std::cerr << "Failed to retrieve problemset" << std::endl;
        return 1;
    }

    // Парсинг JSON с информацией о задачах
    json json_response = json::parse(response.text);

    // Проходка по списку задач и для каждой создание соответствующей папки и файла
    for (const auto& problem : json_response["result"]["problems"]) {
        std::string contestId = problem["contestId"].dump();
        std::string problemId = problem["index"].dump();
        problemId.erase(remove(problemId.begin(), problemId.end(), '\"'), problemId.end());
        std::string folderPath = "problems/" + contestId + problemId;
        std::string filePath = folderPath + "/" + contestId + problemId + ".html";
        // Проверка на существование папки
        if (fs::exists(folderPath)) {
            std::cout << "Folder for problem " << contestId << problemId << " already exists. Skipping." << std::endl;
            continue;
        }
        // Создание директории для задачи
        fs::create_directories(folderPath);

        // Формирование URL-адреса задачи
        std::string url = "https://codeforces.com/problemset/problem/" + contestId + "/" + problemId;

        // Отправка GET-запроса и получение HTML-страницы
        cpr::Response problemResponse = cpr::Get(cpr::Url{ url });

        // Проверка статуса запроса
        if (problemResponse.status_code != 200) {
            std::cerr << "Failed to retrieve webpage for problem " << contestId << problemId << std::endl;
            continue;
        }

        // Инициализация парсера HTML
        htmlDocPtr doc = htmlReadMemory(problemResponse.text.c_str(), problemResponse.text.size(), nullptr, nullptr, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
        if (doc == nullptr) {
            std::cerr << "Failed to parse HTML for problem " << contestId << problemId << std::endl;
            continue;
        }

        // Поиск условия задачи
        xmlNode* rootNode = xmlDocGetRootElement(doc);
        xmlNode* statementNode = nullptr;

        statementNode = FindNodeWithClass(rootNode, "problem-statement");

        // Извлечение HTML из узла условия задачи
        std::string statementHTML;
        if (statementNode != nullptr) {
            statementHTML = ExtractHtmlFromNode(statementNode);
        }
        else {
            std::cerr << "Failed to find statement node for problem " << contestId << problemId << std::endl;
            continue;
        }
        

        // Запись текста в файл
        std::ofstream outputFile(filePath);
        if (!outputFile.is_open()) {
            std::cerr << "Failed to open output file for problem " << contestId << problemId << std::endl;
            continue;
        }

        outputFile << statementHTML;;

        outputFile.close();

        std::cout << "Problem statement for " << contestId << problemId << " has been saved to " << filePath << std::endl;

        // Освобождение ресурсов парсера HTML
        xmlFreeDoc(doc);
        xmlCleanupParser();
    }

    return 0;
}
