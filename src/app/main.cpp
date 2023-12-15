#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "parser.hpp"
// #include "parser.hpp" // Для работы с библиотекой
// Сама библиотека подключается на этапе сборки, поэтому за примерами придется обращаться в гитхаб

/*
В целом main.cpp будет файлом для работы с программой в консольной строке.
Здесь пока можно играться с парсингом задач, но по итогу хотелось бы, чтобы ты обернул все красиво в функции
и сохранил в файле src/lib/parcer.cpp, а я просто дергал здесь их
*/

#include <iostream>
#include <string>
#include "parser.hpp"

int main() {
    std::string input;
    std::cout << "Enter a directory path: ";
    std::getline(std::cin, input);

    std::string language;
    std::cout << "Enter a language code (ru/eng): ";
    std::getline(std::cin, language);

    std::int16_t count;
    std::cout << "Enter a number of tasks (0 for all): ";
    std::cin >> count;

    json json_response = GetJson();
    parsing(input, language, json_response, count);

    return 0;
}