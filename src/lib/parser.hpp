#ifndef PARSE
#define PARSE

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>

#include "html.hpp"
#include <html2md.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <libxml/HTMLparser.h>


using json = nlohmann::json;
namespace fs = std::filesystem;

std::string ExtractTextFromNode(xmlNode* node);

xmlNode* FindNodeWithClass(xmlNode* node, const char* className);

json GetJson();

void parsing(const std::string& path_to_problems,const std::string& lang, json json_response,int numProblems);

void processText(std::string& text);

void template_html(std::string& htmlString);

void editString(std::string& aString);


class ProblemSetGenerator {
    private:
    std::string pathToFolder_;
    std::string lowest_rating;
    std::string highest_rating;
    std::string tag_;

    public:
    /**
     * @brief Construct a new Problem Set Generator object
     * 
     * @param pathToFolder 
     * @param ratings 
     * @param tag 
     */
    ProblemSetGenerator(std::string pathToFolder, std::string ratings, std::string tag);

    void generateProblemSet();
};

#endif