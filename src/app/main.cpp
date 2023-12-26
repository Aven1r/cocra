#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "parser.hpp"
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


    parsing(args.input, args.language,args.count);

    return 0;
}
