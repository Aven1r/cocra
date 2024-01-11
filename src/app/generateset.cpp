#include "parser.hpp"
#include "generator.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

class Args {
public:
    std::string pathToFolder;
    std::string ratings; 
    std::string tag;
    void parse(int argc, char* argv[]);
};

void Args::parse(int argc, char* argv[]) {
    if(argc > 1) pathToFolder = argv[1];
    if(argc > 2) ratings = argv[2];
    if(argc > 3){
        tag = argv[3];
    }else{
        tag = "all";
    }
}

int main(int argc, char* argv[]) {
    Args args;
    args.parse(argc, argv);

    
    ProblemSetGenerator problemSetGenerator(args.pathToFolder, args.ratings, args.tag);
    problemSetGenerator.generateProblemSet();

    return 0;
}