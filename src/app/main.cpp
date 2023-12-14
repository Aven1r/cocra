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

int main() {
    json json_response = GetJson();
    parsing("C:/Users", "ru", json_response);
}