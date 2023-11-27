#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "parser.hpp"
// #include "html.hpp" // Для работы с библиотекой
// Сама библиотека подключается на этапе сборки, поэтому за примерами придется обращаться в гитхаб

/*
В целом main.cpp будет файлом для работы с программой в консольной строке.
Здесь пока можно играться с парсингом задач, но по итогу хотелось бы, чтобы ты обернул все красиво в функции
и сохранил в файле src/lib/parcer.cpp, а я просто дергал здесь их
*/

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
        // TODO - Дать возможность пользователю изменить путь
        std::string folderPath = "problems/" + contestId + problemId;
        std::string filePath = folderPath + "/" + contestId + problemId + ".md";
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

        // Извлечение текста из узла условия задачи
        std::string statementText;
        if (statementNode != nullptr) {
            statementText = ExtractTextFromNode(statementNode);
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

        outputFile << statementText;

        outputFile.close();

        std::cout << "Problem statement for " << contestId << problemId << " has been saved to " << filePath << std::endl;

        // Освобождение ресурсов парсера HTML
        xmlFreeDoc(doc);
        xmlCleanupParser();
    }

    return 0;
}