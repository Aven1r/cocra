#include "parser.hpp"


std::string ExtractTextFromNode(xmlNode* node) {
    std::string html;

    xmlBufferPtr buffer = xmlBufferCreate();
    if (buffer == nullptr) {
        std::cerr << "Failed to create buffer" << std::endl;
        return html;
    }

    xmlNodeDump(buffer, node->doc, node, 0, 0);

    html = reinterpret_cast<char*>(buffer->content);

    size_t found = html.find("$$$");
    while (found != std::string::npos) {
        html.replace(found, 3, "$");
        found = html.find("$$$", found + 1);
    }

    xmlBufferFree(buffer);

    std::string newHtml;
    size_t pos = 0;
    while (pos < html.size()) {
        if (html.compare(pos, 4, "</p>") == 0 || html.compare(pos, 6, "</div>") == 0) {
            newHtml += html.substr(pos, (html.find('>', pos) - pos) + 1);
            newHtml += '\n'; // Добавляем перенос строки
            
            pos += (html.find('>', pos) - pos) + 1;
        }
        else {
            newHtml += html[pos];
            ++pos;
        }
    }

    return newHtml;
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

json GetJson() {
    cpr::Response response = cpr::Get(cpr::Url{ "https://codeforces.com/api/problemset.problems" });
    if (response.status_code != 200) {
        std::cerr << "Failed to retrieve problemset" << std::endl;
        return 1;
    }

    // Парсинг JSON с информацией о задачах
    json json_response = json::parse(response.text);
    return json_response;
}
void parsing(const std::string& path_to_problems, const std::string& lang, json json_response) {
    // Проходка по списку задач и для каждой создание соответствующей папки и файла
    std::string folderPath = path_to_problems + "/" + "problems/";
        for (const auto& problem : json_response["result"]["problems"]) {
            std::string contestId = problem["contestId"].dump();
            std::string problemId = problem["index"].dump();
            problemId.erase(remove(problemId.begin(), problemId.end(), '\"'), problemId.end());
            // TODO - Дать возможность пользователю изменить путь
            std::string folderPathproblem = folderPath + contestId + problemId;
            std::string filePath = folderPathproblem + "/" + contestId + problemId + ".md";
            // Проверка на существование папки
            if (fs::exists(folderPathproblem)) {
                std::cout << "Folder for problem " << contestId << problemId << " already exists. Skipping." << std::endl;
                continue;
            }
            // Создание директории для задачи

            // Формирование URL-адреса задачи
            std::string url = "https://codeforces.com/problemset/problem/" + contestId + "/" + problemId + "?locale=" + lang;

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

            // Извлечение текста из узла условия задачи
            std::string statementText;
            if (statementNode != nullptr) {
                statementText = ExtractTextFromNode(statementNode);
            }
            else {
                std::cerr << "Failed to find statement node for problem " << contestId << problemId << std::endl;
                continue;
            }
            html::parser p;
            html::node_ptr node = p.parse(statementText);

            statementText = node->to_text();
            fs::create_directories(folderPathproblem);

            // Запись текста в файл
            std::ofstream outputFile(filePath);
            if (!outputFile.is_open()) {
                std::cerr << "Failed to open output file for problem " << contestId << problemId << std::endl;
                continue;
            }

            outputFile << statementText;

            outputFile.close();

            std::cout << "Problem statement for " << contestId << problemId << " has been saved to " << filePath << std::endl;

            // Освобождение ресурсов парсера HTML
            xmlFreeDoc(doc);
            xmlCleanupParser();
        }
}