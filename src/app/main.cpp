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
class Args {
public:
    std::string input;
    std::string language; 
    int count;
    void parse(int argc, char* argv[]);
    
};

void Args::parse(int argc, char* argv[]) {
    if(argc > 1) input = argv[1];
    if(argc > 2) language = argv[2];
    if(argc > 3) count = atoi(argv[3]);
}

int main(int argc, char* argv[]) {
    Args args;
    args.parse(argc, argv);

    json json_response = GetJson();
    parsing(args.input, args.language, json_response, args.count);

    return 0;
}